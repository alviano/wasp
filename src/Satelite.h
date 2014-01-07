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
#include "util/Trace.h"
#include "util/Options.h"
#include "Clause.h"
#include "stl/UnorderedSet.h"

class Satelite
{
    public:
        inline Satelite( Solver& s );
        inline ~Satelite();
        bool simplify();
        inline void onStrengtheningClause( Clause* clause );
        inline void onAddingVariable( Variable* var ){ touched.push_back( var ); }
        inline void onDeletingClause( Clause* clause );        
        
    private:
        inline void onAddingClause( Clause* clause );
        inline bool subset( const Clause* c1, const Clause* c2 );
        bool isSubsumed( Clause* clause, Literal literal );
        void findSubsumed( const Clause* clause );
        void findSubsumedForSelfSubsumption( const Clause* clause, Literal literal );
        void selfSubsume( Clause* clause );
        bool tryToEliminate( Variable* variable );
        bool tryToEliminateByDefinition( Variable* variable );
        bool tryToEliminateByDefinition( Variable* variable, unsigned sign );
        bool tryToSubstitute( Variable* variable, unsigned sign, Clause* result );
        
        inline void touchVariablesInClause( Clause* clause );
        
        bool propagateTopLevel();        

        Solver& solver;
        vector< Clause* > strengthened;
        unordered_set< Clause* > insertedInStrengthened;
        
        vector< Literal > trueLiterals;
        vector< Variable* > touched;
        vector< Clause* > added;        
        
        unordered_set< Variable* > insertedInTouched;
        bool ok;
        
        bool active;
};

Satelite::Satelite(
    Solver& s ) : solver( s ), ok( true ), active( false )
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
    if( active )
    {
        assert( clause != NULL );
        if( insertedInStrengthened.insert( clause ).second )
        {
            trace_msg( satelite, 3, "Strengthening clause " << *clause );
            strengthened.push_back( clause );
            touchVariablesInClause( clause );
        }
    }
}

void
Satelite::onDeletingClause(
    Clause* clause )
{
    if( active )
    {
        trace_msg( satelite, 3, "Deleting clause " << *clause );
        touchVariablesInClause( clause );
    }
}

void
Satelite::touchVariablesInClause(
    Clause* clause )
{
    assert( !clause->hasBeenDeleted() );

    for( unsigned int j = 0; j < clause->size(); j++ )
    {
        Variable* variable = clause->getAt( j ).getVariable();
        
        if( variable->isUndefined() && variable->numberOfOccurrences() > 0 )
        {
            if( insertedInTouched.insert( variable ).second )
                touched.push_back( variable );
        }
    }
}

void
Satelite::onAddingClause(
    Clause* clause )
{
    assert( active );
    trace_msg( satelite, 3, "Adding clause " << *clause );
    added.push_back( clause );
    touchVariablesInClause( clause );
}

#endif
