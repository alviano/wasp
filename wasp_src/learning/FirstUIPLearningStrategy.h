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
 * File:   FirstUIPLearningStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 27 July 2013, 10.17
 */

#ifndef FIRSTUIPLEARNINGSTRATEGY_H
#define	FIRSTUIPLEARNINGSTRATEGY_H

#include "LearningStrategy.h"
#include <cassert>
#include <unordered_set>
#include <list>
using namespace std;

class FirstUIPLearningStrategy : public LearningStrategy
{
    public:
        inline FirstUIPLearningStrategy();
        
        virtual void onNavigatingLiteral( Literal* );
        virtual Clause* learnClause( Literal* conflictLiteral, Solver& solver );                
        
    private:
        Literal* getNextToConsider();
        inline void clearDataStructures();
        void addLiteralInLearnedClause( Literal* literal );
        inline void addLiteralToNavigate( Literal* literal );                
        
        unordered_set< Literal* > addedLiterals;
        unsigned int decisionLevel;
        Clause* learnedClause;
        list< Literal* > literalsOfTheSameLevel;        
};

FirstUIPLearningStrategy::FirstUIPLearningStrategy() : LearningStrategy()
{
    learnedClause = NULL;
    decisionLevel = 0;
}
        
void
FirstUIPLearningStrategy::addLiteralToNavigate( 
    Literal* literal )
{
    if( addedLiterals.insert( literal ).second )
        literalsOfTheSameLevel.push_back( literal );
}

void
FirstUIPLearningStrategy::clearDataStructures()
{
    learnedClause = NULL;
    literalsOfTheSameLevel.clear();
    addedLiterals.clear();
}

#endif	/* FIRSTUIPLEARNINGSTRATEGY_H */

