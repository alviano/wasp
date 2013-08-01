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

#include <queue>

#include "SATSolver.h"
#include "AbstractBuilder.h"
#include "stl/UnorderedSet.h"

void 
SATSolver::init()
{
    
}

void 
SATSolver::solve()
{
    printProgram();
    
    Literal* choice;
    while( !undefinedLiterals.empty() )
    {
        if( !conflict )
        {
            choice = undefinedLiterals.at( 0 );
            assert( choice != NULL );
            incrementCurrentDecisionLevel();
            onLiteralAssigned( choice, FALSE, NULL );
            cout << "Choice " << *choice << endl;
        }
        else
        {
            conflict = false;
            onLiteralAssigned( choice, TRUE, NULL );            
            cout << "Flipping " << *choice << endl;
        }
        
        assert( choice->isUndefined() );
        while( hasNextLiteralToPropagate() )
        {
            pair< Literal*, TruthValue >& next = getNextLiteralToPropagate();

            Literal* literalToPropagate = next.first;
            cout << "Propagating " << *literalToPropagate << " " << ( next.second == TRUE ? "TRUE" : "FALSE" ) << endl;
            
            PositiveLiteral* posLiteral = literalToPropagate->getPositiveLiteral();
            assert( posLiteral != NULL );
            if( undefinedLiterals.erase( posLiteral ) )
            {
                assignedLiterals.push_back( posLiteral );
            }
            
            conflict = next.second == TRUE ? !literalToPropagate->setTrue() : !literalToPropagate->setFalse();
            if( conflict )
            {
                if( getCurrentDecisionLevel() == 0 )
                {
                    cout << "INCO" << endl;
                    return;
                }                
                unrollOne();
                
                break;
            }
            literalToPropagate->unitPropagation( *this );            
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