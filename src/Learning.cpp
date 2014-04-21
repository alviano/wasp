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

#include "Learning.h"
#include "Clause.h"
#include "Literal.h"
#include "Solver.h"

#include <cassert>

#ifndef NDEBUG
bool
Learning::isVisitedVariablesEmpty() const
{
    for( unsigned i = 1; i <= solver.numberOfVariables(); ++i )
        if( solver.getVariable( i )->visited() == numberOfCalls )
            return false;
    return true;
}
#endif

Clause*
Learning::onConflict(
    Literal conflictLiteral,
    Clause* conflictClause )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != Literal::null );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( isVisitedVariablesEmpty() );
    
    learnedClause = solver.newClause();
    learnedClause->setLearned();
    decisionLevel = solver.getCurrentDecisionLevel();

    trace_msg( learning, 2, "Starting First UIP Learning Strategy. Current Level: " << decisionLevel );
    
    //Compute implicants of the conflicting literal.
    if( conflictClause->isLearned() )    
        solver.updateActivity( conflictClause );
    
    conflictClause->onLearning( this, conflictLiteral );
    //assert_msg( conflictLiteral.getVariable()->getImplicant() != NULL, "Conflict literal " << conflictLiteral << " has no implicant" ); // FIXME: I added this assert. Is it right? It is true only for tight programs.
    
    if( conflictLiteral.getVariable()->getImplicant() != NULL  )
        conflictLiteral.getVariable()->getImplicant()->onLearning( this, conflictLiteral.getOppositeLiteral() );
    
    trace( learning, 2, "Conflict literal: %s.\n", toString( conflictLiteral ).c_str() );
    addLiteralToNavigate( conflictLiteral );
    assert( conflictLiteral.getVariable()->visited() == numberOfCalls );
    solver.startIterationOnAssignedVariable();
    
    //If there is only one element, this element is the first UIP.
    while( pendingVisitedVariables > 1 )
    {
        //Get next literal.
        Literal currentLiteral = getNextLiteralToNavigate();
        trace( learning, 3, "Navigating %s for calculating UIP.\n", toString( currentLiteral ).c_str() );
        
        Clause* implicant = currentLiteral.getVariable()->getImplicant();
        //Compute implicants of the literal.
        if( implicant != NULL )
        {
            if( implicant->isLearned() )
                solver.updateActivity( implicant );
            implicant->onLearning( this, currentLiteral );
        }
    }

    Literal firstUIP = getNextLiteralToNavigate();
    trace( learning, 2, "First UIP: %s.\n", toString( firstUIP ).c_str() );
    
    if( learnedClause->size() > 1 )
        simplifyLearnedClause( learnedClause );

    learnedClause->addLiteralInLearnedClause( firstUIP );    
    
    assert( learnedClause->size() > 0 );
    
    if( learnedClause->size() >= 2 )
    {
        assert( learnedClause->getAt( learnedClause->size() - 1 ).getDecisionLevel() == solver.getCurrentDecisionLevel() );
        learnedClause->swapLiterals( 0, learnedClause->size() - 1 );
        learnedClause->swapLiterals( 1, maxPosition != 0 ? maxPosition : learnedClause->size() - 1 );
        assert( learnedClause->getAt( 0 ).getDecisionLevel() == solver.getCurrentDecisionLevel() );
    }
    
    trace( learning, 1, "Learned Clause: %s.\n", toString( *learnedClause ).c_str() );
    
    return learnedClause;
}

Literal
Learning::getNextLiteralToNavigate()
{    
    while( true )
    {
        assert( solver.hasNextAssignedVariable() );
        Literal next = solver.getOppositeLiteralFromLastAssignedVariable();

        assert( !next.isUndefined() );
        solver.unrollLastVariable();
        assert( next.isUndefined() );

        if( next.getVariable()->visited() == numberOfCalls )
        {
            --pendingVisitedVariables;
            return next;
        }        
    }    
}

void
Learning::addLiteralInLearnedClause( 
    Literal literal )
{
    assert( "Learned clause is not initialized." && learnedClause != NULL );

    if( literal.getVariable()->visited() != numberOfCalls )
    {
        literal.getVariable()->visited() = numberOfCalls;
        assert( !literal.isUndefined() );        
        
        if( literal.getDecisionLevel() > maxDecisionLevel )
        {
            maxDecisionLevel = literal.getDecisionLevel();
            maxPosition = learnedClause->size();
        }
        learnedClause->addLiteralInLearnedClause( literal );
    }
}

void
Learning::addLiteralToNavigate( 
    Literal literal )
{
    if( literal.getVariable()->visited() != numberOfCalls )
    {
        literal.getVariable()->visited() = numberOfCalls;
        ++pendingVisitedVariables;    
    }
}

void
Learning::onNavigatingLiteral( 
    Literal literal )
{
    assert( literal != Literal::null );
    assert( literal.getDecisionLevel() > 0 );

    solver.onLiteralInvolvedInConflict( literal );
    if( literal.getDecisionLevel() == decisionLevel )
        addLiteralToNavigate( literal );
    else
        addLiteralInLearnedClause( literal );    
}

