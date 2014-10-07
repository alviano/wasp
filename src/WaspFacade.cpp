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

#include "MinisatHeuristic.h"

void
WaspFacade::readInput()
{
    char tmp;
    cin >> tmp;

    if( !cin.good() && !cin.eof() )
    {   
        ErrorMessage::errorDuringParsing( "Unexpected symbol." );
    }    

    cin.putback( tmp );
    switch ( tmp )
    {
        case COMMENT_DIMACS:
        case FORMULA_INFO_DIMACS:
        {
            Dimacs dimacs( solver );
            dimacs.parse();
            solver.setOutputBuilder( new DimacsOutputBuilder() );
            break;
        }

        default:
        {
            GringoNumericFormat gringo( solver );
            gringo.parse();
//            solver.setOutputBuilder( new WaspOutputBuilder() );
            break;
        }
    }
}

void
WaspFacade::solve()
{
    if( printProgram )
    {
        solver.printProgram();
        return;
    }   
    
    if( solver.preprocessing() )
    {
        if( printDimacs )
        {
            solver.printDimacs();
            return;
        }
        
        if( !solver.hasOptimizationAggregate() )
        {            
            while( solver.solve() )
            {
                solver.printAnswerSet();
                trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
                if( ++numberOfModels >= maxModels )
                {
                    trace_msg( enumeration, 1, "Enumerated " << maxModels << "." );
                    break;
                }
                else if( !solver.addClauseFromModelAndRestart() )
                {
                    trace_msg( enumeration, 1, "All models have been found." );
                    break;
                }
            }
        }
        else
        {
            solveWithWeakConstraints();
        }
    }

    if( numberOfModels == 0 )
    {
        trace_msg( enumeration, 1, "No model found." );
        solver.foundIncoherence();
    }
    
//    solver.printLearnedClauses();
}

void
WaspFacade::setDeletionPolicy(
    DELETION_POLICY deletionPolicy,
    unsigned int /*deletionThreshold*/ )
{
    switch( deletionPolicy )
    {
//        case RESTARTS_BASED_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new RestartsBasedDeletionStrategy( solver ) );
//            break;
//            
//        case MINISAT_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new MinisatDeletionStrategy( solver ) );
//            break;
//            
//        case GLUCOSE_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new GlueBasedDeletionStrategy( solver, deletionThreshold ) );
//            break;
//
//        case AGGRESSIVE_DELETION_POLICY:
//        default:
//            heuristic->setDeletionStrategy( new AggressiveDeletionStrategy( solver ) );
//            break;
    }
}

void
WaspFacade::setDecisionPolicy(
    DECISION_POLICY decisionPolicy,
    unsigned int /*threshold*/ )
{
    switch( decisionPolicy )
    {
//        case HEURISTIC_BERKMIN:
//            assert( threshold > 0 );
//            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, threshold ) );
//            break;
//            
//        case HEURISTIC_BERKMIN_CACHE:
//            assert( threshold > 0 );
//            heuristic->setDecisionStrategy( new BerkminHeuristicWithCache( solver, threshold ) );            
//            break;
//        
//        case HEURISTIC_FIRST_UNDEFINED:
//            heuristic->setDecisionStrategy( new FirstUndefinedHeuristic( solver ) );
//            break;
//            
        case HEURISTIC_MINISAT:
            solver.setMinisatHeuristic();
            break;
//    
//        default:
//            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, 512 ) );
//            break;
    }
}

