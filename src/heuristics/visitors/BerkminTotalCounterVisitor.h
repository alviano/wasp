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
 * File:   BerkminTotalCounterVisitor.h
 * Author: Carmine Dodaro
 *
 * Created on 03 September 2013, 16.00
 */

#ifndef BERKMINTOTALCOUNTERVISITOR_H
#define	BERKMINTOTALCOUNTERVISITOR_H

#include "HeuristicVisitor.h"

class HeuristicCounterForLiteral;

class BerkminTotalCounterVisitor : public HeuristicVisitor
{
    public:
        inline BerkminTotalCounterVisitor();
        void onNavigatingVariable( Variable* );
        
    protected:
        virtual void choosePolarity( Variable* ) = 0;
        BERKMIN_HEURISTIC_COUNTER getLiteralCounter( const HeuristicCounterForLiteral* heuristicCounter ) const;
        BERKMIN_HEURISTIC_COUNTER getTotalCounter( const Variable* ) const;
        BERKMIN_HEURISTIC_COUNTER maxCounter;
};

BerkminTotalCounterVisitor::BerkminTotalCounterVisitor() : HeuristicVisitor(), maxCounter( 0 )
{
}
#endif	/* HIGHERGLOBALCOUNTERVISITOR_H */

