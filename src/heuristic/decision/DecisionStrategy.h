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

#ifndef DECISIONSTRATEGY_H
#define DECISIONSTRATEGY_H

#include <cassert>

class Literal;
class Solver;
class Variable;

class DecisionStrategy
{
    public:
        virtual ~DecisionStrategy() {}
        virtual Literal makeAChoice() = 0;
        
        virtual void onLearning() = 0;
        virtual void onRestart() = 0;
        
        virtual void onNewVariable( Variable& variable ) = 0;
        
        virtual void onLiteralInvolvedInConflict( Literal literal ) = 0;
};

#endif	/* DECISIONSTRATEGY_H */

