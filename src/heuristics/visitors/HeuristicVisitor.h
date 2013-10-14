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
 * File:   HeuristicVisitor.h
 * Author: Carmine Dodaro
 *
 * Created on 03 September 2013, 16.50
 */

#ifndef HEURISTICVISITOR_H
#define	HEURISTICVISITOR_H

class Variable;
class Literal;

#include <cassert>
#include <cstdlib>
#include "../../Constants.h"

class HeuristicVisitor
{
    public:
        inline HeuristicVisitor();
        virtual void onNavigatingVariable( Variable* ) = 0;
        inline bool hasChosenLiteral() const;
        Literal getChosenLiteral();
        
    protected:
        inline void setChosenLiteral( Variable* variable, bool polarity );        
        
    private:
        Variable* chosenVariable;
        bool chosenPolarity;
};

HeuristicVisitor::HeuristicVisitor() : chosenVariable( NULL ), chosenPolarity( false )
{
}

void
HeuristicVisitor::setChosenLiteral(
    Variable* variable,
    bool polarity )
{
    assert( "Variable must be set." && variable != NULL );
    chosenVariable = variable;
    chosenPolarity = polarity;
}

bool
HeuristicVisitor::hasChosenLiteral() const
{
    return chosenVariable != NULL;
}

#endif	/* HEURISTICVISITOR_H */

