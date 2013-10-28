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

#ifndef ACTIVITYBASEDHEURISTIC_H
#define ACTIVITYBASEDHEURISTIC_H

#include "ThreeStrategiesHeuristic.h"

class ActivityBasedHeuristic : public ThreeStrategiesHeuristic
{
public:
    struct ClauseData : public Heuristic::ClauseData
    {
        inline ClauseData() { activity = 0.0; }
        inline ClauseData( const ClauseData& init ) : activity( init.activity ) {}
        double activity;
        
    private:
        ClauseData& operator=( const ClauseData& right ) { assert( 0 ); activity = right.activity; return *this; }
    };

    inline ActivityBasedHeuristic() {}
    virtual ~ActivityBasedHeuristic() {}

    virtual void initClauseData( Clause* clause ) { clause->setHeuristicData( new ClauseData() ); }
};

class ActivityBased
{
public:
    inline ActivityBasedHeuristic::ClauseData* getHeuristicData( Clause& clause ) { return static_cast< ActivityBasedHeuristic::ClauseData* >( clause.getHeuristicData() ); }
    inline const ActivityBasedHeuristic::ClauseData* getHeuristicData( const Clause& clause ) { return static_cast< const ActivityBasedHeuristic::ClauseData* >( clause.getHeuristicData() ); }
};


#endif
