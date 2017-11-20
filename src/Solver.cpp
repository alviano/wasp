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
#include "propagators/HCComponent.h"
#include "propagators/ReductBasedCheck.h"
#include "propagators/UnfoundedBasedCheck.h"
#include "weakconstraints/WeakInterface.h"
#include <algorithm>
#include <stdint.h>
#include <vector>
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
    
//    while( !poolOfClauses.empty() )
//    {
//        assert( poolOfClauses.back() );
//        delete poolOfClauses.back();
//        poolOfClauses.pop_back();
//    }
    
    while( !gusDataVector.empty() )
    {
        delete gusDataVector.back();
        gusDataVector.pop_back();
    }
    
    while( !propagators.empty() )
    {
        assert( propagators.back() );
        delete propagators.back();
        propagators.pop_back();
    }
    
    while( !disjunctionPropagators.empty() )
    {
        assert( disjunctionPropagators.back() );
        delete disjunctionPropagators.back();
        disjunctionPropagators.pop_back();
    }
    
    while( !externalPropagators.empty() )
    {
        assert( externalPropagators.back() );
        delete externalPropagators.back();
        externalPropagators.pop_back();
    }
    
    while( !cyclicComponents.empty() )
    {
        delete cyclicComponents.back();
        cyclicComponents.pop_back();
    }
    
    while( !hcComponents.empty() )
    {
        delete hcComponents.back();
        hcComponents.pop_back();
    }
    
    delete satelite;
    delete restart;
//    delete optimizationAggregate;
    delete dependencyGraph;
    
    while( !variableDataStructures.empty() )
    {
        delete variableDataStructures.back();
        variableDataStructures.pop_back();
    }
    
    delete choiceHeuristic;
    
    for( unsigned int i = 1; i <= variables.numberOfVariables(); i++ )
    {        
        if( variables.hasBeenEliminatedByDistribution( i ) )
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
    
    for( unsigned int j = 0; j < optimizationLiterals.size(); j++ )
        for( unsigned int i = 0; i < optimizationLiterals[ j ].size(); i++ )
            delete optimizationLiterals[ j ][ i ];
}

void
Solver::initFrom(
    Solver& solver )
{   
    assert( solver.restart != NULL );
    assert( this->restart == NULL );    
    this->restart = solver.restart->clone();
    this->glucoseHeuristic_ = solver.glucoseHeuristic_;
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
    
//    for( unsigned int i = 0; i < aggregates.size(); i++ )
//    {
//        aggregates[ i ]->reset( *this );
//    }
        
    while( fromLevelToPropagators.size() - 1 > level )
    {
        unsigned int oldSize = fromLevelToPropagators.back();
        while( propagatorsForUnroll.size() > oldSize )
        {
            Propagator* prop = propagatorsForUnroll.back();
            prop->reset( *this );
            propagatorsForUnroll.pop_back();
            prop->setInVectorOfUnroll( UINT_MAX );
        }
        fromLevelToPropagators.pop_back();
    }    
    
    for( unsigned int i = 0; i < hcComponents.size(); i++ )
        hcComponents[ i ]->reset();
    
//    if( optimizationAggregate != NULL )
//    {
//        optimizationAggregate->reset( *this );
//    }
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
    
    unrollToZero();
    simplifyOnRestart();    
    clearConflictStatus();
    
    return addClauseFromModel( clause );
}

