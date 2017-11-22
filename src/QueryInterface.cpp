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
#include "util/WaspAssert.h"
#include "Solver.h"
#include "util/WaspConstants.h"

QueryInterface::QueryInterface(
    Solver& s ) : solver( s )
{
}

void
QueryInterface::solve()
{
    if( !computeFirstModel() )
    {
        solver.foundIncoherence();
        return;
    }
    
    switch( wasp::Options::queryAlgorithm )
    {            
        case ITERATIVE_COHERENCE_TESTING:
            iterativeCoherenceTesting();
            break;
            
        case OVERESTIMATE_REDUCTION:
            overestimateReduction();
            break;
            
        case COREBASED_QUERIES:
            {
            unsigned int param = candidates.size();
            if( wasp::Options::chunkSize != UINT_MAX )
                param = wasp::Options::chunkSize;
            else if( wasp::Options::chunkPercentage != UINT_MAX )
                param = ( candidates.size() * wasp::Options::chunkPercentage ) / 100;
            param = max(param, (unsigned int) 1);
            coreBasedAlgorithm( param );
            }
            break;
            
        case PREFERENCE_QUERIES:
            preferenceAlgorithm();
            break;
            
        case CHUNK_DYNAMIC:
        case CHUNK_STATIC:
            {
            unsigned int param = 2;
            if( wasp::Options::chunkSize != UINT_MAX )
                param = wasp::Options::chunkSize;
            else if( wasp::Options::chunkPercentage != UINT_MAX )
                param = ( candidates.size() * wasp::Options::chunkPercentage ) / 100;    
            
            if( param == 1 )
                iterativeCoherenceTesting();
            else if( param >= candidates.size() )
                overestimateReduction();
            else if( wasp::Options::queryAlgorithm == CHUNK_STATIC )                    
                chunkStaticAlgorithm( param );
            else if( wasp::Options::queryAlgorithm == CHUNK_DYNAMIC )
                chunkDynamicAlgorithm( param );
            }   
            break;
        default:
            WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for query answering." );
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
        goto end;

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
            goto end;
    }
    
    end:;
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        Var v = candidates[ i ];
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

Clause*
QueryInterface::computeClauseForChunk(
    unsigned int chunkSize,
    Var& auxVar )
{
    Clause* clause = new Clause();
    unsigned int limit = chunkSize > candidates.size() ? candidates.size() : chunkSize;
    for( unsigned int i = 0; i < limit; i++ )
    {
        assert( solver.isUndefined( candidates[ i ] ) );
        clause->addLiteral( Literal( candidates[ i ], NEGATIVE ) );    
    }
    solver.addVariableRuntime();
    auxVar = solver.numberOfVariables();
    clause->addLiteral( Literal( auxVar, POSITIVE ) );
    clause->setCanBeDeleted( false );    
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
    if( wasp::Options::queryVerbosity == 2 )
        cout << "Number of possible answers: " << candidates.size() << endl;
    else
    {
        cout << "Candidates:";
        for( unsigned int i = 0; i < candidates.size(); i++ )
            cout << " " << Literal( candidates[ i ], POSITIVE );
        cout << endl;
    }
}

void
QueryInterface::reduceCandidatesForChunk(
    unsigned int chunkSize )
{
    assert( solver.getCurrentDecisionLevel() == 0 );
    unsigned int limit = chunkSize > candidates.size() ? candidates.size() : chunkSize;
    unsigned int j = 0;
    for( unsigned int i = 0; i < candidates.size(); i++ )
    {
        candidates[ j ] = candidates[ i ];
        if( i >= limit )
        {
            if( solver.isUndefined( candidates[ i ] ) )
                j++;
            else if( solver.isTrue( candidates[ i ] ) )
                addAnswer( candidates[ i ] );
        }
    }
    candidates.shrink( j );
    printCandidates();
}

void
QueryInterface::chunkDynamicAlgorithm(
    unsigned int chunkSize )
{
    assert( chunkSize < candidates.size() );    
    solver.unrollToZero();
    solver.clearConflictStatus();
    solver.turnOffSimplifications();
    Var auxVar = 0;
    Clause* clausePointer = computeClauseForChunk( chunkSize, auxVar );
    assert( auxVar != 0 );
    unsigned int size = clausePointer->size();            
    
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );    
    if( !solver.addClauseRuntime( clausePointer ) )
        return;

    vector< Literal > assumptions;    
    while( true )
    {
        assumptions.clear();
        assumptions.push_back( Literal( auxVar, NEGATIVE ) );
        if( solver.solve( assumptions ) == COHERENT )
        {
            reduceCandidates();
            solver.unrollToZero();
            solver.clearConflictStatus();
        }
        else
        {
            unsigned int limit = chunkSize > candidates.size() ? candidates.size() : chunkSize;
            for( unsigned int i = 0; i < limit; i++ )
                addAnswer( candidates[ i ] );
            solver.unrollToZero();
            solver.clearConflictStatus();                
            reduceCandidatesForChunk( chunkSize );            
        }
        if( candidates.empty() )
            return;
        
        if( size > 2 )
        {
            assert_msg( clausePointer->size() >= 2, "CLAUSE SIZE " << clausePointer->size() );
            solver.detachClause( *clausePointer );
        }
        #ifndef NDEBUG
        bool res = 
        #endif
        solver.addClauseRuntime( Literal( auxVar, POSITIVE ) );
        assert( res );
        auxVar = 0;
        clausePointer = computeClauseForChunk( chunkSize, auxVar );
        
        assert( auxVar != 0 );
        size = clausePointer->size();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );

        #ifndef NDEBUG
        res = 
        #endif
        solver.addClauseRuntime( clausePointer );
        assert( res );
    }        
}

