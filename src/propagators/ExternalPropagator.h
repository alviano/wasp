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

#ifndef WASP_EXTERNALPROPAGATOR_H
#define WASP_EXTERNALPROPAGATOR_H

#include "Propagator.h"
#include "../stl/Vector.h"
class Clause;
class Interpreter;

#include <string>
#include <vector>
using namespace std;

#define method_plugins_addedVarName "addedVarName"
#define method_plugins_onVariableElimination "onVariableElimination" 
#define method_plugins_getLiterals "getLiterals"
#define method_plugins_getVariablesToFreeze "getVariablesToFreeze"
#define method_plugins_onStartingSolver "onStartingSolver"
#define method_plugins_onLitTrue "onLiteralTrue"
#define method_plugins_onLitsTrue "onLiteralsTrue"
#define method_plugins_onLiteralsUndefined "onLiteralsUndefined"
#define method_plugins_getReason "getReason"
#define method_plugins_getReasonForLiteral "getReasonForLiteral"
#define method_plugins_simplifyAtLevelZero "simplifyAtLevelZero"
#define method_plugins_onAnswerSet "onAnswerSet"
#define method_plugins_checkAnswerSet "checkAnswerSet"
#define method_plugins_checkPartialInterpretation "checkPartialInterpretation"
#define method_plugins_getReasonsForCheckFailure "getReasonsForCheckFailure"
#define method_plugins_foundLB "onNewLowerBound" 
#define method_plugins_foundUB "onNewUpperBound" 
#define method_plugins_endPropagation "endPropagation"
#define method_plugins_addWeakConstraints "addWeakConstraints"
#define method_plugins_addWeightsForWeakConstraints "addWeightsForWeakConstraints"
#define method_plugins_scriptdirectory "setScriptDirectory"

#define attribute_plugins_atomNames "atomNames"

class ExternalPropagator : public Propagator
{
    public:
        ExternalPropagator( const string& filename, unsigned int interpr, Solver& solver, string scriptDirectory );        
        virtual ~ExternalPropagator();
        
        virtual bool onLiteralFalse( Solver& solver, Literal literal, PropagatorData );
        virtual void reset( const Solver& solver );
        virtual void simplifyAtLevelZero( Solver& solver );
        
        virtual void addedVarName( Var var, const string& name );
        virtual void onAtomElimination( Var var );
        virtual void endParsing( Solver& solver ) { attachWatches( solver ); }        
        virtual void onAnswerSet( const Solver& solver );
        virtual void foundLowerBound( uint64_t lb );
        virtual void foundUpperBound( uint64_t ub );
        virtual bool checkAnswerSet( Solver& solver );
        virtual bool checkPartialInterpretation( Solver& solver );
        virtual Clause* getReasonForCheckFailure( Solver& solver );        
        virtual void endUnitPropagation( Solver& solver );
        virtual void endPropagation( Solver& solver );
        virtual void onStartingSolver();
        
        virtual bool hasWeakConstraintsToAdd( const Solver& solver );
        virtual uint64_t addWeakConstraints( const Solver& solver, vector< Clause* >& weakConstraints, vector< uint64_t >& weights_ );
        virtual bool isForLazyWeakConstraints() const { return check_addWeakConstraints; }

    protected:
        ExternalPropagator();
        inline ExternalPropagator( const ExternalPropagator& orig );
        void checkWellFormed();
        void checkWellFormed( const string& );
        void computeReason( Solver& solver, const vector< int >& output );
        Clause* getReason( Solver&, Literal lit );
        void handleConflict( Solver&, Literal );
        void clearClausesToDelete();
        void attachWatches( Solver& solver );
        void checkIdOfLiteral( const Solver& solver, int id ) const;
        
        Vector< Literal > trail;
        Vector< Literal > trueLiterals;
        Interpreter* interpreter;
        Vector< Clause* > clausesToDelete;
        char* fn;
        bool check_addedVarName;
        bool check_onAtomElimination;
        bool check_simplifyAtLevelZero;
        bool check_onAnswerSet;
        bool check_checkAnswerSet;
        bool check_checkPartialInterpretation;
        bool check_getReasonForLiteral;
        bool check_getReasonForCheckFailure;
        bool check_onNewLowerBound;
        bool check_onNewUpperBound;
        bool check_onLitsTrue;
        bool check_onLitTrue;
        bool check_endPropagation;
        bool check_addWeakConstraints;
        bool check_addWeightsForWeakConstraints;
        
        bool check_attribute_atomNames;
        
        vector< int > weakConstraints;
        vector< uint64_t > weights;
};

#endif

