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

#ifndef UNFOUNDEDFREEHC_H
#define	UNFOUNDEDFREEHC_H

#include <vector>
#include "util/Assert.h"
#include "stl/Vector.h"
#include "PostPropagator.h"
#include "Literal.h"
#include "Solver.h"
using namespace std;

class Clause;
class Learning;
class Rule;

class UnfoundedFreeHC : public PostPropagator
{
    friend ostream& operator<<( ostream& out, const UnfoundedFreeHC& component );
    public:
        UnfoundedFreeHC( vector< GUSData* >& gusData_, Solver& s, unsigned numberOfInputAtoms );
        ~UnfoundedFreeHC();

        virtual void reset();
        virtual Clause* getClauseToPropagate( Learning& learning );
        virtual bool onLiteralFalse( Literal literal );
        
        inline void addHCVariable( Var v );
        
        void processRule( Rule* rule );
        inline unsigned int size() const { return hcVariables.size(); }
        inline Var getVariable( unsigned int pos ) const { assert( pos < hcVariables.size() ); return hcVariables[ pos ]; }        
        
        void computeReasonForUnfoundedAtom( Var v, Learning& learning );        
        void setHasToTestModel( bool b ) { hasToTestModel = b; }        
        void setId( unsigned int i ) { id = i; }
        
        void processBeforeStarting();
        
    private:
        vector< GUSData* >& gusData;
        Vector< Literal > trail;
        Solver& solver;
        Solver checker;

        vector< Var > hcVariables;
        vector< Var > hcVariablesNotTrueAtLevelZero;
        vector< Var > externalVars;
        
        Vector< Var > unfoundedSet;
        unsigned int varsAtLevelZero;
        
        unsigned int numberOfAttachedVars;
        
        unsigned int id : 30;
        unsigned int isConflictual : 1;
        unsigned int hasToTestModel : 1;        
        
        unsigned int numberOfCalls;
        vector< unsigned int > inUnfoundedSet;
        
        unsigned int low;
        unsigned int high;
        vector< Clause* > toDelete;
        
        inline UnfoundedFreeHC( const UnfoundedFreeHC& orig );

        inline GUSData& getGUSData( Var v ) { assert( v < gusData.size() ); return *( gusData[ v ] ); }
        inline Var addFreshVariable() { checker.addVariableRuntime(); return checker.numberOfVariables(); }        
        inline bool sameComponent( Var v ) const { return solver.getHCComponent( v ) == this; }
        
        inline bool isInUnfoundedSet( Var v ) const { return inUnfoundedSet[ v ] == numberOfCalls; }
        
        inline void setInUnfoundedSet( Var v )
        {
            assert( v < inUnfoundedSet.size() ); 
            inUnfoundedSet[ v ] = numberOfCalls;
            unfoundedSet.push_back( v );
        }
        
        void addClausesForHCAtoms();
        
        inline void attachVar( Var v );        
        
        void initDataStructures();
        void checkModel( vector< Literal >& assumptions );        
        
        void testModel();
        void computeAssumptions( vector< Literal >& assumptions );
        void iterationInternalLiterals( vector< Literal >& assumptions );
        void iterationExternalLiterals( vector< Literal >& assumptions );               
        
        inline bool addLiteralInClause( Literal lit, Clause* clause );
        inline void addExternalVariable( Var v );
        
        #ifdef TRACE_ON
        template< class T >
        void printVector( const vector< T >& v, const string& description )
        {            
            trace_tag( cerr, modelchecker, 2 );
            cerr << description << ": ";
            if( !v.empty() )
            {
                cerr << v[ 0 ];
                for( unsigned int i = 1; i < v.size(); i++ )
                    cerr << ", " << v[ i ];
            }
            else
                cerr << "empty";
            cerr << endl;
        }
                
        void printVector( const Vector< Literal >& v, const string& description )
        {            
            trace_tag( cerr, modelchecker, 2 );
            cerr << description << ": ";
            if( !v.empty() )
            {
                cerr << v[ 0 ];
                for( unsigned int i = 1; i < v.size(); i++ )
                    cerr << ", " << v[ i ];
            }
            else
                cerr << "empty";
            cerr << endl;
        }
        
        void printVector( const Vector< Var >& v, const string& description )
        {            
            trace_tag( cerr, modelchecker, 2 );
            cerr << description << ": ";
            if( !v.empty() )
            {
                cerr << Literal( v[ 0 ] );
                for( unsigned int i = 1; i < v.size(); i++ )
                    cerr << ", " << Literal( v[ i ] );
            }
            else
                cerr << "empty";
            cerr << endl;
        }
        #endif
};

void
UnfoundedFreeHC::addHCVariable(
    Var v )
{
    assert( find( hcVariables.begin(), hcVariables.end(), v ) == hcVariables.end() );
    hcVariables.push_back( v );
    attachVar( v );
    solver.setComponent( v, NULL );
    solver.setHCComponent( v, this );
    
    checker.addVariable();    
    getGUSData( v ).unfoundedVarForHCC = checker.numberOfVariables();
    checker.addVariable();
    getGUSData( v ).headVarForHCC = checker.numberOfVariables();  
    
    trace_msg( modelchecker, 1, "Adding variable: " << Literal( v, POSITIVE ) << " with id: " << v 
            << " - u" << Literal( v, POSITIVE ) << " with id: " << getGUSData( v ).unfoundedVarForHCC
            << " - h" << Literal( v, POSITIVE ) << " with id: " << getGUSData( v ).headVarForHCC );
//    #ifdef TRACE_ON
//    VariableNames::setName( getGUSData( v ).unfoundedVarForHCC, "u" + VariableNames::getName( v ) );
//    VariableNames::setName( getGUSData( v ).headVarForHCC, "h" + VariableNames::getName( v ) );
//    #endif
}

void
UnfoundedFreeHC::attachVar(
    Var v )
{
    numberOfAttachedVars++;
    solver.setFrozen( v );
    solver.addPostPropagator( Literal( v, POSITIVE ), this );
    solver.addPostPropagator( Literal( v, NEGATIVE ), this );
}

bool
UnfoundedFreeHC::addLiteralInClause(
    Literal lit,
    Clause* clause )
{
    bool retVal = checker.isTrue( lit ) ? false : true;
    if( !checker.isUndefined( lit ) )
        clause->addLiteral( lit );
    return retVal;
}

void
UnfoundedFreeHC::addExternalVariable(
    Var v )
{
    //TODO: FIX ME with a smarter strategy!
    if( find( externalVars.begin(), externalVars.end(), v ) != externalVars.end() )
        return;
    
    externalVars.push_back( v );
    attachVar( v );
}

#endif