void
WaspFacade::setOutputPolicy(
    OUTPUT_POLICY outputPolicy )
{
    switch( outputPolicy )
    {
        case COMPETITION_OUTPUT:
            solver.setOutputBuilder( new CompetitionOutputBuilder() );
            break;
            
        case DIMACS_OUTPUT:
            solver.setOutputBuilder( new DimacsOutputBuilder() );
            break;
            
        case SILENT_OUTPUT:
            solver.setOutputBuilder( new SilentOutputBuilder() );
            break;
            
        case THIRD_COMPETITION_OUTPUT:
            solver.setOutputBuilder( new ThirdCompetitionOutputBuilder() );
            break;
            
        case WASP_OUTPUT:
        default:
            solver.setOutputBuilder( new WaspOutputBuilder() );
            break;
    }
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

void
WaspFacade::algorithmOpt()
{
    trace_msg( weakconstraints, 1, "Starting algorithm OPT" );
    solver.addPreferredChoicesFromOptimizationLiterals();    
    while( solver.solve() )
    {
        numberOfModels++;
        solver.printAnswerSet();
        unsigned int modelCost = solver.computeCostOfModel(); 
        
        solver.printOptimizationValue( modelCost );
        trace_msg( weakconstraints, 2, "Decision level of solver: " << solver.getCurrentDecisionLevel() );
        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;
        
//        /*Implement a better policy of unroll*/
//        solver.doRestart();
//        solver.simplifyOnRestart();
//        solver.clearConflictStatus();
        
//        solver.removePrefChoices();
        
        if( !solver.updateOptimizationAggregate( modelCost ) )
        {
            trace_msg( weakconstraints, 2, "Failed updating of optimization aggregate: return" );
            break;        
        }
    }
    
    if( numberOfModels > 0 )
        solver.optimumFound();    
}

void
WaspFacade::algorithmMgd()
{
    unsigned int minCost = UINT_MAX;
    while( solver.solve() )
    {
        numberOfModels++;
        unsigned int modelCost = solver.computeCostOfModel();
        if( modelCost < minCost )
        {
            minCost = modelCost;
            solver.printAnswerSet();        
            solver.printOptimizationValue( modelCost );
        }

        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;

        solver.addVariableRuntime();
        
        vector< Literal > literals;
        vector< unsigned int > weights;
        
        Literal aggregateLiteral( solver.numberOfVariables(), POSITIVE );
        
        literals.push_back( aggregateLiteral.getOppositeLiteral() );
        weights.push_back( 0 );
        for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
        {
            Literal l = solver.getOptimizationLiteral( i ).lit;
            unsigned weight = solver.getOptimizationLiteral( i ).weight;
            assert( !solver.isUndefined( l ) );
            if( solver.getDecisionLevel( l ) == 0 )
            {
                if( solver.isTrue( l ) )
                    modelCost -= weight;

                continue;
            }

            if( !solver.getOptimizationLiteral( i ).isRemoved() )
            {
                if( solver.isTrue( l ) )
                {
                    literals.push_back( l );
                    weights.push_back( weight );
                    solver.getOptimizationLiteral( i ).remove();
                }
            }
            else
            {
                literals.push_back( l );
                weights.push_back( weight );                    
            }
        }
        
        if( !createFalseAggregate( literals, weights, modelCost ) )
            break;
    }

    if( numberOfModels > 0 )
        solver.optimumFound();
}

bool visited( unsigned int element, const vector< unsigned int >& v, unsigned int numberOfCalls )
{
    assert( element > 0 && element < v.size() );
    return v[ element ] == numberOfCalls;
}

void visit( unsigned int element, vector< unsigned int >& v, unsigned int numberOfCalls )
{
    assert( element > 0 && element < v.size() );
    v[ element ] = numberOfCalls;
}

void
WaspFacade::algorithmOll()
{    
    trace_msg( weakconstraints, 1, "Starting algorithm OLL" );
    vector< Literal > assumptionsAND;
    vector< Literal > assumptionsOR;
    
    computeAssumptionsAND( assumptionsAND );
    
    vector< unsigned int > inUnsatCore;
    unsigned int numberOfCalls = 0;
    
    for( unsigned int i = 0; i <= solver.numberOfVariables(); i++ )
        inUnsatCore.push_back( numberOfCalls );
    
    unsigned int originalNumberOfVariables = solver.numberOfVariables();
    
    unordered_map< Var, OllData* > guardMap;

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    while( !solver.solve( assumptionsAND, assumptionsOR ) )
    {
        vector< unsigned int > auxVariablesInUnsatCore;
        ++numberOfCalls;
        assert( solver.getUnsatCore() != NULL );
        const Clause& unsatCore = *( solver.getUnsatCore() );

        //The incoherence does not depend on weak constraints
        if( unsatCore.size() == 0 )
            return;

        solver.clearConflictStatus();
        if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
            return;
        
        for( unsigned int i = 0; i < unsatCore.size(); i++ )
        {
            Var v = unsatCore[ i ].getVariable();
            visit( v, inUnsatCore, numberOfCalls );
            if( v > originalNumberOfVariables )
                auxVariablesInUnsatCore.push_back( v );
        }
        
        vector< Literal > literals;
        vector< unsigned int > weights;
        
        unsigned int minWeight = computeMinWeight( inUnsatCore, numberOfCalls );        
        processCoreOll( inUnsatCore, numberOfCalls, literals, weights, minWeight );
        trace_msg( weakconstraints, 2, "Adding aggregate from unsat core" );
        if( !addAggregateOll( inUnsatCore, guardMap, literals, weights, 2 ) )
            return;        
        
        for( unsigned int i = 0; i < auxVariablesInUnsatCore.size(); i++ )
        {
            Var guardId = auxVariablesInUnsatCore[ i ];
            assert( guardMap.find( guardId ) != guardMap.end() );
            
            OllData* ollData = guardMap[ guardId ];
            if( ollData->bound_ < ollData->literals_.size() )
            {
                trace_msg( weakconstraints, 3, "Incrementing by one bound (" << ollData->bound_ <<  ") of aggregate " << guardId );
                assert( ollData->literals_.size() > 0 );
                if( !addAggregateOll( inUnsatCore, guardMap, ollData->literals_, ollData->weights_, ollData->bound_ + 1 ) )
                    return;
            }
        }
        
        assumptionsAND.clear();
        computeAssumptionsAND( assumptionsAND );        
    }
    
    numberOfModels++;
    solver.printAnswerSet();
    solver.printOptimizationValue( solver.computeCostOfModel() );
    solver.optimumFound();
}

bool
WaspFacade::createFalseAggregate(
    const vector< Literal >& literals,
    const vector< unsigned int >& weights,
    unsigned int bound )
{
    assert( literals.size() == weights.size() );
    if( !solver.doRestart() )
        return false;

    Aggregate* aggregate = solver.createAggregate( literals, weights );   
    
    assert( literals.size() >= 1 );
    solver.addClause( literals[ 0 ] );
    solver.attachAggregate( *aggregate );
    if( !aggregate->updateBound( solver, bound ) )
        return false;

    assert( solver.isFalse( literals[ 0 ].getOppositeLiteral() ) );
    aggregate->onLiteralFalse( solver, literals[ 0 ].getOppositeLiteral(), -1 );    

    solver.addAggregate( aggregate );
    
    if( solver.conflictDetected() )
        return false;
    
    return true;
}

Aggregate*
WaspFacade::createAggregate(
    Var aggrId,
    const vector< Literal >& literals,
    const vector< unsigned int >& weights )
{
    assert( literals.size() == weights.size() );

    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral( Literal( aggrId, NEGATIVE ), 0 );

    for( unsigned int i = 0; i < literals.size(); i++ )
        aggregate->addLiteral( literals[ i ], weights[ i ] );

    return aggregate;
}

bool
WaspFacade::processAndAddAggregate(
    Aggregate* aggregate,
    unsigned int bound )
{    
    trace_msg( weakconstraints, 3, "Processing aggregate " << *aggregate << " - with bound " << bound );
        
    if( !aggregate->updateBound( solver, bound ) )
    {
        trace_msg( weakconstraints, 4, "Failed updating of the bound: aggregate is false" );
        solver.addClause( aggregate->getLiteral( 1 ) );
        assert( solver.isFalse( aggregate->getLiteral( 1 ).getVariable() ) );
    }
    else
    {
        if( aggregate->isTrue() )
            solver.addClause( aggregate->getLiteral( 1 ).getOppositeLiteral() );
        solver.attachAggregate( *aggregate );
    }

    solver.addAggregate( aggregate );
    
    trace_msg( weakconstraints, 3, "Adding aggregate " << *aggregate );
    assert( !solver.conflictDetected() );
    return true;
}

void
WaspFacade::processCoreOll(
    const vector< unsigned int >& inUnsatCore,
    unsigned int numberOfCalls,
    vector< Literal >& literals,
    vector< unsigned int >& weights,
    unsigned int minWeight )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm OLL" );
    unsigned int originalSize = solver.numberOfOptimizationLiterals();
    for( unsigned int i = 0; i < originalSize; i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( i );        
        if( optLitData.isRemoved() )
            continue;

        trace_msg( weakconstraints, 3, "Considering literal " << optLitData.lit << " - weight " << optLitData.weight );
        Var v = optLitData.lit.getVariable();
        if( visited( v, inUnsatCore, numberOfCalls ) )
        {
            trace_msg( weakconstraints, 4, "is in unsat core" );
            literals.push_back( optLitData.lit );
//            weights.push_back( optLitData.weight );
            weights.push_back( 1 );
            optLitData.remove();
            
            if( optLitData.weight > minWeight )
                solver.addOptimizationLiteral( optLitData.lit, optLitData.weight - minWeight, UINT_MAX, true );            
        }
    }
}

