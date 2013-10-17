/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   DecisionHeuristic.h
 * Author: Carmine Dodaro
 *
 * Created on 02 September 2013, 12.42
 */

#ifndef DECISIONHEURISTIC_H
#define	DECISIONHEURISTIC_H

#include <cassert>
#include "../Literal.h"
class Solver;

class DecisionHeuristic
{
    public:
        inline DecisionHeuristic();
        virtual ~DecisionHeuristic() {}
        inline Literal makeAChoice( Solver& solver );
        virtual void onLearning( Solver& solver ) = 0;
        virtual void onRestarting( Solver& solver ) = 0;

    protected:
        virtual Literal makeAChoiceProtected( Solver& solver ) = 0;
        inline Literal getChosenLiteral();
        inline Variable* getChosenVariable();

        inline bool hasChosenLiteral() const;
        inline void setChosenVariable( Variable* variable );
        inline void setChosenPolarity( bool polarity );        

    private:
        inline void reset();
        Variable* chosenVariable;
        bool chosenPolarity;
};

DecisionHeuristic::DecisionHeuristic() : chosenVariable( NULL )
{
}

Literal
DecisionHeuristic::makeAChoice(
    Solver& solver )
{
    reset();
    return makeAChoiceProtected( solver );
}

Variable*
DecisionHeuristic::getChosenVariable()
{
    return chosenVariable;
}

Literal
DecisionHeuristic::getChosenLiteral()
{
    assert( chosenVariable != NULL );
    return chosenPolarity ? Literal( chosenVariable ) : Literal( chosenVariable, false );
}

bool
DecisionHeuristic::hasChosenLiteral() const
{
    return chosenVariable != NULL;
}

void
DecisionHeuristic::setChosenVariable(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    chosenVariable = variable;
}

void
DecisionHeuristic::setChosenPolarity(
    bool polarity )
{
    chosenPolarity = polarity;
}

void
DecisionHeuristic::reset()
{
    chosenVariable = NULL;
}

#endif	/* DECISIONHEURISTIC_H */

