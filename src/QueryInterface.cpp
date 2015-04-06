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

#include "QueryInterface.h"
#include "util/Assert.h"
#include "Solver.h"
#include "util/Constants.h"

QueryInterface::QueryInterface(
    Solver& s ) : solver( s )
{
}

void
QueryInterface::computeCautiousConsequences(
    unsigned int ALGORITHM )
{
    if( !computeFirstModel() )
    {
        solver.foundIncoherence();
        return;
    }
    
    switch( ALGORITHM )
    {            
        case ITERATIVE_COHERENCE_TESTING:
            iterativeCoherenceTesting();
            break;
            
        case OVERESTIMATE_REDUCTION:
            overestimateReduction();
            break;
            
        default:
            ErrorMessage::errorGeneric( "Inserted invalid algorithm for query answering." );
    }
        
    solver.printCautiousConsequences( answers );
}

void
QueryInterface::iterativeCoherenceTesting()
{
    solver.unrollToZero();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    
    vector< Literal > assumptions;    
    while( !candidates.empty() )
    {
        Var v = candidates.back();
        if( !solver.isUndefined( v ) )
        {
            assert( solver.getDecisionLevel( v ) == 0 );
            if( solver.isTrue( v ) )
                addAnswer( v );
            
            candidates.pop_back();            
            continue;
        }
        
        assert( solver.isUndefined( v ) );
        assumptions.push_back( Literal( v, NEGATIVE ) );
        unsigned int result = solver.solve( assumptions );
        
        if( result == COHERENT )
            reduceCandidates();
        else
        {
            assert( result == INCOHERENT );        
            addAnswer( v );
            candidates.pop_back();
        }
        
        solver.unrollToZero();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );        
        assumptions.clear();
    }        
}

void
QueryInterface::overestimateReduction()
{        
    solver.turnOffSimplifications();
    Clause* clausePointer = computeClauseFromCandidates();
    unsigned int size = clausePointer->size();    
    solver.unrollToZero();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    if( !solver.addClauseRuntime( clausePointer ) )
        return;

    while( solver.solve() == COHERENT )
    {
        if( size > 2 )
        {
            assert( clausePointer->size() > 2 );
            solver.detachClause( *clausePointer );
        }

        clausePointer = computeClauseFromCandidates();
        size = clausePointer->size();
        solver.unrollToZero();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );

        if( !solver.addClauseRuntime( clausePointer ) )
            return;
    }
    
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        Var v = candidates[ i ];
        assert( solver.isTrue( v ) && solver.getDecisionLevel( v ) == 0 );
        addAnswer( v );
    }
}

bool
QueryInterface::computeFirstModel()
{
    unsigned int result = solver.solve();
    if( result == INCOHERENT )
        return false;
    
    assert( result == COHERENT );
    computeCandidates();    
    return true;
}

void
QueryInterface::computeCandidates()
{
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        Var v = i;
        if( VariableNames::isHidden( v ) )
            continue;

        assert_msg( !solver.hasBeenEliminated( v ), "Variable " << Literal( v, POSITIVE ) << " has been deleted" );
        if( !solver.isTrue( v ) )
            continue;
        
        if( solver.getDecisionLevel( v ) == 0 )
            addAnswer( v );
        else
            candidates.push_back( v );
    }
}

void
QueryInterface::reduceCandidates()
{
    unsigned int j = 0;
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        Var v = candidates[ j ] = candidates[ i ];
        assert( !solver.isUndefined( v ) );
        if( !solver.isTrue( v ) )
            continue;
        
        if( solver.getDecisionLevel( v ) == 0 )
            addAnswer( v );
        else
            j++;
    }    
    candidates.shrink( j );
    printCandidates();
}

Clause*
QueryInterface::computeClauseFromCandidates()
{
    Clause* clause = new Clause();
    unsigned int j = 0;
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        Var v = candidates[ j ] = candidates[ i ];
        assert( !solver.isUndefined( v ) );
        if( !solver.isTrue( v ) )
            continue;
        
        if( solver.getDecisionLevel( v ) == 0 )
            addAnswer( v );
        else
        {
            clause->addLiteral( Literal( v, NEGATIVE ) );
            j++;
        }
    }    
    candidates.shrink( j );
    clause->setCanBeDeleted( false );
    printCandidates();
    return clause;
}

void
QueryInterface::addAnswer(
    Var v )
{
    if( wasp::Options::queryVerbosity >= 1 )
        cout << "Certain answer: " << Literal( v, POSITIVE ) << endl;
    answers.push_back( v );
}

void
QueryInterface::printCandidates()
{
    if( wasp::Options::queryVerbosity < 2 )
        return;
    cout << "Number of possible answers: " << candidates.size() << endl;
}