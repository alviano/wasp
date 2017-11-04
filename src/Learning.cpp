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
#include "propagators/HCComponent.h"
#include "util/ExtendedStatistics.h"

#include <cassert>

#ifndef NDEBUG
bool
Learning::isVisitedVariablesEmpty() const
{
    for( unsigned i = 0; i < visited.size(); ++i )
        if( isVisited( i, numberOfCalls ) )
            return false;
    return true;
}
#endif

Clause*
Learning::onConflict(
    Literal conflictLiteral,
    Reason* conflictClause )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != Literal::null );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( "Conflict clause is not null" && conflictClause != NULL );
    assert( isVisitedVariablesEmpty() );
    
    assert( lastDecisionLevel.empty() );
    learnedClause = solver.newClause();
    learnedClause->setLearned();
    decisionLevel = solver.getCurrentDecisionLevel();

    trace_msg( learning, 2, "Starting First UIP Learning Strategy. Current Level: " << decisionLevel );
    trace_msg( learning, 2, "Conflict literal: " << conflictLiteral << " - Conflict implicant: " << *conflictClause << ( conflictClause->isLearned() ? " (learned)" : " (original)" ) );    
    estatistics( &solver, onNewConflictLiteral( conflictLiteral ) );
    //Increment heuristic activity
    if( conflictClause->isLearned() ) 
        solver.learnedClauseUsedForConflict( ( Clause* ) conflictClause );
    
    setVisited( conflictLiteral.getVariable(), numberOfCalls );
    pendingVisitedVariables++;

    //Compute implicants of the conflicting literal   
    conflictClause->onLearning( solver, this, conflictLiteral.getOppositeLiteral() );

    if( solver.getImplicant( conflictLiteral.getVariable() ) != NULL  )
        solver.getImplicant( conflictLiteral.getVariable() )->onLearning( solver, this, conflictLiteral );
    
    addLiteralToNavigate( conflictLiteral );
    assert( isVisited( conflictLiteral.getVariable(), numberOfCalls ) );
    solver.startIterationOnAssignedVariable();
    
    //If there is only one element, this element is the first UIP
    while( pendingVisitedVariables > 1 )
    {
        //Get next literal
        Literal currentLiteral = getNextLiteralToNavigate();        
        trace_msg( learning, 3, "Navigating " << currentLiteral << " for calculating the UIP" );
        
        Reason* implicant = solver.getImplicant( currentLiteral.getVariable() );
        if( implicant == NULL )
        {
            trace_msg( learning, 4, currentLiteral << " has no implicant" );
            continue;
        }
        
        lastDecisionLevel.push_back( currentLiteral.getVariable() );
        trace_msg( learning, 4, "The implicant of " << currentLiteral << " is " << *implicant << ( implicant->isLearned() ? " (learned)" : " (original)" ) );
        if( implicant->isLearned() )
            solver.learnedClauseUsedForConflict( ( Clause* ) implicant );
        implicant->onLearning( solver, this, currentLiteral );        
    }

    Literal firstUIP = getNextLiteralToNavigate();
    trace_msg( learning, 2, "First UIP: " << firstUIP );
    
    trace_msg( learning, 3, "Clause before simplification: " << *learnedClause );
    if( learnedClause->size() > 1 )
        simplifyLearnedClause( learnedClause );
    trace_msg( learning, 3, "Clause after simplification: " << *learnedClause );
    
    learnedClause->addLiteralInLearnedClause( firstUIP );    
    assert_msg( sameDecisionLevelOfSolver( firstUIP ), "UIP " << firstUIP << " is not of the same level of the solver. UIP dl: " << solver.getDecisionLevel( firstUIP ) << " - solver dl: " << solver.getCurrentDecisionLevel() );    
    assert_msg( learnedClause->size() > 0, "Learned clause cannot be empty!" );
    
    if( learnedClause->size() >= 2 )
    {
        assert_msg( sameDecisionLevelOfSolver( learnedClause->getAt( learnedClause->size() - 1 ) ), "Learned clause: " << *learnedClause << " - literal in last position has decision level " << solver.getDecisionLevel( learnedClause->getAt( learnedClause->size() - 1 ) ) << ". Solver decision level: " << solver.getCurrentDecisionLevel() );
        learnedClause->swapLiterals( 0, learnedClause->size() - 1 );
        learnedClause->swapLiterals( 1, maxPosition != 0 ? maxPosition : learnedClause->size() - 1 );
        assert( sameDecisionLevelOfSolver( learnedClause->getAt( 0 ) ) );
        
        if( solver.glucoseHeuristic() )
        {
            unsigned int lbd = solver.computeLBD( *learnedClause );             
            bool hasToComputeMaxDecisionLevel = !solver.incremental() && solver.minimisationWithBinaryResolution( *learnedClause, lbd );
            assert( sameDecisionLevelOfSolver( learnedClause->getAt( 0 ) ) );
            if( learnedClause->size() == 1 )
                return learnedClause;
            
            if( hasToComputeMaxDecisionLevel )            
            {
                computeMaxDecisionLevel( *learnedClause );
                learnedClause->swapLiterals( 1, maxPosition );
                lbd = solver.computeLBD( *learnedClause );            
            }
        
            learnedClause->setLbd( lbd );

            for( unsigned int i = 0; i < lastDecisionLevel.size(); i++ )
            {
                Var v = lastDecisionLevel[ i ];
                Reason* implicant = solver.getImplicant( v );

                assert( solver.getDecisionLevel( v ) != 0 );
                if( implicant != NULL && implicant->isLearned() )
                {
                    Clause* c = ( Clause* ) implicant;
                    if( c->lbd() < lbd )
                        solver.onLitInImportantClause( Literal( v, POSITIVE ) );
                }
            }        
        }
    }
    trace_msg( learning, 1, "Learned Clause: " << *learnedClause );    
    
    return learnedClause;
}

