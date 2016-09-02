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

#include "PredicateMinimization.h"
#include "Solver.h"
#include "Clause.h"

#include <set>
using namespace std;

unsigned int
PredicateMinimization::minimize()
{
    assert( !solver.conflictDetected() );
    solver.copyAtomToMinimize( atomsToMinimize );
    if( wasp::Options::checkTrivialSolutionPredMin )
    {
        if( checkTrivialSolution() )
            return COHERENT;
    }
    
    switch( wasp::Options::predMinimizationAlgorithm )
    {
        case PREDMIN_ENUMERATION:
            return enumerationBC();
            
        case PREDMIN_GUESS_AND_CHECK:
            return guessAndCheck();
        
        case PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE:
            return guessAndCheckAndMinimize();
            
        case PREDMIN_GUESS_AND_CHECK_AND_SPLIT:
            return guessAndCheckAndSplit();
            
        case NO_PREDMINIMIZATION:
        default:
            ErrorMessage::errorGeneric( "Invalid option for predicate minimization" );            
    }    
    return 0;
}

bool
PredicateMinimization::computeFirstModel()
{
    unsigned int result = solver.solve();
    assert( result == COHERENT || result == INCOHERENT );
    return result != INCOHERENT;    
}

unsigned int
PredicateMinimization::countTrue()
{
    unsigned int count = 0;
    for( unsigned int i = 0; i < atomsToMinimize.size(); i++ )
        if( solver.isTrue( atomsToMinimize[ i ] ) )
            count++;
    return count;
}

void
PredicateMinimization::computeTrueVars()
{
    trueVars.clear();
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        if( solver.isTrue( i ) )
            trueVars.push_back( i );    
    trace_action( predmin, 2, { trace_tag( cerr, predmin, 2 ); printVectorOfVars( trueVars, "Found answer set" ); } );
}

void
PredicateMinimization::printTrueVars()
{    
    OutputBuilder* output = solver.getOutputBuilder();
    output->startModel();
    for( unsigned int i = 0; i < trueVars.size(); i++ )
        output->printVariable( trueVars[ i ], true );
    output->endModel();
}

unsigned int
PredicateMinimization::enumerationBC()
{
    trace_msg( predmin, 1, "Starting enumeration algorithm" );
    solver.turnOffSimplifications();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    
    if( !computeFirstModel() )
        return INCOHERENT;
    
    unsigned int min = countTrue();
    trace_msg( predmin, 2, "First answer set contains " << min << " true atoms" );
    computeTrueVars();    
    if( min == 0 || !solver.addClauseFromModelAndRestart() )
    {
        printTrueVars();
        return COHERENT;
    }
    
    while( solver.solve() == COHERENT )
    {
        trace_msg( predmin, 2, "Coherent" );
        unsigned int count = countTrue();
        trace_msg( predmin, 3, "New answer set contains " << count << " true atoms" );
        if( count < min )
        {
            computeTrueVars();
            min = count;
            trace_msg( predmin, 4, "Updated min to " << min );
        }
        if( min == 0 || !solver.addClauseFromModelAndRestart() )
            break;
        
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );        
    }
    printTrueVars();
    return COHERENT;
}

unsigned int
PredicateMinimization::guessAndCheck()
{
    trace_msg( predmin, 1, "Starting algorithm guess and check" );
    solver.turnOffSimplifications();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    
    if( !computeFirstModel() )
        return INCOHERENT;

    if( checkAnswerSet() )
        return COHERENT;

    solver.unrollToZero();
    solver.clearConflictStatus();
    
    while( solver.solve() == COHERENT )
    {
        if( checkAnswerSet() )
            return COHERENT;
        
        solver.unrollToZero();
        solver.clearConflictStatus();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );        
    }
    return COHERENT;
}

bool
PredicateMinimization::checkAnswerSet()
{
    vector< Literal > assumptions;
    Clause* clause = new Clause();
    computeTrueVars();
    trace_msg( predmin, 2, "Checking answer set" );
    for( unsigned int i = 0; i < atomsToMinimize.size(); i++ )
    {
        Var v = atomsToMinimize[ i ];
        assert( !solver.isUndefined( v ) );
        assert( !solver.hasBeenEliminated( v ) );
        
        if( solver.getDecisionLevel( v ) == 0 )
            continue;
        if( solver.isTrue( v ) )
            clause->addLiteral( Literal( v, NEGATIVE ) );
        else
        {
            clause->addLiteral( Literal( v, POSITIVE ) );
            assumptions.push_back( Literal( v, NEGATIVE ) );
        }
    }
    solver.unrollToZero();
    solver.clearConflictStatus();
    clause->setCanBeDeleted( false );
    trace_action( predmin, 3, { trace_tag( cerr, predmin, 3 ); printVectorOfLiterals( assumptions, "Assumptions" ); } );
    if( solver.addClauseRuntime( clause ) && solver.solve( assumptions ) != INCOHERENT )
    {
        trace_msg( predmin, 4, "The answer set is not minimal" );
        return false;
    }
    
    trace_msg( predmin, 4, "The answer set is minimal: stop." );
    printTrueVars();
    return true;    
}

unsigned int
PredicateMinimization::guessAndCheckAndMinimize()
{
    trace_msg( predmin, 1, "Starting algorithm guess and check with minimization of answer set" );
    solver.turnOffSimplifications();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    
    if( !computeFirstModel() )
        return INCOHERENT;

    minimizeAnswerSet();
    return COHERENT;    
}

