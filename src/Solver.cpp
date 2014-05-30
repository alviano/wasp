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
        aggregates[ i ]->reset();
    }
    
    if( optimizationAggregate != NULL )
    {
        optimizationAggregate->reset();
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
        Variable* v = variables[ i ];
        assert( !v->isUndefined() );
        
        trace_msg( enumeration, 3, "Checking literal " << *v << " with decision level " << v->getDecisionLevel() << " and its implicant is " << ( v->hasImplicant() ? "not null" : "null" ) );
        if( !v->hasImplicant() && v->getDecisionLevel() != 0 )
        {
            if( v->isTrue() )
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
            Variable* variableToPropagate = getNextVariableToPropagate();
            unitPropagation( variableToPropagate );

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
                minisatHeuristic.variableDecayActivity();                
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
        Variable* variableToPropagate;
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
                minisatHeuristic.variableDecayActivity();                
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
                    if( clauseToPropagate->getAt( 1 ).getDecisionLevel() < getCurrentDecisionLevel() )
                    {
                        clearConflictStatus();                    
                        trace( solving, 2, "Learned clause from propagator and backjumping to level %d.\n", clauseToPropagate->getAt( 1 ).getDecisionLevel() );            
                        unroll( clauseToPropagate->getAt( 1 ).getDecisionLevel() );
                    }
                    addLearnedClause( clauseToPropagate );
                    assert( !clauseToPropagate->getAt( 0 ).isTrue() );
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
Solver::unitPropagation(
    Variable* variable )
{
    assert( "Variable to propagate has not been set." && variable != NULL );
    trace_msg( solving, 2, "Propagating: " << *variable << " as " << ( variable->isTrue() ? "true" : "false" ) );
    variable->shortPropagation( *this );
    
    if( conflictDetected() )
        return;
    
    variable->unitPropagation( *this );
}

void
Solver::postPropagation(
    Variable* variable )
{
    assert( "Variable to propagate has not been set." && variable != NULL );
    if( conflictDetected() )
        return;
    trace_msg( solving, 2, "Post Propagating: " << *variable << " as " << ( variable->isTrue() ? "true" : "false" ) );    
    variable->propagation( *this );
    
    if( conflictDetected() )
        return;

    variable->postPropagation( *this );
    
    assert( !conflictDetected() );
}

void
Solver::propagateAtLevelZero(
    Variable* variable )
{
    assert( "Variable to propagate has not been set." && variable != NULL );    
    variable->propagateAtLevelZero( *this );    
    if( !conflictDetected() && hasPropagators() )
        postPropagation( variable );
}

void
Solver::propagateAtLevelZeroSatelite(
    Variable* variable )
{
    if( variable->hasBeenEliminated() )    
        return;
    
    assert( "Variable to propagate has not been set." && variable != NULL );    
    variable->propagateAtLevelZeroSatelite( *this );
    if( !conflictDetected() && hasPropagators() )
        postPropagation( variable );
}

void
Solver::printProgram() const
{
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        cout << *( *it ) << endl;
    }
    
    for( unsigned int i = 0; i < aggregates.size(); i++ )
    {
        cout << *aggregates[ i ] << endl;
    }
}

void
Solver::printDimacs() const
{
    cout << "p cnf " << numberOfClauses() << " " << numberOfVariables() << endl;
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        (**it).printDimacs();
    }
}

unsigned int
Solver::getNumberOfUndefined() const
{
    unsigned countUndef = 0;
    for( unsigned int i = 1; i <= variables.numberOfVariables(); i++ )
    {
        Variable const* var = variables[ i ];
        if( var->isUndefined() )
        {
            countUndef++;
        }
    }

    return countUndef;
}

bool
Solver::allClausesSatisfied() const
{
    for( ConstClauseIterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        Clause& clause = *( *it );
        if( !clause.isSatisfied() )
        {
//            cout << clause << endl;
            return false;
        }
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
        if( clause.size() > 2 && !clause.isLocked() && ( numberOfDeletions < toDelete || clause.activity() < threshold ) )
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
        if( current->removeSatisfiedLiterals() )
        {
            current->detachClause();            
            if( current->isLearned() )
                literalsInLearnedClauses -= size;
            else
                literalsInClauses -= size;
            assert( !current->isLocked() );
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
Solver::checkVariablesState() const
{
    assert( currentDecisionLevel == 0 );
    for( unsigned i = 0; i < clauses.size(); i++ )
    {
        Clause* clause = clauses[ i ];
        for( unsigned j = 0; j < clause->size(); j++ )
            if( !clause->getAt( j ).isUndefined() )
            {
                cout << j << " " << clause->getAt( j ).isTrue() << endl;
                cout << clause->getAt( j ).numberOfOccurrences() << endl;
                for( unsigned k = 0; k < clause->getAt( j ).numberOfOccurrences(); k++)
                    cout << *clause->getAt(j).getOccurrence(k) << endl;
                cout << *clause << endl;
                return false;                
            }
    }
    
    return true;
}
