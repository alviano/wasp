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

    if( signs[ chosenVariable ] != UINT_MAX )
        return Literal( chosenVariable, signs[ chosenVariable ] );
    
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
    for( unsigned int i = 0; i < vars.size(); )
    {
        if( !solver.isUndefined( vars[ i ] ) )
        {
            assert_msg( solver.getDecisionLevel( vars[ i ] ) == 0, "Variable " << vars[ i ] << " has not been inferred at level 0.");            
            vars[ i ] = vars.back();
            vars.pop_back();
        }
        else
        {       
            heap.pushNoCheck( vars[ i ] );
            ++i;
        }
    }
}
