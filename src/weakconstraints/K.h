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

#ifndef WASP_K_H
#define WASP_K_H

#include "WeakInterface.h"

class K : public WeakInterface
{
    public:
        inline K( Solver& s ) : WeakInterface( s ), kthreshold( wasp::Options::kthreshold ) {}
        virtual ~K();
        virtual unsigned int run();        
        unsigned int runWeighted();
        unsigned int runUnweighted();

    protected:
        bool processCoreK( uint64_t minWeight );
        bool addCardinalityConstraintK( vector< Literal >& literals, uint64_t bound );
        bool addAggregateK( vector< Literal >& literals, uint64_t bound );
        bool foundUnsat();
    
    private:
        int kthreshold;
        void addImplication( Literal l1, Literal l2 );
        void initCounters( int& b, int& m, int& N, unsigned int size );
};

#endif