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

#ifndef USECLAUSEDATA_H
#define USECLAUSEDATA_H
 
#include "../Clause.h"

template< class H >
class UseClauseData
{
public:
    inline typename H::ClauseData* getHeuristicData( Clause& clause ) { return static_cast< typename H::ClauseData* >( clause.getHeuristicData() ); }
    inline const typename H::ClauseData* getHeuristicData( const Clause& clause ) { return static_cast< const typename H::ClauseData* >( clause.getHeuristicData() ); }
};

#endif