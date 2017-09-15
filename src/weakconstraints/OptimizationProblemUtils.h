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

#ifndef OPTIMIZATIONPROBLEMUTILS_H
#define	OPTIMIZATIONPROBLEMUTILS_H

#include <climits>
#include <cstdint>
class Solver;

class OptimizationProblemUtils
{
    public:
        inline static void incrementLb( uint64_t value ) { lb_ += value; }
        inline static uint64_t lb() { return lb_; }
        inline static uint64_t ub() { return ub_; }
        inline static unsigned int level() { return level_; }
        inline static void setLowerBound( uint64_t value ) { lb_ = value; }
        inline static void setUpperBound( uint64_t value ) { ub_ = value; }
        inline static void setLevel( unsigned int value ) { level_ = value; }
        inline static void initLevel( unsigned int value ) { level_ = value; lb_ = 0; ub_ = UINT64_MAX; }
        static uint64_t foundAnswerSet( Solver& solver );

    private:
        static uint64_t lb_;
        static uint64_t ub_;
        static unsigned int level_;
};

#endif