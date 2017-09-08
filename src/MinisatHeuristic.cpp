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
#include "Literal.h"
#include "Solver.h"

Literal
MinisatHeuristic::makeAChoice()
{
    trace_msg( heuristic, 1, "Starting MiniSAT heuristic" );
    while( !preferredChoices.empty() )
    {
        Literal lit = preferredChoices.top();
        preferredChoices.pop();
        if( solver.isUndefined( lit ) )
            return lit;
    }
    
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
    Vector< Var > tmp;
    for( unsigned int i = 1; i < vars.size(); i++ )
        tmp.push_back( vars[ i ].var() );
        
    for( unsigned int i = 0; i < tmp.size(); )
    {
        if( !solver.isUndefined( tmp[ i ] ) )
        {
            assert_msg( solver.getDecisionLevel( tmp[ i ] ) == 0, "Variable " << tmp[ i ] << " has not been inferred at level 0.");
            tmp[ i ] = tmp.back();
            tmp.pop_back();
        }
        else
        {       
            heap.pushNoCheck( tmp[ i ] );
            ++i;
        }
    }
    
//    for( unsigned int i = 1; i < vars.size(); i++ )
//    {
//        assert_msg( solver.isUndefined( vars[ i ].var() ) || solver.getDecisionLevel( vars[ i ].var() ) == 0, "Variable " << vars[ i ].var() << " has not been inferred at level 0.");        
//        if( solver.isUndefined( i ) )
//            heap.pushNoCheck( i );
//    }
}
