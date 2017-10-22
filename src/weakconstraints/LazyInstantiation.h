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

#ifndef WASP_LAZYINSTANTIATION_H
#define	WASP_LAZYINSTANTIATION_H

#include "WeakInterface.h"
#include "One.h"
#include "../AnswerSetListener.h"
using namespace std;

class Solver;

class LazyInstantiation : public One, public AnswerSetListener
{
    public:
        LazyInstantiation( Solver& s, ExternalPropagator* ext );
        virtual ~LazyInstantiation() {}
        
        virtual void foundAnswerSet() {}
        unsigned int run();
    private:
        inline LazyInstantiation( const LazyInstantiation& );
        ExternalPropagator* externalPropagator;
        bool handleAnswerSet();        
};

#endif