Literal
Learning::getNextLiteralToNavigate()
{    
    while( true )
    {
        assert( solver.hasNextAssignedVariable() );
        Literal next = solver.getOppositeLiteralFromLastAssignedVariable();
        assert( !solver.isUndefined( next.getVariable() ) );
        solver.unrollLastVariable();
        assert( solver.isUndefined( next.getVariable() ) );
        
        assert_msg( solver.getCurrentDecisionLevel() == solver.getDecisionLevel( next ), "Mismatch of solver decision level " << solver.getCurrentDecisionLevel() << " and level of next literal " << next << " whose level is " << solver.getDecisionLevel( next ) );
        if( isVisited( next.getVariable(), numberOfCalls ) )
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

    Var v = literal.getVariable();
    if( !isVisited( v, numberOfCalls ) )
    {
        setVisited( v, numberOfCalls );
        //This assert is not true in case of partial checks
//        assert( !solver.isUndefined( v ) );
        
        unsigned int dl = solver.getDecisionLevel( v );
        if( dl > maxDecisionLevel )
        {
            maxDecisionLevel = dl;
            maxPosition = learnedClause->size();
        }
        learnedClause->addLiteralInLearnedClause( literal );
        estatistics( &solver, onLiteralAddInLearnedClause( literal ) );
    }
}

void
Learning::addLiteralToNavigate( 
    Literal literal )
{    
    Var v = literal.getVariable();
    if( !isVisited( v, numberOfCalls ) )
    {
        estatistics( &solver, onLiteralUsedInConflictResolution( literal ) );
        setVisited( v, numberOfCalls );
        ++pendingVisitedVariables;
    }
}

void
Learning::onNavigatingLiteral( 
    Literal literal )
{
    assert( literal != Literal::null );    
    assert( solver.getDecisionLevel( literal ) > 0 );    
    solver.onLitInvolvedInConflict( literal );
 
    if( solver.getDecisionLevel( literal ) == decisionLevel )    
        addLiteralToNavigate( literal );    
    else
        addLiteralInLearnedClause( literal );    
}

void
Learning::onNavigatingLiteralForUnfoundedSetLearning(
    Literal literal )
{
    assert( literal != Literal::null );
    assert( solver.isUndefined( literal ) || solver.getDecisionLevel( literal.getVariable() ) > 0 );
    addLiteralInLearnedClause( literal );    
}

void
Learning::simplifyLearnedClause(
    Clause* lc )
{
    assert( lc != NULL );
    assert( lc->size() > 1 );

    statistics( &solver, startShrinkingLearnedClause( lc->size() + 1 ) );
    Clause& learnedClause = *lc;
    learnedClause.swapLiteralsNoWatches( 0, maxPosition );
    maxPosition = 0;
    
    trace_msg( learning, 1, "Simplifying learned clause " << *lc );
    
    for( unsigned int i = 1; i < learnedClause.size(); )
    {
        trace_msg( learning, 5, "Considering literal " << learnedClause.getAt( i ) );
        if( allMarked( solver.getImplicant( learnedClause.getAt( i ).getVariable() ), learnedClause.getAt( i ) ) )
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
    
    if( allMarked( solver.getImplicant( learnedClause.getAt( 0 ).getVariable() ), learnedClause.getAt( 0 ) ) )
    {
        trace_msg( learning, 5, "Removing literal " << learnedClause.getAt( 0 ) );
        learnedClause.swapLiteralsNoWatches( 0, learnedClause.size() - 1 );
        learnedClause.removeLastLiteralNoWatches();
        maxDecisionLevel = 0;
        for( unsigned int i = 0; i < learnedClause.size(); i++ )
        {
            Literal literal = learnedClause.getAt( i );
            unsigned int dl = solver.getDecisionLevel( literal.getVariable() );
            if( dl > maxDecisionLevel )
            {
                maxDecisionLevel = dl;
                maxPosition = i;
            }
        }
    }
    
    statistics( &solver, endShrinkingLearnedClause( lc->size() + 1 ) );
}

bool
Learning::allMarked(
    Reason* clause,
    Literal literal )
{    
    if( clause == NULL || solver.isUndefined( literal ) || solver.getDecisionLevel( literal ) == 0 ) { trace_msg( learning, 5, "All marked on NULL clause" ); return false; }
    trace_msg( learning, 5, "All marked on clause " << *clause );
    Literal lit = solver.createFromAssignedVariable( literal.getVariable() );
    return( clause->onNavigatingLiteralForAllMarked( solver, this, lit.getOppositeLiteral() ) );    
}

bool
Learning::onNavigatingLiteralForAllMarked(
    Literal literal )
{
    if( isVisited( literal.getVariable(), numberOfCalls ) ) { trace_msg( learning, 5, "Literal " << literal << " has been visited." ); return true; }
    trace_msg( learning, 5, "Navigating " << literal << " for all marked" );
    if( !allMarked( solver.getImplicant( literal.getVariable() ), literal  ) ) return false;
        
    trace_msg( learning, 5, "Literal " << literal << " set as visited" );
    setVisited( literal.getVariable(), numberOfCalls );        
    return true;
}

void
Learning::resetVariablesNumberOfCalls()
{
    for( unsigned i = 1; i < visited.size(); ++i )
        setVisited( i, 0 );    
}

Clause*
Learning::learnClausesFromUnfoundedSet(
    Vector< Var >& unfoundedSet )
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
        Var v = unfoundedSet[ i ];
        Component* component = solver.getComponent( v );
        assert( component != NULL );
        component->onLearningForUnfounded( v, *this );        
    }        
    
    if( learnedClause->size() > 1 )
        simplifyLearnedClause( learnedClause );

    if( learnedClause->size() >= 2 )
        learnedClause->swapLiterals( 0, maxPosition );
    
    trace_msg( learning, 1, "Reason for loop formula: " << *learnedClause );
    return learnedClause;
}

