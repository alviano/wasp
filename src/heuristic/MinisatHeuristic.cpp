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

#include "MinisatHeuristic.h"

#include <cassert>
#include "../Literal.h"
#include "../Solver.h"

Literal
MinisatHeuristic::makeAChoice()
{
    Literal lit = Literal::null;    
    do
    {
        if( preferredChoices.empty() )
            lit = makeAChoiceProtected();
        else
        {
            lit = preferredChoices.top();
            preferredChoices.pop();
            if( !solver.isUndefined( lit ) )
                lit = Literal::null;
        }        
    } while ( lit == Literal::null );
    assert( solver.isUndefined( lit ) );
    return lit;
}

Literal
MinisatHeuristic::makeAChoiceProtected()
{
    trace_msg( heuristic, 1, "Starting MiniSAT heuristic" );    
    chosenVariable = 0;
    //randomChoice();
    
    if( chosenVariable == 0 )
    {
        // Activity based decision:
        do
        {
            assert( !heap.empty() );
            chosenVariable = heap.top();
            heap.pop();
            assert( chosenVariable != 0 );
        } while( !solver.isUndefined( chosenVariable ) );    
    }

    assert_msg( chosenVariable != 0, "The chosen variable has not been set" );
    assert_msg( solver.isUndefined( chosenVariable ), "The literal must be undefined" );
    trace_msg( heuristic, 1, "Ending MiniSAT heuristic" );

    if( vars[ chosenVariable ].hasSign() )
        return Literal( chosenVariable, !vars[ chosenVariable ].sign() );
    
    //FIXME: Maybe in future we want to add the right minisat policy    
    if( solver.getCachedTruthValue( chosenVariable ) != UNDEFINED )
        return solver.getCachedTruthValue( chosenVariable ) == TRUE ? Literal( chosenVariable, POSITIVE ) : Literal( chosenVariable, NEGATIVE );
        
    return Literal( chosenVariable, NEGATIVE );
}

// Returns a random float 0 <= x < 1. Seed must never be 0.
inline double drand( double& seed )
{
    seed *= 1389796;
    int q = ( int )( seed / 2147483647 );
    seed -= ( double )q * 2147483647;
    return seed / 2147483647;
}

// Returns a random integer 0 <= x < size. Seed must never be 0.
inline int irand( double& seed, int size) { return ( int )( drand( seed ) * size ); }


//void
//MinisatHeuristic::randomChoice()
//{
//    double random_seed = 91648253; //FIXME: it should become an option.
//    double random_var_freq = 0;
//    
//    if( drand( random_seed ) < random_var_freq && !heap.empty() )
//    {
//        chosenVariable = heap[ irand( random_seed, heap.size() ) ];
//    }
//}

void
MinisatHeuristic::simplifyVariablesAtLevelZero()
{
    //This code is to guarantee the compatibility with the previous heuristic of WASP.
    Vector< Var > undefinedVars;
    for( unsigned int i = 1; i < vars.size(); i++ )
        undefinedVars.push_back( i );
        
    for( unsigned int i = 0; i < undefinedVars.size(); )
    {
        if( !solver.isUndefined( undefinedVars[ i ] ) )
        {
            assert_msg( solver.getDecisionLevel( undefinedVars[ i ] ) == 0, "Variable " << undefinedVars[ i ] << " has not been inferred at level 0.");
            undefinedVars[ i ] = undefinedVars.back();
            undefinedVars.pop_back();
        }
        else
        {       
            heap.pushNoCheck( undefinedVars[ i ] );
            ++i;
        }
    }

    if( wasp::Options::initSign == INIT_SIGN_MINISAT_ALLTRUE )
    {
        for( unsigned int i = 0; i < undefinedVars.size(); i++ )
            setSign( undefinedVars[ i ] );
    }    
    
    if( wasp::Options::initMinisatHeuristic == INIT_MINISAT_ALL_EQUALS && wasp::Options::initValue == 0 ) return;
    unsigned int maxScore = 0;
    for ( unsigned int i = 0; i < undefinedVars.size(); i++ )
    {
        Var v = undefinedVars[ i ];
        assert( solver.isUndefined( v ) );
        unsigned int score = 0;
        computeScore( v, score );        
        maxScore = max( maxScore, score );
        act[ v ] = ( double ) score * -1.0;        
	}    
    if( maxScore == 0 ) return;
    double ms = ( double ) maxScore;
	for ( unsigned int i = 0; i < undefinedVars.size(); i++ )
    {
        Var v = undefinedVars[ i ]; 
        if ( act[ v ] < 0 )
        {
            Activity d = act[ v ] * -1.0;
            act[ v ] = 0.0;
            init( v, d / ms );
        }
        assert( act[ v ] >= 0 );
	}    
}

