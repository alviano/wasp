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

#include "Solver.h"
#include "input/Dimacs.h"
#include <algorithm>
#include <stdint.h>
using namespace std;

Solver::~Solver()
{
    while( !clauses.empty() )
    {
        assert( clauses.back() );
        delete clauses.back();
        clauses.pop_back();
    }
    
    while( !learnedClauses.empty() )
    {
        assert( learnedClauses.back() );
        delete learnedClauses.back();
        learnedClauses.pop_back();
    }
    
    while( !poolOfClauses.empty() )
    {
        assert( poolOfClauses.back() );
        delete poolOfClauses.back();
        poolOfClauses.pop_back();
    }
    
    while( !gusDataVector.empty() )
    {
        if( gusDataVector.back() )
            delete gusDataVector.back();
        gusDataVector.pop_back();
    }
    
    while( !aggregates.empty() )
    {
        assert( aggregates.back() );
        delete aggregates.back();
        aggregates.pop_back();
    }
    
    if( outputBuilder != NULL )
        delete outputBuilder;
    
    if( satelite != NULL )
        delete satelite;
    
    if( restart != NULL )
        delete restart;
    
    if( optimizationAggregate != NULL )
        delete optimizationAggregate;
    
    
    while( !variableDataStructures.empty() )
    {
        delete variableDataStructures.back();
        variableDataStructures.pop_back();
    }
    
    delete minisatHeuristic;
    
    for( unsigned int i = 1; i <= variables.numberOfVariables(); i++ )
    {        
        if( variables.getSignOfEliminatedVariable( i ) == ELIMINATED_BY_DISTRIBUTION )
        {
            Literal pos( i, POSITIVE );
            Vector< Clause* >& allPosOccs = getDataStructure( pos ).variableAllOccurrences;
            for( unsigned int j = 0; j < allPosOccs.size(); j++ )
                delete allPosOccs[ j ];

            Vector< Clause* >& allNegOccs = getDataStructure( pos.getOppositeLiteral() ).variableAllOccurrences;
            for( unsigned int j = 0; j < allNegOccs.size(); j++ )
                delete allNegOccs[ j ];
        }
    }
}

void
Solver::unroll(
    unsigned int level )
{
    assert_msg( !unrollVector.empty(), "There is nothing to unroll" );
    assert_msg( level < unrollVector.size(), "Level " << level << " is greater than unrollVector size " << unrollVector.size() );
    assert_msg( currentDecisionLevel >= level, "Level " << level << " is greater than current decision level " << currentDecisionLevel );
    assert( "Vector for unroll is inconsistent" && variables.numberOfAssignedLiterals() >= unrollVector[ level ] );    
    unsigned int toUnroll = variables.numberOfAssignedLiterals() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        unrollLastVariable();
        toUnroll--;
    }
    
    while( toPop > 0 )
    {
        unrollVector.pop_back();
        toPop--;
    }
    
    variables.onUnroll();
    
    for( unsigned int i = 0; i < aggregates.size(); i++ )
    {
        aggregates[ i ]->reset( *this );
    }
    
    if( optimizationAggregate != NULL )
    {
        optimizationAggregate->reset( *this );
    }
}

bool
Solver::addClauseFromModelAndRestart()
{
    assert( variables.numberOfAssignedLiterals() > 0 );
    
    trace_msg( enumeration, 2, "Creating the clause representing the model." );
    Clause* clause = newClause(); //new Clause();
    
    for( unsigned int i = 1; i <= variables.numberOfVariables(); i++ )
    {
        Var v = i;
        assert( !isUndefined( v ) );
        
        trace_msg( enumeration, 3, "Checking literal " << v << " with decision level " << getDecisionLevel( v ) << " and its implicant is " << ( hasImplicant( v ) ? "not null" : "null" ) );
        if( !hasImplicant( v ) && getDecisionLevel( v ) != 0 )
        {
            if( isTrue( v ) )
            {
                Literal lit( v, NEGATIVE );
                trace_msg( enumeration, 2, "Adding literal " << lit << " in clause." );
                clause->addLiteral( lit );
            }
            else
            {
                Literal lit( v );
                trace_msg( enumeration, 2, "Adding literal " << lit << " in clause." );
                clause->addLiteral( lit );
            }
        }
    }    
    
    if( clause->size() == 0 )
    {
        releaseClause( clause );
        return false;
    }
    
    this->doRestart();
    simplifyOnRestart();    
    clearConflictStatus();
    
    return addClauseFromModel( clause );
}

