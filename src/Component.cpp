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
#include "util/Options.h"
#include "Learning.h"
#include "Clause.h"
#include "Solver.h"
#include "outputBuilders/CompetitionOutputBuilder.h"

ostream& operator<<( ostream& o, const Component& c )
{
    o << "[";
    for( unsigned int i = 0; i < c.size(); i++ )
        o << " " << VariableNames::getName( c.getVariable( i ) );
    o << " ]";
    return o;
}

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
            if( solver.glucoseHeuristic() )
                clauseToPropagate->setLbd( solver.computeLBD( *clauseToPropagate ) );

            trace_msg( unfoundedset, 2, "Reasons of unfounded sets: " << *clauseToPropagate );
            goto begin;
        }
    }
    else
    {
        assert( clauseToPropagate != NULL );
        
        for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
        {
            Var tmp = unfoundedSet[ i ];                
            if( solver.isTrue( tmp ) )
            {
                unfoundedSet[ i ] = unfoundedSet.back();
                unfoundedSet.back() = tmp;
                break;
            }
        }
        
        Var variable;
        do
        {
            variable = unfoundedSet.back();
            unfoundedSet.pop_back();
            
            getGUSData( variable ).inQueue = false;
            setVariableFounded( variable, true );
            
            if( !solver.isFalse( variable ) )
            {
                Clause* loopFormula = new Clause();
                loopFormula->copyLiterals( *clauseToPropagate );
                if( solver.glucoseHeuristic() )
                    loopFormula->setLbd( clauseToPropagate->lbd() );

                if( solver.isTrue( variable ) )
                {
                    if( solver.getDecisionLevel( variable ) > 0 )
                        loopFormula->addLiteral( Literal( variable, NEGATIVE ) );
                    
                    if( loopFormula->size() > 1 )
                    {
                        unsigned int maxDecisionLevel = solver.getDecisionLevel( loopFormula->getAt( 1 ) );
                        unsigned int maxPosition = 1;

                        for( unsigned int i = 2; i < loopFormula->size(); i++ )
                        {
                            unsigned int dl = solver.getDecisionLevel( loopFormula->getAt( i ) );
                            if( dl > maxDecisionLevel )
                            {
                                maxDecisionLevel = dl;
                                maxPosition = i;
                            }
                        }
                        loopFormula->swapLiterals( 1, maxPosition );
                        
                        if( solver.getDecisionLevel( loopFormula->getAt( 0 ) ) <= solver.getDecisionLevel( loopFormula->getAt( 1 ) ) )
                            loopFormula->swapLiterals( 0, 1 );
                        
                        if( solver.glucoseHeuristic() )
                            loopFormula->setLbd( solver.computeLBD( *loopFormula ) );
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
                solver.onLearningALoopFormulaFromGus();
                return loopFormula;
            }
        } while( solver.isFalse( variable ) && !unfoundedSet.empty() );
    }
    
    goto begin;
//    assert( 0 );
//    return NULL;
}

void
Component::computeGUS()
{
    trace_msg( unfoundedset, 1, "Starting the computation of unfounded sets for component " << id << " " << *this );
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); )
    {
        Var variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable;
        if( !solver.isFalse( variable ) )
        {
            trace_msg( unfoundedset, 1, "Variable " << Literal( variable, POSITIVE ) << " lost the source pointer" );        
            propagateLiteralLostSourcePointer( Literal( variable ) );
            i++;
        }
        else
        {
            trace_msg( unfoundedset, 1, "Variable " << Literal( variable, POSITIVE ) << " is false" );        
            variablesWithoutSourcePointer[ i ] = variablesWithoutSourcePointer.back(); //variablesWithoutSourcePointer[ variablesWithoutSourcePointer.size() - 1 ];            
            variablesWithoutSourcePointer.pop_back();
            setVariableFounded( varId, true );
            getGUSData( varId ).inQueue = false;
        }
    }
    
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable;
        getGUSData( varId ).inQueue = false;
        if( getGUSData( varId ).founded )
            continue;
        
        trace_msg( unfoundedset, 1, "Looking for a new source pointer for the variable " << Literal( variable, POSITIVE ) );
        lookForANewSourcePointer( varId );
    }

    assert( unfoundedSet.empty() );

    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ i ];
        unsigned int varId = variable;
        if( !getGUSData( varId ).founded )
        {
            trace_msg( unfoundedset, 1, "The variable " << Literal( variable, POSITIVE ) << " is in the unfounded set");    
            assert( !solver.isFalse( variable ) );
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
        Var variable = variablesWithoutSourcePointer.back();
        variablesWithoutSourcePointer.pop_back();
        getGUSData( variable ).inQueue = false;
        setVariableFounded( variable, true );
//        resetSourcePointer( variable->getId() );
    }
    
    while( !unfoundedSet.empty() )
    {
        Var variable = unfoundedSet.back();
        unfoundedSet.pop_back();
        getGUSData( variable ).inQueue = false;
        setVariableFounded( variable, true );
    }
    
    if( clauseToPropagate != NULL )
    {
        delete clauseToPropagate;
        clauseToPropagate = NULL;
    }
}

