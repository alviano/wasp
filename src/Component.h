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

#ifndef _COMPONENT_H
#define	_COMPONENT_H

#include <vector>
#include <cassert>
#include <iostream>

#include "stl/Vector.h"
#include "PostPropagator.h"
#include "Literal.h"
#include "GUSData.h"
#include "Learning.h"
#include "util/Options.h"
#include "util/Trace.h"

class Clause;
using namespace std;

//typedef unsigned int heap_handle;
//class Component;
//class ComponentComparator
//{
//    public:
//        inline bool operator()( const Component* c1, const Component* c2 ) const;
//};

class Component : public PostPropagator
{
	public:
		inline Component( vector< GUSData* >& gusData_ ) : PostPropagator(), gusData( gusData_ ), clauseToPropagate( NULL ), selfLoop( 0 ) {}
		inline ~Component() {}
        
        virtual bool onLiteralFalse( Literal lit );

        inline bool isCyclic() const { return selfLoop || variablesInComponent.size() > 1; }
		inline void setSelfLoop() { selfLoop = 1; }
		inline unsigned int size() const { return variablesInComponent.size(); }

		inline void addVariable( unsigned int var ) { variablesInComponent.push_back( var ); }
		inline unsigned int getVariable( unsigned int i ){ assert( i < variablesInComponent.size() ); return variablesInComponent[ i ]; }

        inline void setId( unsigned int id ) { this->id = id; }
        inline unsigned int getId() const { return id; }
        
        virtual Clause* getClauseToPropagate( Learning& learning );

        inline void setAuxVariable( unsigned int varId ) { getGUSData( varId ).aux = true; }
        inline void addExternalLiteralForVariable( unsigned int varId, Literal lit ) { getGUSData( varId ).externalLiterals.push_back( lit ); }
        inline void addInternalLiteralForVariable( unsigned int varId, Literal lit ) { getGUSData( varId ).internalLiterals.push_back( lit ); }
        inline void addVariablePossiblySupportedByLiteral( Variable* var, Literal lit ) { gusData[ lit.getVariable()->getId() ]->possiblySupportedByThis[ lit.getSign() ].push_back( var ); }
        inline void addAuxVariableSupportedByLiteral( Variable* var, Literal lit ) { gusData[ lit.getVariable()->getId() ]->auxVariablesSupportedByThis[ lit.getSign() ].push_back( var ); }
        
        inline void variableHasNoSourcePointer( Variable* var );
        inline void onLearningForUnfounded( unsigned int id, Learning& );
        
    protected:
        virtual void reset();

	private:
		inline Component( const Component& orig );
		vector< unsigned int > variablesInComponent;
        vector< GUSData* >& gusData;
        Vector< Variable* > variablesWithoutSourcePointer;
        Vector< Variable* > unfoundedSet;
        Clause* clauseToPropagate;
        
		unsigned int selfLoop : 1;
        unsigned int id : 31;
        
        inline bool propagateFalseForGUS( Literal lit );
        inline void propagateLiteralLostSourcePointer( Literal lit );
        inline bool iterationOnSupportedByThisExternal( Literal lit );
        inline bool iterationOnSupportedByThisInternal( Literal lit );
        inline void iterationOnAuxSupportedByThis( Literal lit );        

        inline void foundSourcePointer( unsigned int id );
        inline void lookForANewSourcePointer( unsigned int id );
        inline void propagateSourcePointer( unsigned int id, Literal literal );
        inline void addExternalLiteral( unsigned int id, Literal literal );
        inline void addInternalLiteral( unsigned int id, Literal literal );
        inline void resetSourcePointer( unsigned int id );

        inline void setVariableFounded( unsigned int id, bool value );
        
        inline void addVariableSupportedByLiteral( Variable* variable, Literal literal );
        
        inline GUSData& getGUSData( unsigned int id );
        
        void computeGUS();
        
        #ifndef NDEBUG
        bool checkSourcePointersStatus()
        {
            for( unsigned int i = 0; i < variablesInComponent.size(); i++ )
            {
                unsigned int id = variablesInComponent[ i ];
                if( !getGUSData( id ).founded )
                {
                    cerr << "Variable " << *getGUSData( id ).variable << " is not founded " << getGUSData( id ).numberOfSupporting << endl;
                    return false;
                }
                
                if( getGUSData( id ).numberOfSupporting != 0 )
                {
                    cerr << "Variable " << *getGUSData( id ).variable << " has a number of supporting greater than 0" << endl;
                    return false;
                }
            }
            
            return true;
        }
        #endif
};

//bool ComponentComparator::operator()( const Component* c1, const Component* c2 ) const { return c1->getId() < c2->getId(); }