void
Learning::onNavigatingLiteralForUnfoundedSetLearning(
    Literal literal )
{
    assert( literal != Literal::null );
    assert( literal.getDecisionLevel() > 0 );
    addLiteralInLearnedClause( literal );    
}

void
Learning::simplifyLearnedClause(
    Clause* lc )
{    
//    return;
    assert( lc != NULL );
    assert( lc->size() > 1 );

    statistics( startShrinkingLearnedClause( lc->size() + 1 ) );
    Clause& learnedClause = *lc;
    learnedClause.swapLiteralsNoWatches( 0, maxPosition );
    maxPosition = 0;
    
    trace_msg( learning, 1, "Simplifying learned clause " << *lc );
    
    for( unsigned int i = 1; i < learnedClause.size(); )
    {
        trace_msg( learning, 5, "Considering literal " << learnedClause.getAt( i ) );
        if( allMarked( learnedClause.getAt( i ).getVariable()->getImplicant(), learnedClause.getAt( i ) ) )
        {
            trace_msg( learning, 5, "Removing literal " << learnedClause.getAt( i ) );
            learnedClause.swapLiteralsNoWatches( i, learnedClause.size() - 1 );
            learnedClause.removeLastLiteralNoWatches();
        }
        else
        {
            i++;
        }
        
    }
    
    if( allMarked( learnedClause.getAt( 0 ).getVariable()->getImplicant(), learnedClause.getAt( 0 ) ) )
    {
        trace_msg( learning, 5, "Removing literal " << learnedClause.getAt( 0 ) );
        learnedClause.swapLiteralsNoWatches( 0, learnedClause.size() - 1 );
        learnedClause.removeLastLiteralNoWatches();
        maxDecisionLevel = 0;
        for( unsigned int i = 0; i < learnedClause.size(); i++ )
        {
            Literal literal = learnedClause.getAt( i );
            if( literal.getDecisionLevel() > maxDecisionLevel )
            {
                maxDecisionLevel = literal.getDecisionLevel();
                maxPosition = i;
            }
        }
    }
    
    statistics( endShrinkingLearnedClause( lc->size() + 1 ) );
}

//bool
//Learning::allMarked(
//    const Clause* clause )
//{
//    if( clause == NULL )
//    {
//        trace_msg( learning, 5, "All marked on NULL clause" );
//        return false;
//    }
//    trace_msg( learning, 5, "All marked on clause " << *clause );
//    
//    for( unsigned int i = 1; i < clause->size(); i++ )
//    {
//        trace_msg( learning, 5, "Considering literal " << clause->getAt( i ) << " in position " << i );
//
//        if( clause->getAt( i ).getVariable()->visited() != numberOfCalls )
//        {
//            if( allMarked( clause->getAt( i ).getVariable()->getImplicant() ) )
//            {
//                trace_msg( learning, 5, "Literal " << clause->getAt( i ) << " set as visited" );
//                clause->getAt( i ).getVariable()->visited() = numberOfCalls;
//            }
//            else
//            {
//                return false;
//            }
//        }
//        else
//        {
//            trace_msg( learning, 5, "Literal " << clause->getAt( i ) << " has been visited." );
//        }
//    }
//    
//    return true;
//}

bool
Learning::allMarked(
    Clause* clause,
    Literal literal )
{
    if( clause == NULL )
    {
        trace_msg( learning, 5, "All marked on NULL clause" );
        return false;
    }
    trace_msg( learning, 5, "All marked on clause " << *clause );
    
    if( !clause->onNavigatingLiteralForAllMarked( this, literal ) )
        return false;
    
    return true;
}

bool
Learning::onNavigatingLiteralForAllMarked(
    Literal literal )
{
    if( literal.getVariable()->visited() != numberOfCalls )
    {
        if( allMarked( literal.getVariable()->getImplicant(), literal ) )
        {
            trace_msg( learning, 5, "Literal " << literal << " set as visited" );
            literal.getVariable()->visited() = numberOfCalls;
        }
        else
        {
            return false;
        }
    }
    else
    {
        trace_msg( learning, 5, "Literal " << literal << " has been visited." );
    }
    
    return true;
}

void
Learning::resetVariablesNumberOfCalls()
{
    for( unsigned i = 1; i <= solver.numberOfVariables(); ++i )
    {
        solver.getVariable( i )->visited() = 0;
    }
}

Clause*
Learning::learnClausesFromUnfoundedSet(
    Vector< Variable* >& unfoundedSet )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( isVisitedVariablesEmpty() );

    learnedClause = solver.newClause();
    learnedClause->setLearned();
    decisionLevel = solver.getCurrentDecisionLevel();

    trace_msg( learning, 2, "Starting First UIP Learning Strategy. Current Level: " << decisionLevel );

    for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
    {
        Variable* variable = unfoundedSet[ i ];
        variable->onLearningForUnfounded( *this );
    }
    
    if( learnedClause->size() > 1 )
        simplifyLearnedClause( learnedClause );

    if( learnedClause->size() >= 2 )
        learnedClause->swapLiterals( 0, maxPosition );
    
    trace( learning, 1, "Learned Clause: %s.\n", toString( *learnedClause ).c_str() );
    return learnedClause;
}
