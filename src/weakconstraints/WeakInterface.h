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

#ifndef WEAKINTERFACE_H
#define WEAKINTERFACE_H

#include <vector>
#include <iostream>
using namespace std;
#include "../util/Trace.h"
#include "../util/Assert.h"
#include "../Literal.h"
#include "../Solver.h"

class WeakInterface
{
    public:
        WeakInterface( Solver& s ) : solver( s ), numberOfCalls( 0 ), lb( 0 ), ub( UINT64_MAX ), disjCoresPreprocessing( false ), stratification( true ), weight( UINT64_MAX ) {}
        virtual ~WeakInterface() {}
        virtual unsigned int run() = 0;
        
        inline void setDisjCoresPreprocessing( bool value ) { disjCoresPreprocessing = value; }
        inline void setLowerBound( unsigned int value ) { lb = value; }
        inline void setStratification( bool value ) { stratification = value; }
        
    protected:
        bool disjointCorePreprocessing();
        bool createFalseAggregate( const vector< Literal >& literals, const vector< uint64_t >& weights, uint64_t bound );
        Aggregate* createAndReturnFalseAggregate( const vector< Literal >& literals, const vector< uint64_t >& weights, uint64_t bound );
        Aggregate* createAggregate( Var aggrId, const vector< Literal >& literals, const vector< uint64_t >& weights );
        bool processAndAddAggregate( Aggregate* aggregate, uint64_t bound );        
        inline void computeAssumptionsAND();
        uint64_t computeMinWeight();
        inline Var addAuxVariable();
        inline bool visited( Var v, unsigned int value ) const { assert( v > 0 && v < inUnsatCore.size() ); return inUnsatCore[ v ] == value; }
        inline bool visited( Var v ) const { return visited( v, numberOfCalls ); }
        inline void visit( Var v ) { assert( v > 0 && v < inUnsatCore.size() ); inUnsatCore[ v ] = numberOfCalls; }
        inline void initInUnsatCore();
        inline void preprocessingWeights();
        inline void computeAssumptionsANDStratified();
        inline bool changeWeight();
        
        virtual bool foundUnsat() { return true; }

        Solver& solver;
        vector< unsigned int > inUnsatCore;
        unsigned int numberOfCalls;
        vector< Literal > assumptions;
        
        uint64_t lb;
        uint64_t ub;
        
        bool disjCoresPreprocessing;
        bool stratification;

    private:
        vector< uint64_t > weights;        
        uint64_t weight;        
        
        inline void computeAssumptionsANDOnlyOriginal( unsigned int originalNumberOfOptLiterals );
};

void
WeakInterface::computeAssumptionsAND()
{    
    solver.sortOptimizationLiterals();
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        if( solver.getOptimizationLiteral( i ).isRemoved() )
            continue;
        assumptions.push_back( solver.getOptimizationLiteral( i ).lit.getOppositeLiteral() );
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "AssumptionsAND: [";
        for( unsigned int i = 0; i < assumptions.size(); i++ )
            cerr << " " << assumptions[ i ];
        cerr << " ]" << endl;
    });
}

void
WeakInterface::computeAssumptionsANDOnlyOriginal(
    unsigned int originalNumberOfOptLiterals )
{    
    solver.sortOptimizationLiterals();
    for( unsigned int i = 0; i < originalNumberOfOptLiterals; i++ )
    {
        if( solver.getOptimizationLiteral( i ).isRemoved() )
            continue;
        assumptions.push_back( solver.getOptimizationLiteral( i ).lit.getOppositeLiteral() );
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "AssumptionsAND: [";
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
    solver.sortOptimizationLiterals();
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
        weights.push_back( solver.getOptimizationLiteral( i ).weight );    
        
    vector< uint64_t >::iterator it = unique( weights.begin(), weights.end() );
    weights.erase( it, weights.end() );
}

void
WeakInterface::computeAssumptionsANDStratified()
{
    solver.sortOptimizationLiterals();
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        if( solver.getOptimizationLiteral( i ).isRemoved() )
            continue;
        if( solver.getOptimizationLiteral( i ).weight >= this->weight )
            assumptions.push_back( solver.getOptimizationLiteral( i ).lit.getOppositeLiteral() );        
    }
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "AssumptionsAND: [";
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
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        if( solver.getOptimizationLiteral( i ).isRemoved() )
            continue;
        
        uint64_t currentWeight = solver.getOptimizationLiteral( i ).weight;
        if( currentWeight < weight && currentWeight <= ub && currentWeight > max )
            max = currentWeight;        
    }

    weight = max;
    return weight != 0;    
}

#endif