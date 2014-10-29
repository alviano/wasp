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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <cassert>
#include <iostream>

#include "stl/Vector.h"
#include "PostPropagator.h"
#include "Literal.h"
#include "GUSData.h"
#include "util/Options.h"
#include "util/Trace.h"

class Learning;
class Solver;
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
    friend ostream& operator<<( ostream& o, const Component& c );
    public:
        inline Component( vector< GUSData* >& gusData_, Solver& s ) : PostPropagator(), solver( s ), gusData( gusData_ ), clauseToPropagate( NULL ), id( 0 ), removed( 0 ) {}
        inline ~Component() {}
        
        virtual bool onLiteralFalse( Literal lit );

        inline bool isCyclic() const { return variablesInComponent.size() > 1; }
        inline unsigned int size() const { return variablesInComponent.size(); }

        inline void addVariable( unsigned int var ) { variablesInComponent.push_back( var ); }
        inline Var getVariable( unsigned int i ) const { assert( i < variablesInComponent.size() ); return variablesInComponent[ i ]; }

        inline void setId( unsigned int id ) { this->id = id; }
        inline unsigned int getId() const { return id; }
        
        virtual Clause* getClauseToPropagate( Learning& learning );

        inline bool isAuxVariable( unsigned int varId ) { return getGUSData( varId ).aux; }
        inline void setAuxVariable( unsigned int varId ) { getGUSData( varId ).aux = true; }
        inline void addExternalLiteralForVariable( unsigned int varId, Literal lit ) { addExternalLiteral( varId, lit ); /*getGUSData( varId ).externalLiterals.push_back( lit );*/ }
        inline void addInternalLiteralForVariable( unsigned int varId, Literal lit ) { addInternalLiteral( varId, lit ); /*getGUSData( varId ).internalLiterals.push_back( lit );*/ }
        inline void addVariablePossiblySupportedByLiteral( Var var, Literal lit ) { gusData[ lit.getVariable() ]->possiblySupportedByThis[ lit.getSign() ].push_back( var ); }
        inline void addAuxVariableSupportedByLiteral( Var var, Literal lit ) { gusData[ lit.getVariable() ]->auxVariablesSupportedByThis[ lit.getSign() ].push_back( var ); }
        
        inline void variableHasNoSourcePointer( Var var );
        void onLearningForUnfounded( unsigned int id, Learning& );
        
        void remove() { removed = 1; }
        bool isRemoved() const { return removed; }
        
    protected:
        virtual void reset();

    private:
        inline Component( const Component& orig );
        Solver& solver;
        vector< unsigned int > variablesInComponent;
        vector< GUSData* >& gusData;
        Vector< Var > variablesWithoutSourcePointer;
        Vector< Var > unfoundedSet;
        Clause* clauseToPropagate;        
        
        unsigned int id : 31;
        unsigned int removed : 1;        
        
        bool propagateFalseForGUS( Literal lit );
        inline void propagateLiteralLostSourcePointer( Literal lit );
        bool iterationOnSupportedByThisExternal( Literal lit );
        bool iterationOnSupportedByThisInternal( Literal lit );
        void iterationOnAuxSupportedByThis( Literal lit );        

        void foundSourcePointer( unsigned int id );
        inline void lookForANewSourcePointer( unsigned int id );
        inline void propagateSourcePointer( unsigned int id, Literal literal );
        inline void addExternalLiteral( unsigned int id, Literal literal );
        inline void addInternalLiteral( unsigned int id, Literal literal );
        inline void resetSourcePointer( unsigned int id );

        inline void setVariableFounded( unsigned int id, bool value );
        
        void addVariableSupportedByLiteral( Var variable, Literal literal );
        
        inline GUSData& getGUSData( unsigned int id );
        
        void computeGUS();
        
        #ifndef NDEBUG
        bool checkSourcePointersStatus();        
        #endif
};

//bool ComponentComparator::operator()( const Component* c1, const Component* c2 ) const { return c1->getId() < c2->getId(); }

void
Component::propagateLiteralLostSourcePointer(
    Literal lit )
{
    iterationOnSupportedByThisInternal( lit );
    iterationOnAuxSupportedByThis( lit );
}

void
Component::variableHasNoSourcePointer(
    Var variable )
{    
    assert( !getGUSData( variable ).inQueue );
    
    setVariableFounded( variable, false );
    getGUSData( variable ).inQueue = true;
    
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
    else
        getGUSData( id ).literals.push_back( literal ); 
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
Component::resetSourcePointer(
    unsigned int id )
{
    if( !getGUSData( id ).aux )
    {
        Literal sourcePointer = getGUSData( id ).sourcePointer;        
//        assert_msg( sourcePointer != Literal::null, "Variable " << *( getGUSData( id ).variable ) << " has no source pointer" );        
        if( sourcePointer != Literal::null )
        {
            trace_msg( unfoundedset, 3, "Set " << sourcePointer << " as source pointer for variable " << getGUSData( id ).variable );
            addVariableSupportedByLiteral( getGUSData( id ).variable, sourcePointer );
        }
    }
    else
    {
        trace_msg( unfoundedset, 3, "Aux variable " << getGUSData( id ).variable << " has a source pointer" );
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