void
QueryInterface::computeCandidatesForChunk(
    Vector< Var >& candidatesForChunk,
    unsigned int chunkSize )
{
    while( !candidates.empty() )
    {
        Var c = candidates.back();
        candidates.pop_back();
        if( solver.isUndefined( c ) )
        {
            candidatesForChunk.push_back( c );
            if( candidatesForChunk.size() == chunkSize )
                return;
        }
    }    
}

Clause*
QueryInterface::computeClauseFromChunkCandidates(
    Vector< Var >& candidatesForChunk,
    Var& auxVar )
{
    assert( !candidatesForChunk.empty() );
    Clause* clause = new Clause();
    for( unsigned int i = 0; i < candidatesForChunk.size(); i++ )
    {
        assert( solver.isUndefined( candidatesForChunk[ i ] ) );
        clause->addLiteral( Literal( candidatesForChunk[ i ], NEGATIVE ) );
    }
    
    if( auxVar == 0 )
    {
        solver.addVariableRuntime();
        auxVar = solver.numberOfVariables();
    }
    clause->addLiteral( Literal( auxVar, POSITIVE ) );
    clause->setCanBeDeleted( false );    
    return clause;
}

void
QueryInterface::chunkStaticAlgorithm(
    unsigned int chunkSize )
{
    assert( chunkSize < candidates.size() );    
    solver.unrollToZero();
    solver.clearConflictStatus();
    solver.turnOffSimplifications();
    
    Vector< Var > candidatesForChunk;
    computeCandidatesForChunk( candidatesForChunk, chunkSize );
    
    Var auxVar = 0;
    Clause* clausePointer = computeClauseFromChunkCandidates( candidatesForChunk, auxVar );
    unsigned int size = clausePointer->size();
    assert( auxVar != 0 );
    
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );    
    if( !solver.addClauseRuntime( clausePointer ) )
        return;

    vector< Literal > assumptions;
    while( true )
    {
        assumptions.clear();
        assumptions.push_back( Literal( auxVar, NEGATIVE ) );
        if( solver.solve( assumptions ) == COHERENT )
        {
            reduceCandidates();
            unsigned int j = 0;
            for( unsigned int i = 0; i < candidatesForChunk.size(); i++ )
            {
                candidatesForChunk[ j ] = candidatesForChunk[ i ];
                if( solver.isTrue( candidatesForChunk[ i ] ) )
                {
                    if( solver.getDecisionLevel( candidatesForChunk[ i ] ) == 0 )
                        addAnswer( candidatesForChunk[ i ] );
                    else
                        j++;
                }
            }
            candidatesForChunk.shrink( j );
            solver.unrollToZero();
            solver.clearConflictStatus();
        }
        else
        {
            solver.unrollToZero();
            solver.clearConflictStatus();
            for( unsigned int i = 0; i < candidatesForChunk.size(); i++ )
            {
                addAnswer( candidatesForChunk[ i ] );
                #ifndef NDEBUG
                bool res = 
                #endif
                solver.addClauseRuntime( Literal( candidatesForChunk[ i ],  POSITIVE ) );
                assert( res );
            }
            candidatesForChunk.clear();            
        }
        
        if( candidates.empty() && candidatesForChunk.empty() )
            return;
        
        if( candidatesForChunk.empty() )
        {
            computeCandidatesForChunk( candidatesForChunk, chunkSize );
            #ifndef NDEBUG
            bool res = 
            #endif
            solver.addClauseRuntime( Literal( auxVar, POSITIVE ) );
            assert( res );
            auxVar = 0;
        }
        
        if( size > 2 )
        {
            assert( clausePointer->size() > 2 );
            solver.detachClause( *clausePointer );
        }
        clausePointer = computeClauseFromChunkCandidates( candidatesForChunk, auxVar );
        assert( auxVar != 0 );
        size = clausePointer->size();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );

        #ifndef NDEBUG
        bool res = 
        #endif
        solver.addClauseRuntime( clausePointer );
        assert( res );
    }
}

