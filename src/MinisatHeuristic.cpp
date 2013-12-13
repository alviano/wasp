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

    Activity max;
    Variable* variable;
    
    unsigned int i = 0;
    for( ; i < variables.size(); ++i )
    {
        variable = variables[ i ];
        if( variable->isUndefined() )
        {
            chosenVariable = variable;
            max = variable->activity();
            break;
        }
    }
    
    for( ; i < variables.size(); ++i )
    {
        variable = variables[ i ];
        if( variable->isUndefined() )
        {
            if( variable->activity() > max )
            {
                chosenVariable = variable;
                max = variable->activity();
            }
        }
    }
    
    assert( "The chosen variable has not been set." && chosenVariable != NULL );
    assert( "The literal must be undefined." && chosenVariable->isUndefined() );
    trace( heuristic, 1, "Ending Minisat Heuristic.\n" );
    return Literal( chosenVariable, NEGATIVE );
}