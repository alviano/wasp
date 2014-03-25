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

#include "Component.h"
#include "Literal.h"
#include "Variable.h"
#include "util/Options.h"
#include "Learning.h"
#include "Clause.h"

Clause*
Component::getClauseToPropagate(
    Learning& learning )
{
    begin:;
    if( unfoundedSet.empty() )
    {
        if( clauseToPropagate )
        {
            delete clauseToPropagate;
            clauseToPropagate = NULL;
        }
        
        if( variablesWithoutSourcePointer.empty() )
        {
            assert( checkSourcePointersStatus() );
            return NULL;
        }

        computeGUS();
        
        if( unfoundedSet.empty() )
        {
            assert( checkSourcePointersStatus() );
            return NULL;
        }
        else
        {
            assert( clauseToPropagate == NULL );
            clauseToPropagate = learning.learnClausesFromUnfoundedSet( unfoundedSet );
            trace_msg( unfoundedset, 2, "Reasons of unfounded sets: " << *clauseToPropagate );
            goto begin;
        }
    }
    else
    {
        assert( clauseToPropagate != NULL );
        
        Variable* variable;
        do
        {
            variable = unfoundedSet.back();
            unfoundedSet.pop_back();
            
            getGUSData( variable->getId() ).inQueue = false;
            setVariableFounded( variable->getId(), true );
            
            if( !variable->isFalse() )
            {
                Clause* loopFormula = new Clause();
                loopFormula->copyLiterals( *clauseToPropagate );

                if( variable->isTrue() )
                {
                    if( variable->getDecisionLevel() > 0 )
                        loopFormula->addLiteral( Literal( variable, NEGATIVE ) );
                    
                    if( loopFormula->size() > 1 )
                    {
                        unsigned int maxDecisionLevel = loopFormula->getAt( 1 ).getDecisionLevel();
                        unsigned int maxPosition = 1;

                        for( unsigned int i = 2; i < loopFormula->size(); i++ )
                        {
                            unsigned int dl = loopFormula->getAt( i ).getDecisionLevel();
                            if( dl > maxDecisionLevel )
                            {
                                maxDecisionLevel = dl;
                                maxPosition = i;
                            }
                        }
                        loopFormula->swapLiterals( 1, maxPosition );
                        
                        if( loopFormula->getAt( 0 ).getDecisionLevel() <= loopFormula->getAt( 1 ).getDecisionLevel() )
                            loopFormula->swapLiterals( 0, 1 );
                    }                                        
                    
                    //unfoundedSet.clear();
                    reset();
                }                
                else
                {
                    loopFormula->addLiteral( Literal( variable, NEGATIVE ) );
                    if( loopFormula->size() >= 2 )
                    {
                        loopFormula->swapLiterals( 0, loopFormula->size() - 1 );
                        loopFormula->swapLiterals( 1, loopFormula->size() - 1 );
                    }
                }

                trace_msg( unfoundedset, 2, "Adding loop formula: " << *loopFormula );                
                loopFormula->setLearned();
                return loopFormula;
            }
        } while( variable->isFalse() && !unfoundedSet.empty() );
    }
    
    goto begin;
//    assert( 0 );
//    return NULL;
}

void
Component::computeGUS()
{
    trace_msg( unfoundedset, 1, "Starting the computation of unfounded sets" );
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); )
    {
        Variable* variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable->getId();
        if( !variable->isFalse() )
        {
            trace_msg( unfoundedset, 1, "Variable " << *variable << " lost the source pointer" );        
            propagateLiteralLostSourcePointer( Literal( variable ) );
            i++;
        }
        else
        {
            trace_msg( unfoundedset, 1, "Variable " << *variable << " is false" );        
            variablesWithoutSourcePointer[ i ] = variablesWithoutSourcePointer.back(); //variablesWithoutSourcePointer[ variablesWithoutSourcePointer.size() - 1 ];            
            variablesWithoutSourcePointer.pop_back();
            setVariableFounded( varId, true );
            getGUSData( varId ).inQueue = false;
        }
    }
    
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Variable* variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable->getId();
        getGUSData( varId ).inQueue = false;
        if( getGUSData( varId ).founded )
            continue;
        
        trace_msg( unfoundedset, 1, "Looking for a new source pointer for the variable " << *variable );
        lookForANewSourcePointer( varId );
    }

    assert( unfoundedSet.empty() );

    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Variable* variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable->getId();
        if( !getGUSData( varId ).founded )
        {
            trace_msg( unfoundedset, 1, "The variable " << *variable << " is in the unfounded set");    
            assert( !variable->isFalse() );
            unfoundedSet.push_back( variable );
        }
    }
    variablesWithoutSourcePointer.clear();
}

void
Component::reset()
{
    while( !variablesWithoutSourcePointer.empty() )
    {
        Variable* variable = variablesWithoutSourcePointer.back();
        variablesWithoutSourcePointer.pop_back();
        getGUSData( variable->getId() ).inQueue = false;
        setVariableFounded( variable->getId(), true );
//        resetSourcePointer( variable->getId() );
    }
    
    while( !unfoundedSet.empty() )
    {
        Variable* variable = unfoundedSet.back();
        unfoundedSet.pop_back();
        getGUSData( variable->getId() ).inQueue = false;
        setVariableFounded( variable->getId(), true );
    }
    
    if( clauseToPropagate != NULL )
    {
        delete clauseToPropagate;
        clauseToPropagate = NULL;
    }
}

bool
Component::onLiteralFalse(
    Literal lit,
    int )
{
    trace_msg( unfoundedset, 1, "Literal " << lit << " is false" );
    return propagateFalseForGUS( lit );
}