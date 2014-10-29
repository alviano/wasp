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
        WeakInterface( Solver& s ) : solver( s ), numberOfCalls( 0 ) {}
        virtual ~WeakInterface() {}
        virtual unsigned int run() = 0;
        
    protected:
        bool createFalseAggregate( const vector< Literal >& literals, const vector< unsigned int >& weights, unsigned int bound );
        Aggregate* createAggregate( Var aggrId, const vector< Literal >& literals, const vector< unsigned int >& weights );
        bool processAndAddAggregate( Aggregate* aggregate, unsigned int bound );        
        inline void computeAssumptionsAND();
        unsigned int computeMinWeight();
        inline Var addAuxVariable();
        inline bool visited( Var v, unsigned int value ) const { assert( v > 0 && v < inUnsatCore.size() ); return inUnsatCore[ v ] == value; }
        inline bool visited( Var v ) const { return visited( v, numberOfCalls ); }
        inline void visit( Var v ) { assert( v > 0 && v < inUnsatCore.size() ); inUnsatCore[ v ] = numberOfCalls; }
        inline void initInUnsatCore();        
        Solver& solver;
        vector< unsigned int > inUnsatCore;
        unsigned int numberOfCalls;
        vector< Literal > assumptionsAND;
        vector< Literal > assumptionsOR;        
};

void
WeakInterface::computeAssumptionsAND()
{    
    solver.sortOptimizationLiterals();
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
WeakInterface::addAuxVariable()
{
    solver.addVariableRuntime();
    inUnsatCore.push_back( 0 );
    return solver.numberOfVariables();        
}

void
WeakInterface::initInUnsatCore()
{
    for( unsigned int i = 0; i <= solver.numberOfVariables(); i++ )
        inUnsatCore.push_back( numberOfCalls );
}

#endif