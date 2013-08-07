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
 * File:   LearningStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 27 July 2013, 9.51
 */

#ifndef LEARNINGSTRATEGY_H
#define	LEARNINGSTRATEGY_H

#include "RestartsStrategy.h"

#include <cassert>

class Clause;
class Literal;
class Solver;

class LearningStrategy
{
    public:
        inline LearningStrategy( RestartsStrategy* restartsStrategy );
        virtual ~LearningStrategy()
        {
            if( restartsStrategy )
                delete restartsStrategy;
        }
        
        virtual void onNavigatingLiteral( Literal* ) = 0;
        virtual void onConflict( Literal* conflictLiteral, Solver& solver ) = 0;        
        
    protected:
        
        /**
         * The decision level of the conflict.
         */
        unsigned int decisionLevel;
        
        /**
         * The new learned clause.
         */
        Clause* learnedClause;
        
        /**
         * The strategy used for restarts.
         */
        RestartsStrategy* restartsStrategy;             
};

LearningStrategy::LearningStrategy(
    RestartsStrategy* strategy ): decisionLevel( 0 ), learnedClause( NULL )
{
    assert( "The strategy for restarts must be initialized." && strategy != NULL );
    restartsStrategy = strategy;
}

#endif	/* LEARNINGSTRATEGY_H */

