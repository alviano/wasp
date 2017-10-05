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
#include "../AnswerSetListener.h"
class Solver;

class OptimizationProblemUtils : public AnswerSetListener
{
    public:        
        inline OptimizationProblemUtils( Solver& s ) : solver( s ), lb_( 0 ), ub_( UINT64_MAX ), level_( 0 ) {}        
        inline void incrementLb( uint64_t value ) { lb_ += value; }
        inline uint64_t lb() { return lb_; }
        inline uint64_t ub() { return ub_; }
        inline unsigned int level() { return level_; }
        inline void setLowerBound( uint64_t value ) { lb_ = value; }
        inline void setUpperBound( uint64_t value ) { ub_ = value; }
        inline void setLevel( unsigned int value ) { level_ = value; }
        inline void initLevel( unsigned int value ) { level_ = value; lb_ = 0; ub_ = UINT64_MAX; }
        
        virtual void foundAnswerSet();

    private:
        Solver& solver;
        uint64_t lb_;
        uint64_t ub_;
        unsigned int level_;        
};

#endif