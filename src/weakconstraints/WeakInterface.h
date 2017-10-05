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

#ifndef WASP_WEAKINTERFACE_H
#define WASP_WEAKINTERFACE_H

#include <vector>
#include <iostream>
using namespace std;
#include "../util/WaspTrace.h"
#include "../util/WaspAssert.h"
#include "../Literal.h"
#include "../Solver.h"
#include "OptimizationProblemUtils.h"

class WeakInterface
{
    public:
        WeakInterface( Solver& s ) : solver( s ), numberOfCalls( 0 ), disjCoresPreprocessing( false ), mixedApproach( false ), weight( UINT64_MAX ) 
        {
            utils = new OptimizationProblemUtils( s );
        }
        virtual ~WeakInterface() { delete utils; }
        unsigned int solve();        
        
        inline void setDisjCoresPreprocessing( bool value ) { disjCoresPreprocessing = value; }               
        inline void setMixedApproach() { mixedApproach = true; }
        
    protected:
        virtual unsigned int run() = 0;
        bool disjointCorePreprocessing();
        bool createFalseAggregate( const vector< Literal >& literals, const vector< uint64_t >& weights, uint64_t bound );
        Aggregate* createAndReturnFalseAggregate( const vector< Literal >& literals, const vector< uint64_t >& weights, uint64_t bound );
        Aggregate* createAggregate( Var aggrId, const vector< Literal >& literals, const vector< uint64_t >& weights );
        Aggregate* createAggregateCount( Var aggrId, const vector< Literal >& literals );
        bool createAggregateFromOptimizationLiterals();
        bool processAndAddAggregate( Aggregate* aggregate, uint64_t bound );        
        inline void computeAssumptions();
        void resetSolver();
        uint64_t computeMinWeight();
        inline Var addAuxVariable();
        inline bool visited( Var v, unsigned int value ) const { assert( v > 0 && v < inUnsatCore.size() ); return inUnsatCore[ v ] == value; }
        inline bool visited( Var v ) const { return visited( v, numberOfCalls ); }
        inline void visit( Var v ) { assert( v > 0 && v < inUnsatCore.size() ); inUnsatCore[ v ] = numberOfCalls; }
        inline void initInUnsatCore();
        inline void preprocessingWeights();
        inline void computeAssumptionsStratified();
        inline bool changeWeight();
        bool hardening();        
        
        virtual bool foundUnsat() { return true; }

        Solver& solver;
        vector< unsigned int > inUnsatCore;
        unsigned int numberOfCalls;
        vector< Literal > assumptions;                
        
        bool disjCoresPreprocessing;                
        bool mixedApproach;
        
        inline void incrementLb( uint64_t value ) { utils->incrementLb( value ); }
        inline uint64_t lb() { return utils->lb(); }
        inline uint64_t ub() { return utils->ub(); }
        inline unsigned int level() { return utils->level(); }
        inline void setLowerBound( unsigned int value ) { utils->setLowerBound( value ); }
        inline void setUpperBound( uint64_t value ) { utils->setUpperBound( value ); }
        
    private:
        vector< uint64_t > weights;        
        uint64_t weight;                
        
        inline void clearAssumptions() { assumptions.clear(); }
        inline void computeAssumptionsOnlyOriginal( unsigned int originalNumberOfOptLiterals );
        const Clause* minimizeUnsatCoreWithProgression( const Clause* );        
        const Clause* minimizeUnsatCoreWithLinearSearch( const Clause* );
        
        OptimizationProblemUtils* utils;
};

void
WeakInterface::computeAssumptions()
{    
    solver.sortOptimizationLiterals( level() );
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        if( solver.getOptimizationLiteral( level(), i ).isRemoved() )
            continue;
        assumptions.push_back( solver.getOptimizationLiteral( level(), i ).lit.getOppositeLiteral() );
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "Assumptions: [";
        for( unsigned int i = 0; i < assumptions.size(); i++ )
            cerr << " " << assumptions[ i ];
        cerr << " ]" << endl;
    });
}

void
WeakInterface::computeAssumptionsOnlyOriginal(
    unsigned int originalNumberOfOptLiterals )
{    
    solver.sortOptimizationLiterals( level() );
    for( unsigned int i = 0; i < originalNumberOfOptLiterals; i++ )
    {
        if( solver.getOptimizationLiteral( level(), i ).isRemoved() )
            continue;
        assumptions.push_back( solver.getOptimizationLiteral( level(), i ).lit.getOppositeLiteral() );
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "Assumptions: [";
        for( unsigned int i = 0; i < assumptions.size(); i++ )
            cerr << " " << assumptions[ i ];
        cerr << " ]" << endl;
    });
}

Var
WeakInterface::addAuxVariable()
{
    solver.addVariableRuntime();
    while( inUnsatCore.size() <= solver.numberOfVariables() )
        inUnsatCore.push_back( 0 );
    return solver.numberOfVariables();        
}

void
WeakInterface::initInUnsatCore()
{
//    for( unsigned int i = 0; i <= solver.numberOfVariables(); i++ )
    while( inUnsatCore.size() <= solver.numberOfVariables() )
        inUnsatCore.push_back( numberOfCalls );
}

void
WeakInterface::preprocessingWeights()
{
    solver.sortOptimizationLiterals( level() );
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
        weights.push_back( solver.getOptimizationLiteral( level(), i ).weight );    
        
    vector< uint64_t >::iterator it = unique( weights.begin(), weights.end() );
    weights.erase( it, weights.end() );
}

void
WeakInterface::computeAssumptionsStratified()
{
    solver.sortOptimizationLiterals( level() );
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        if( solver.getOptimizationLiteral( level(), i ).isRemoved() )
            continue;
        if( solver.getOptimizationLiteral( level(), i ).weight >= this->weight )
            assumptions.push_back( solver.getOptimizationLiteral( level(), i ).lit.getOppositeLiteral() );        
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "Assumptions: [";
        for( unsigned int i = 0; i < assumptions.size(); i++ )
            cerr << " " << assumptions[ i ];
        cerr << " ]" << endl;
    });
}

bool
WeakInterface::changeWeight()
{
    if( weight == 0 )
        return false;
    uint64_t max = 0;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        if( solver.getOptimizationLiteral( level(), i ).isRemoved() )
            continue;
        
        uint64_t currentWeight = solver.getOptimizationLiteral( level(), i ).weight;
        if( currentWeight < weight && currentWeight <= ub() && currentWeight > max )
            max = currentWeight;        
    }

    weight = max;
    return weight != 0;    
}

#endif
