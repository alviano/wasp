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

#ifndef WASP_OPT_H
#define WASP_OPT_H

#include "WeakInterface.h"

class Opt : public WeakInterface
{
    public:
        Opt( Solver& s, bool disable = false ) : WeakInterface( s ), disableprefchoices_( disable ), aggregate( NULL ), varId( 0 ) {}
        unsigned int run();
        bool updateOptimizationAggregate( uint64_t modelCost );
        bool completedLevel();
        Literal getAssumptionToAdd() { return varId == 0 ? Literal::null : Literal( varId, NEGATIVE ); }
        inline void resetVarId() { varId = 0; }
        inline void createVarId() { assert( varId == 0 ); varId = addAuxVariable(); }
        
    private:
        bool disableprefchoices_;
        Aggregate* aggregate;
        Var varId;
        unsigned int numberOfModels;
        void createOptimizationAggregate( uint64_t modelCost );
        bool foundModel();
        void flipLatestChoice( vector< bool >& checked );
        void backjump();
        void basic();
        void basicBT();
};

#endif