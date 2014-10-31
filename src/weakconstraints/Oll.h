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

#ifndef OLL_H
#define OLL_H

#include "WeakInterface.h"
#include <unordered_map>
using namespace std;

class OllData
{
    public:    
        inline OllData( unsigned int bound, unsigned int guard, unsigned int weight, vector< Literal >& literals, vector< unsigned int >& weights )        
        {
            bound_ = bound;
            guardId_ = guard;
            weight_ = weight;
            literals_.swap( literals );
            weights_.swap( weights );
        }
        
        unsigned int bound_;
        unsigned int guardId_;
        unsigned int weight_;
        vector< Literal > literals_;
        vector< unsigned int > weights_;        
};

class Oll : public WeakInterface
{
    public:
        inline Oll( Solver& s ) : WeakInterface( s ) {}
        virtual ~Oll() {}
        virtual unsigned int run();

    protected:
        bool processCoreOll( vector< Literal >& literals, vector< unsigned int >& weights, unsigned int minWeight );
        bool addAggregateOll( vector< Literal >& literals, vector< unsigned int >& weights, unsigned int bound, unsigned int weightOfOptimizationLiteral );
        inline Var addBinaryClauseForAggregateOll( Var aggrId, unsigned int weightOfOptimizationLiteral );
        unordered_map< Var, OllData* > guardMap;
};

Var
Oll::addBinaryClauseForAggregateOll(
    Var aggrId,
    unsigned int weightOfOptimizationLiteral )
{    
    Literal lit( addAuxVariable(), POSITIVE );
    solver.addOptimizationLiteral( lit, weightOfOptimizationLiteral, UINT_MAX, true );    
    solver.addClause( lit, Literal( aggrId, NEGATIVE ) );
    
    assert( !solver.isFalse( aggrId ) );
    if( solver.isTrue( aggrId ) )
    {
        #ifndef NDEBUG
        bool res =
        #endif
        solver.addClauseRuntime( lit );
        assert( res );
    }
    
    return lit.getVariable();
}


#endif