bool 
Solver::solve()
{
    trace( solving, 1, "Starting solving.\n" );
    
    if( hasNextVariableToPropagate() )
        goto propagationLabel;
    
    while( hasUndefinedLiterals() )
    {
        /*
        {
            static unsigned int PROVA = 0;
            static time_t PROVA_TIME = time( 0 );


            unsigned int end = 5001;
            unsigned int printValue = 100;

            if( ++PROVA > end ) {
                cerr << "PROVA END!" << endl;
                return false;
            }
            else if( ++PROVA % printValue == 0 )
            {
                cout << PROVA << " " << learnedClauses.size() <<  " " << ( time( 0 ) - PROVA_TIME ) << endl;
            }
        }
        //*/
        
        if( hasToDelete() )
        {
            deleteClauses();
        }

        assert( !conflictDetected() );
        chooseLiteral();
        
        propagationLabel:;
        while( hasNextVariableToPropagate() )
        {
            nextValueOfPropagation--;
            Var variableToPropagate = getNextVariableToPropagate();
            propagate( variableToPropagate );

            if( conflictDetected() )
            {
                trace( solving, 1, "Conflict detected.\n" );
                if( getCurrentDecisionLevel() == 0 )
                {
                    trace( solving, 1, "Conflict at level 0: return. \n");
                    statistics( endSolving() );
                    return false;
                }
                
                if( !analyzeConflict() )
                {
                    statistics( endSolving() );
                    return false;
                }
                minisatHeuristic->variableDecayActivity();                
                assert( hasNextVariableToPropagate() || getCurrentDecisionLevel() == 0 );
            }
        }
    }
    
    completeModel();
    assert_msg( getNumberOfUndefined() == 0, "Found a model with " << getNumberOfUndefined() << " undefined variables." );
    assert_msg( allClausesSatisfied(), "The model found is not correct." );
    
    statistics( endSolving() );
    return true;
}

bool 
Solver::solvePropagators()
{
    trace( solving, 1, "Starting solving.\n" );
    if( hasNextVariableToPropagate() )
        goto propagationLabel;

    if( !postPropagators.empty() )
        goto postPropagationLabel;
    
    while( hasUndefinedLiterals() )
    {
        if( hasToDelete() )
            deleteClauses();
        
        assert( !conflictDetected() );
        chooseLiteral();
        
        propagationLabel:;
        Var variableToPropagate;
        while( hasNextVariableToPropagate() )
        {
            nextValueOfPropagation--;
            variableToPropagate = getNextVariableToPropagate();
            propagateWithPropagators( variableToPropagate );

            conflict:;
            if( conflictDetected() )
            {
                trace( solving, 1, "Conflict detected.\n" );                
                if( getCurrentDecisionLevel() == 0 )
                {
                    trace( solving, 1, "Conflict at level 0: return. \n");
                    statistics( endSolving() );
                    return false;
                }
                
                if( !analyzeConflict() )
                {
                    statistics( endSolving() );
                    return false;
                }
                minisatHeuristic->variableDecayActivity();                
                assert( hasNextVariableToPropagate() || getCurrentDecisionLevel() == 0 );
            }
        }
        
        postPropagationLabel:;
        while( !postPropagators.empty() )
        {
            PostPropagator* postPropagator = postPropagators.back();
            Clause* clauseToPropagate = postPropagator->getClauseToPropagate( learning );
            if( clauseToPropagate == NULL )
            {
                postPropagators.pop_back();
                postPropagator->onRemoving();                                            
            }
            else
            {
                unsigned int size = clauseToPropagate->size();
                statistics( onLearningFromPropagators( size ) );
                if( size == 0 )
                {
                    clearConflictStatus();
                    delete clauseToPropagate;
                    return false;
                }
                else if( size == 1 )
                {
                    if( getCurrentDecisionLevel() != 0 )
                    {
                        clearConflictStatus();                    
                        doRestart();
                    }
                    assignLiteral( clauseToPropagate->getAt( 0 ) );
                    delete clauseToPropagate;
                }
                else
                {
                    if( getDecisionLevel( clauseToPropagate->getAt( 1 ) ) < getCurrentDecisionLevel() )
                    {
                        clearConflictStatus();                    
                        trace( solving, 2, "Learned clause from propagator and backjumping to level %d.\n", getDecisionLevel( clauseToPropagate->getAt( 1 ) ) );            
                        unroll( getDecisionLevel( clauseToPropagate->getAt( 1 ) ) );
                    }
                    addLearnedClause( clauseToPropagate );
                    assert( !isTrue( clauseToPropagate->getAt( 0 ) ) );
                    assignLiteral( clauseToPropagate );
                    onLearning( clauseToPropagate );
                }
                
                if( conflictDetected() )
                    goto conflict;
                else
                    goto propagationLabel;
            }
        }
    }
    
    completeModel();
    assert_msg( getNumberOfUndefined() == 0, "Found a model with " << getNumberOfUndefined() << " undefined variables." );
    assert_msg( allClausesSatisfied(), "The model found is not correct." );
    
    statistics( endSolving() );
    return true;
}