void
PredicateMinimization::minimizeAnswerSet()
{
    vector< Var > candidates;
    vector< Literal > assumptions;
    for( unsigned int i = 0; i < atomsToMinimize.size(); i++ )
    {
        Var v = atomsToMinimize[ i ];
        assert( !solver.isUndefined( v ) );
        assert( !solver.hasBeenEliminated( v ) );
        
        if( solver.getDecisionLevel( v ) == 0 )
            continue;
        if( solver.isTrue( v ) )
            candidates.push_back( v );
        else
            assumptions.push_back( Literal( v, NEGATIVE ) );
    }
    
    begin:;
    computeTrueVars();
    
    Clause* clause = new Clause();
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        Var v = candidates[ i ];
        assert( solver.isTrue( v ) && solver.getDecisionLevel( v ) > 0 );
        clause->addLiteral( Literal( v, NEGATIVE ) );        
    }
    trace_action( predmin, 3, { trace_tag( cerr, predmin, 3 ); printVectorOfVars( candidates, "Candidates" ); } );
    trace_action( predmin, 3, { trace_tag( cerr, predmin, 3 ); printVectorOfLiterals( assumptions, "Assumptions" ); } );    
    solver.unrollToZero();
    solver.clearConflictStatus();
    clause->setCanBeDeleted( false );
    
    for(unsigned int i = 0; i < assumptions.size(); i++)
        if(!solver.addClauseRuntime(assumptions[i]))
            goto end;
    assumptions.clear();
    
    if( solver.addClauseRuntime( clause ) && solver.solve() != INCOHERENT )
    {
        trace_msg( predmin, 4, "The answer set is not minimal. Try to minimize the new answer set." );    
        clause->setCanBeDeleted( true );
        unsigned int j = 0;
        for( unsigned int i = 0; i < candidates.size(); i++ )
        {
            Var v = candidates[ j ] = candidates[ i ];
            if( solver.getDecisionLevel( v ) == 0 )
                continue;
            
            if( solver.isTrue( v ) )
                j++;
            else
                assumptions.push_back( Literal( v, NEGATIVE ) );
        }
        candidates.resize( j );
        goto begin;
    }   
    trace_msg( predmin, 4, "The answer set is minimal: stop." );
    end:;
    printTrueVars();
}

unsigned int
PredicateMinimization::guessAndCheckAndSplit()
{
    trace_msg( predmin, 1, "Starting algorithm guess and check and split" );
    solver.turnOffSimplifications();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    
    if( !computeFirstModel() )
        return INCOHERENT;

    minimizeAnswerSetSplit();
    return COHERENT; 
}

void
PredicateMinimization::minimizeAnswerSetSplit()
{
    vector< Var > candidates;
    vector< Literal > assumptions;
    for( unsigned int i = 0; i < atomsToMinimize.size(); i++ )
    {
        Var v = atomsToMinimize[ i ];
        assert( !solver.isUndefined( v ) );
        assert( !solver.hasBeenEliminated( v ) );
        
        if( solver.getDecisionLevel( v ) == 0 )
            continue;
        if( solver.isTrue( v ) )
            candidates.push_back( v );
        else
            assumptions.push_back( Literal( v, NEGATIVE ) );
    }
    
    computeTrueVars();
    
    begin:;
    trace_action( predmin, 3, { trace_tag( cerr, predmin, 3 ); printVectorOfVars( candidates, "Candidates" ); } );
    trace_action( predmin, 3, { trace_tag( cerr, predmin, 3 ); printVectorOfLiterals( assumptions, "Assumptions" ); } );
    if( candidates.empty() )
    {
        trace_msg( predmin, 4, "No more candidates: stop." );
        printTrueVars();
        return;
    }
    
    solver.unrollToZero();
    solver.clearConflictStatus();
    for( unsigned int i = 0; i < assumptions.size(); i++ )
        if( !solver.addClauseRuntime( assumptions[ i ] ) )
        {
            candidates.clear();
            break;
        }
    assumptions.clear();    
    
    Var lastCandidate = candidates.back();
    candidates.pop_back();    
    trace_msg( predmin, 4, "Considering variable " << Literal( lastCandidate, POSITIVE ) << " as candidate");
    assumptions.push_back( Literal( lastCandidate, NEGATIVE ) );
    
    if( solver.solve( assumptions ) == INCOHERENT )
    {
        trace_msg( predmin, 5, "No answer set found: the variable must be true");
        assumptions.pop_back();
        solver.unrollToZero();
        solver.clearConflictStatus();
        
        if( !solver.addClauseRuntime( Literal( lastCandidate, POSITIVE ) ) )
            candidates.clear();
    }
    else
    {
        trace_msg( predmin, 5, "Found answer set: the variable is not needed");
        computeTrueVars();
        unsigned int j = 0;
        for( unsigned int i = 0; i < candidates.size(); i++ )
        {
            Var v = candidates[ j ] = candidates[ i ];
            if( solver.getDecisionLevel( v ) == 0 )
                continue;
            
            if( solver.isTrue( v ) )
                j++;
            else
                assumptions.push_back( Literal( v, NEGATIVE ) );            
        }
        candidates.resize( j );        
    }
    goto begin;
}

bool
PredicateMinimization::checkTrivialSolution()
{    
    vector< Literal > assumptions;
    for( unsigned int i = 0; i < atomsToMinimize.size(); i++ )
    {
        Var v = atomsToMinimize[ i ];
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.hasBeenEliminated( v ) );
        if( !solver.isUndefined( v ) )
            continue;        
        assumptions.push_back( Literal( v, NEGATIVE ) );
    }
    if( solver.solve( assumptions ) == INCOHERENT )
    {
        solver.unrollToZero();
        solver.clearConflictStatus();
        return false;
    }
    solver.printAnswerSet();
    return true;
}