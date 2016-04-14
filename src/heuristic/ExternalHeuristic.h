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

#ifndef EXTERNALHEURISTIC_H
#define EXTERNALHEURISTIC_H

#define method_addedVarName "addedVarName"

//Preprocessing
#define method_onStartingParsing "onStartingParsing"
#define method_onFinishedParsing "onFinishedParsing"
#define method_onStartingSimplifications "onStartingSimplifications"
#define method_onFinishedSimplifications "onFinishedSimplifications"
#define method_onVariableElimination "onVariableElimination"

//Learning
#define method_onConflict "onConflict"
#define method_onLitInvolvedInConflict "onLitInvolvedInConflict"
#define method_onLearningClause "onLearningClause"
#define method_onLitAtLevelZero "onLitAtLevelZero"
#define method_onLitInImportantClause "onLitInImportantClause"
//#define method_onLitInLearntClause "onLitInLearntClause"

//Solving
#define method_onNewClause "onNewClause"
#define method_onStartingSolver "onStartingSolver"
#define method_onDeletion "onDeletion"
#define method_onRestart "onRestart"
#define method_onAnswerSet "onAnswerSet"
#define method_onLitsTrue "onLiteralsTrue"
#define method_onVarUndefined "onVariableUndefined"

//Choice
#define method_choiceVars "choiceVars"
#define method_onChoiceContradictory "onChoiceContradictory"
#define method_ignorePolarity "ignorePolarity"
#define method_partialInterpretation "partialInterpretation"

//ASP
#define method_onUnfoundedSet "onUnfoundedSet"
#define method_onLoopFormula "onLoopFormula"

//Fallback
#define method_fallback "fallback"
#define method_initFallback "initFallback"
#define method_factorFallback "factorFallback"
#define method_signFallback "signFallback"

#define error_choicevars "Method " method_choiceVars " is not well-formed: see the documentation for more information"
#define error_initfallback "Method " method_initFallback " is not well-formed: see the documentation for more information"
#define error_factorfallback "Method " method_factorFallback " is not well-formed: see the documentation for more information"

#include "interpreters/Interpreter.h"
#include "HeuristicStrategy.h"
#include "MinisatHeuristic.h"
class ExternalHeuristic : public HeuristicStrategy
{
    public:        
        ExternalHeuristic( Solver& solver, char* filename, unsigned int interpr );
        ~ExternalHeuristic();

        void addedVarName( Var var, const string& name );
        void onConflict();
        void onDeletion();
        void onFinishedParsing();
        void onFinishedSimplifications();
        void onLearningClause( unsigned int lbd, const Clause* clause );
        void onLitAtLevelZero( Literal lit );
        void onLitInvolvedInConflict( Literal lit );
        void onRestart();
        void onAnswerSet();
        void onStartingSolver( unsigned int nVars, unsigned int nClauses );
        void onLitInImportantClause( Literal lit );
        void onVariableElimination( Var var );
        void onStartingParsing();
        void onStartingSimplifications();
        void onUnfoundedSet( const Vector< Var >& unfoundedSet );
        void onLoopFormula( const Clause* clause );
        void onNewClause( const Clause* clause );
        void onNewBinaryClause( Literal lit1, Literal lit2 );
        
        void onLitTrue( Literal lit );
        void onVarUndefined( Var v );
        
        void initFallback();
        void factorFallback();
        void signFallback();

        void onNewVariable( Var v )
        {
            if( minisatHeuristic )
                minisatHeuristic->onNewVariable( v );
        }
        void onNewVariableRuntime( Var v )
        {
            if( minisatHeuristic )
                minisatHeuristic->onNewVariableRuntime( v );
        }
        
    protected:
        virtual Literal makeAChoiceProtected();   
    
    private:
        const static unsigned int CHOICE = 0;
        const static unsigned int DO_RESTART = 1;
        const static unsigned int FALLBACK_HEURISTIC = 2;
        const static unsigned int UNROLL = 3;
        const static unsigned int TRIGGER_INCOHERENCE = 4;
        
        void sendTrueLiterals();

        //mandatory
        void choiceVars( vector< int >& vars, int& status );
        
        void clearStatus();
        void resetPreviousChoices();
        
        //mandatory
        void onChoiceContradictory( int choice );
        
        Interpreter* interpreter;
        MinisatHeuristic* minisatHeuristic;
        
        bool check_addedVarName;        
        bool check_onConflict;
        bool check_onDeletion;
        bool check_onFinishedParsing;
        bool check_onStartingSimplifications;
        bool check_onFinishedSimplifications;
        bool check_onLearningClause;
        bool check_onLitAtLevelZero;
        bool check_onLitInvolvedInConflict;
        bool check_onRestart;
        bool check_onAnswerSet;
        bool check_onStartingSolver;
        bool check_onLitInImportantClause;
        bool check_onVariableElimination;
        bool check_onStartingParsing;
        bool check_onUnfoundedSet;
        bool check_onLoopFormula;
        bool check_initFallback;
        bool check_factorFallback;
        bool check_signFallback;
        bool check_onNewClause;
        bool check_onLitsTrue;
        bool check_onVarUndefined;
        
        vector< int > choices;
        vector< int > interpretationToSend;
        int status;
        int numberOfFallbackSteps;
        Var unrollVariable;
        vector< Literal > previousChoices;
        bool ignorePolarity;
        bool check_partialInterpretation;
};

#endif