Clause*
Learning::learnClausesFromDisjunctiveUnfoundedSet(
    Vector< Var >& unfoundedSet )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( isVisitedVariablesEmpty() );

    learnedClause = solver.newClause();
    learnedClause->setLearned();
    decisionLevel = solver.getCurrentDecisionLevel();

    unsigned int minDecisionLevel = UINT_MAX;
    unsigned int pos = UINT_MAX;
    for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
    {
        Var tmp = unfoundedSet[ i ];
        assert( solver.isTrue( tmp ) || solver.isUndefined( tmp ) );
        
        HCComponent* component = solver.getHCComponent( tmp );
        component->computeReasonForUnfoundedAtom( tmp, *this );
        
        unsigned int dl = solver.getDecisionLevel( tmp );
        if( dl < minDecisionLevel )
        {
            minDecisionLevel = dl;
            pos = i;
        }
    }    
    
    assert( minDecisionLevel != UINT_MAX );
    assert( pos < unfoundedSet.size() );
    if( minDecisionLevel != 0 || solver.isUndefined( unfoundedSet[ pos ] ) )
        addLiteralInLearnedClause( Literal( unfoundedSet[ pos ], NEGATIVE ) );
    
    if( learnedClause->size() > 1 )
        simplifyLearnedClause( learnedClause );

    if( learnedClause->size() >= 2 )
        Learning::sortClause( learnedClause, solver );
    
    if( solver.glucoseHeuristic() )
        learnedClause->setLbd( solver.computeLBD( *learnedClause ) );
        
    trace_msg( learning, 1, "Reason for disjunctive loop formula: " << *learnedClause );
    return learnedClause;
}

