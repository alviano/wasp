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

#ifndef HEURISTICSTRATEGY_H
#define	HEURISTICSTRATEGY_H

#include <cassert>
#include <vector>
#include <cstring>
using namespace std;

#include "../util/Constants.h"
#include "../Literal.h"
#include "../stl/Vector.h"
class Solver;
class Clause;

class HeuristicStrategy
{
    public:
        inline HeuristicStrategy( Solver& s ) : solver( s ) {}
        virtual ~HeuristicStrategy() {}
        
        Literal makeAChoice();
        
        virtual void addedVarName( Var var, const string& name ) = 0;
        virtual void onNewVariable( Var v ) = 0;
        virtual void onNewVariableRuntime( Var v ) = 0;        
        
        //Preprocessing
        virtual void onStartingParsing() = 0;
        virtual void onFinishedParsing() = 0;
        virtual void onStartingSimplifications() = 0;
        virtual void onFinishedSimplifications() = 0;
        virtual void onVariableElimination( Var var ) = 0;

        //Learning
        virtual void onConflict() = 0;
        virtual void onLitInvolvedInConflict( Literal lit ) = 0;
        virtual void onLearningClause( unsigned int lbd, const Clause* clause ) = 0;
        virtual void onLitAtLevelZero( Literal lit ) = 0;
        virtual void onLitInImportantClause( Literal lit ) = 0;

        //Solving
        virtual void onNewClause( const Clause* ) = 0;
        virtual void onNewBinaryClause( Literal lit1, Literal lit2 ) = 0;
        virtual void onStartingSolver( unsigned int nVars, unsigned int nClauses ) = 0;
        virtual void onDeletion() = 0;
        virtual void onRestart() = 0;
        virtual void onAnswerSet() = 0;
        virtual void onLitTrue( Literal lit ) = 0;
        virtual void onVarUndefined( Var v ) = 0;
        
        //ASP
        virtual void onUnfoundedSet( const Vector< Var >& ) = 0;
        virtual void onLoopFormula( const Clause* ) = 0;

        inline void addPreferredChoice( Literal lit ){ assert( lit != Literal::null ); preferredChoices.push_back( lit ); }
        inline void removePrefChoices() { preferredChoices.clear(); }
        
    protected:
        virtual Literal makeAChoiceProtected() = 0;
        Solver& solver;
        
    private:
        vector< Literal > preferredChoices;

};

#endif