/*void
QueryInterface::coreBasedAlgorithm() {            
    vector< Literal > assumptions;    
    reset( assumptions );
    
    unsigned int numberOfCalls = 0;
    vector< unsigned int > inUnsatCore;
    while( inUnsatCore.size() <= solver.numberOfVariables() )
        inUnsatCore.push_back( numberOfCalls );
    solver.setComputeUnsatCores( true );
    
    while( !candidates.empty() )
    {
        reset( assumptions );
        for( unsigned int i = 0; i < candidates.size(); i++ ) {
            assumptions.push_back( Literal( candidates[ i ], NEGATIVE ) );
        }        
        
        while( true ) {
            unsigned int result = solver.solve( assumptions );
            if( result == COHERENT ) {
                reduceCandidates();
                break;
            }
            else {
                assert( result == INCOHERENT );
                numberOfCalls++;    
                processCore( numberOfCalls, inUnsatCore );
                unsigned int j = 0;
                
                vector< Var > core;
                for( unsigned int i = 0; i < assumptions.size(); i++ ) {
                    assumptions[ j ] = assumptions[ i ];
                    Var v = assumptions[ i ].getVariable();
                    assert( v < inUnsatCore.size() );
                    if( inUnsatCore[ v ] != numberOfCalls )
                        j++;
                    else
                        core.push_back( v );
                }
                assumptions.resize( j );
            
                assert( core.size() != 0 );
                if( core.size() == 1 ) {
                    addAnswer( core[ 0 ] );
                    candidates.findAndRemove( core[ 0 ] );
                }
                else if( assumptions.size() == 0 ) {
                    iterativeCoherenceTesting();
                    return;
                }
            }            
        }
    }
}*/

void
QueryInterface::coreBasedAlgorithm(
    unsigned int chunkSize )
{
    vector< Literal > assumptions;    
    reset( assumptions );
    
    unsigned int numberOfCalls = 0;
    vector< unsigned int > inUnsatCore;
    while( inUnsatCore.size() <= solver.numberOfVariables() )
        inUnsatCore.push_back( numberOfCalls );
    solver.setComputeUnsatCores( true );
    
    while( !candidates.empty() )
    {
        reset( assumptions );
        for( unsigned int i = 0; i < chunkSize && i < candidates.size(); i++ )
            assumptions.push_back( Literal( candidates[ i ], NEGATIVE ) );        
        
        while( true ) {
            unsigned int result = solver.solve( assumptions );
            if( result == COHERENT ) {
                reduceCandidates();
                break;
            }
            else {
                assert( result == INCOHERENT );
                numberOfCalls++;    
                processCore( numberOfCalls, inUnsatCore );
                unsigned int j = 0;
                
                vector< Var > core;
                for( unsigned int i = 0; i < assumptions.size(); i++ ) {
                    assumptions[ j ] = assumptions[ i ];
                    Var v = assumptions[ i ].getVariable();
                    assert( v < inUnsatCore.size() );
                    if( inUnsatCore[ v ] != numberOfCalls )
                        j++;
                    else
                        core.push_back( v );
                }
                assumptions.resize( j );
            
                assert( core.size() != 0 );
                if( core.size() == 1 ) {
                    addAnswer( core[ 0 ] );
                    candidates.findAndRemove( core[ 0 ] );
                }
                
                if( assumptions.size() == 0 ) {
                    iterativeCoherenceTesting();
                    break;
                }
            }            
        }
    }
}

void
QueryInterface::reset( vector< Literal >& assumptions )
{
    solver.unrollToZero();
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    assumptions.clear();
}

void
QueryInterface::processCore(
    unsigned int numberOfCalls,
    vector< unsigned int >& inUnsatCore )
{
    const Clause* core = solver.getUnsatCore();
    assert( core != NULL );
    const Clause& unsatCore = *( core );
    assert( unsatCore.size() > 0 );    
    solver.clearConflictStatus();
    solver.unrollToZero();

    for( unsigned int i = 0; i < unsatCore.size(); i++ ) {
        Var v = unsatCore[ i ].getVariable();
        assert( v > 0 && v < inUnsatCore.size() );
        inUnsatCore[ v ] = numberOfCalls;
    }
}

void
QueryInterface::preferenceAlgorithm()
{
    unsigned int size = UINT_MAX;
    while( !candidates.empty() && size != candidates.size() )
    {
        solver.unrollToZero();
        assert( solver.getCurrentDecisionLevel() == 0 );
        assert( !solver.conflictDetected() );
        vector< Literal > preferences;
        for( unsigned int i = 0; i < candidates.size(); i++ )
            preferences.push_back( Literal( candidates[ i ], NEGATIVE ) );
        solver.removePrefChoices();        
        solver.addPrefChoices(preferences);        
        #ifndef NDEBUG
        unsigned int result = 
        #endif
        solver.solve();
        assert( result == COHERENT );

        size = candidates.size();        
        reduceCandidates();
    }

    for( unsigned int i = 0; i < candidates.size(); i++ )
        addAnswer( candidates[ i ] );
}