bool
Learning::sameDecisionLevelOfSolver(
    Literal lit ) const
{
    return solver.getDecisionLevel( lit.getVariable() ) == solver.getCurrentDecisionLevel();
}

void
Learning::computeMaxDecisionLevel(
    const Clause& lc )
{
    assert( lc.size() >= 2 );
    maxDecisionLevel = solver.getDecisionLevel( lc[ 1 ] );
    maxPosition = 1;
    for( unsigned int i = 2; i < lc.size(); i++ )
    {
        unsigned int dl = solver.getDecisionLevel( lc[ i ] );
        if( dl > maxDecisionLevel )
        {
            maxDecisionLevel = dl;
            maxPosition = i;
        }
    }
}

void
Learning::sortClause(
    Clause* clause,
    const Solver& solver )
{
    assert( clause->size() > 1 );
    unsigned int max1 = ( solver.isUndefined( clause->getAt( 0 ) ) ) ? UINT_MAX : solver.getDecisionLevel( clause->getAt( 0 ) );
    unsigned int max2 = 0;
    unsigned int posMax1 = 0;
    unsigned int posMax2 = 0;
    
    for( unsigned int i = 1; i < clause->size(); i++ )
    {
        Literal lit = clause->getAt( i );        
        unsigned int dl = solver.getDecisionLevel( lit );                
        if( solver.isUndefined( lit ) )
            dl = UINT_MAX;
        
        if( dl > max1 )
        {
            max2 = max1;
            posMax2 = posMax1;
            max1 = dl;
            posMax1 = i;
        }
        else if( dl > max2 )
        {
            max2 = dl;
            posMax2 = i;
        }
    }
    
    clause->swapLiterals( 0, posMax1 );
    clause->swapLiterals( 1, posMax2 != 0 ? posMax2 : posMax1 );    
}

