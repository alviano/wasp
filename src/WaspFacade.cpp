/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "WaspFacade.h"

#include "Restart.h"

#include "input/GringoNumericFormat.h"

#include "outputBuilders/WaspOutputBuilder.h"
#include "outputBuilders/SilentOutputBuilder.h"
#include "outputBuilders/ThirdCompetitionOutputBuilder.h"
#include "outputBuilders/CompetitionOutputBuilder.h"
#include "outputBuilders/DimacsOutputBuilder.h"

#include "outputBuilders/MultiOutputBuilder.h"
#include "QueryInterface.h"
#include "outputBuilders/IdOutputBuilder.h"
#include "outputBuilders/NoopOutputBuilder.h"
#include "Enumeration.h"

void
WaspFacade::readInput(
    istream& in )
{
    char tmp;
    in >> tmp;

    if( !in.good() && !in.eof() )
        WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );

    in.putback( tmp );
    switch ( tmp )
    {
        case COMMENT_DIMACS:
        case FORMULA_INFO_DIMACS:
        {
            DimacsOutputBuilder* d = new DimacsOutputBuilder();            
            solver.setOutputBuilder( d );
            Dimacs dimacs( solver );
            dimacs.parse();
            if( dimacs.isMaxsat() )
                d->setMaxsat();
            delete outputBuilder;
            outputBuilder = d;
            greetings();
            break;
        }

        default:
        {
            GringoNumericFormat gringo( solver );
            gringo.parse( in );
//            solver.setOutputBuilder( new WaspOutputBuilder() );
            greetings();
            break;
        }
    }
}

void
WaspFacade::solve()
{
    if(runtime_) { WaspErrorMessage::errorGeneric("Calling method solve twice."); return; }
    runtime_ = true;

    if(disableVE_) solver.disableVariableElimination();
    if(!solver.preprocessing()) { solver.foundIncoherence(); return; }
    
    statistics( &solver, startSolving() );
    solver.onStartingSolver();

    if(wasp::Options::queryAlgorithm != NO_QUERY) { QueryInterface queryInterface( solver ); queryInterface.solve(); }    
    else if(!solver.isOptimizationProblem() && !wasp::Options::useLazyWeakConstraints) { Enumeration enumeration(solver); enumeration.solve(); }
    else handleWeakConstraints();
    
    return;
}

void WaspFacade::handleWeakConstraints() {
    NoopOutputBuilder* tmp = new NoopOutputBuilder();
    if( wasp::Options::printOnlyOptimum && wasp::Options::maxModels > 1 )
        solver.setOutputBuilder( tmp );
    unsigned int result = solveWithWeakConstraints();
    switch( result ) {
        case COHERENT:
            cerr << "ERROR";
            break;

        case INCOHERENT:
            if( wasp::Options::printOnlyOptimum && wasp::Options::maxModels > 1 )
                solver.setOutputBuilder( outputBuilder );
            solver.foundIncoherence();
            break;

        case OPTIMUM_FOUND:
        default:
            if( wasp::Options::maxModels > 1 ) {
                solver.unrollToZero();
                solver.clearConflictStatus();
                if( wasp::Options::printOnlyOptimum )
                    solver.setOutputBuilder( outputBuilder );
                solver.setMinimizeUnsatCore(false);
                Enumeration enumeration(solver);
                enumeration.solve();
                if( wasp::Options::printOnlyOptimum )
                    tmp->print();
                solver.optimumFound();
            }
            else
                solver.optimumFound();
            break;
    }
    delete tmp;
}

void
WaspFacade::setOutputPolicy(
    OUTPUT_POLICY outputPolicy )
{
    delete outputBuilder;
    switch( outputPolicy )
    {
        case COMPETITION_OUTPUT:
            outputBuilder = new CompetitionOutputBuilder();
            break;
            
        case DIMACS_OUTPUT:
            outputBuilder = new DimacsOutputBuilder();
            break;
            
        case SILENT_OUTPUT:
            outputBuilder = new SilentOutputBuilder();
            break;
            
        case THIRD_COMPETITION_OUTPUT:
            outputBuilder = new ThirdCompetitionOutputBuilder();
            break;
            
        case MULTI:
            outputBuilder = new MultiOutputBuilder();
            break;
            
        case ID_OUTPUT:
            outputBuilder = new IdOutputBuilder();
            break;
            
        case WASP_OUTPUT:
        default:
            outputBuilder = new WaspOutputBuilder();
            break;
    }
    solver.setOutputBuilder( outputBuilder );
}

void
WaspFacade::setRestartsPolicy(
    RESTARTS_POLICY restartsPolicy,
    unsigned int threshold )
{
    assert( threshold > 0 );
    Restart* restart;
    switch( restartsPolicy )
    {
        case GEOMETRIC_RESTARTS_POLICY:
            restart = new Restart( threshold, false );
            solver.setRestart( restart );
            break;            

        case SEQUENCE_BASED_RESTARTS_POLICY:
        default:
            restart = new Restart( threshold, true );
            solver.setRestart( restart );
            break;
    }
}

