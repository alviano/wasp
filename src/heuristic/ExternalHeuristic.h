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

#ifndef WASP_EXTERNALHEURISTIC_H
#define WASP_EXTERNALHEURISTIC_H

//Learning
#define method_onConflict "onConflict"
#define method_onLitInvolvedInConflict "onLitInvolvedInConflict"
#define method_onLearningClause "onLearningClause"
#define method_onLitInImportantClause "onLitInImportantClause"

//Solving
#define method_onNewClause "onNewClause"
#define method_onDeletion "onDeletion"
#define method_onRestart "onRestart"

//Choice
#define method_selectLiteral "selectLiteral"

//ASP
#define method_onUnfoundedSet "onUnfoundedSet"
#define method_onLoopFormula "onLoopFormula"

//Fallback
#define method_initFallback "initFallback"
#define method_factorFallback "factorFallback"
#define method_signFallback "signFallback"

#define error_selectLiteral "Method " method_selectLiteral " is not well-formed: see the documentation for more information"
#define error_initfallback "Method " method_initFallback " is not well-formed: see the documentation for more information"
#define error_factorfallback "Method " method_factorFallback " is not well-formed: see the documentation for more information"

#include "../interpreters/Interpreter.h"
#include "MinisatHeuristic.h"
class ExternalHeuristic : public MinisatHeuristic
{
    public:        
        ExternalHeuristic( Solver& solver, char* filename, unsigned int interpr, string scriptDirectory );
        virtual ~ExternalHeuristic();

        void onConflictProtected();
        void onDeletion();
        void onLearningClause( unsigned int lbd, const Clause* clause );
        void onLitInImportantClauseProtected( Literal lit );
        void onLitInvolvedInConflictProtected( Literal lit );
        void onLoopFormula( const Clause* clause );
        void onNewBinaryClauseProtected( Literal lit1, Literal lit2 );
        void onNewClause( const Clause* clause );
        void onRestart();        
        void onUnfoundedSet( const Vector< Var >& unfoundedSet );                
        
    protected:
        virtual Literal makeAChoiceProtected();   
        virtual void onFinishedSimplificationsProtected() { initFallback(); factorFallback(); signFallback(); }
    
    private:
        const static unsigned int CHOICE = 0;
        const static unsigned int DO_RESTART = 1;
        const static unsigned int FALLBACK_HEURISTIC = 2;
        const static unsigned int UNROLL = 3;
        
        void initFallback();
        void factorFallback();
        void signFallback();
        
        void selectLiteral();
        
        Interpreter* interpreter;
        
        bool check_onConflict;
        bool check_onDeletion;
        bool check_onLearningClause;
        bool check_onLitInImportantClause;
        bool check_onLitInvolvedInConflict;
        bool check_onLoopFormula;
        bool check_onNewClause;
        bool check_onRestart;
        bool check_onUnfoundedSet;
        
        bool check_initFallback;
        bool check_factorFallback;
        bool check_signFallback;                
        
        int status;
        int value;
};

#endif
