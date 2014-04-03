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
//
//void
//Satelite::findSubsumed(
//    const Clause* clause )
//{
//    trace_msg( satelite, 1, "Starting subsumption for the clause " << *clause );
//    Literal literal = clause->getLiteralWithMinOccurrences();
//
//    trace_msg( satelite, 1, "Literal with the min number of occurrences is " << literal );
//    literal.startIterationOverOccurrences();
//
//    while( literal.hasNextOccurrence() )
//    {
//        Clause* current = literal.nextOccurence();
//
//        trace_msg( satelite, 2, "Considering clause " << *current );
//        if( clause != current && subset( clause, current ) )
//        {
//            trace_msg( satelite, 1, "Clause " << *clause << " subsumes clause " << *current );
//            current->detachClauseToAllLiterals( literal );
//            solver.markClauseForDeletion( current );
//        }
//    }
//}
//
//void
//Satelite::findSubsumedForSelfSubsumption(
//    const Clause* clause,
//    Literal literal )
//{
//    trace_msg( satelite, 1, "Starting self subsumption for the clause " << *clause << " and literal " << literal );
//    literal.startIterationOverOccurrences();
//    
//    while( literal.hasNextOccurrence() )
//    {
//        Clause* current = literal.nextOccurence();
//        
//        trace_msg( satelite, 2, "Considering clause " << *current );
//        if( clause != current && subset( clause, current ) )
//        {
//            trace_msg( satelite, 1, "Strengthening " << *current << " by removing literal " << literal );
//            current->removeLiteral( literal );
//
//            if( current->size() == 1 )
//            {
//                trueLiterals.push_back( current->getAt( 0 ) );
//                current->detachClauseToAllLiterals( Literal::null );
//                solver.markClauseForDeletion( current );
//            }
//            else
//            {
//                this->onStrengtheningClause( current );
//                trace_msg( satelite, 2, "Clause after removing literal is: " << *current );
//            }
//        }
//    }
//}
//
//void
//Satelite::selfSubsume(
//    Clause* clause )
//{    
//    trace_msg( satelite, 2, "Self subsume. Clause: " << *clause );
//    for( unsigned int i = 0; i < clause->size(); i++ )
//    {
//        trace_msg( satelite, 1, "Flipping literal " << clause->getAt( i ) );
//        clause->flipLiteralAt( i );
//        findSubsumedForSelfSubsumption( clause, clause->getAt( i ) );
//        trace_msg( satelite, 1, "Restoring literal " << clause->getAt( i ) );
//        clause->flipLiteralAt( i );
//    }
//}
//
//bool
//Satelite::simplificationsSatelite()
//{
//    added.reserve( solver.numberOfClauses() );
//    
//    for( unsigned int i = 0; i < solver.numberOfClauses(); ++i )
//    {
//        Clause* current = solver.clauseAt( i );
//        assert( !current->hasBeenDeleted() );        
//        added.push_back( current );
//    }
//    
//    do
//    {
//        vector< bool > seenLiterals( 2 * solver.numberOfVariables(), false );
//
//        vector< Clause* > S0; //From minisat paper
//        vector< Clause* > S1;
//        unordered_set< Clause* > insertedInS0;
//        unordered_set< Clause* > insertedInS1;
//        
//        for( unsigned int i = 0; i < added.size(); i++ )
//        {
//            Clause* c = added[ i ];
//            
//            if( c->hasBeenDeleted() )
//                continue;
//
//            for( unsigned int j = 0; j < c->size(); j++ )
//            {
//                Literal lit = c->getAt( j );
//                assert_msg( lit.getVariable()->getId() > 0, "Literal " << lit << " has id " << lit.getVariable()->getId() );
//                unsigned int litId = 2 * ( lit.getVariable()->getId() ) - lit.getSign() - 1;
//                assert_msg( litId < seenLiterals.size(), "Literal id is " << litId << " while the total number of literals is " << seenLiterals.size() );
//                if( seenLiterals[ litId ] )
//                    continue;
//
//                seenLiterals[ litId ] = true;
//
//                for( unsigned int k = 0; k < lit.numberOfOccurrences(); k++ )
//                {
//                    Clause* current = lit.getOccurrence( k );
//                    assert( !current->hasBeenDeleted() );
//                    if( insertedInS0.insert( current ).second )
//                    {
//                        S0.push_back( current );
//                    }
//                }
//
//                Literal oppositeLiteral = lit.getOppositeLiteral();
//                for( unsigned int k = 0; k < oppositeLiteral.numberOfOccurrences(); k++ )
//                {
//                    Clause* current = oppositeLiteral.getOccurrence( k );
//                    assert( !current->hasBeenDeleted() );
//                    if( insertedInS1.insert( current ).second )
//                    {
//                        S1.push_back( current );
//                    }
//                }
//            }
//
//            if( insertedInS1.insert( c ).second )
//                S1.push_back( c );
//
//        }
//        added.clear();
//
//        for( unsigned int i = 0; i < S1.size(); i++ )
//        {
//            Clause* current = S1[ i ];
//            if( !current->hasBeenDeleted() )
//                selfSubsume( current );
//        }
//
//        if( !propagateTopLevel() )
//            return false;        
//
//        while( !strengthened.empty() )
//        {
//            vector< Clause* > strengthenedCopy; 
//            strengthened.swap( strengthenedCopy );
//            assert( strengthened.empty() );
//            insertedInStrengthened.clear();
//
//            for( unsigned int i = 0; i < strengthenedCopy.size(); i++ )
//            {
//                Clause* current = strengthenedCopy[ i ];
//                if( current->hasBeenDeleted() )
//                    continue;
//
//                if( insertedInS1.find( current ) == insertedInS1.end() )
//                    selfSubsume( current );
//            }
//
//            if( !propagateTopLevel() )
//                return false;
//        }
//        
//        for( unsigned int i = 0; i < S0.size(); ++i )
//        {
//            Clause* current = S0[ i ];
//            if( !current->hasBeenDeleted() )
//                findSubsumed( current );
//        }        
//        
//        vector< Variable* > touchedCopy;
//        do
//        {
//            touchedCopy.clear();
//            touched.swap( touchedCopy );        
//            insertedInTouched.clear();
//            assert( touched.empty() );
//            
//            for( unsigned int i = 0; i < touchedCopy.size(); i++ )
//            {
//                tryToEliminate( touchedCopy[ i ] );
//                if( !ok )
//                    return false;
//            }
//        } while( !touched.empty() );        
//
//    } while( !added.empty() );    
//    
//    return true;
//}
//
bool
Satelite::tryToEliminate(
    Variable* variable )
{
    trace_msg( satelite, 2, "Trying to eliminate variable " << *variable );
    if( !variable->isUndefined() )
        return false;
    
    if( variable->cost() > wasp::Options::maxCost )
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
//    else
//        return tryToEliminateByDistribution( variable );    
        
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
            // DONE: propagate at level 0
            //Propagate the opposite literal
            Literal literal( variable, 1 - sign );
            ok = solver.propagateLiteralAsDeterministicConsequenceSatelite( literal );
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
    
    Clause* resultCopy = new Clause();
    
    for( unsigned int i = 0; i < result->size(); i++ )
    {
        Literal lit = result->getAt( i );
        if( lit.getVariable() != variable )
        {
            resultCopy->addLiteral( lit );
        }
    }    
    
    return tryToSubstitute( variable, sign, resultCopy );
}