//Clause*
//Learning::analyzeFinal(
//    Literal lit )
//{
//    ++numberOfCalls;
//    clearDataStructures();
//    assert_msg( learnedClause == NULL, "Learned clause has to be NULL in the beginning" );
//    assert( isVisitedVariablesEmpty() );
// 
//    trace_msg( weakconstraints, 1, "Computing unsat core" );
//    learnedClause = solver.newClause();
//    learnedClause->setLearned();
//
//    vector< Literal > toVisit;
//    toVisit.push_back( lit );
//    
//    while( !toVisit.empty() )
//    {
//        Literal currentLiteral = toVisit.back();
//        trace_msg( weakconstraints, 2, "Considering literal " << currentLiteral );
//        Var currentVariable = currentLiteral.getVariable();
//        toVisit.pop_back();
//                
//        if( isVisited( currentVariable, numberOfCalls ) )
//            continue;
//        
////        if( ( solver.isAssumptionAND( currentLiteral ) || solver.isAssumptionAND( currentLiteral.getOppositeLiteral() ) ) )//|| solver.isAssumptionOR( currentLiteral ) ) )
//        if( solver.isAssumptionAND( currentVariable ) )//|| solver.isAssumptionOR( currentLiteral ) ) )
//            addLiteralInLearnedClause( currentLiteral );
//        else
//            setVisited( currentVariable, numberOfCalls );        
//        
//        Reason* reason = solver.getImplicant( currentVariable );
//        if( reason != NULL )
//        {
//            trace_msg( weakconstraints, 3, "Reason for literal " << currentLiteral << " is " << *reason );
//            reason->onNavigatingForUnsatCore( solver, toVisit, currentLiteral );
//        }
//        else
//        {
//            trace_msg( weakconstraints, 3, "Literal " << currentLiteral << " is a choice (or level 0)" );
//        }
//                
//    }
//
//    trace_msg( weakconstraints, 1, "Unsat core " << *learnedClause );
//    return learnedClause;
//}

Clause*
Learning::analyzeFinal(
    Literal lit )
{
    ++numberOfCalls;
    clearDataStructures();
    assert_msg( learnedClause == NULL, "Learned clause has to be NULL in the beginning" );
    assert( isVisitedVariablesEmpty() );
 
    trace_msg( weakconstraints, 1, "Computing unsat core" );
    learnedClause = solver.newClause();
    learnedClause->setLearned();

    assert( solver.isAssumption( lit.getVariable() ) );
    learnedClause->addLiteralInLearnedClause( lit );
    if( solver.getCurrentDecisionLevel() == 0 )
    {
        trace_msg( weakconstraints, 2, "Conflict at level 0. Unsat core " << *learnedClause );
        return learnedClause;
    }

    setVisited( lit.getVariable(), numberOfCalls );
    solver.startIterationOnAssignedVariable();
    while( solver.hasNextAssignedVariable() )
    {        
        Literal next = solver.getOppositeLiteralFromLastAssignedVariable();
        Var nextVar = next.getVariable();
        trace_msg( weakconstraints, 3, "Considering literal " << next );
        if( solver.getDecisionLevel( nextVar ) == 0 )
        {
            trace_msg( weakconstraints, 4, "its decision level is 0: stop" );
            break;
        }
        
//        assert( !solver.isUndefined( next.getVariable() ) );
//        solver.unrollLastVariable();
//        assert( solver.isUndefined( next.getVariable() ) );

        if( !isVisited( nextVar, numberOfCalls ) )
        {
            trace_msg( weakconstraints, 4, "it is not in the core: skip" );
            continue;
        }
        
        Reason* reason = solver.getImplicant( nextVar );
        if( reason == NULL )
        {
            assert( solver.isAssumption( nextVar ) );
            assert( solver.getDecisionLevel( nextVar ) > 0 );
            trace_msg( weakconstraints, 4, "it was a choice: added" );
            if( solver.isAssumption( next ) )
                learnedClause->addLiteralInLearnedClause( next );
            else {
                assert( solver.isAssumption( next.getOppositeLiteral() ) );
                learnedClause->addLiteralInLearnedClause( next.getOppositeLiteral() );
            }
        }
        else
        {
            trace_msg( weakconstraints, 4, "its reason is " << *reason );
            reason->onNavigatingForUnsatCore( solver, visited, numberOfCalls, next );            
        }        
    }
    
    trace_msg( weakconstraints, 2, "Unsat core " << *learnedClause );
    return learnedClause;
}