void
Solver::shortPropagation(
    Var variable )
{
    assert( !conflictDetected() );
            
    Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );    
//    Vector< Literal >& binary = variableBinaryClauses[ ( getTruthValue( variable ) >> 1 ) ];    
    Vector< Literal >& binary = getDataStructure( complement ).variableBinaryClauses;
    
    trace_msg( solving, 2, "Propagation of binary clauses for literal " << complement );
    for( unsigned i = 0; i < binary.size(); ++i )
    {
        if( conflictDetected() )
            break;
        
        Literal lit = binary[ i ];
        if( !isTrue( lit ) )
        {
            trace_msg( solving, 5, "Inferring " << lit << " as true" );        
            assignLiteral( lit, variables.getReasonForBinaryClauses( variable ) );
        }
    }
}

void
Solver::unitPropagation(
    Var variable )
{
    assert( !conflictDetected() );
    trace_msg( solving, 5, "Unit propagation" );
    Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );
//    WatchedList< Clause* >& wl = variableWatchedLists[ ( getTruthValue( variable ) >> 1 ) ];
    WatchedList< Clause* >& wl = getDataStructure( complement ).variableWatchedLists;

    unsigned j = 0;
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        Clause* clause = wl[ j ] = wl[ i ];
        assert_msg( clause != NULL, "Next clause to propagate is null." );
        trace_msg( solving, 5, "Considering clause " << *clause );
        if( onLiteralFalse( *clause, complement ) )
        {
            trace_msg( solving, 6, "Inferring literal " << clause->getAt( 0 ) << " using clause " << *clause );
            assignLiteral( clause );
            if( conflictDetected() )
            {
                while( i < wl.size() )
                    wl[ j++ ] = wl[ i++ ];
                break;
            }
            ++j;
        }
        else if( clause->getAt( 1 ) == complement )
        {
            assert( !conflictDetected() );
            ++j;
        }
        else
            assert( !conflictDetected() );
    }
    wl.shrink( j );
}

void
Solver::propagation(
    Var variable )
{
    assert( !conflictDetected() );
    trace_msg( solving, 5, "Propagation" );
    Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );

//    Vector< pair< Propagator*, int > >& wl = variablePropagators[ ( getTruthValue( variable ) >> 1 ) ];
    Vector< pair< Propagator*, int > >& wl = getDataStructure( complement ).variablePropagators;
    
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        if( conflictDetected() )
            break;
        Propagator* propagator = wl[ i ].first;
        assert( "Post propagator is null." && propagator != NULL );
        propagator->onLiteralFalse( *this, complement, wl[ i ].second );        
    }
}

void
Solver::postPropagation(
    Var variable )
{
    assert( !conflictDetected() );    
    trace_msg( solving, 5, "Post propagation" );
    Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );
    
//    Vector< pair< PostPropagator*, int > >& wl = variablePostPropagators[ ( getTruthValue( variable ) >> 1 ) ];
    Vector< pair< PostPropagator*, int > >& wl = getDataStructure( complement ).variablePostPropagators;    
    
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        PostPropagator* postPropagator = wl[ i ].first;
        assert( "Post propagator is null." && postPropagator != NULL );
        bool res = postPropagator->onLiteralFalse( complement, wl[ i ].second );
        
        if( res )            
            addPostPropagator( postPropagator );
    }    
}

