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
 * File:   GeometricRestartsStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 6 August 2013, 21.13
 */

#ifndef GEOMETRICRESTARTSSTRATEGY_H
#define	GEOMETRICRESTARTSSTRATEGY_H

#include "RestartsStrategy.h"

class GeometricRestartsStrategy : public RestartsStrategy
{
    public:
        inline GeometricRestartsStrategy( unsigned int threshold );
        virtual bool onLearningClause();
        virtual void onLearningUnaryClause();
        
    protected:
        virtual void computeNextRestartValue();
};

GeometricRestartsStrategy::GeometricRestartsStrategy(
    unsigned int threshold ) : RestartsStrategy( threshold )
{
    nextRestartValue = threshold;   
}


#endif	/* GEOMETRICRESTARTSSTRATEGY_H */

