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
 * File:   RestartsStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 6 August 2013, 21.10
 */

#ifndef RESTARTSSTRATEGY_H
#define	RESTARTSSTRATEGY_H

class RestartsStrategy
{
    public:
        inline RestartsStrategy();
        inline virtual ~RestartsStrategy() {};
        inline RestartsStrategy( unsigned int threshold );
        virtual bool onLearningClause() = 0;
        virtual void onLearningUnaryClause() = 0;
        
    protected:
        virtual void computeNextRestartValue() = 0;
        unsigned int nextRestartValue;
        unsigned int conflictsCount;        
        unsigned int threshold;
};

RestartsStrategy::RestartsStrategy(
    unsigned int t ) : conflictsCount( 0 ), threshold( t )
{
}

RestartsStrategy::RestartsStrategy() : conflictsCount( 0 ), threshold( 0 )
{
}

#endif	/* RESTARTSSTRATEGY_H */