bool
Component::onLiteralFalse(
    Literal lit )
{
    trace_msg( unfoundedset, 1, "Literal " << lit << " is false " << *this );
    return propagateFalseForGUS( lit );
}

bool
Component::iterationOnSupportedByThisExternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variable supported by literal " << lit << " externally" );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable();

    Vector< Var >& wl = getGUSData( varId ).supportedByThisExternalRule[ sign ];

    unsigned i = 0;
    unsigned j = 0;
    for( ; i < wl.size(); ++i )
    {
        Var variable = wl[ j ] = wl[ i ];
        assert( solver.getComponent( variable ) != NULL );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true" ) << " and " << ( ( getGUSData( variable ).inQueue ) ? "in queue" : "not in queue" ) );
        if( solver.getComponent( variable )->getId() == id && !solver.isFalse( variable ) && !( getGUSData( variable ).inQueue ) )
            variableHasNoSourcePointer( variable );
        else
            ++j;
    }
    wl.shrink( j );
    return i != j;
}
bool
Component::iterationOnSupportedByThisInternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variable supported by literal " << lit << " internally" );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable();

    Vector< Var >& wl = getGUSData( varId ).supportedByThisInternalRule[ sign ];

    unsigned i = 0;
    unsigned j = 0;
    for( ; i < wl.size(); ++i )
    {
        Var variable = wl[ j ] = wl[ i ];
        assert( solver.getComponent( variable ) != NULL && solver.getComponent( variable )->getId() == id );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true" ) << " and " << ( ( getGUSData( variable ).inQueue ) ? "in queue" : "not in queue" ) );
        if( !solver.isFalse( variable ) && !( getGUSData( variable ).inQueue ) )
            variableHasNoSourcePointer( variable );
        else
            ++j;
    }
    wl.shrink( j );
    return i != j;
}

void
Component::iterationOnAuxSupportedByThis(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on aux variable supported by literal " << lit );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable();

    vector< Var >& vec = getGUSData( varId ).auxVariablesSupportedByThis[ sign ];
    for( unsigned int i = 0; i < vec.size(); i++ )
    {
        Var variable = vec[ i ];
        assert( getGUSData( variable ).aux );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true/undefined" ) << " and " << ( ( getGUSData( variable ).inQueue ) ? "in queue" : "not in queue" ) );
        if( !solver.isFalse( variable ) )
        {
            if( !getGUSData( variable ).inQueue )
                variableHasNoSourcePointer( variable );

            getGUSData( variable ).numberOfSupporting++;            
        }
    }
}

void
Component::foundSourcePointer(
    unsigned int id )
{
    Var variableWithSourcePointer = getGUSData( id ).variable;
    
    for( unsigned int i = 0; i < getGUSData( id ).possiblySupportedByThis[ POSITIVE ].size(); i++ )
    {
        Var var = getGUSData( id ).possiblySupportedByThis[ POSITIVE ][ i ];        
        
        if( !solver.isFalse( var ) && !getGUSData( var ).founded )
        {
            trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << Literal( var, POSITIVE ) );
            propagateSourcePointer( var, Literal( variableWithSourcePointer ) );
        }
    }
    
    for( unsigned int i = 0; i < getGUSData( id ).auxVariablesSupportedByThis[ POSITIVE ].size(); i++ )
    {
        Var var = getGUSData( id ).auxVariablesSupportedByThis[ POSITIVE ][ i ];
        
        if( !solver.isFalse( var ) )
        {
            assert_msg( !getGUSData( var ).founded, "Variable " << Literal( var, POSITIVE ) << " is founded" );
            trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << var );
            propagateSourcePointer( var, Literal( variableWithSourcePointer ) );
        }
    }
}