unsigned int 
Solver::solveWithoutPropagators(
    vector< Literal >& assumptions )
{
    time_t START_TIME = time( 0 );
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
        
        deleteClausesIfNecessary();

        assert( !conflictDetected() );
        if( !chooseLiteral( assumptions ) )
        {
            trace_msg( solving, 1, "INCONSISTENT" );
            return INCOHERENT;
        }
        if( ++numberOfChoices > maxNumberOfChoices  || numberOfRestarts > maxNumberOfRestarts || ( time( 0 ) - START_TIME ) > maxNumberOfSeconds )
            return INTERRUPTED;        
        
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
                    return INCOHERENT;
                }
                
                if( !analyzeConflict() )
                {
                    trace_msg( solving, 1, "INCONSISTENT" );
                    return INCOHERENT;
                }
                choiceHeuristic->onConflict();
                assert_msg( hasNextVariableToPropagate() || getCurrentDecisionLevel() == numberOfAssumptions || getCurrentDecisionLevel() == 0, getCurrentDecisionLevel() << " != " << numberOfAssumptions );
            }
        }
        
        restartIfNecessary();
    }
    
    completeModel();
    assert_msg( getNumberOfUndefined() == 0, "Found a model with " << getNumberOfUndefined() << " undefined variables." );
    assert_msg( allClausesSatisfied(), "The model found is not correct." );        
    
    if( modelIsValidUnderAssumptions( assumptions ) )
    {
        notifyAnswerSet();
        return COHERENT;
    }    
    return INCOHERENT;
}

bool
Solver::handlePropagatorFailure(
    ExternalPropagator* propagator )
{
    Clause* clausePointer = propagator->getReasonForCheckFailure( *this );
    if( clausePointer == NULL )
    {
        assert( getCurrentDecisionLevel() == 0 );
        return !conflictDetected();
    }
    
    Clause& clause = *clausePointer;
    unsigned int size = clause.size();
    statistics( this, onLearningFromPropagators( size ) );
    assert( !conflictDetected() );
    if( size == 0 )
    {
        delete clausePointer;
        return false;
    }
    else if( size == 1 )
    {
        unrollToZero();
        assignLiteral( clause[ 0 ] );
        delete clausePointer;
    }
    else
    {
        assert( !isUndefined( clause[ 1 ] ) );
        unsigned int dl = getDecisionLevel( clause[ 1 ] );
        if( dl < getCurrentDecisionLevel() )
        {
            trace( solving, 2, "Learned clause from check failure and backjumping to level %d.\n", dl );
            unroll( dl );
        }

        addLearnedClause( clausePointer, false );
        onLearning( clausePointer );

        assert( !isUndefined( clause[ 1 ] ) );
        assert( !isTrue( clause[ 0 ] ) );
        assignLiteral( clausePointer );        
    }
    return true;
}

