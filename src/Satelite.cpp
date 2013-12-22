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

#include "Satelite.h"
#include "Solver.h"

// TODO: remove the suffix Malvi
bool
Satelite::isSubsumed(
    Clause* clause,
    Literal literal )
{
    for( unsigned i = 0; i < literal.numberOfOccurrences(); ++i )
    {
        Clause* current = literal.getOccurrence( i );

        trace_msg( satelite, 2, "Considering clause " << *current );
        if( clause != current && subset( current, clause ) )
            return true;
    }
    
    return false;
}

void
Satelite::findSubsumed(
    const Clause* clause )
{
    trace_msg( satelite, 1, "Starting subsumption for the clause " << *clause );
    Literal literal = clause->getLiteralWithMinOccurrences();

    trace_msg( satelite, 1, "Literal with the min number of occurrences is " << literal );
    literal.startIterationOverOccurrences();

    while( literal.hasNextOccurrence() )
    {
        Clause* current = literal.nextOccurence();

        trace_msg( satelite, 2, "Considering clause " << *current );
        if( clause != current && subset( clause, current ) )
        {
            trace_msg( satelite, 1, "Clause " << *clause << " subsumes clause " << *current );
            current->detachClauseToAllLiterals( literal );
            current->markAsDeleted();
        }
    }
}

void
Satelite::findSubsumedForSelfSubsumption(
    const Clause* clause,
    Literal literal )
{
    trace_msg( satelite, 1, "Starting self subsumption for the clause " << *clause << " and literal " << literal );
    literal.startIterationOverOccurrences();
    
    while( literal.hasNextOccurrence() )
    {
        Clause* current = literal.nextOccurence();
        
        trace_msg( satelite, 2, "Considering clause " << *current );
        if( clause != current && subset( clause, current ) )
        {
            trace_msg( satelite, 1, "Strengthening " << *current << " by removing literal " << literal );
            current->removeLiteral( literal );

            if( current->size() == 1 )
            {
                trueLiterals.push_back( current->getAt( 0 ) );
                current->detachClauseToAllLiterals( Literal::null );
                current->markAsDeleted();
            }
            else
            {
                this->onStrengtheningClause( current );
                trace_msg( satelite, 2, "Clause after removing literal is: " << *current );
            }
        }
    }
}

void
Satelite::selfSubsume(
    Clause* clause )
{    
    trace_msg( satelite, 2, "Self subsume. Clause: " << *clause );
    for( unsigned int i = 0; i < clause->size(); i++ )
    {
        trace_msg( satelite, 1, "Flipping literal " << clause->getAt( i ) );
        clause->flipLiteralAt( i );
        findSubsumedForSelfSubsumption( clause, clause->getAt( i ) );
        trace_msg( satelite, 1, "Restoring literal " << clause->getAt( i ) );
        clause->flipLiteralAt( i );
    }    
}

bool
Satelite::simplify()
{
    for( unsigned int i = 0; i < solver.numberOfClauses(); ++i )
    {
        Clause* current = solver.clauseAt( i );
        if( !current->hasBeenDeleted() )
            selfSubsume( current );
    }

    do
    {
        while( !trueLiterals.empty() )
        {
            Literal lit = trueLiterals.back();
            trueLiterals.pop_back();
            if( !lit.isTrue() )
            {
                if( !solver.propagateLiteralAsDeterministicConsequenceSatelite( lit ) )
                    return false;
            }
        }

        while( !strengthened.empty() )
        {
            Clause* clause = strengthened.back();            
            strengthened.pop_back();
            inserted.erase( clause );
            
            if( !clause->hasBeenDeleted() )
                selfSubsume( clause );
        }
        
        // TODO: check position of variable elimination and limit to "reasonable" variables!
        for( unsigned i = 1; i <= solver.numberOfVariables(); ++i )
        {
            tryToEliminate( solver.getVariable( i ) );
        }
    } while( !trueLiterals.empty() );
    
    for( unsigned int i = 0; i < solver.numberOfClauses(); ++i )
    {
        Clause* current = solver.clauseAt( i );
        if( !current->hasBeenDeleted() )
            findSubsumed( current );
    }

    return true;
}

