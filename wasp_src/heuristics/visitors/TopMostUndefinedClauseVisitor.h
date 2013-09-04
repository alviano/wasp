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
 * File:   TopMostUndefinedClauseVisitor.h
 * Author: Carmine Dodaro
 *
 * Created on 03 September 2013, 17.08
 */

#ifndef TOPMOSTUNDEFINEDCLAUSEVISITOR_H
#define	TOPMOSTUNDEFINEDCLAUSEVISITOR_H

#include "BerkminTotalCounterVisitor.h"

class TopMostUndefinedClauseVisitor : public BerkminTotalCounterVisitor
{
    public:
        inline TopMostUndefinedClauseVisitor();
        void choosePolarity( Literal* literal, Literal* oppositeLiteral );
};

TopMostUndefinedClauseVisitor::TopMostUndefinedClauseVisitor() : BerkminTotalCounterVisitor()
{
}

#endif	/* TOPMOSTUNDEFINEDCLAUSEVISITOR_H */

