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

#ifndef WASP_SATELITE_H
#define WASP_SATELITE_H

#include <iostream>
#include <queue>
using namespace std;

#include "util/WaspAssert.h"
#include "util/WaspTrace.h"
#include "util/WaspOptions.h"
#include "Clause.h"
#include "stl/UnorderedSet.h"
#include "stl/Heap.h"

class Satelite;

struct EliminationComparator
{
    const Satelite& satelite;
    inline bool operator()( Var x, Var y ) const;
    EliminationComparator( const Satelite& s ) : satelite( s ) {}
};

class Satelite
{
    public:
        inline Satelite( Solver& s );
        inline ~Satelite();
        inline bool simplify();
//        bool simplificationsSatelite();
        bool simplificationsMinisat2();
        inline void onStrengtheningClause( Clause* clause );
//        inline void onAddingVariable( Variable* var );
        
        inline void onDeletingClause( Clause* clause );

        inline unsigned getSubsumptionLimit() const { return subsumptionLimit; }
        
        inline void addTrueLiteral( Literal l ) { trueLiterals.push_back( l ); }
        unsigned int cost( Var v ) const;
        
        inline void disableVariableElimination() { enabledVariableElimination = false; }
        
    private:
        inline void onAddingClause( Clause* clause );
        inline bool subset( const Clause* c1, const Clause* c2 );
        bool isSubsumed( Clause* clause, Literal literal );
//        void findSubsumed( const Clause* clause );
//        void findSubsumedForSelfSubsumption( const Clause* clause, Literal literal );
//        void selfSubsume( Clause* clause );
        bool tryToEliminate( Var variable );
        bool tryToEliminateByDefinition( Var variable );
        bool tryToEliminateByDefinition( Literal lit );
        bool tryToSubstitute( Literal lit, Clause* result );
        bool tryToEliminateByDistribution( Var variable );
        void substitute( Var variable, vector< Clause* >& newClauses );        
        
        inline bool eliminateVariable( Var variable );
        bool backwardSubsumptionCheck();
        
        void touchVariablesInClause( Clause* clause );
        void gatherTouchedClauses();
        void gatherTouchedClauses( Var variable );
        queue< Clause* > subsumptionQueue;
        
        bool propagateTopLevel();        
        inline void addClauseInSubsumptionQueue( Clause* clause );
        void checkSubsumptionForClause( Clause* clause, Literal bestLiteral );        

        Solver& solver;
//        vector< Clause* > strengthened;
//        unordered_set< Clause* > insertedInStrengthened;
        
        vector< bool > touchedVariables;
        vector< Literal > trueLiterals;
//        vector< Variable* > touched;
//        vector< Clause* > added;        
//        
//        unordered_set< Variable* > insertedInTouched;
        bool ok;
        bool active;
        unsigned int numberOfTouched;        
        unsigned int clauseLimit; //A variable is not eliminated if it produces a resolvent with a length above this limit.
        unsigned int subsumptionLimit; //Do not check if subsumption against a clause larger than this.        
        bool enabledVariableElimination;
        Heap< EliminationComparator > elim_heap;
};

Satelite::Satelite(
    Solver& s ) : solver( s ), ok( true ), active( false ), 
                  numberOfTouched( 0 ), clauseLimit( 0 ), 
                  subsumptionLimit( 1000 ), enabledVariableElimination( true ),
                  elim_heap( EliminationComparator( *this ) )
{
    touchedVariables.push_back( false );
}

Satelite::~Satelite()
{
}

bool EliminationComparator::operator()( Var x, Var y ) const { return satelite.cost( x ) < satelite.cost( y ); }    

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
//        assert( clause != NULL );
//        if( insertedInStrengthened.insert( clause ).second )
//        {
//            trace_msg( satelite, 3, "Strengthening clause " << *clause );
//            strengthened.push_back( clause );
//            touchVariablesInClause( clause );
//        }
        assert( clause != NULL );
        trace_msg( satelite, 3, "Strengthening clause " << *clause );
        
        addClauseInSubsumptionQueue( clause );
        touchVariablesInClause( clause );
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
Satelite::onAddingClause(
    Clause* clause )
{
    assert( active );
    trace_msg( satelite, 3, "Adding clause " << *clause );
    //added.push_back( clause );
    addClauseInSubsumptionQueue( clause );
    touchVariablesInClause( clause );
}

bool
Satelite::eliminateVariable(
    Var variable )
{
    if( tryToEliminate( variable ) )//tryToEliminateByDistribution( variable ) )
    {
        if( !ok )
            return false;
        return backwardSubsumptionCheck();        
    }
    
    return true;
}

//void
//Satelite::onAddingVariable( 
//    Variable* variable )
//{ 
//    assert( touchedVariables.size() == variable->getId() );
//    touchedVariables.push_back( true );
//    numberOfTouched++;
//    elim_heap.pushNoCheck( variable );
//}

void
Satelite::addClauseInSubsumptionQueue(
    Clause* clause )
{
    assert( clause != NULL && !clause->hasBeenDeleted() );
    
    if( !clause->isInQueue() )
    {
        clause->setInQueue();
        subsumptionQueue.push( clause );
    }
}

bool
Satelite::simplify()
{
    if( !wasp::Options::simplifications )
        return true;
    active = true;
    bool result = simplificationsMinisat2();
    active = false;
    
    return result;
}

#endif
