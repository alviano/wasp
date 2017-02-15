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

#ifndef WASP_OUTPUTBUILDER_H
#define WASP_OUTPUTBUILDER_H

#include "../util/WaspConstants.h"
#include "../stl/Vector.h"

#include <iostream>
using namespace std;

class OutputBuilder
{
    public:
        OutputBuilder() {}
        virtual ~OutputBuilder() {}
        virtual void startModel() = 0;
        virtual void printVariable( Var v, bool isTrue ) = 0;
        virtual void endModel() = 0;
        virtual void onProgramIncoherent() = 0;
        virtual void greetings(){}
        virtual void foundModelOptimization( const Vector< uint64_t >& costs );
        virtual void optimumFound();
        virtual void foundLowerBound( uint64_t );
        virtual void foundUpperBound( uint64_t );
        virtual void onFinish();
        virtual void onKill();
        virtual void foundModel(){}
    
};

#endif