bool
Satelite::tryToSubstitute(
    Variable* variable,
    unsigned sign,
    Clause* definition )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );

    assert( trueLiterals.empty() );
    trace_msg( satelite, 5, "Trying to substitute " << *variable << ( sign == POSITIVE ? " positive" : " negative" ) << ". Its definition is " << *definition );
    vector< Clause* > newClauses;
    
    for( unsigned i = 0; i < definition->size(); ++i )
    {
//        if( definition->getAt( i ).getVariable() == variable )
//            continue;

        for( unsigned j = 0; j < variable->numberOfOccurrences( sign ); ++j )
        {            
            Clause* clause = variable->getOccurrence( j, sign );
            if( clause->contains( definition->getAt( i ) ) )
                continue;

            Clause* newClause = new Clause();
            for( unsigned k = 0; k < clause->size(); ++k )
                newClause->addLiteral( clause->getAt( k ) == Literal( variable, sign ) ? definition->getAt( i ).getOppositeLiteral() : clause->getAt( k ) );
            
            // DONE: remove duplicates and destroy if tautological
            #ifndef NDEBUG
            bool tautological =
            #endif
            newClause->removeDuplicatesAndCheckIfTautological();
            
            assert_msg( !tautological, "Clause " << *newClause << " is tautological" );
            if( newClause->size() == 1 )
            {
                trueLiterals.push_back( newClause->getAt( 0 ) );
                delete newClause;
            }
            else
                newClauses.push_back( newClause );      
        }
    }
    
    for( unsigned j = 0; j < variable->numberOfOccurrences( 1 - sign ); ++j )
    {
        Clause* clause = variable->getOccurrence( j, 1 - sign );
         
        if( clause->containsAnyComplementOf( definition ) )        
            continue;

        Clause* newClause = new Clause();
        for( unsigned k = 0; k < clause->size(); ++k )
        {
            if( clause->getAt( k ).getVariable() == variable )
            {
                for( unsigned i = 0; i < definition->size(); ++i )
                {
                    if( definition->getAt( i ).getVariable() != variable )
                        newClause->addLiteral( definition->getAt( i ) );
                }
            }
            else
            {
                newClause->addLiteral( clause->getAt( k ) );
            }
        }
        
        // DONE: remove duplicates and destroy if tautological
        if( newClause->removeDuplicatesAndCheckIfTautological() )    
            delete newClause;        
        else
        {
            if( newClause->size() == 1 )
            {
                trueLiterals.push_back( newClause->getAt( 0 ) );
                delete newClause;
            }
            else
                newClauses.push_back( newClause );
        }
    }

    if( newClauses.size() > variable->numberOfOccurrences( POSITIVE ) + variable->numberOfOccurrences( NEGATIVE ) )
    {
        delete definition;
        for( unsigned int i = 0; i < newClauses.size(); ++i )
        {
            delete newClauses[ i ];
        }
        trueLiterals.clear();
        return false;
    }
    
    substitute( variable, newClauses );
    
    assert_msg( variable->numberOfOccurrences() == 0, "Variable " << *variable << " has been eliminated but has still " << variable->numberOfOccurrences() << " occurrences" );
    trace_msg( satelite, 2, "Eliminated variable " << *variable );   
    
    ok = propagateTopLevel();
    
    solver.onEliminatingVariable( variable, sign, definition );
    return true;
}

