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

#ifndef _GUSDATA
#define _GUSDATA

#include "Literal.h"

class GUSData
{
    public:
        inline GUSData();
        inline ~GUSData();
        
        vector< Literal > externalLiterals;
        vector< Literal > internalLiterals;
        WatchedList< Variable* > supportedByThisExternalRule[ 2 ];
        WatchedList< Variable* > supportedByThisInternalRule[ 2 ];
        vector< Variable* > auxVariablesSupportedByThis[ 2 ];
        vector< Variable* > possiblySupportedByThis[ 2 ];
        
        bool aux;
        bool founded;
        bool inQueue;
        
        Variable* variable;
        
        /* CONJUNCTIVE */
        vector< Literal > literals;
        unsigned int numberOfSupporting;
        
        /* DISJUNCTIVE */
        Literal sourcePointer;
};

GUSData::GUSData() : aux( false ), founded( true ), inQueue( false ), variable( NULL ), numberOfSupporting( 0 ), sourcePointer( Literal::null )
{
}

GUSData::~GUSData()
{
}

#endif