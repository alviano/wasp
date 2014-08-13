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
    for( unsigned i = 0; i < solver.numberOfOccurrences( literal ); ++i )
    {
        Clause* current = solver.getOccurrence( literal, i );

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
    Var variable )
{
    trace_msg( satelite, 2, "Trying to eliminate variable " << VariableNames::getName( variable ) );
    if( !solver.isUndefined( variable ) )
        return false;
    
    if( solver.cost( variable ) > wasp::Options::maxCost )
        return false;
    
    Literal pos( variable, POSITIVE );
    Literal neg( variable, NEGATIVE );
    
    if( solver.numberOfOccurrences( pos ) >=8 && solver.numberOfOccurrences( neg ) >=8 )
        return false;
    
    if( solver.numberOfOccurrences( pos ) >= 3 && solver.numberOfOccurrences( neg ) >= 3 )
    {
        unsigned literals = 0;
        for( unsigned i = 0; i < solver.numberOfOccurrences( pos ); ++i )
            literals += solver.getOccurrence( pos, i )->size();
        for( unsigned i = 0; i < solver.numberOfOccurrences( neg ); ++i )
            literals += solver.getOccurrence( neg, i )->size();
            
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
    Literal lit )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );
    Var variable = lit.getVariable();
    Literal oppositeLiteral = lit.getOppositeLiteral();

    Clause negatedDefinition;
    for( unsigned i = 0; i < solver.numberOfOccurrences( oppositeLiteral ); ++i )
    {
        Clause* clause = solver.getOccurrence( oppositeLiteral, i );
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
            ok = solver.propagateLiteralAsDeterministicConsequenceSatelite( oppositeLiteral );
            return true;
        }
    }
    
    negatedDefinition.addLiteral( Literal( lit ) );
    
    Clause* result = NULL;
    for( unsigned i = 0; i < solver.numberOfOccurrences( lit ); ++i )
    {
        Clause* clause = solver.getOccurrence( lit, i );
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
    
    return tryToSubstitute( lit, resultCopy );
}

bool
Satelite::tryToSubstitute(
    Literal lit,
    Clause* definition )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );
    assert( trueLiterals.empty() );
    
    Var variable = lit.getVariable();    
    Literal oppositeLit = lit.getOppositeLiteral();
    trace_msg( satelite, 5, "Trying to substitute " << VariableNames::getName( variable ) << ( lit.getSign() == POSITIVE ? " positive" : " negative" ) << ". Its definition is " << *definition );
    vector< Clause* > newClauses;  
    
    for( unsigned i = 0; i < definition->size(); ++i )
    {
//        if( definition->getAt( i ).getVariable() == variable )
//            continue;

        for( unsigned j = 0; j < solver.numberOfOccurrences( lit ); ++j )
        {            
            Clause* clause = solver.getOccurrence( lit, j );
            if( clause->contains( definition->getAt( i ) ) )
                continue;

            Clause* newClause = new Clause();
            for( unsigned k = 0; k < clause->size(); ++k )
                newClause->addLiteral( clause->getAt( k ) == lit ? definition->getAt( i ).getOppositeLiteral() : clause->getAt( k ) );
            
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
    
    for( unsigned j = 0; j < solver.numberOfOccurrences( oppositeLit ); ++j )
    {
        Clause* clause = solver.getOccurrence( oppositeLit, j );
         
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

    if( newClauses.size() > solver.numberOfOccurrences( variable ) )
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
    
    assert_msg( solver.numberOfOccurrences( variable ) == 0, "Variable " << VariableNames::getName( variable ) << " has been eliminated but has still " << solver.numberOfOccurrences( variable ) << " occurrences" );    
    trace_msg( satelite, 2, "Eliminated variable " << VariableNames::getName( variable ) );
    
    solver.onEliminatingVariable( variable, lit.getSign(), definition );
    ok = propagateTopLevel();
    return true;
}

bool
Satelite::tryToEliminateByDefinition(
    Var variable )
{    
    Literal pos( variable, POSITIVE );
    Literal neg( variable, NEGATIVE );
    if( solver.numberOfOccurrences( pos ) <= 1 && solver.numberOfOccurrences( neg ) <= 1 )
        return false;
    
    if( tryToEliminateByDefinition( pos ) || tryToEliminateByDefinition( neg ) )
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
        if( !solver.isTrue( lit ) )
        {
            if( !solver.propagateLiteralAsDeterministicConsequenceSatelite( lit ) )
                return false;
        }
    }
    
    return true;
}

bool
Satelite::tryToEliminateByDistribution( 
    Var variable )
{
    trace_msg( satelite, 5, "Trying to eliminate " << VariableNames::getName( variable ) << " by distribution" );

    vector< Clause* > newClauses;
    Literal neg( variable );
    Literal pos( variable );    
    
    for( unsigned int i = 0; i < solver.numberOfOccurrences( pos ); i++ )
    {
        for( unsigned int j = 0; j < solver.numberOfOccurrences( neg ); j++ )
        {
            Clause* newClause = new Clause();            
            Clause* firstClause = solver.getOccurrence( pos, i );
            Clause* secondClause = solver.getOccurrence( neg, i );

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
                
                if( newClauses.size() > solver.numberOfOccurrences( variable ) || newClause->size() > clauseLimit )
                {
                    for( unsigned int i = 0; i < newClauses.size(); ++i )
                        delete newClauses[ i ];
                    trueLiterals.clear();
                    return false;
                }
            }
        }
    }

    solver.removeAllClauses( variable );    
    
    for( unsigned int i = 0; i < newClauses.size(); i++ )
    {
        Clause* c = newClauses[ i ];
        assert( c->size() >= 2 );        
        onAddingClause( c );
        solver.addClause( c );        
    }
    
    trace_msg( satelite, 2, "Eliminated variable " << VariableNames::getName( variable ) );   
    
    solver.onEliminatingVariable( variable, ELIMINATED_BY_DISTRIBUTION, NULL );
    ok = propagateTopLevel();    
    return true;
}