void
Solver::propagateAtLevelZeroSatelite(
    Var variable )
{
    if( hasBeenEliminated( variable ) )
        return;
    
    assert( !conflictDetected() );
    {
        Literal literal = variables.createLiteralFromAssignedVariable( variable );        
    
//        Vector< Clause* >& wl = variableAllOccurrences[ 1 - ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( literal ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];            
            assert( !clause->hasBeenDeleted() );
            trace_msg( solving, 5, "Considering clause " << *clause );
            detachClauseFromAllLiterals( *clause, literal );            
            markClauseForDeletion( clause );
        }
        wl.clearAndDelete();
    }
    
    {
        assert( !conflictDetected() );
        trace_msg( solving, 2, "Propagating " << variables.createOppositeLiteralFromAssignedVariable( variable ) << " as false at level 0" );
        Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );        
        
//        Vector< Clause* >& wl = variableAllOccurrences[ ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( complement ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            assert_msg( clause != NULL, "Next clause to propagate is null" );
            trace_msg( solving, 5, "Considering clause " << *clause );
            clause->removeLiteral( complement );
            if( clause->size() == 1 )
            {
                if( !isTrue( clause->getAt( 0 ) ) )
                {
                    trace_msg( solving, 5, "Assigning literal " << clause->getAt( 0 ) << " as true" );
                    assignLiteral( clause->getAt( 0 ) );
                    if( conflictDetected() )
                        break;
                }
                detachClauseFromAllLiterals( *clause );
                markClauseForDeletion( clause );
            }
            else
            {
                onStrengtheningClause( clause );
                assert( !conflictDetected() );
            }
        }
        wl.clearAndDelete();
    }    
    if( !conflictDetected() && hasPropagators() )
    {
        propagation( variable );
        if( !conflictDetected() )
            postPropagation( variable );
    }
}

void
Solver::propagateAtLevelZero(
    Var variable )
{
    assert( !conflictDetected() );    
    {
        trace_msg( solving, 2, "Propagating " << variables.createLiteralFromAssignedVariable( variable ) << " as true at level 0" );
        Literal literal = variables.createLiteralFromAssignedVariable( variable );
//        Vector< Clause* >& wl = variableAllOccurrences[ 1 - ( getTruthValue( variable ) >> 1 ) ];       
        Vector< Clause* >& wl = getDataStructure( literal ).variableAllOccurrences;
     
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            trace_msg( solving, 5, "Considering clause " << *clause );
            detachClauseFromAllLiterals( *clause, literal );
            deleteClause( clause );
        }
        wl.clearAndDelete();
    }
    
    {
        assert( !conflictDetected() );
        trace_msg( solving, 2, "Propagating " << variables.createOppositeLiteralFromAssignedVariable( variable ) << " as false at level 0" );
        Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );
        
//        Vector< Clause* >& wl = variableAllOccurrences[ ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( complement ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            assert_msg( clause != NULL, "Next clause to propagate is null" );
            trace_msg( solving, 5, "Considering clause " << *clause );
            clause->removeLiteral( complement );
            if( clause->size() == 1 )
            {
                if( !isTrue( clause->getAt( 0 ) ) )
                {
                    trace_msg( solving, 5, "Assigning literal " << clause->getAt( 0 ) << " as true" );
                    assignLiteral( clause->getAt( 0 ) );
                    if( conflictDetected() )
                        break;
                }
                detachClauseFromAllLiterals( *clause );
                deleteClause( clause );
                
            }
            else
                assert( !conflictDetected() );
        }
        wl.clearAndDelete();
    }

//    assert( variableAllOccurrences[ POSITIVE ].size() == 0 );
//    assert( variableAllOccurrences[ NEGATIVE ].size() == 0 );
    assert( getDataStructure( Literal( variable, POSITIVE ) ).variableAllOccurrences.size() == 0 );
    assert( getDataStructure( Literal( variable, NEGATIVE ) ).variableAllOccurrences.size() == 0 );    

    if( !conflictDetected() && hasPropagators() )
    {
        propagation( variable );
        if( !conflictDetected() )
            postPropagation( variable );
    }
}

void
Solver::printProgram() const
{
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
        cout << *( *it ) << endl;
    
    for( unsigned int i = 0; i < aggregates.size(); i++ )
        cout << *aggregates[ i ] << endl;    
}