void
Component::lookForANewSourcePointer(
    unsigned int id )
{
    if( !getGUSData( id ).aux )
    {
        vector< Literal >& externalLiterals = getGUSData( id ).externalLiterals;
        for( unsigned int i = 0; i < externalLiterals.size(); i++ )
        {
            if( !solver.isFalse( externalLiterals[ i ] ) )
            {
                trace_msg( unfoundedset, 1, "Literal " << getGUSData( id ).externalLiterals[ i ] << " is an external source pointer of " << getGUSData( id ).variable );
                propagateSourcePointer( id, externalLiterals[ i ] );
                return;
            }
        }

        vector< Literal >& internalLiterals = getGUSData( id ).internalLiterals;
        for( unsigned int i = 0; i < internalLiterals.size(); i++ )
        {
            if( !solver.isFalse( internalLiterals[ i ] ) && getGUSData( internalLiterals[ i ].getVariable() ).founded )
            {
                trace_msg( unfoundedset, 1, "Literal " << getGUSData( id ).internalLiterals[ i ] << " is an internal source pointer of " << getGUSData( id ).variable );
                propagateSourcePointer( id, internalLiterals[ i ] );
                return;
            }
        }
    }    
}

void
Component::addVariableSupportedByLiteral(
    Var variable,
    Literal literal )
{
    Var tmp = literal.getVariable();
    if( solver.inTheSameComponent( tmp, variable ) )
        getGUSData( tmp ).supportedByThisInternalRule[ literal.getSign() ].push_back( variable );
    else
        getGUSData( tmp ).supportedByThisExternalRule[ literal.getSign() ].push_back( variable );        
}

void
Component::onLearningForUnfounded(
    unsigned int id,
    Learning& learning )
{
    if( !getGUSData( id ).aux )
    {
        for( unsigned int i = 0; i < getGUSData( id ).externalLiterals.size(); i++ )
        {
            Literal literal = getGUSData( id ).externalLiterals[ i ];
            assert( solver.isFalse( literal ) );
            if( solver.getDecisionLevel( literal ) > 0 )
                learning.onNavigatingLiteralForUnfoundedSetLearning( literal );        
        }

        for( unsigned int i = 0; i < getGUSData( id ).internalLiterals.size(); i++ )
        {
            Literal literal = getGUSData( id ).internalLiterals[ i ];
            if( solver.getDecisionLevel( literal ) > 0 && getGUSData( literal.getVariable() ).founded )
            {
                GUSData& element = getGUSData( literal.getVariable() );
                assert( solver.isFalse( literal ) );

                if( !element.aux )
                {
                    learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
                }
                else
                {
                    bool ignore = false;
                    unsigned int min = UINT_MAX;
                    unsigned int minPos = UINT_MAX;
                    for( unsigned int j = 0; j < element.literals.size(); j++ )
                    {
                        Literal lit = element.literals[ j ];
                        if( lit.isPositive() && !getGUSData( lit.getVariable() ).founded )
                        {
                            ignore = true;
                            break;
                        }

                        if( solver.isFalse( lit ) )
                        {
                            unsigned int dl = solver.getDecisionLevel( lit );
                            if( dl == 0 )
                            {
                                ignore = true;
                                break;
                            }
                            else if( dl < min )
                            {
                                min = dl;
                                minPos = j;
                            }
                        }
                    }
                    if( ignore )
                        continue;

                    if( minPos == UINT_MAX )
                    {
                        learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
                    }
                    else
                    {
                        assert_msg( minPos < element.literals.size(), "Literal " << literal << " " << minPos << " >= " << element.literals.size() );
                        learning.onNavigatingLiteralForUnfoundedSetLearning( element.literals[ minPos ] );
                    }
                }
            }
        }
    }
}

bool
Component::propagateFalseForGUS(
    Literal lit )
{
    bool res1 = iterationOnSupportedByThisExternal( lit );
    
    bool res2 = false;
    if( solver.getComponent( lit.getVariable() ) == this )
        res2 = iterationOnSupportedByThisInternal( lit );
    
    return res1 || res2;
}

#ifndef NDEBUG
bool
Component::checkSourcePointersStatus()
{
    for( unsigned int i = 0; i < variablesInComponent.size(); i++ )
    {
        unsigned int id = variablesInComponent[ i ];                
        assert( solver.getComponent( id )->getId() == this->id );
        if( !getGUSData( id ).founded )
        {
            cerr << "Variable " << Literal( getGUSData( id ).variable, POSITIVE ) << " is not founded " << getGUSData( id ).numberOfSupporting << endl;
            return false;
        }

        if( getGUSData( id ).numberOfSupporting != 0 )
        {
            cerr << "Variable " << Literal( getGUSData( id ).variable, POSITIVE ) << " has a number of supporting greater than 0" << endl;
            return false;
        }
    }

    return true;
}
#endif