void merge(int left,int center,int right,vector<Literal>& literals,vector<uint64_t>& weights)
{
    Literal* auxLiterals = new Literal[right+1];
    uint64_t* auxWeights = new uint64_t[right+1];

    int i, j;
    for(i=center+1; i>left; i--) { auxLiterals[i-1]=literals[i-1]; auxWeights[i-1]=weights[i-1]; }
    for(j=center; j<right; j++) { auxLiterals[right+center-j]=literals[j+1]; auxWeights[right+center-j]=weights[j+1]; }
    for( int k = left; k <= right; k++ ) {
        if(auxWeights[j]>auxWeights[i]) { literals[k]=auxLiterals[j]; weights[k]=auxWeights[j--]; }
        else { literals[k]=auxLiterals[i]; weights[k]=auxWeights[i++]; }
    }

    delete [] auxWeights; delete [] auxLiterals;
}

void mergesort(int left,int right,vector<Literal>& literals,vector<uint64_t>& weights )
{
    if( left < right ) {
        int center = (left+right)/2;
        mergesort(left,center,literals,weights);
        mergesort(center+1,right,literals,weights);
        merge(left,center,right,literals,weights);
    }
}

bool
WaspFacade::addPseudoBooleanConstraint(
    vector<Literal>& lits,
    vector<uint64_t>& weights,
    uint64_t bound)
{
    if(solver.conflictDetected() || !ok_) return false;
    solver.unrollToZero();
    if(lits.size() != weights.size())
        WaspErrorMessage::errorGeneric("The size of literals must be equal to the size of weights in pseudoBoolean constraints.");

    if(bound == 0) return true;
    uint64_t differentWeights = UINT64_MAX;
    uint64_t sumOfWeights = 0;
    unsigned int j = 0;
    for(unsigned int i = 0; i < lits.size(); i++) {
        lits[i]=lits[j];    weights[i]=weights[j];
        addVariables(lits[i].getVariable());        
        if(solver.hasBeenEliminated(lits[i].getVariable())) WaspErrorMessage::errorGeneric("Trying to add a deleted variable to aggregate.");
        if(solver.isFalse(lits[i]) || weights[i] == 0) continue;
        if(solver.isTrue(lits[i])) {
            if(bound <= weights[i]) return true;
            bound -= weights[i];
            continue;
        }
        
        if(weights[i] > bound) { weights[i] = bound; }
        if(differentWeights == UINT64_MAX)
            differentWeights = weights[i];            
        else if(differentWeights != weights[i])
            differentWeights = UINT64_MAX-1;
        sumOfWeights += weights[i];
        j++;
    }
    lits.resize(j);    weights.resize(j);
    if(sumOfWeights < bound) { ok_ = false; return false; }
    if(differentWeights < UINT64_MAX-1 && differentWeights >= bound) { return addClause(lits); }
    
    mergesort(0, lits.size()-1, lits, weights);
        
    if(runtime_) solver.addVariableRuntime();
    else solver.addVariable();
    Var aggrId = solver.numberOfVariables();
    solver.setFrozen( aggrId );
    Aggregate* aggregate = new Aggregate();
    Literal aggregateLiteral = solver.getLiteral( aggrId );
    aggregate->addLiteral(aggregateLiteral.getOppositeLiteral(), 0);    
    #ifndef NDEBUG
    uint64_t previousWeight = UINT64_MAX;
    #endif    
    for( unsigned int i = 0; i < lits.size(); i++ ) {
        assert(lits[i].getVariable() <= solver.numberOfVariables());
        assert(solver.isUndefined(lits[i].getVariable()));
        assert(!solver.hasBeenEliminated(lits[i].getVariable()));
        aggregate->addLiteral( lits[i], weights[i] );
        solver.setFrozen( lits[i].getVariable() );
        assert_msg( previousWeight >= weights[i], "Literals must be sorted in increasing order" );
        assert( previousWeight = weights[i] );        
    }
    assert( aggregate->size() >= 1 );    
    bool res = solver.addClauseRuntime(aggregateLiteral);
    if(!res) { ok_ = false; return false; }        
    
    if( !aggregate->updateBound( solver, bound ) ) { delete aggregate; ok_ = false; return false; }

    solver.attachAggregate( *aggregate );
    solver.addAggregate( aggregate );
    
    assert( solver.isTrue( aggregateLiteral ) );
    aggregate->onLiteralFalse( solver, aggregateLiteral.getOppositeLiteral(), 1 );
    
    if( solver.conflictDetected() ) { ok_ = false; return false; }        
    return ok_;
}

unsigned int
WaspFacade::solve(const vector<Literal>& assumptions, vector<Literal>& conflict)
{
    if(!ok_) { return INCOHERENT; }
    //First call
    if(!runtime_) {
        runtime_ = true;
        if(disableVE_) solver.disableVariableElimination();
        if(!solver.preprocessing()) { ok_ = false; return INCOHERENT; }
        solver.turnOffSimplifications();
        solver.setComputeUnsatCores(true);
        statistics(&solver, startSolving());
        solver.onStartingSolver();
    }
    conflict.clear();
    solver.unrollToZero();
    vector<Literal> assumptions_;
    for(unsigned int i = 0; i < assumptions.size(); i++) {
        if(solver.hasBeenEliminated(assumptions[i].getVariable())) WaspErrorMessage::errorGeneric("Assumption literal has been eliminated. (Assumptions must be frozen).");
        assumptions_.push_back(assumptions[i]);
    }
    
    unsigned int res = solver.solve(assumptions_);
    if( res == INCOHERENT ) {
        assert(solver.getUnsatCore() != NULL);
        const Clause& core = *solver.getUnsatCore();
        for( unsigned int i = 0; i < core.size(); i++ )
            conflict.push_back(core.getAt(i));
        if(core.size() == 0) ok_ = false;
        solver.unrollToZero();
        solver.clearConflictStatus();    
    }
    return res;
}
