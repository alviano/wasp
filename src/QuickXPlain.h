/*
 *
 *  Copyright 2015 Mario Alviano, Carmine Dodaro, Francesco Ricca, and Philip
 *  Gasteiger.
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

#ifndef QUICKXPLAIN_H
#define QUICKXPLAIN_H

#include <vector>

#include "Literal.h"

using namespace std;

class WaspFacade;

/**
 * QuickXPlain (QXP) algorithm to determine preferred conflicts
 * if INCOHERENT. For details on the algorithm see
 * Junker 2004; QuickXPlain - Preferred Explanations and Relaxations over constraint-based problems
 */
class QuickXPlain
{
    public:
        inline QuickXPlain( WaspFacade& waspFacade_ ) : waspFacade( waspFacade_ ) { }
        void minimizeUnsatCore(const vector< Literal >& unsatCore, vector< Literal >& conflict, unsigned int level = 1);

    private:
        void minimizeUnsatCore( unsigned int level, const vector< Literal >& toCheck, const vector< Literal >& addedToCheck, const vector< Literal >& toSplit, vector<Literal>& result);

        void partitionVector( const vector< Literal >& toSplit, vector< Literal >& v1, vector< Literal >& v2 );
        void appendToVector( const vector< Literal >& v, const vector< Literal >& toAppend, vector<Literal>& result );
        vector<Literal> conflict_;
        WaspFacade& waspFacade;
};

#endif
