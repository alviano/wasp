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

Literal
MinisatHeuristic::makeAChoice()
{
    trace( heuristic, 1, "Starting Minisat Heuristic.\n" );
    if( !preferredChoices.empty() )
    {
        for( unsigned int i = 0; i < preferredChoices.size(); i++ )
        {
            if( preferredChoices[ i ].isUndefined() )
                return preferredChoices[ i ];
        }        
    }
    
    chosenVariable = NULL;    
    //randomChoice();
    
    if( chosenVariable == NULL )
    {
        // Activity based decision:
        do
        {
            assert( !heap.empty() );
    //        if( heap.empty() )
    //        {
    //            next = var_Undef;
    //            break;
    //        }
    //        else
                chosenVariable = heap.top();
                heap.pop();
                assert( chosenVariable != NULL );
        } while( !chosenVariable->isUndefined() );    
    }

//    Activity max;
//    Variable* variable;
//    
//    unsigned int i = 0;
//    for( ; i < variables.size(); ++i )
//    {
//        variable = variables[ i ];
//        if( variable->isUndefined() )
//        {
//            chosenVariable = variable;
//            max = variable->activity();
//            break;
//        }
//    }
//    
//    for( ; i < variables.size(); ++i )
//    {
//        variable = variables[ i ];
//        if( variable->isUndefined() )
//        {
//            if( variable->activity() > max )
//            {
//                chosenVariable = variable;
//                max = variable->activity();
//            }
//        }
//    }
//    
    assert( "The chosen variable has not been set." && chosenVariable != NULL );
    assert( "The literal must be undefined." && chosenVariable->isUndefined() );
    trace( heuristic, 1, "Ending Minisat Heuristic.\n" );
    
    //FIXME: Maybe in future we want to add the right minisat policy    
    if( chosenVariable->getCachedTruthValue() != UNDEFINED )
        return chosenVariable->getCachedTruthValue() == TRUE ? Literal( chosenVariable, POSITIVE ) : Literal( chosenVariable, NEGATIVE );
        
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