unsigned int 
Solver::solvePropagators(
    vector< Literal >& assumptions )
{
    time_t START_TIME = time( 0 );    
    trace( solving, 1, "Starting solving.\n" );    
    if( hasNextVariableToPropagate() )
        goto propagationLabel;
    
    if( !postPropagators.empty() )
        goto postPropagationLabel;    
    
    while( hasUndefinedLiterals() )
    {        
        deleteClausesIfNecessary();
        
        assert( !conflictDetected() );
        if( !chooseLiteral( assumptions ) )
        {
            trace_msg( solving, 1, "Failure occurs while choosing a new Literal" );
            return INCOHERENT;
        }
        if( ++numberOfChoices > maxNumberOfChoices || numberOfRestarts > maxNumberOfRestarts || ( time( 0 ) - START_TIME ) > maxNumberOfSeconds )
            return INTERRUPTED;        
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
                learnedFromConflicts++;
                trace_msg( solving, 1, "Conflict detected at level " << getCurrentDecisionLevel() );
                if( getCurrentDecisionLevel() == 0 )
                {
                    trace( solving, 1, "Conflict at level 0: return. \n");
                    return INCOHERENT;
                }
                
                if( !analyzeConflict() )
                {
                    trace_msg( solving, 1, "Failure occurs during the computation of the clause" );
                    return INCOHERENT;
                }
                choiceHeuristic->onConflict();
                assert_msg( hasNextVariableToPropagate() || getCurrentDecisionLevel() == numberOfAssumptions || getCurrentDecisionLevel() == 0, getCurrentDecisionLevel() << " != " << numberOfAssumptions );
            }
        }
        
        #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
        for( unsigned int i = 0; i < propagatorsAfterUnit.size(); i++ )
        {
            propagatorsAfterUnit[ i ]->endUnitPropagation( *this );
            if( conflictDetected() )
                goto conflict;
            else if( hasNextVariableToPropagate() )
                goto propagationLabel;
        }
        #endif    
        
        postPropagationLabel:;
        while( !postPropagators.empty() )
        {
            PostPropagator* postPropagator = postPropagators.back();
            Clause* clauseToPropagate = postPropagator->getClauseToPropagate( learning );
            if( clauseToPropagate == NULL )
            {
                
                assert( !conflictDetected() );
                assert( !hasNextVariableToPropagate() );                
                postPropagators.pop_back();
                postPropagator->onRemoving();
            }
            else
            {
                assert( !conflictDetected() );
                if( wasp::Options::heuristicPartialChecks )
                {
                    if( learnedFromPropagators > ( learnedFromConflicts * 0.5 ) )
                        wasp::Options::forwardPartialChecks = false;
                    else
                        wasp::Options::forwardPartialChecks = true;
                }
                unsigned int size = clauseToPropagate->size();
                statistics( this, onLearningFromPropagators( size ) );
                if( size == 0 )
                {
                    clearConflictStatus();
                    delete clauseToPropagate;
                    return INCOHERENT;
                }
                else if( size == 1 )
                {
                    if( getCurrentDecisionLevel() != 0 )
                    {
                        clearConflictStatus();
                        unrollToZero();
                    }
                    assignLiteral( clauseToPropagate->getAt( 0 ) );
                    delete clauseToPropagate;
                }
                else
                {
                    if( !isUndefined( clauseToPropagate->getAt( 1 ) ) && getDecisionLevel( clauseToPropagate->getAt( 1 ) ) < getCurrentDecisionLevel() )
                    {
                        clearConflictStatus();
                        trace( solving, 2, "Learned clause from propagator and backjumping to level %d.\n", getDecisionLevel( clauseToPropagate->getAt( 1 ) ) );
                        unroll( getDecisionLevel( clauseToPropagate->getAt( 1 ) ) );
                    }
                    
                    if( postPropagator->hasToAddClause() )
                    {
                        addLearnedClause( clauseToPropagate, false );
                        onLearning( clauseToPropagate );
                    }
                    if( !isUndefined( clauseToPropagate->getAt( 1 ) ) )
                    {
                        assert( !isTrue( clauseToPropagate->getAt( 0 ) ) );
                        assignLiteral( clauseToPropagate );
                    }                    
                }
                
                if( conflictDetected() )
                    goto conflict;
                else
                    goto propagationLabel;
            }
        }                
        
        #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
        for( unsigned int i = 0; i < propagatorsAttachedToEndPropagation.size(); i++ )
        {
            propagatorsAttachedToEndPropagation[ i ]->endPropagation( *this );
            if( conflictDetected() )
                goto conflict;
            else if( hasNextVariableToPropagate() )
                goto propagationLabel;
        }
        #endif
        
        restartIfNecessary();

        #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
        for( unsigned int i = 0; i < propagatorsAttachedToPartialChecks.size(); i++ )
            if( !propagatorsAttachedToPartialChecks[ i ]->checkPartialInterpretation( *this ) )
            {
                if( !handlePropagatorFailure( propagatorsAttachedToPartialChecks[ i ] ) )
                    return INCOHERENT;
                if( conflictDetected() )
                    goto conflict;
                else
                    goto propagationLabel;
            }
        #endif
    }
    
    #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
    for( unsigned int i = 0; i < propagatorsAttachedToCheckAnswerSet.size(); i++ )
        if( !propagatorsAttachedToCheckAnswerSet[ i ]->checkAnswerSet( *this ) )
        {
            if( !handlePropagatorFailure( propagatorsAttachedToCheckAnswerSet[ i ] ) )
                return INCOHERENT;
            if( conflictDetected() )
                goto conflict;
            else
                goto propagationLabel;
        }
    #endif    
    completeModel();
    assert_msg( getNumberOfUndefined() == 0, "Found a model with " << getNumberOfUndefined() << " undefined variables." );
    assert_msg( allClausesSatisfied(), "The model found is not correct." );
    
    if( modelIsValidUnderAssumptions( assumptions ) )
    {
        notifyAnswerSet();
        return COHERENT;
    }    
    return INCOHERENT;
}

