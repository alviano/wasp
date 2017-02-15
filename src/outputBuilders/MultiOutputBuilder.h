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

#ifndef WASP_MULTIOUTPUTBUILDER_H
#define WASP_MULTIOUTPUTBUILDER_H

#include "CompetitionOutputBuilder.h"
#include <sstream>
using namespace std;

class MultiOutputBuilder : public CompetitionOutputBuilder
{
    public:
        MultiOutputBuilder() : lastModel(""), optimum( false ) {}
        virtual void foundModelOptimization( const Vector< uint64_t >& costs );
        virtual void optimumFound();
        virtual void startModel();
        virtual void printVariable( Var v, bool );
        virtual void endModel();
        virtual void onProgramIncoherent();        
        virtual void onFinish();        
        virtual void onKill();
        virtual void foundLowerBound( uint64_t lb );
        
    private:
        string lastModel;
        stringstream modelStream;
        bool optimum;
        
        string lastWeight;        
};

#endif