bool
Satelite::tryToEliminateByDefinition(
    Variable* variable )
{
    if( variable->numberOfOccurrences( POSITIVE ) <= 1 && variable->numberOfOccurrences( NEGATIVE ) <= 1 )
        return false;
    
    if( tryToEliminateByDefinition( variable, POSITIVE ) || tryToEliminateByDefinition( variable, NEGATIVE ) )
        return true;
    
    return false;
}

bool
Satelite::propagateTopLevel()
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
    
    return true;
}

bool
Satelite::tryToEliminateByDistribution( 
    Variable* variable )
{
    trace_msg( satelite, 5, "Trying to eliminate " << *variable << " by distribution" );

    vector< Clause* > newClauses;
    for( unsigned int i = 0; i < variable->numberOfOccurrences( POSITIVE ); i++ )
    {
        for( unsigned int j = 0; j < variable->numberOfOccurrences( NEGATIVE ); j++ )
        {
            Clause* newClause = new Clause();            
            Clause* firstClause = variable->getOccurrence( i, POSITIVE );
            Clause* secondClause = variable->getOccurrence( j, NEGATIVE );

            bool tautological = false;
            for( unsigned int k = 0; k < firstClause->size(); k++ )
            {
                Literal lit = firstClause->getAt( k );
                if( lit.getVariable() != variable )
                    newClause->addLiteral( lit );
            }
                        
            for( unsigned int k = 0; k < secondClause->size(); k++ )
            {
                Literal lit = secondClause->getAt( k );
                if( lit.getVariable() == variable )
                    continue;

                if( newClause->contains( lit ) )
                    continue;
                else if( newClause->contains( lit.getOppositeLiteral() ) )
                {
                    tautological = true;
                    break;
                }
                else
                {
                    newClause->addLiteral( lit );
                }
            }

            //The new clause is tautological
            if( tautological ) 
                delete newClause;                
            else if( newClause->size() == 1 )
            {
                trueLiterals.push_back( newClause->getAt( 0 ) );
                delete newClause;
            }
            else
            {
                newClauses.push_back( newClause );
                
                if( newClauses.size() > variable->numberOfOccurrences() || newClause->size() > clauseLimit )
                {
                    for( unsigned int i = 0; i < newClauses.size(); ++i )
                        delete newClauses[ i ];
                    trueLiterals.clear();
                    return false;
                }
            }
        }
    }

    variable->removeAllClauses( solver );
    
    for( unsigned int i = 0; i < newClauses.size(); i++ )
    {
        Clause* c = newClauses[ i ];
        assert( c->size() >= 2 );        
        onAddingClause( c );
        solver.addClause( c );        
    }
    
    trace_msg( satelite, 2, "Eliminated variable " << *variable );   
    
    solver.onEliminatingVariable( variable, ELIMINATED_BY_DISTRIBUTION, NULL );
    ok = propagateTopLevel();    
    return true;
}