bool
Component::propagateFalseForGUS(
    Literal lit )
{
    bool res1 = iterationOnSupportedByThisExternal( lit );
    bool res2 = iterationOnSupportedByThisInternal( lit );
    
    return res1 || res2;
}

void
Component::propagateLiteralLostSourcePointer(
    Literal lit )
{
    iterationOnSupportedByThisInternal( lit );
    iterationOnAuxSupportedByThis( lit );
}

bool
Component::iterationOnSupportedByThisExternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variable supported by literal " << lit << " externally" );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable()->getId();

    bool add = false;
    WatchedList< Variable* >& wl = getGUSData( varId ).supportedByThisExternalRule[ sign ];

    wl.startIteration();
    while( wl.hasNext() )
    {
        Variable* variable = wl.next();        
        trace_msg( unfoundedset, 3, "Considering variable " << *variable << " which is " << ( variable->isFalse() ? "false" : "true" ) << " and " << ( ( getGUSData( variable->getId() ).inQueue ) ? "in queue" : "not in queue" ) );
        if( !variable->isFalse() && !( getGUSData( variable->getId() ).inQueue ) )
        {
            variableHasNoSourcePointer( variable );
            add = true;
            wl.remove( variable );
        }
    }
    
    return add;
}

bool
Component::iterationOnSupportedByThisInternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variable supported by literal " << lit << " internally" );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable()->getId();

    bool add = false;
    WatchedList< Variable* >& wl = getGUSData( varId ).supportedByThisInternalRule[ sign ];

    wl.startIteration();
    while( wl.hasNext() )
    {
        Variable* variable = wl.next();
        trace_msg( unfoundedset, 3, "Considering variable " << *variable << " which is " << ( variable->isFalse() ? "false" : "true" ) << " and " << ( ( getGUSData( variable->getId() ).inQueue ) ? "in queue" : "not in queue" ) );
        if( !variable->isFalse() && !( getGUSData( variable->getId() ).inQueue ) )
        {
            variableHasNoSourcePointer( variable );
            add = true;
            wl.remove( variable );
        }
    }
    
    return add;
}

void
Component::iterationOnAuxSupportedByThis(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on aux variable supported by literal " << lit );
    unsigned int sign = lit.getSign();
    unsigned int varId = lit.getVariable()->getId();

    vector< Variable* >& vec = getGUSData( varId ).auxVariablesSupportedByThis[ sign ];
    for( unsigned int i = 0; i < vec.size(); i++ )
    {
        Variable* variable = vec[ i ];
        assert( getGUSData( variable->getId() ).aux );
        trace_msg( unfoundedset, 3, "Considering variable " << *variable << " which is " << ( variable->isFalse() ? "false" : "true" ) << " and " << ( ( getGUSData( variable->getId() ).inQueue ) ? "in queue" : "not in queue" ) );
        if( !variable->isFalse() )
        {
            if( !getGUSData( variable->getId() ).inQueue )
                variableHasNoSourcePointer( variable );

            getGUSData( variable->getId() ).numberOfSupporting++;            
        }
    }
}

void
Component::variableHasNoSourcePointer(
    Variable* variable )
{
    unsigned int id = variable->getId();
    assert( !getGUSData( id ).inQueue );
    
    setVariableFounded( id, false );
    getGUSData( id ).inQueue = true;
    
    variablesWithoutSourcePointer.push_back( variable );    
}

//void
//Component::variableHasNoSourcePointer(
//    unsigned int id )
//{
//    assert( !gusData[ id ]->inQueue );
//
//    gusData[ id ]->founded = false;
//    gusData[ id ]->inQueue = true;
//
//    assert( variable->getComponent() != NULL );
//    if( !variable->getComponent()->hasBeenInserted() )
//    {
//        variable->getComponent()->setInserted( true );
//        componentsToProcess.pushNoCheck( variable->getComponent() );
//    }
//
//    assert( variable->getComponent()->getId() < variablesWithoutSourcePointer.size() );
//    variablesWithoutSourcePointer[ variable->getComponent()->getId() ]->push_back( variable );
//}

