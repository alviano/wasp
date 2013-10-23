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

/* 
 * File:   GeometricRestartStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 6 August 2013, 21.13
 */

#ifndef GEOMETRICRESTARTSSTRATEGY_H
#define	GEOMETRICRESTARTSSTRATEGY_H

#include <cassert>
using namespace std;

#include "RestartStrategy.h"

class GeometricRestartStrategy : public RestartStrategy
{
    public:
        inline GeometricRestartStrategy( unsigned int nextRestartValue = 100 );
        virtual bool hasToRestart();
        virtual void onRestart();
        
    private:
        unsigned int conflictsCount;        
        unsigned int nextRestartValue;        
};

GeometricRestartStrategy::GeometricRestartStrategy(
    unsigned int n ) : conflictsCount( 0 ), nextRestartValue( n )
{
    assert( n > 100 );
}


#endif	/* GEOMETRICRESTARTSSTRATEGY_H */

