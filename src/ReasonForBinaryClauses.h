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

#ifndef WASP_REASONFORBINARYCLAUSES_H
#define WASP_REASONFORBINARYCLAUSES_H

#include <iostream>
using namespace std;
#include "util/WaspConstants.h"

#include "Reason.h"

class ReasonForBinaryClauses : public Reason
{
    public:        
        inline ReasonForBinaryClauses( Var v ) : varId( v ) {}
        virtual ~ReasonForBinaryClauses();
        virtual void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        virtual bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );        
        virtual ostream& print( ostream& o ) const;
        void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& toVisit, unsigned int numberOfCalls, Literal lit );
        
    private:
        Var varId;
        
};

#endif