void
Solver::printDimacs() const
{
    cout << "p cnf " << numberOfClauses() << " " << numberOfVariables() << endl;
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        Clause& clause = **it;
                
        for( unsigned i = 0; i < clause.size(); i++ )
            cout << clause[ i ].getId() << " ";
        cout << "0" << endl;        
    }
}

unsigned int
Solver::getNumberOfUndefined() const
{
    unsigned countUndef = 0;
    for( unsigned int i = 1; i <= variables.numberOfVariables(); i++ )
    {
        if( isUndefined( i ) )
            countUndef++;     
    }

    return countUndef;
}

bool
Solver::allClausesSatisfied() const
{
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        Clause& clause = *( *it );
        if( !isSatisfied( clause ) )        
            return false;
    }

    return true;
}

bool compareClauses( Clause* c1, Clause* c2 ){ return c1->activity() < c2->activity(); }

void
Solver::deleteClauses()
{
    ClauseIterator i = learnedClauses_begin();
    ClauseIterator j = learnedClauses_begin();
    Activity threshold = deletionCounters.increment / numberOfLearnedClauses();
    
    sort( learnedClauses.begin(), learnedClauses.end(), compareClauses );
    
    unsigned int numberOfDeletions = 0;
    unsigned int size = numberOfLearnedClauses();
    unsigned int toDelete = size / 2;
    while( i != learnedClauses.end() )
    {
        Clause& clause = **i;
        if( clause.size() > 2 && !isLocked( clause ) && ( numberOfDeletions < toDelete || clause.activity() < threshold ) )
        {
            deleteLearnedClause( i );
            numberOfDeletions++;
        }
        else
        {
            *j = *i;
            ++j;
        }
        
        ++i;
    }

    finalizeDeletion( size - numberOfDeletions );
    statistics( onDeletion( size, numberOfDeletions ) );
}

//void
//Solver::deleteClauses()
//{
//    Activity activitySum = 0;
//    unsigned int activityCount = 0;
//    Activity threshold = deletionCounters.increment / numberOfLearnedClauses();      
//    
//    ClauseIterator i = learnedClauses_begin();
//    ClauseIterator j = learnedClauses_begin();
//    
//    unsigned int size = numberOfLearnedClauses();
//    unsigned int halfSize = size / 2;    
//    unsigned int numberOfDeletions = 0;
//
//    assert( i != learnedClauses_end() );
//    
//    do
//    {
//        Clause& clause = **i;
//        
//        if( !clause.isLocked() )
//        {            
//            activitySum += clause.activity();
//            ++activityCount;
//            if ( clause.activity() < threshold )
//            {
//                deleteLearnedClause( i );
//                numberOfDeletions++;
//            }
//            else
//            {
//                *j = *i;
//                j++;
//            }
//        }
//        else
//        {
//            *j = *i;
//            j++;
//        }
//
//        ++i;
//    } while( i != learnedClauses_end() );
//    
//    finalizeDeletion( size - numberOfDeletions );
//    
//    numberOfDeletions = 0;
//    size = numberOfLearnedClauses();
//
//    if( activityCount > 0 && numberOfDeletions < halfSize )
//    {
//        activitySum = activitySum / activityCount;
//        i = learnedClauses_begin();
//        j = learnedClauses_begin();
//        assert( i != learnedClauses_end() );
//        do
//        {
//            if( numberOfDeletions < halfSize )
//            {
//                Clause& clause = **i;
//
//                if( !clause.isLocked() && clause.activity() < activitySum )
//                {
//                    deleteLearnedClause( i );
//                    numberOfDeletions++;
//                }
//                else
//                {
//                    *j = *i;
//                    j++;
//                }
//            }
//            else
//            {
//                *j = *i;
//                j++;
//            }
//            ++i;
//        } while( i != learnedClauses_end() );
//    }
//    
//    finalizeDeletion( size - numberOfDeletions );
//    statistics( onDeletion( size, numberOfDeletions ) );
//}

void
Solver::updateActivity( 
    Clause* learnedClause )
{
    assert_msg( learnedClause != NULL, "It is not possible to update activity of NULL clauses.");
    if( ( learnedClause->activity() += deletionCounters.increment ) > 1e20 )
    {
        for( ClauseIterator it = learnedClauses_begin(); it != learnedClauses_end(); ++it )
        {
            ( *it )->activity() *= 1e-20;
        }

        deletionCounters.increment *= 1e-20;
    }
}

