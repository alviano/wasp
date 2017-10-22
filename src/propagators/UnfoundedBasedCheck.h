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

#ifndef WASP_UNFOUNDEDBASEDCHECK_H
#define	WASP_UNFOUNDEDBASEDCHECK_H

#include <vector>
#include "../util/WaspAssert.h"
#include "../stl/Vector.h"
#include "HCComponent.h"
#include "../Literal.h"
#include "../Solver.h"
using namespace std;

class Clause;
class Learning;
class Rule;

class UnfoundedBasedCheck : public HCComponent
{
    public:
        UnfoundedBasedCheck( vector< GUSData* >& gusData_, Solver& s, unsigned numberOfInputAtoms );
        inline ~UnfoundedBasedCheck(){}

        virtual bool onLiteralFalse( Literal literal );                
        
        void processRule( Rule* rule );      
        void processComponentBeforeStarting();
        
    private:
        inline void addHCVariableProtected( Var v );
        vector< Var > hcVariablesNotTrueAtLevelZero;
        vector< Var > externalVars;

        unsigned int varsAtLevelZero;        
        unsigned int numberOfAttachedVars;
                
        inline UnfoundedBasedCheck( const UnfoundedBasedCheck& orig );                        
        
        void addClausesForHCAtoms();
        
        inline void attachVar( Var v ){ numberOfAttachedVars++; attachLiterals( Literal( v, POSITIVE ) ); }    
        
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
UnfoundedBasedCheck::addHCVariableProtected(
    Var v )
{    
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

bool
UnfoundedBasedCheck::addLiteralInClause(
    Literal lit,
    Clause* clause )
{
    bool retVal = checker.isTrue( lit ) ? false : true;
    if( !checker.isUndefined( lit ) )
        clause->addLiteral( lit );
    return retVal;
}

void
UnfoundedBasedCheck::addExternalVariable(
    Var v )
{
    //TODO: FIX ME with a smarter strategy!
    if( find( externalVars.begin(), externalVars.end(), v ) != externalVars.end() )
        return;
    
    externalVars.push_back( v );
    attachVar( v );
}

#endif