bool
Satelite::tryToEliminate(
    Variable* variable )
{
    if( !variable->isUndefined() )
        return false;
    if( variable->numberOfOccurrences( POSITIVE ) >= 8 && variable->numberOfOccurrences( NEGATIVE ) >= 8 )
        return false;
    
    if( variable->numberOfOccurrences( POSITIVE ) >= 3 && variable->numberOfOccurrences( NEGATIVE ) >= 3 )
    {
        unsigned literals = 0;
        for( unsigned i = 0; i < variable->numberOfOccurrences( POSITIVE ); ++i )
            literals += variable->getOccurrence( i, POSITIVE )->size();
        for( unsigned i = 0; i < variable->numberOfOccurrences( NEGATIVE ); ++i )
            literals += variable->getOccurrence( i, NEGATIVE )->size();
            
        if( literals > 300 )
            return false;
    }
    
    if( tryToEliminateByDefinition( variable ) )
        return true;
    
    return false;
}

bool
Satelite::tryToEliminateByDefinition(
    Variable* variable,
    unsigned sign )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );

    Clause negatedDefinition;
    for( unsigned i = 0; i < variable->numberOfOccurrences( 1 - sign ); ++i )
    {
        Clause* clause = variable->getOccurrence( i, 1 - sign );
        if( clause->size() == 2 )
        {
            Literal literal = clause->getAt( 0 ).getVariable() == variable ? clause->getAt( 1 ).getOppositeLiteral() : clause->getAt( 0 ).getOppositeLiteral();
            negatedDefinition.addLiteral( literal );
        }
    }
    
    for( unsigned i = 0; i < negatedDefinition.size(); ++i )
    {
        if( isSubsumed( &negatedDefinition, negatedDefinition.getAt( i ) ) )
        {
            // TODO: propagate at level 0
            return true;
        }
    }
    
    negatedDefinition.addLiteral( Literal( variable, sign ) );
    
    Clause* result = NULL;
    for( unsigned i = 0; i < variable->numberOfOccurrences( sign ); ++i )
    {
        Clause* clause = variable->getOccurrence( i, sign );
        if( subset( clause, &negatedDefinition ) )
        {
            if( result == NULL || result->size() >= clause->size() )
                result = clause;
        }
    }
    
    if( result == NULL )
        return false;
    
    return tryToSubstitute( variable, sign, result );
}

bool
Satelite::tryToSubstitute(
    Variable* variable,
    unsigned sign,
    Clause* result )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );

    vector< Clause* > newClauses;
    
    for( unsigned i = 0; i < result->size(); ++i )
    {
        if( result->getAt( i ).getVariable() == variable )
            continue;
        
        for( unsigned j = 0; j < variable->numberOfOccurrences( sign ); ++j )
        {
            Clause* clause = variable->getOccurrence( j, sign );
            if( clause->contains( result->getAt( i ) ) )
                continue;

            Clause* newClause = new Clause();
            for( unsigned k = 0; k < clause->size(); ++k )
                newClause->addLiteral( clause->getAt( k ) == Literal( variable, sign ) ? result->getAt( i ).getOppositeLiteral() : clause->getAt( k ) );
            
            // TODO: remove duplicates and destroy if tautological
            newClauses.push_back( newClause );
        }
    }
    
    for( unsigned j = 0; j < variable->numberOfOccurrences( 1 - sign ); ++j )
    {
        Clause* clause = variable->getOccurrence( j, 1 - sign );
        if( clause->containsAnyComplementOf( result ) )
            continue;
            
        Clause* newClause = new Clause();
        for( unsigned k = 0; k < clause->size(); ++k )
        {
            if( clause->getAt( k ).getVariable() == variable )
            {
                for( unsigned i = 0; i < result->size(); ++i )
                {
                    if( result->getAt( i ).getVariable() != variable )
                        newClause->addLiteral( result->getAt( i ) );
                }
            }
            else
            {
                newClause->addLiteral( clause->getAt( k ) );
            }
        }
        
        // TODO: remove duplicates and destroy if tautological
        newClauses.push_back( newClause );
    }
    
    if( newClauses.size() > variable->numberOfOccurrences( POSITIVE ) + variable->numberOfOccurrences( NEGATIVE ) )
        return false;
    
    // TODO: substitute
    return true;
}

bool
Satelite::tryToEliminateByDefinition(
    Variable* variable )
{
    // FIXME: In satelite <= 2 is used. Check why.
    if( variable->numberOfOccurrences( POSITIVE ) <= 1 && variable->numberOfOccurrences( NEGATIVE ) <= 1 )
        return false;
    
    if( tryToEliminateByDefinition( variable, POSITIVE ) || tryToEliminateByDefinition( variable, NEGATIVE ) )
        return true;
    
    return false;
}