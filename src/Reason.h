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

#ifndef WASP_REASON_H
#define WASP_REASON_H

#include <iostream>
#include <vector>
using namespace std;

class Learning;
class Literal;
class Solver;

class Reason
{
    friend ostream& operator<<( ostream& o, const Reason& r )
    {
        return r.print( o );
    }
    
    public:
        virtual ~Reason() {}
        virtual void onLearning( const Solver& solver, Learning* strategy, Literal lit ) = 0;
        virtual bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit ) = 0;
        virtual bool isLearned() const { return false; }
        virtual ostream& print( ostream& o ) const = 0;
        virtual void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit ) = 0;
};

#endif