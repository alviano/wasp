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
        
        inline void addHCVariable( Var v ) { inUnfoundedSet[ v ] |= 4; hcVariables.push_back( v ); }
        inline bool isExternal( Var v ) const { assert( numberOfCalls == 0 ); assert( v < inUnfoundedSet.size() ); return inUnfoundedSet[ v ] & 3; }
        bool addExternalLiteral( Literal lit );
        
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
        
        vector< GUSData* >& gusData;
        Vector< Literal > trail;
        Solver& solver;
        Solver checker;

        vector< Var > hcVariables;
        vector< Literal > externalLiterals;
        Vector< Var > unfoundedSet;
        
        Vector< Clause* > learnedClausesFromChecker;
        
        vector< Var > generatorToCheckerId;
        vector< Var > checkerToGeneratorId;
        
        Vector< unsigned int > inUnfoundedSet;
        unsigned int numberOfCalls;
        
        bool hasToTestModel;
        
        unsigned int numberOfAtoms;
        unsigned int id;

        void testModel();
        void computeAssumptions( vector< Literal >& assumptionsAND, vector< Literal >& assumptionsOR );
        
        inline Var getCheckerVarFromExternalLiteral( Literal l ) const { return l.isPositive() ? l.getVariable() : generatorToCheckerId[ l.getVariable() ]; }
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
            if( l.getVariable() <= numberOfAtoms )
                return l;
                        
            assert( l.getVariable() < checkerToGeneratorId.size() );
            assert( checkerToGeneratorId[ l.getVariable() ] > 0 );
            return Literal( checkerToGeneratorId[ l.getVariable() ], NEGATIVE );           
        }        
};

#endif
