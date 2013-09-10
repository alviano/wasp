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
 * File:   AuxLiteral.h
 * Author: Carmine Dodaro
 *
 * Created on 09 September 2013, 19.46
 */

#ifndef AUXLITERAL_H
#define	AUXLITERAL_H

#include "PositiveLiteral.h"

/**
 * This class represents a literal mapping a body.
 * 
 * For instance consider the rule a :- b, c.
 * An auxLiteral representing the body "b, c" is inserted in the solver. 
 * 
 */
class AuxLiteral : public PositiveLiteral
{
    public:
        inline AuxLiteral();
        virtual void supportPropagation( Solver& solver );
        virtual void onLearning( LearningStrategy* strategy );
        inline void setWaspRule( WaspRule* waspRule );
        
    private:
        WaspRule* waspRule;
};

AuxLiteral::AuxLiteral() : PositiveLiteral()
{
}

void
AuxLiteral::setWaspRule(
    WaspRule* waspR )
{
    assert( waspR != NULL );
    waspRule = waspR;
}

#endif	/* POSITIVELITERAL_H */

