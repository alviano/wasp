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
            solver.deleteClause( current );
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
    } while( !trueLiterals.empty() );
    
    for( unsigned int i = 0; i < solver.numberOfClauses(); )
    {
        Clause* current = solver.clauseAt( i );
        if( current->hasBeenDeleted() )
        {
            solver.deleteClause( current );
        }
        else
        {
            findSubsumed( current );
            ++i;
        }
    }

    return true;
}