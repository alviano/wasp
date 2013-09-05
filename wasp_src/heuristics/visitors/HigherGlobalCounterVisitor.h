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
 * File:   HigherGlobalCounterVisitor.h
 * Author: Carmine Dodaro
 *
 * Created on 03 September 2013, 17.04
 */

#ifndef HIGHERGLOBALCOUNTERVISITOR_H
#define	HIGHERGLOBALCOUNTERVISITOR_H

#include "BerkminTotalCounterVisitor.h"

class Literal;
class Solver;

class HigherGlobalCounterVisitor : public BerkminTotalCounterVisitor
{
    public:
        inline HigherGlobalCounterVisitor( Solver* solver );
        
    protected:
        /**
         * This function performs a lookahead step in order to choose the
         * polarity of a literal.
         * 
         * @param literal A literal L.
         * @param oppositeLiteral A literal with the opposite polarity of L.
         */
        virtual void choosePolarity( Literal* literal, Literal* oppositeLiteral );
        
        /**
         * An internal function which estimates the effects of propagating a
         * literal.
         * 
         * @param literal The literal to propagate.
         * @return an unsigned integer which measures how many literals are
         * inferred and the number of clauses in which the literal is watched.
         * 
         */
        virtual unsigned int estimatePropagation( Literal* literal );
        
    private:
        Solver* solver;
};

HigherGlobalCounterVisitor::HigherGlobalCounterVisitor( 
    Solver* s ) : BerkminTotalCounterVisitor(), solver( s )
{
}

#endif	/* HIGHERGLOBALCOUNTERVISITOR_H */