void
Satelite::substitute(
    Var variable,
    vector< Clause* >& newClauses )
{
    solver.markAllClauses( variable );
    
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
    if( solver.numberOfVariables() > 250000 )
        return true;
    
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        assert( touchedVariables.size() == i );
        touchedVariables.push_back( true );
        numberOfTouched++;
        elim_heap.pushNoCheck( i );
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
            Var v = elim_heap.removeMin();            
            if( !solver.isUndefined( v ) || solver.hasBeenEliminated( v ) || solver.isFrozen( v ) || !enabledVariableElimination )
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
        Var variable = solver.getVariableWithMinOccurrences( *clause );
        
        assert( variable != 0 );
        trace_msg( satelite, 1, "Variable with the min number of occurrences is " << VariableNames::getName( variable ) );
        
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
    Var variable )
{    
    Literal pos( variable, POSITIVE );
    Literal neg( variable, NEGATIVE );
    for( unsigned int i = 0; i < solver.numberOfOccurrences( pos ); i++ )
    {
        Clause* clause = solver.getOccurrence( pos, i );
        assert( clause != NULL && !clause->hasBeenDeleted() );
        addClauseInSubsumptionQueue( clause );
    }
    
    for( unsigned int i = 0; i < solver.numberOfOccurrences( neg ); i++ )
    {
        Clause* clause = solver.getOccurrence( neg, i );
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
            gatherTouchedClauses( i );
            touchedVariables[ i ] = false;
        }
    }
}

void
Satelite::checkSubsumptionForClause(
    Clause* clause,
    Literal bestLiteral )
{
    solver.checkSubsumptionForClause( clause, bestLiteral );
}

void
Satelite::touchVariablesInClause(
    Clause* clause )
{
    assert( !clause->hasBeenDeleted() );

    for( unsigned int j = 0; j < clause->size(); j++ )
    {
        Var variable = clause->getAt( j ).getVariable();
        
        if( solver.isUndefined( variable ) && !solver.hasBeenEliminated( variable ) && solver.numberOfOccurrences( variable ) > 0 )
        {
            if( !touchedVariables[ variable ] )
            {
                numberOfTouched++;
                if( elim_heap.inHeap( variable ) )
                    elim_heap.increase( variable );
            }
        }
    }
}

unsigned int
Satelite::cost(
    Var v ) const
{
    return solver.cost( v );
}