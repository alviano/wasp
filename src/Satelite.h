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

#ifndef SATELITE_H
#define SATELITE_H

#include <iostream>
using namespace std;

#include "util/Assert.h"
#include "Clause.h"

class Satelite
{
    public:
        inline Satelite( Solver& s );
        inline ~Satelite();
        bool simplify();
        inline void onStrengtheningClause( Clause* clause );

    private:
        inline bool subset( const Clause* c1, const Clause* c2 );
        bool isSubsumed( Clause* clause, Literal literal );
        void findSubsumed( const Clause* clause );
        void findSubsumedForSelfSubsumption( const Clause* clause, Literal literal );
        void selfSubsume( Clause* clause );
        bool tryToEliminate( Variable* variable );
        bool tryToEliminateByDefinition( Variable* variable );
        bool tryToEliminateByDefinition( Variable* variable, unsigned sign );
        bool tryToSubstitute( Variable* variable, unsigned sign, Clause* result );

        Solver& solver;
        vector< Clause* > strengthened;
        unordered_set< Clause* > inserted;
        
        vector< Literal > trueLiterals;        
};

Satelite::Satelite(
    Solver& s ) : solver( s )
{
}

Satelite::~Satelite()
{
}

bool
Satelite::subset(
    const Clause* c1,
    const Clause* c2 )
{
    if( c1->size() > c2->size() )
        return false;
    
    if( ( c1->getSignature() & ~c2->getSignature() ) != 0 )
        return false;
    
    return c1->isSubsetOf( c2 );
}

void
Satelite::onStrengtheningClause(
    Clause* clause )
{
    assert( clause != NULL );
    if( inserted.insert( clause ).second )
        strengthened.push_back( clause );
}


#endif
