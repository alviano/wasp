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

#ifndef WASP_REDUCTBASEDCHECK_H
#define	WASP_REDUCTBASEDCHECK_H

#include <vector>
#include "../util/WaspAssert.h"
#include "../stl/Vector.h"
#include "HCComponent.h"
#include "../Literal.h"
#include "../input/Rule.h"
using namespace std;

class Clause;
class Learning;

class ReductBasedCheck : public HCComponent
{    
    public:
        ReductBasedCheck( vector< GUSData* >& gusData_, Solver& s, unsigned numberOfInputAtoms );
        inline ~ReductBasedCheck(){}

        virtual bool onLiteralFalse( Literal literal );

        void addClauseToChecker( Clause* c );                
                              
        void processRule( Rule* rule );
        void processComponentBeforeStarting();

    private:                
        inline ReductBasedCheck( const ReductBasedCheck& orig );
        void createDefiningRules( Rule* rule, Clause* clause );        
        void createInitialClauseAndSimplifyHCVars();
                
        void clearUnfoundedSetCandidates();
        Clause* computeClause();
        
        bool addExternalLiteral( Literal lit );
        bool addExternalLiteralForInternalVariable( Literal lit );                
        
        inline unsigned int externalLiteralsSize() const { return externalLiterals.size(); }
        inline Literal getExternalLiteral( unsigned int pos ) const { assert( pos < externalLiterals.size() ); return externalLiterals[ pos ]; }        
        
        void addHCVariableProtected( Var v );
        void initDataStructures();
        void checkModel( vector< Literal >& assumptions );        
                
        vector< Literal > externalLiterals;
        
        vector< Var > generatorToCheckerId;
        
        unsigned int numberOfAtoms;        
        
        Literal assumptionLiteral;        
        
        unsigned int numberOfExternalLiterals;
        unsigned int numberOfInternalVariables;
        unsigned int numberOfZeroLevel;
        unsigned int removedHCVars;
        
        Literal literalToAdd;
        
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
