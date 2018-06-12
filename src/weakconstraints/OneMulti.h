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

#ifndef WASP_ONEMULTI_H
#define WASP_ONEMULTI_H

#include "WeakInterface.h"

class OneMulti : public WeakInterface
{
    public:
        inline OneMulti( Solver& s ) : WeakInterface( s ) {}
        virtual ~OneMulti();
        virtual unsigned int run();        
        unsigned int runWeighted();
        unsigned int runUnweighted();

    protected:
        bool processCoreOne( vector< Literal >& literals, vector< uint64_t >& weights, vector< Var >& newVars, uint64_t minWeight, unsigned int& n );
        bool addAggregateOne( vector< Literal >& literals, vector< uint64_t >& weights, vector< Var >& bounds );
        
        bool processCoreOne( vector< Literal >& literals, vector< uint64_t >& weights, uint64_t minWeight, unsigned int& n );
        bool addAggregateOne( vector< Literal >& literals, vector< uint64_t >& weights, uint64_t bound );
        
        bool foundUnsat();
};

#endif