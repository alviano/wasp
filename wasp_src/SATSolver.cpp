/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

#include "SATSolver.h"
#include "AbstractBuilder.h"
#include "stl/UnorderedSet.h"

void 
SATSolver::init()
{
    printProgram();    
}

void 
SATSolver::solve()
{
    Literal* choice;
    unsigned int count = 0;
    while( !undefinedLiterals.empty() )
    {
        if( !conflict )
        {
            choice = undefinedLiterals.at( 0 );
            assert( choice != NULL );
            incrementCurrentDecisionLevel();
            assert( choice->isUndefined() );        
            onLiteralAssigned( choice, FALSE, NULL );
            cout << "Choice " << *choice << endl;
        }
        
        unsigned int countOfPropagatingLiterals = 0;
        while( hasNextLiteralToPropagate() )
        {
            Literal* literalToPropagate = getNextLiteralToPropagate();

            literalToPropagate->setOrderInThePropagation( countOfPropagatingLiterals++ );
            literalToPropagate->unitPropagation( *this );
            if( conflict )
            {
                if( getCurrentDecisionLevel() == 0 )
                {
                    cout << "UNSAT" << endl;
                    return;
                }

                conflict = false;
                conflictLiteral->setOrderInThePropagation( countOfPropagatingLiterals++ );
                learningStrategy->onConflict( conflictLiteral, *this );
                conflictLiteral = NULL;                
                assert( hasNextLiteralToPropagate() );
            }
        }
    }
    
    cout << "Model:";
    for( list< PositiveLiteral* >::iterator it = assignedLiterals.begin(); it != assignedLiterals.end(); ++it )
    {
        PositiveLiteral* tmp = *it;
        if( tmp->isTrue() )
        {
            cout << " " << *tmp;
        }
    }
    cout << endl;
}