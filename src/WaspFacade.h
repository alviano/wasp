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

#ifndef WASPFACADE_H
#define WASPFACADE_H

#include <cassert>
#include <vector>
#include <unordered_map>
using namespace std;

#include "util/Constants.h"
#include "Solver.h"
#include "input/Dimacs.h"

struct OllData
{
    unsigned int bound_;
    unsigned int guardId_;
    vector< Literal > literals_;
    vector< unsigned int > weights_;

    void addElement( unsigned int bound, unsigned int guard, vector< Literal >& literals, vector< unsigned int >& weights )
    {
        bound_ = bound;
        guardId_ = guard;
        literals_.swap( literals );
        weights_.swap( weights );
    }
};

class WaspFacade
{
    public:
        inline WaspFacade();
        inline ~WaspFacade(){}
        
        void readInput();
        void solve();
        
        inline void greetings(){ solver.greetings(); }
        
        void setDeletionPolicy( DELETION_POLICY, unsigned int deletionThreshold );
        void setDecisionPolicy( DECISION_POLICY, unsigned int heuristicLimit );
        void setOutputPolicy( OUTPUT_POLICY );
        void setRestartsPolicy( RESTARTS_POLICY, unsigned int threshold );

        inline void setMaxModels( unsigned int max ) { maxModels = max; }
        inline void setPrintProgram( bool printProgram ) { this->printProgram = printProgram; }
        inline void setPrintDimacs( bool printDimacs ) { this->printDimacs = printDimacs; }
        void setExchangeClauses( bool exchangeClauses ) { solver.setExchangeClauses( exchangeClauses ); }
        
        inline void setWeakConstraintsAlgorithm( WEAK_CONSTRAINTS_ALG alg ) { weakConstraintsAlg = alg; }
        
        inline void solveWithWeakConstraints();        
        
    private:
        Solver solver;        
        
        void algorithmOpt();
        void algorithmMgd();
        void algorithmOll();
        inline void algorithmMgdOll() { solver.isWeighted() ? algorithmMgd() : algorithmOll(); }
        
        unsigned int numberOfModels;
        unsigned int maxModels;
        bool printProgram;
        bool printDimacs;

        WEAK_CONSTRAINTS_ALG weakConstraintsAlg;
        
        bool createFalseAggregate( const vector< Literal >& literals, const vector< unsigned int >& weights, unsigned int bound );
        Aggregate* createAggregate( Var aggrId, const vector< Literal >& literals, const vector< unsigned int >& weights );
        bool processAndAddAggregate( Aggregate* aggregate, unsigned int bound );
        
        inline void computeAssumptionsAND( vector< Literal >& assumptionsAND );
        void processCoreOll( const vector< unsigned int >& inUnsatCore, unsigned int numberOfCalls, vector< Literal >& literals, vector< unsigned int >& weights, unsigned int minWeight );
        bool addAggregateOll( vector< unsigned int >& inUnsatCore, unordered_map< Var, OllData* >& guardMap, vector< Literal >& literals, vector< unsigned int >& weights, unsigned int bound );
        unsigned int computeMinWeight( const vector< unsigned int >& inUnsatCore, unsigned int numberOfCalls );
        inline Var addAuxVariable( vector< unsigned int >& inUnsatCore );
        inline Var addBinaryClauseForAggregateOll( vector< unsigned int >& inUnsatCore, Var aggrId );                
};

WaspFacade::WaspFacade() : numberOfModels( 0 ), maxModels( 1 ), printProgram( false ), printDimacs( false ), weakConstraintsAlg( OPT )
{    
}

void
WaspFacade::solveWithWeakConstraints()
{
    switch( weakConstraintsAlg )
    {
        case BCD:
            assert( 0 );
            break;
            
        case OLL:
            algorithmOll();
            break;
            
        case MGD:
            algorithmMgd();
            break;

        case OPT:
            algorithmOpt();
            break;

        case MGDOLL:    
        default:
            algorithmMgdOll();
            break;            
    }
}

void
WaspFacade::computeAssumptionsAND(
    vector< Literal >& assumptionsAND )
{    
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        if( solver.getOptimizationLiteral( i ).isRemoved() )
            continue;
        assumptionsAND.push_back( solver.getOptimizationLiteral( i ).lit.getOppositeLiteral() );
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "AssumptionsAND: [";
        for( unsigned int i = 0; i < assumptionsAND.size(); i++ )
            cerr << " " << assumptionsAND[ i ];
        cerr << " ]" << endl;
    });
}

Var
WaspFacade::addAuxVariable(
    vector< unsigned int >& inUnsatCore )
{
    solver.addVariableRuntime();
    inUnsatCore.push_back( 0 );
    return solver.numberOfVariables();        
}

Var
WaspFacade::addBinaryClauseForAggregateOll(
    vector< unsigned int >& inUnsatCore,
    Var aggrId )
{    
    Literal lit( addAuxVariable( inUnsatCore ), POSITIVE );
    solver.addOptimizationLiteral( lit, 1, UINT_MAX, true );    
    solver.addClause( lit, Literal( aggrId, NEGATIVE ) );
    
    assert( !solver.isFalse( aggrId ) );
    if( solver.isTrue( aggrId ) )
        solver.addClause( lit );
    
    return lit.getVariable();
}

#endif
