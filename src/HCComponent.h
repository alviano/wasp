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

class HCComponent : public PostPropagator
{
    friend ostream& operator<<( ostream& out, const HCComponent& component );
    public:
        HCComponent( vector< GUSData* >& gusData_, Solver& s, unsigned numberOfInputAtoms );
        ~HCComponent();

        virtual void reset();

        virtual Clause* getClauseToPropagate( Learning& learning );
        virtual bool onLiteralFalse( Literal literal );

        void addClauseToChecker( Clause* c, Var headAtom );
        
        inline void addHCVariable( Var v ) { inUnfoundedSet[ v ] |= 4; hcVariables.push_back( v ); numberOfInternalVariables++; }
//        inline bool isExternal( Var v ) const { assert( numberOfCalls == 0 ); assert( v < inUnfoundedSet.size() ); return inUnfoundedSet[ v ] & 3; }
        bool addExternalLiteral( Literal lit );
        bool addExternalLiteralForInternalVariable( Literal lit );
        
        inline unsigned int size() const { return hcVariables.size(); }
        inline Var getVariable( unsigned int pos ) const { assert( pos < hcVariables.size() ); return hcVariables[ pos ]; }
        
        inline unsigned int externalLiteralsSize() const { return externalLiterals.size(); }
        inline Literal getExternalLiteral( unsigned int pos ) const { assert( pos < externalLiterals.size() ); return externalLiterals[ pos ]; }
        
        void computeReasonForUnfoundedAtom( Var v, Learning& learning );
        
        GUSData& getGUSData( Var v ) { assert( v < gusData.size() ); return *( gusData[ v ] ); }
        void printLearnedClausesOfChecker() { checker.printLearnedClauses(); }
        void setHasToTestModel( bool b ) { hasToTestModel = b; }
        
        void setId( unsigned int i ) { id = i; }
        unsigned int getId() const { return id; }
        
        void addLearnedClausesFromChecker( Clause* c );
        
    private:                
        inline HCComponent( const HCComponent& orig );

        bool isInUnfoundedSet( Var v ) { assert_msg( v < inUnfoundedSet.size(), "v = " << v << "; inUnfoundedSet.size() = " << inUnfoundedSet.size() ); return inUnfoundedSet[ v ] == numberOfCalls; }
        void setInUnfoundedSet( Var v ) { assert_msg( v < inUnfoundedSet.size(), "v = " << v << "; inUnfoundedSet.size() = " << inUnfoundedSet.size() ); inUnfoundedSet[ v ] = numberOfCalls; }
        
        bool sameComponent( Var v ) const;
        
        void createInitialClauseAndSimplifyHCVars();
        
        void clearUnfoundedSetCandidates();
        Clause* computeClause();
        
        Var addFreshVariable();
        
        void initDataStructures();
        void checkModel( vector< Literal >& assumptions );
        
        void sendLearnedClausesToSolver();

        vector< GUSData* >& gusData;
        Vector< Literal > trail;
        Solver& solver;
        Solver checker;

        vector< Var > hcVariables;
        vector< Literal > externalLiterals;
        Vector< Var > unfoundedSet;
        Vector< Literal > unfoundedSetCandidates;
        
        Vector< Clause* > learnedClausesFromChecker;
        
        vector< Var > generatorToCheckerId;
        vector< Var > checkerToGeneratorId;
//        vector< bool > usedAuxVars;        
        
        Vector< unsigned int > inUnfoundedSet;
        unsigned int numberOfCalls;
        
        bool hasToTestModel;        
        
        unsigned int numberOfAtoms;
        unsigned int id;
        
        Literal assumptionLiteral;
        bool isConflictual;
        
        unsigned int numberOfExternalLiterals;
        unsigned int numberOfInternalVariables;
        unsigned int numberOfZeroLevel;
        unsigned int removedHCVars;
        
        Literal literalToAdd;
        
        unsigned int low;
        unsigned int high;
        
        void testModel();
        void computeAssumptions( vector< Literal >& assumptions );
        void iterationInternalLiterals( vector< Literal >& assumptions );
        void iterationExternalLiterals( vector< Literal >& assumptions );
        
        inline Var getCheckerVarFromExternalLiteral( Literal l ) const
        { 
            if( l.isNegative() || sameComponent( l.getVariable() ) )
                return generatorToCheckerId[ l.getVariable() ];
            else            
                return l.getVariable();            
//            return l.isPositive() ? l.getVariable() : generatorToCheckerId[ l.getVariable() ]; 
        }
        inline Var getCheckerTrueVar( Var v ) const { return v; }
        inline Var getCheckerFalseVar( Var v ) const
        {
            assert( v < generatorToCheckerId.size() );
            assert( generatorToCheckerId[ v ] > 0 );
            
            //Can be UINT_MAX;
            return generatorToCheckerId[ v ];
        }

        inline Literal getGeneratorLiteralFromCheckerLiteral( Literal l )
        {
            Var checkerVar = l.getVariable();
            if( checkerVar <= numberOfAtoms )
                return l;

            assert( checkerVar < checkerToGeneratorId.size() );
            Var v = checkerToGeneratorId[ checkerVar ];
            assert( v > 0 && v <= solver.numberOfVariables() );
            return ( solver.getHCComponent( v ) != this ) ? Literal( v, NEGATIVE ) : Literal( v, POSITIVE );
        }     
        
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

#endif