void
Satelite::substitute(
    Variable* variable,
    vector< Clause* >& newClauses )
{
    variable->markAllClauses( solver );
    
    for( unsigned int i = 0; i < newClauses.size(); i++ )
    {
        Clause* c = newClauses[ i ];
        assert( c->size() >= 2 );        
        onAddingClause( c );
        solver.addClause( c );        
    }
}

bool
Satelite::simplificationsMinisat2()
{
    assert( solver.callSimplifications() );
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        assert( touchedVariables.size() == i );
        touchedVariables.push_back( true );
        numberOfTouched++;
        elim_heap.pushNoCheck( solver.getVariable( i ) );
    }
    
    assert( numberOfTouched != 0 );
    
    while( numberOfTouched > 0 && elim_heap.size() > 0 )
    {
        gatherTouchedClauses();
        numberOfTouched = 0;

        if( !backwardSubsumptionCheck() )
            return false;
        
        ok = propagateTopLevel();
        if( !ok )
            return false;
        
        while( !elim_heap.empty() )
        {
            Variable* v = elim_heap.removeMin();
            if( !v->isUndefined() || v->hasBeenEliminated() || v->isFrozen() )//!solver.tight() )
                continue;
            
            if( !eliminateVariable( v ) )
                return false;
        }
    }

    return true;
}

bool
Satelite::backwardSubsumptionCheck()
{
    while( !subsumptionQueue.empty() )
    {
        Clause* clause = subsumptionQueue.front();
        clause->resetInQueue();
        subsumptionQueue.pop();

        if( clause->hasBeenDeleted() )
            continue;

        trace_msg( satelite, 1, "Starting subsumption for the clause " << *clause );
        Variable* variable = clause->getVariableWithMinOccurrences();
        
        assert( variable != NULL );
        trace_msg( satelite, 1, "Variable with the min number of occurrences is " << *variable );
        
        Literal literal( variable, POSITIVE );
        checkSubsumptionForClause( clause, literal );
        checkSubsumptionForClause( clause, literal.getOppositeLiteral() );
        
        ok = propagateTopLevel();
        if( !ok )
            return false;
    }

    return true;
}

void
Satelite::gatherTouchedClauses(
    Variable* variable )
{    
    for( unsigned int i = 0; i < variable->numberOfOccurrences( POSITIVE ); i++ )
    {
        Clause* clause = variable->getOccurrence( i, POSITIVE );
        assert( clause != NULL && !clause->hasBeenDeleted() );
        addClauseInSubsumptionQueue( clause );
    }
    
    for( unsigned int i = 0; i < variable->numberOfOccurrences( NEGATIVE ); i++ )
    {
        Clause* clause = variable->getOccurrence( i, NEGATIVE );
        assert( clause != NULL && !clause->hasBeenDeleted() );
        addClauseInSubsumptionQueue( clause );
    }
}

void
Satelite::gatherTouchedClauses()
{
    for( unsigned int i = 1; i < touchedVariables.size(); i++ )
    {
        if( touchedVariables[ i ] )
        {
            gatherTouchedClauses( solver.getVariable( i ) );
            touchedVariables[ i ] = false;
        }
    }
}

void
Satelite::checkSubsumptionForClause(
    Clause* clause,
    Literal bestLiteral )
{
    bestLiteral.checkSubsumptionForClause( solver, clause );
}