void
Component::foundSourcePointer(
    unsigned int id )
{
    Variable* variableWithSourcePointer = getGUSData( id ).variable;
    
    for( unsigned int i = 0; i < getGUSData( id ).possiblySupportedByThis[ POSITIVE ].size(); i++ )
    {
        Variable* var = getGUSData( id ).possiblySupportedByThis[ POSITIVE ][ i ];        
        
        if( !var->isFalse() && !getGUSData( var->getId() ).founded )
        {
            trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << *var );
            propagateSourcePointer( var->getId(), Literal( variableWithSourcePointer ) );
        }
    }
    
    for( unsigned int i = 0; i < getGUSData( id ).auxVariablesSupportedByThis[ POSITIVE ].size(); i++ )
    {
        Variable* var = getGUSData( id ).auxVariablesSupportedByThis[ POSITIVE ][ i ];
        
        if( !var->isFalse() )
        {
            assert_msg( !getGUSData( var->getId() ).founded, "Variable " << *var << " is founded" );
            trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << *var );
            propagateSourcePointer( var->getId(), Literal( variableWithSourcePointer ) );
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
            if( !externalLiterals[ i ].isFalse() )
            {
                trace_msg( unfoundedset, 1, "Literal " << getGUSData( id ).externalLiterals[ i ] << " is an external source pointer of " << *getGUSData( id ).variable );
                propagateSourcePointer( id, externalLiterals[ i ] );
                return;
            }
        }

        vector< Literal >& internalLiterals = getGUSData( id ).internalLiterals;
        for( unsigned int i = 0; i < internalLiterals.size(); i++ )
        {
            if( !internalLiterals[ i ].isFalse() && getGUSData( internalLiterals[ i ].getVariable()->getId() ).founded )
            {
                trace_msg( unfoundedset, 1, "Literal " << getGUSData( id ).internalLiterals[ i ] << " is an internal source pointer of " << *getGUSData( id ).variable );
                propagateSourcePointer( id, internalLiterals[ i ] );
                return;
            }
        }
    }    
}

void
Component::addVariableSupportedByLiteral(
    Variable* variable,
    Literal literal )
{
    Variable* tmp = literal.getVariable();
    if( tmp->inTheSameComponent( variable ) )
        getGUSData( tmp->getId() ).supportedByThisInternalRule[ literal.getSign() ].add( variable );
    else
        getGUSData( tmp->getId() ).supportedByThisExternalRule[ literal.getSign() ].add( variable );        
}

void
Component::propagateSourcePointer(
    unsigned int id,
    Literal literal )
{
    if( !getGUSData( id ).aux )
    {
        assert( !getGUSData( id ).founded );
        getGUSData( id ).sourcePointer = literal;
        setVariableFounded( id, true );
//        addVariableSupportedByLiteral( getGUSData( id ).variable, literal );
        foundSourcePointer( id );
    }
    else
    {
        assert( getGUSData( id ).numberOfSupporting > 0 );
        getGUSData( id ).numberOfSupporting--;
        if( getGUSData( id ).numberOfSupporting == 0 )
        {
            setVariableFounded( id, true );
            foundSourcePointer( id );
        }
    }
}

void
Component::addExternalLiteral(
    unsigned int id,
    Literal literal )
{
    if( !getGUSData( id ).aux )
        getGUSData( id ).externalLiterals.push_back( literal );    
}

void
Component::addInternalLiteral(
    unsigned int id,
    Literal literal )
{
    if( !getGUSData( id ).aux )
        getGUSData( id ).internalLiterals.push_back( literal );
    else
        getGUSData( id ).literals.push_back( literal );    
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
            assert( literal.isFalse() );
            if( literal.getDecisionLevel() > 0 )
                learning.onNavigatingLiteralForUnfoundedSetLearning( literal );        
        }

        for( unsigned int i = 0; i < getGUSData( id ).internalLiterals.size(); i++ )
        {
            Literal literal = getGUSData( id ).internalLiterals[ i ];            
            if( literal.getDecisionLevel() > 0 && getGUSData( literal.getVariable()->getId() ).founded )
            {
                assert( literal.isFalse() );
                learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
            }
        }
    }    
}

void
Component::resetSourcePointer(
    unsigned int id )
{
    if( !getGUSData( id ).aux )
    {
        Literal sourcePointer = getGUSData( id ).sourcePointer;        
//        assert_msg( sourcePointer != Literal::null, "Variable " << *( getGUSData( id ).variable ) << " has no source pointer" );        
        if( sourcePointer != Literal::null )
        {
            trace_msg( unfoundedset, 3, "Set " << sourcePointer << " as source pointer for variable " << *getGUSData( id ).variable );
            addVariableSupportedByLiteral( getGUSData( id ).variable, sourcePointer );
        }
    }
    else
    {
        trace_msg( unfoundedset, 3, "Aux variable " << *getGUSData( id ).variable << " has a source pointer" );
        getGUSData( id ).numberOfSupporting = 0;
    }
}

void
Component::setVariableFounded(
    unsigned int id,
    bool value )
{
    if( value && !getGUSData( id ).founded )
        resetSourcePointer( id );
    getGUSData( id ).founded = value;    
}

GUSData&
Component::getGUSData(
    unsigned int id )
{
    assert( id < gusData.size() );
    assert( gusData[ id ] != NULL );
    return *gusData[ id ];
}

#endif