unsigned int
Solver::estimateBinaryPropagation(
    Literal lit )
{
    assert( !conflictDetected() );    
    setTrue( lit );
    Var variable = lit.getVariable();
    unsigned int orig = numberOfAssignedLiterals();
    shortPropagation( variable );
    unsigned int value = numberOfAssignedLiterals() - orig;
    if( conflictDetected() ) { conflictLiteral = Literal::null; conflictClause = NULL; }
    while( numberOfAssignedLiterals() != value ) variables.unrollLastVariable();
    return value;
}

void
Solver::shortPropagation(
    Var variable )
{
    assert( !conflictDetected() );
    
    Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );    
//    Vector< Literal >& binary = variableBinaryClauses[ ( getTruthValue( variable ) >> 1 ) ];    
    Vector< Literal >& binary = getDataStructure( complement ).variableBinaryClauses;
    
    trace_msg( solving, 5, "Propagation of binary clauses for literal " << complement );
    for( unsigned i = 0; i < binary.size(); ++i )
    {
        if( conflictDetected() )
            break;
        
        Literal lit = binary[ i ];
        if( !isTrue( lit ) )
        {
            trace_msg( solving, 6, "Inferring " << lit << " as true" );        
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
        trace_msg( solving, 6, "Considering clause " << *clause );
        if( onLiteralFalse( *clause, complement ) )
        {
            trace_msg( solving, 7, "Inferring literal " << clause->getAt( 0 ) << " using clause " << *clause );
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
    Vector< pair< Propagator*, PropagatorData > >& wl = getDataStructure( complement ).variablePropagators;
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        if( conflictDetected() )
            break;
        Propagator* propagator = wl[ i ].first;
        assert( "Post propagator is null." && propagator != NULL );
        bool res = propagator->onLiteralFalse( *this, complement, wl[ i ].second );        
        if( res )
            addInPropagatorsForUnroll( propagator );
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
    Vector< PostPropagator* >& wl = getDataStructure( complement ).variablePostPropagators;    
    
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        PostPropagator* postPropagator = wl[ i ];
        assert( "Post propagator is null." && postPropagator != NULL );
        bool res = postPropagator->onLiteralFalse( complement );
        
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
    
    trace_msg( solving, 5, "Propagating " << variables.createLiteralFromAssignedVariable( variable ) << " as true at level 0 (Satelite)" );
    assert( !conflictDetected() );
    {
        Literal literal = variables.createLiteralFromAssignedVariable( variable );        
    
//        Vector< Clause* >& wl = variableAllOccurrences[ 1 - ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( literal ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];            
            assert( !clause->hasBeenDeleted() );
            trace_msg( solving, 6, "Considering clause " << *clause );
            detachClauseFromAllLiterals( *clause, literal );            
            markClauseForDeletion( clause );
        }
        wl.clearAndDelete();
    }
    
    {
        assert( !conflictDetected() );
        Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );        
        
//        Vector< Clause* >& wl = variableAllOccurrences[ ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( complement ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            assert_msg( clause != NULL, "Next clause to propagate is null" );
            trace_msg( solving, 6, "Considering clause " << *clause );
            clause->removeLiteral( complement );
            if( clause->size() == 1 )
            {
                if( !isTrue( clause->getAt( 0 ) ) )
                {
                    trace_msg( solving, 7, "Assigning literal " << clause->getAt( 0 ) << " as true" );
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
    trace_msg( solving, 5, "Propagating " << variables.createLiteralFromAssignedVariable( variable ) << " as true at level 0" );
    
    shortPropagation( variable );
    if( conflictDetected() )
        return;    
    
    {        
        Literal literal = variables.createLiteralFromAssignedVariable( variable );
//        Vector< Clause* >& wl = variableAllOccurrences[ 1 - ( getTruthValue( variable ) >> 1 ) ];       
        Vector< Clause* >& wl = getDataStructure( literal ).variableAllOccurrences;
     
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            trace_msg( solving, 6, "Considering clause " << *clause );
            detachClauseFromAllLiterals( *clause, literal );
            deleteClause( clause );
        }
        wl.clearAndDelete();
    }
    
    {
        assert( !conflictDetected() );
        Literal complement = variables.createOppositeLiteralFromAssignedVariable( variable );
        
//        Vector< Clause* >& wl = variableAllOccurrences[ ( getTruthValue( variable ) >> 1 ) ];
        Vector< Clause* >& wl = getDataStructure( complement ).variableAllOccurrences;
        
        for( unsigned i = 0; i < wl.size(); ++i )
        {
            Clause* clause = wl[ i ];
            assert_msg( clause != NULL, "Next clause to propagate is null" );
            trace_msg( solving, 6, "Considering clause " << *clause );
            clause->removeLiteral( complement );
            if( clause->size() == 1 )
            {
                if( !isTrue( clause->getAt( 0 ) ) )
                {
                    trace_msg( solving, 7, "Assigning literal " << clause->getAt( 0 ) << " as true" );
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
        {
            cerr << "Clause " << clause << " is not satisfied"  << endl;
            return false;
        }
    }

    return true;
}

bool compareClauses( Clause* c1, Clause* c2 ){ return c1->activity() < c2->activity(); }

void
Solver::minisatDeletion()
{
    ClauseIterator i = learnedClauses_begin();
    ClauseIterator j = learnedClauses_begin();
    Activity threshold = deletionCounters.increment / numberOfLearnedClauses();
    
    stable_sort( learnedClauses.begin(), learnedClauses.end(), compareClauses );    
        
    unsigned int numberOfDeletions = 0;
    unsigned int size = numberOfLearnedClauses();
    unsigned int toDelete = size / 2;
    while( i != learnedClauses.end() )
    {
        Clause& clause = **i;
        if( /*clause.size() > 2 &&*/ !isLocked( clause ) && ( numberOfDeletions < toDelete || clause.activity() < threshold ) )
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
    statistics( this, onDeletion( size, numberOfDeletions ) );
}

bool compareClausesGlucose( Clause* c1Pointer, Clause* c2Pointer )
{
    Clause& c1 = *c1Pointer;
    Clause& c2 = *c2Pointer;
    
    if( c2.size() == 2 )
        return c1.size() > 2;
    else
    {
        assert( c2.size() > 2 );
        if( c1.size() == 2 )
            return false;
        
        assert( c1.size() > 2 );        
        if( c1.lbd() != c2.lbd() )
            return c1.lbd() > c2.lbd();            
        else
            return c1.activity() < c2.activity();
    }
    
    assert( 0 );
    
    //GLUCOSE VERSION:
    
//    bool operator () (CRef x, CRef y) { 
// 
//    // Main criteria... Like in MiniSat we keep all binary clauses
//    if(ca[x].size()> 2 && ca[y].size()==2) return 1;
//    
//    if(ca[y].size()>2 && ca[x].size()==2) return 0;
//    if(ca[x].size()==2 && ca[y].size()==2) return 0;
//    
//    // Second one  based on literal block distance
//    if(ca[x].lbd()> ca[y].lbd()) return 1;
//    if(ca[x].lbd()< ca[y].lbd()) return 0;    
//    
//    
//    // Finally we can use old activity or size, we choose the last one
//        return ca[x].activity() < ca[y].activity();
//	//return x->size() < y->size();
//
//        //return ca[x].size() > 2 && (ca[y].size() == 2 || ca[x].activity() < ca[y].activity()); } 
//    } 
}

void
Solver::glucoseDeletion()
{
    assert( glucoseHeuristic_ );
    ClauseIterator i = learnedClauses_begin();
    ClauseIterator j = learnedClauses_begin();
    
    stable_sort( learnedClauses.begin(), learnedClauses.end(), compareClausesGlucose );
    
    assert( !learnedClauses.empty() );
    if( learnedClauses[ learnedClauses.size() / 2 ]->lbd() <= 3 )
        glucoseData.nbclausesBeforeReduce += glucoseData.specialIncReduceDB;
    
    if( learnedClauses.back()->lbd() <= 5 )
        glucoseData.nbclausesBeforeReduce += glucoseData.specialIncReduceDB;
    
    unsigned int numberOfDeletions = 0;
    unsigned int size = numberOfLearnedClauses();
    unsigned int toDelete = size / 2;
    while( i != learnedClauses.end() )
    {
        Clause& clause = **i;
        if( clause.lbd() > 2 && /*clause.size() > 2 &&*/ clause.canBeDeleted() && !isLocked( clause ) && ( numberOfDeletions < toDelete ) )
        {
            deleteLearnedClause( i );
            numberOfDeletions++;
        }
        else
        {
            if( !clause.canBeDeleted() )
            {
                toDelete++;
                clause.setCanBeDeleted( true );
            }
            *j = *i;
            ++j;
        }
        
        ++i;
    }

    finalizeDeletion( size - numberOfDeletions );
    statistics( this, onDeletion( size, numberOfDeletions ) );
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
//    statistics( this, onDeletion( size, numberOfDeletions ) );
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
    assert( incremental_ || currentDecisionLevel == 0 );
    if( currentDecisionLevel != 0 )
        return;
    
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
        Clause* currentPointer = clauses[ i ];
        Clause& current = *currentPointer;
        
        if( !current.isLearned() && !current.canBeDeleted() )
        {
            ++i;
            continue;
        }

        uint64_t size = current.size();
        if( incremental_ )
        {            
            assert( size > 1 );
            if( isTrue( current[ 0 ] ) || isTrue( current[ 1 ] ) )
            {
                detachClause( current );
                if( current.isLearned() )
                    literalsInLearnedClauses -= size;
                else
                    literalsInClauses -= size;
                if( isLocked( current ) )
                    setImplicant( current[ 0 ].getVariable(), NULL );
                assert( !isLocked( current ) );
                delete currentPointer;
                clauses[ i ] = clauses.back();            
                clauses.pop_back();
            }
            else
            {
                ++i;
            }
        }
        else
        {
            if( removeSatisfiedLiterals( current ) )
            {
                detachClause( current );
                if( current.isLearned() )
                    literalsInLearnedClauses -= size;
                else
                    literalsInClauses -= size;
                assert( !isLocked( current ) );
                delete currentPointer;
                clauses[ i ] = clauses.back();            
                clauses.pop_back();            
            }
            else
            {
                ++i;
                uint64_t newsize = current.size();
                size -= newsize;
                if( current.isLearned() )
                    literalsInLearnedClauses -= size;
                else
                    literalsInClauses -= size;
            }
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

bool
Solver::isSubsumed(    
    Clause* clause )
{    
    Literal lit = getLiteralWithMinOccurrences( *clause );    
    Vector< Clause* >& wl = getDataStructure( lit ).variableAllOccurrences;    
    for( unsigned i = 0; i < wl.size(); ++i )
    {
        Clause& current = *wl[ i ];        
        assert( !current.hasBeenDeleted() );
        if( clause != wl[ i ] )
        {
            if( current.checkEquality( *clause ) )
                return true;
        }
    }    
    
    return false;
}

void
Solver::createCyclicComponents()
{
    assert( dependencyGraph != NULL );
    unsigned int id = 0;
    unsigned int numberOfComponents = dependencyGraph->numberComponents();    
    for( unsigned int i = 0; i < numberOfComponents; i++ )
    {
        vector< Var >& current = dependencyGraph->getComponent( i );
        unsigned int size = current.size();
        if( size > 1 )
        {            
            statistics( this, addCyclicComponent( size ) );
            Component* currentComponent = new Component( gusDataVector, *this );
            cyclicComponents.push_back( currentComponent );
            currentComponent->setId( id++ );
            
            for( unsigned int j = 0; j < size; j++ )
                currentComponent->addVariable( current[ j ] );            
        }
    }
}

HCComponent*
Solver::createHCComponent(
    unsigned numberOfInputAtoms )
{
    switch( wasp::Options::modelcheckerAlgorithm )
    {
        case UNFOUNDED_BASED:
            return new UnfoundedBasedCheck( gusDataVector, *this, numberOfInputAtoms );
        case REDUCT_BASED:
        default:
            return new ReductBasedCheck( gusDataVector, *this, numberOfInputAtoms );            
    }    
}

void
Solver::printLearnedClauses()
{
    for( unsigned int i = 0; i < learnedClauses.size(); i++ )
    {
        cout << learnedClauses[ i ]->size() << " ";
        learnedClauses[ i ]->printOrderedById();
        cout << " " << this << endl;
    }
}

bool
Solver::cleanAndAddLearnedClause(    
    Clause* clause )
{
    assert( clause != NULL );
    if( clause->removeDuplicatesAndFalseAndCheckIfTautological( *this ) )
    {        
        trace_msg( solving, 10, "Found tautological clause: " << *clause );
        delete clause;
        return true;
    }
    
    if( clause->size() == 0 )
    {        
        trace_msg( solving, 10, "Found contradictory (empty) clause" );
        conflictLiteral = Literal::conflict;        
        delete clause;
        return false;
    }
    
    assert( allUndefined( *clause ) );
    if( clause->size() == 1 )
    {
        clearConflictStatus();
        assignLiteral( clause->getAt( 0 ) );
        assert( isTrue( clause->getAt( 0 ) ) );
        assert( !conflictDetected() );
        
        delete clause;

        while( hasNextVariableToPropagate() )
        {
            nextValueOfPropagation--;            
            Var variableToPropagate = getNextVariableToPropagate();
            if( hasPropagators() )
                propagateWithPropagators( variableToPropagate );
            else
                propagate( variableToPropagate );

            if( conflictDetected() )
                return false;
        }
        
        simplifyOnRestart();
    }
    else
    {
        clause->setLearned();
        addLearnedClause( clause, true );    
    }
    return true;
}

void
Solver::addLearnedClause( 
    Clause* learnedClause,
    bool optimizeBinaryClause )
{
    assert( learnedClause != NULL );
    literalsInLearnedClauses += learnedClause->size();
    if( optimizeBinaryClause && learnedClause->size() == 2 )
    {
        learnedClause->setOriginal();
        addBinaryClauseRuntime( learnedClause );
    }
    else
    {
        attachClause( *learnedClause );
        learnedClauses.push_back( learnedClause );        
    }    
}

void
Solver::clearAfterSolveUnderAssumptions(
    const vector< Literal >& assumptions )
{
    for( unsigned int i = 0; i < assumptions.size(); i++ )
        setAssumption( assumptions[ i ], false );    
}

bool compareWeight( OptimizationLiteralData* o1, OptimizationLiteralData* o2 )
{
    return o1->weight > o2->weight;
}

void
Solver::sortOptimizationLiterals( unsigned int level )
{
    if( !isWeighted( level ) )
        return;
    assert( level < optimizationLiterals.size() );
    stable_sort( optimizationLiterals[ level ].begin(), optimizationLiterals[ level ].end(), compareWeight );
}

void
Solver::getChoicesWithoutAssumptions(
    vector< Literal >& assums )
{
    for( unsigned int i = assums.size() + 1; i <= currentDecisionLevel; i++ )
    {
        assert( i < choices.size() );
        assert( isTrue( choices[ i ] ) );
        assert( !hasImplicant( choices[ i ].getVariable() ) );
        assert( getDecisionLevel( choices[ i ] ) == i );
        trace_msg( enumeration, 2, "Adding literal " << choices[ i ] << " in assumptions." );        
        assums.push_back( choices[ i ] );        
    }
}
