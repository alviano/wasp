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
 * File:   SequenceBasedRestartStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 6 August 2013, 21.15
 */

#ifndef SEQUENCEBASEDRESTARTSSTRATEGY_H
#define	SEQUENCEBASEDRESTARTSSTRATEGY_H

#include <cassert>

#include "RestartStrategy.h"

class SequenceBasedRestartStrategy : public RestartStrategy
{
    public:
        inline SequenceBasedRestartStrategy( unsigned int threshold = 32 );
        virtual bool onLearningClause();
        virtual void onLearningUnaryClause();
        
    protected:
        virtual void computeNextRestartValue();
        unsigned int computeRestartNumber( unsigned int i );

        unsigned int nextRestartValue;
        unsigned int conflictsCount;        
        unsigned int threshold;
        unsigned int numberOfRestarts;
};

SequenceBasedRestartStrategy::SequenceBasedRestartStrategy( 
    unsigned int t ) : conflictsCount( 0 ), threshold( t ), numberOfRestarts( 1 )
{
    assert( threshold >= 32 );
    nextRestartValue = computeRestartNumber( numberOfRestarts ) * threshold;
}

#endif	/* SEQUENCEBASEDRESTARTSSTRATEGY_H */

