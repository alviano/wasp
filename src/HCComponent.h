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

#ifndef HCCOMPONENT_H
#define	HCCOMPONENT_H

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

class HCComponent : public PostPropagator
{
    friend ostream& operator<<( ostream& out, const HCComponent& component );
    public:
        HCComponent( vector< GUSData* >& gusData_, Solver& s, unsigned numberOfInputAtoms );
        ~HCComponent();

        virtual void reset();
        virtual Clause* getClauseToPropagate( Learning& learning );
        virtual bool onLiteralFalse( Literal literal );

        void addClauseToChecker( Clause* );        
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
        vector< Literal > externalLiterals;
        Vector< Var > unfoundedSet;
        Vector< Literal > unfoundedSetCandidates;        
        
        vector< Var > generatorToCheckerId;
        
        Vector< unsigned int > inUnfoundedSet;
        unsigned int numberOfCalls;               
        
        unsigned int numberOfAtoms;
        unsigned int id : 30;
        unsigned int isConflictual : 1;
        unsigned int hasToTestModel : 1;
        
        Literal assumptionLiteral;
        Literal literalToAdd;
        
        unsigned int numberOfExternalLiterals;
        unsigned int numberOfInternalVariables;
        unsigned int numberOfZeroLevel;
        unsigned int removedHCVars;
        
        unsigned int low;
        unsigned int high;        
        
        vector< Clause* > toDelete;
        
        inline HCComponent( const HCComponent& orig );

        inline GUSData& getGUSData( Var v ) { assert( v < gusData.size() ); return *( gusData[ v ] ); }
        inline void clearUnfoundedSetCandidates() { unfoundedSetCandidates.shrink( removedHCVars ); }
        inline Var addFreshVariable() { checker.addVariableRuntime(); return checker.numberOfVariables(); }        
        inline bool isInUnfoundedSet( Var v ) { assert_msg( v < inUnfoundedSet.size(), "v = " << v << "; inUnfoundedSet.size() = " << inUnfoundedSet.size() ); return inUnfoundedSet[ v ] == numberOfCalls; }
        inline void setInUnfoundedSet( Var v ) { assert_msg( v < inUnfoundedSet.size(), "v = " << v << "; inUnfoundedSet.size() = " << inUnfoundedSet.size() ); unfoundedSet.push_back( v ); inUnfoundedSet[ v ] = numberOfCalls; }
        inline bool sameComponent( Var v ) const { return solver.getHCComponent( v ) == this; }
        inline Var getCheckerVarFromExternalLiteral( Literal l ) const { return ( l.isNegative() || sameComponent( l.getVariable() ) ) ? generatorToCheckerId[ l.getVariable() ] : l.getVariable(); }
        
        void createInitialClauseAndSimplifyHCVars();
        
        bool addExternalLiteral( Literal lit );
        bool addExternalLiteralForInternalVariable( Literal lit );        
        
        inline void attachLiterals( Literal lit );        
        
        void initDataStructures();
        void checkModel( vector< Literal >& assumptions );        
        
        void testModel();
        void computeAssumptions( vector< Literal >& assumptions );
        void iterationInternalLiterals( vector< Literal >& assumptions );
        void iterationExternalLiterals( vector< Literal >& assumptions );               
        
        inline bool addLiteralInClause( Literal lit, Clause* clause )
        {
            if( checker.isTrue( lit ) )
                return false;

            if( !checker.isFalse( lit ) )
                clause->addLiteral( lit );
            return true;
        }
        
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
HCComponent::addHCVariable(
    Var v )
{
    inUnfoundedSet[ v ] |= 4;
    hcVariables.push_back( v );
    numberOfInternalVariables++; 
    attachLiterals( Literal( v, POSITIVE ) );
    solver.setComponent( v, NULL );
    solver.setHCComponent( v, this );
}

void
HCComponent::attachLiterals(
    Literal lit )
{
    solver.setFrozen( lit.getVariable() );
    solver.addPostPropagator( lit, this );
    solver.addPostPropagator( lit.getOppositeLiteral(), this );
}

#endif