bool
WaspFacade::addAggregateOll(
    vector< unsigned int >& inUnsatCore,
    unordered_map< Var, OllData* >& guardMap,    
    vector< Literal >& literals,
    vector< unsigned int >& weights,
    unsigned int bound )
{
    Var aggrId = addAuxVariable( inUnsatCore );
    Aggregate* aggregate = createAggregate( aggrId, literals, weights );     
    if( !processAndAddAggregate( aggregate, bound ) )
        return false;
    
    if( solver.isFalse( aggrId ) )
        return true;
    
    Var guardId = addBinaryClauseForAggregateOll( inUnsatCore, aggrId );
    OllData* ollData = new OllData();
    ollData->addElement( bound, guardId, literals, weights );
    guardMap[ guardId ] = ollData;
    return true;
}

unsigned int
WaspFacade::computeMinWeight(
    const vector< unsigned int >& inUnsatCore,
    unsigned int numberOfCalls )
{
    unsigned int minWeight = UINT_MAX;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( i );        
        if( optLitData.isRemoved() )
            continue;
        if( visited( optLitData.lit.getVariable(), inUnsatCore, numberOfCalls ) && minWeight > optLitData.weight )
            minWeight = optLitData.weight;
    }
    
    assert( minWeight != UINT_MAX );
    trace_msg( weakconstraints, 2, "Min weight " << minWeight );
    return minWeight;
}