void MinisatHeuristic::computeScore( Var v, unsigned int& score )
{
    switch( wasp::Options::initMinisatHeuristic )
    {
        case INIT_MINISAT_ALL_EQUALS:
            initMinisatVisibleAtoms( v, score );
            initMinisatHiddenAtoms( v, score );
            break;
        
        case INIT_MINISAT_MOMS:
            initMinisatMoms( v, score );
            break;
            
        case INIT_MINISAT_BINARY:
            initMinisatBinary( v, score );
            break;
            
        case INIT_MINISAT_WATCHES:
            initMinisatWatches( v, score );
            break;

        case INIT_MINISAT_PROPAGATORS:
            initMinisatPropagators( v, score );
            break;
            
        case INIT_MINISAT_VISIBLE_ATOMS:
            initMinisatVisibleAtoms( v, score );
            break;
            
        case INIT_MINISAT_HIDDEN_ATOMS:
            initMinisatHiddenAtoms( v, score );
            break;
            
        case INIT_MINISAT_COMBINATION:
            initMinisatCombination( v, score );
            break;
            
        default:
            WaspErrorMessage::errorGeneric( "Invalid value for initialization of minisat heuristic." );
    }
}

void MinisatHeuristic::initMinisatMoms( Var v, unsigned int& score )
{
    //ChosenVariable is 0 if solver has no binary clauses. Otherwise it is 1.
    if( chosenVariable == 0 ) { initMinisatWatches( v, score );  return; }
    
    unsigned int pos = solver.estimateBinaryPropagation( Literal( v, POSITIVE ) );
    unsigned int neg = solver.estimateBinaryPropagation( Literal( v, NEGATIVE ) );
    score = ( ( pos * neg ) << 10 ) + ( pos + neg ) + wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( pos > neg ? v : -v );
}

void MinisatHeuristic::initMinisatBinary( Var v, unsigned int& score )
{
    unsigned pos = solver.getDataStructure( Literal( v, POSITIVE ) ).variableBinaryClauses.size();
    unsigned neg = solver.getDataStructure( Literal( v, NEGATIVE ) ).variableBinaryClauses.size();
    score = ( ( pos * neg ) << 10 ) + pos + neg + wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( pos > neg ? v : -v );
}

void MinisatHeuristic::initMinisatWatches( Var v, unsigned int& score )
{
    unsigned pos = solver.getDataStructure( Literal( v, POSITIVE ) ).variableWatchedLists.size();
    unsigned neg = solver.getDataStructure( Literal( v, NEGATIVE ) ).variableWatchedLists.size();
    score = ( ( pos * neg ) << 10 ) + pos + neg + wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( pos > neg ? v : -v );
}

void MinisatHeuristic::initMinisatPropagators( Var v, unsigned int& score )
{
    unsigned pos = solver.getDataStructure( Literal( v, POSITIVE ) ).variablePropagators.size() + solver.getDataStructure( Literal( v, POSITIVE ) ).variablePostPropagators.size();
    unsigned neg = solver.getDataStructure( Literal( v, NEGATIVE ) ).variablePropagators.size() + solver.getDataStructure( Literal( v, NEGATIVE ) ).variablePostPropagators.size();
    score = ( ( pos * neg ) << 10 ) + pos + neg + wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( pos > neg ? v : -v );
}

void MinisatHeuristic::initMinisatVisibleAtoms( Var v, unsigned int& score )
{
    if( VariableNames::isHidden( v ) ) return;
    score = wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( v % 2 == 0 ? v : -v );
}

void MinisatHeuristic::initMinisatHiddenAtoms( Var v, unsigned int& score )
{
    if( !VariableNames::isHidden( v ) ) return;
    score = wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( v % 2 == 0 ? v : -v );
}

void MinisatHeuristic::initMinisatCombination( Var v, unsigned int& score )
{    
    unsigned pos = solver.getDataStructure( Literal( v, POSITIVE ) ).variablePropagators.size() + solver.getDataStructure( Literal( v, POSITIVE ) ).variablePostPropagators.size();
    unsigned neg = solver.getDataStructure( Literal( v, NEGATIVE ) ).variablePropagators.size() + solver.getDataStructure( Literal( v, NEGATIVE ) ).variablePostPropagators.size();
    
    pos += solver.getDataStructure( Literal( v, POSITIVE ) ).variableWatchedLists.size();
    neg += solver.getDataStructure( Literal( v, NEGATIVE ) ).variableWatchedLists.size();
    
    if( chosenVariable == 1 )
    {
        pos += solver.estimateBinaryPropagation( Literal( v, POSITIVE ) );
        neg += solver.estimateBinaryPropagation( Literal( v, NEGATIVE ) );
    }
    score = ( ( pos * neg ) << 10 ) + pos + neg + wasp::Options::initValue;
    if( wasp::Options::initSign == INIT_SIGN_MINISAT_MIXED ) setSign( pos > neg ? v : -v );    
}
