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
#include "Variable.h"
#include "stl/UnorderedSet.h"

void 
SATSolver::init()
{
    
}

void 
SATSolver::solve()
{
    printProgram();
    
    Variable* choice;
    while( !undefinedVariables.empty() )
    {
        if( !conflict )
        {
            choice = undefinedVariables.at( 0 );
            assert( choice != NULL );
            incrementCurrentDecisionLevel();
            onLiteralAssigned( choice->getPositiveLiteral(), FALSE, NULL );
            cout << "Choice " << *choice << endl;
        }
        else
        {
            conflict = false;
            onLiteralAssigned( choice->getPositiveLiteral(), TRUE, NULL );            
            cout << "Flipping " << *choice << endl;
        }
        
        assert( choice->isUndefined() );
        while( hasNextLiteralToPropagate() )
        {
            pair< Literal*, TruthValue >& next = getNextLiteralToPropagate();

            Literal* literalToPropagate = next.first;
            Variable* var = literalToPropagate->getVariable();
            assert( var != NULL );
            cout << "Propagating " << *literalToPropagate << " " << ( next.second == TRUE ? "TRUE" : "FALSE" ) << endl;
            
            if( undefinedVariables.erase( var ) )
            {
                assignedVariables.push_back( var );
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
            literalToPropagate->getVariable()->unitPropagation( *this );            
        }
    }
    
    cout << "Model:";
    for( list< Variable* >::iterator it = assignedVariables.begin(); it != assignedVariables.end(); ++it )
    {
        Variable* tmp = *it;
        if( tmp->isTrue() )
        {
            cout << " " << *tmp;
        }
    }
    cout << endl;
}