void
Solver::simplifyOnRestart()
{
    if( variables.numberOfAssignedLiterals() == assignedVariablesAtLevelZero || nextValueOfPropagation > 0 )
        return;

    removeSatisfied( learnedClauses );    
    //Maybe in future we want to disable this function.
    removeSatisfied( clauses );

    assignedVariablesAtLevelZero = variables.numberOfAssignedLiterals();
    nextValueOfPropagation = literalsInClauses + literalsInLearnedClauses;
}

void
Solver::removeSatisfied(
    vector< Clause* >& clauses )
{
    assert( currentDecisionLevel == 0 );
    for( unsigned int i = 0; i < clauses.size(); )
    {
        assert_msg( clauses[ i ] != NULL, "Current clause is NULL" );
        Clause* current = clauses[ i ];
        uint64_t size = current->size();
        if( removeSatisfiedLiterals( *current ) )
        {
            detachClause( *current );
            if( current->isLearned() )
                literalsInLearnedClauses -= size;
            else
                literalsInClauses -= size;
            assert( !isLocked( *current ) );
            delete current;
            clauses[ i ] = clauses.back();            
            clauses.pop_back();            
        }
        else
        {
            ++i;
            uint64_t newsize = current->size();
            size -= newsize;
            if( current->isLearned() )
                literalsInLearnedClauses -= size;
            else
                literalsInClauses -= size;
        }
    }
}

bool
Solver::checkVariablesState()
{
    assert( currentDecisionLevel == 0 );
    for( unsigned i = 0; i < clauses.size(); i++ )
    {
        Clause& clause = *clauses[ i ];
        for( unsigned j = 0; j < clause.size(); j++ )
        {
            Literal l = clause[ j ];
            if( !isUndefined( l ) )
            {
                cout << "Literal " << l << " in position " << j << " is " << ( isTrue( l ) ? "true" : "false" ) << " - occurrences " << numberOfOccurrences( l ) << endl;
                cout << "All occurrences: " << endl;
                for( unsigned k = 0; k < numberOfOccurrences( l ); k++ )
                    cout << *( getOccurrence( l, k ) ) << endl;
                cout << clause << endl;
                return false;                
            }
        }
    }
    
    return true;
}

void
Solver::checkSubsumptionForClause(    
    Clause* clause,
    Literal lit )
{
//    Vector< Clause* >& wl = variableAllOccurrences[ lit.getIndex() ];    
    Vector< Clause* >& wl = getDataStructure( lit ).variableAllOccurrences;
    unsigned j = 0;
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        Clause* currentPointer = wl[ j ] = wl[ i ];
        Clause& current = *currentPointer;
        
        assert( !current.hasBeenDeleted() );
        trace_msg( satelite, 2, "Considering clause " << current );
        if( clause != currentPointer && current.size() < getSatelite()->getSubsumptionLimit() )
        {
            SubsumptionData data = clause->subsumes( current );
            if( data == SUBSUMPTION )
            {
                trace_msg( satelite, 1, "Clause " << *clause << " subsumes clause " << current );
                detachClauseFromAllLiterals( current, lit );                
                markClauseForDeletion( currentPointer );                
            }
            else if( data == SELFSUBSUMPTION )
            {
                if( current[ current.size() - 1 ] != lit )
                {
                    if( current.size() > 2 )
                        ++j;
                    else
                        assert( current[ 0 ] == lit );
                    findAndEraseClause( current[ current.size() -  1 ], currentPointer );
                }
                current.removeLastLiteralNoWatches();
                if( current.size() == 1 )
                {
                    getSatelite()->addTrueLiteral( current[ 0 ] );
                    if( current[ 0 ] != lit )
                        detachClauseFromAllLiterals( current );
                    markClauseForDeletion( currentPointer );
                }
                else
                {
                    current.recomputeSignature();
                    onStrengtheningClause( currentPointer );
                    trace_msg( satelite, 2, "Clause after removing literal is: " << current );
                }
            }
            else
            {
                ++j;
            }
        }
        else
        {
            ++j;
        }
    }    
    wl.shrink( j );
}