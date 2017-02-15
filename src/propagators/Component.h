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

#ifndef WASP_COMPONENT_H
#define WASP_COMPONENT_H

#include <vector>
#include <cassert>
#include <iostream>

#include "../stl/Vector.h"
#include "PostPropagator.h"
#include "../Literal.h"
#include "../GUSData.h"
#include "../util/WaspOptions.h"
#include "../util/WaspTrace.h"
#include "../util/WaspAssert.h"

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
        inline Component( vector< GUSData* >& gusData_, Solver& s ) : PostPropagator(), solver( s ), gusData( gusData_ ), clauseToPropagate( NULL ), conflict( 0 ), id( 0 ), done( 0 ), first( 1 ), removed( 0 ), numberOfCalls( 0 ) {}
        ~Component();        
        
        virtual bool onLiteralFalse( Literal lit );

        inline bool isCyclic() const { return variablesInComponent.size() > 1; }
        inline unsigned int size() const { return variablesInComponent.size(); }

        inline void addVariable( unsigned int var ) { variablesInComponent.push_back( var ); }
        inline Var getVariable( unsigned int i ) const { assert( i < variablesInComponent.size() ); return variablesInComponent[ i ]; }

        inline void setId( unsigned int id ) { this->id = id; }
        inline unsigned int getId() const { return id; }
        
        virtual Clause* getClauseToPropagate( Learning& learning );

        inline bool isAuxVariable( unsigned int varId ) { return getGUSData( varId ).isAux(); }
        inline void setAuxVariable( unsigned int varId ) { getGUSData( varId ).setAux(); }
        inline void addExternalLiteralForVariable( unsigned int varId, Literal lit ) { addExternalLiteral( varId, lit ); /*getGUSData( varId ).externalLiterals.push_back( lit );*/ }
        inline void addInternalLiteralForVariable( unsigned int varId, Literal lit ) { addInternalLiteral( varId, lit ); /*getGUSData( varId ).internalLiterals.push_back( lit );*/ }
        inline void addVariablePossiblySupportedByLiteral( Var var, Literal lit ) { gusData[ lit.getVariable() ]->possiblySupportedByThis[ lit.getSign() ].push_back( var ); }
        inline void addAuxVariableSupportedByLiteral( Var var, Literal lit ) { gusData[ lit.getVariable() ]->auxVariablesSupportedByThis[ lit.getSign() ].push_back( var ); }
        
        inline void variableHasNoSourcePointer( Var var );
        void onLearningForUnfounded( unsigned int id, Learning& );
        
        void remove() { removed = 1; }
        bool isRemoved() const { return removed; }
        
        Clause* inferFalsityOfUnfoundedAtoms();
        void conflictOnUnfoundedAtom( Clause* clause, Var variable );                
        
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
        Vector< Clause* > clausesToDelete;
                
        Var conflict;        
        
        unsigned int id : 29;        
        unsigned int done : 1;
        unsigned int first : 1;
        unsigned int removed : 1;
        
        vector< unsigned int > added;
        unsigned int numberOfCalls;
        
        bool propagateFalseForGUS( Literal lit );
        inline void propagateLiteralLostSourcePointer( Literal lit );
        bool iterationOnSupportedByThisExternal( Literal lit );
        bool iterationOnSupportedByThisInternal( Literal lit );
        void iterationOnAuxSupportedByThis( Literal lit );        

        void foundSourcePointer( Var var, vector< Var >& vars, vector< Literal >& lits );
        inline void lookForANewSourcePointer( Var var );
        void propagateSourcePointer( Var var, Literal literal );
        inline void addExternalLiteral( Var var, Literal literal );
        inline void addInternalLiteral( Var var, Literal literal );
        inline void resetSourcePointer( Var var );
        inline Clause* handleConflict();

//        inline void setVariableFounded( unsigned int id, bool value );
        inline void setFounded( Var var );
        inline void setUnfounded( Var var );
        
        void addVariableSupportedByLiteral( Var variable, Literal literal );
        
        void removeFalseAtomsAndPropagateUnfoundedness();
        
        inline GUSData& getGUSData( Var var );
        
        void computeGUS();
        void computeGUSFirst();
        bool computeUnfoundedSet( Var var );
        
        inline void visit( Var var ) { assert_msg( var < added.size(), var << " >= " << added.size() ); added[ var ] = numberOfCalls; }
        inline bool visited( Var var ) const { assert_msg( var < added.size(), var << " >= " << added.size() ); return added[ var ] == numberOfCalls; }
        
        inline Var updateClauseToPropagate();
        
        virtual bool hasToAddClause() const;

        
        #ifndef NDEBUG
        bool checkSourcePointersStatus();
        bool checkFoundedStatus();
        void printVariablesWithoutSourcePointer();
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
    assert( !getGUSData( variable ).isInQueue() );
    assert( !variablesWithoutSourcePointer.existElement( variable ) );

    setUnfounded( variable );
    getGUSData( variable ).setInQueue();
    variablesWithoutSourcePointer.push_back( variable );
}

void
Component::addExternalLiteral(
    Var var,
    Literal literal )
{
    if( !getGUSData( var ).isAux() )
        getGUSData( var ).externalLiterals.push_back( literal );
    else
        getGUSData( var ).literals.push_back( literal ); 
}

void
Component::addInternalLiteral(
    Var var,
    Literal literal )
{
    if( !getGUSData( var ).isAux() )
        getGUSData( var ).internalLiterals.push_back( literal );
    else
        getGUSData( var ).literals.push_back( literal );
}

void
Component::resetSourcePointer(
    Var var )
{
    assert( getGUSData( var ).isFounded() );
    if( !getGUSData( var ).isAux() )
    {
        Literal sourcePointer = getGUSData( var ).sourcePointer;
//        assert_msg( sourcePointer != Literal::null, "Variable " << *( getGUSData( id ).var() ) << " has no source pointer" );
        if( sourcePointer != Literal::null )
        {
            trace_msg( unfoundedset, 3, "Set " << sourcePointer << " as source pointer for variable " << Literal( var, POSITIVE ) );
            addVariableSupportedByLiteral( var, sourcePointer );
        }
    }
    else
    {        
        trace_msg( unfoundedset, 3, "Aux variable " << Literal( var, POSITIVE ) << " has a source pointer" );
        getGUSData( var ).numberOfSupporting = 0;
    }
}

void
Component::setUnfounded(
    Var var )
{
    getGUSData( var ).setUnfounded();
}

void
Component::setFounded(
    Var var )
{    
    if( getGUSData( var ).isFounded() )
    {
        assert_msg( !getGUSData( var ).isAux() || getGUSData( var ).numberOfSupporting == 0, "Aux: " << Literal( var, POSITIVE ) << " - Without source: " << getGUSData( var ).numberOfSupporting );
        return;
    }

    getGUSData( var ).setFounded();
    resetSourcePointer( var );
}

GUSData&
Component::getGUSData(
    Var var )
{
    assert( var < gusData.size() );
    assert( gusData[ var ] != NULL );
    return *gusData[ var ];
}

#endif
