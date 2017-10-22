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

#ifndef WASP_GUSDATA
#define WASP_GUSDATA

#include "Literal.h"
#include <vector>
#include "stl/Vector.h"
using namespace std;

class Clause;

class GUSData
{
    public:
        inline GUSData();
        ~GUSData();

        vector< Literal > externalLiterals;
        vector< Literal > internalLiterals;
        Vector< Var > supportedByThisExternalRule[ 2 ];
        Vector< Var > supportedByThisInternalRule[ 2 ];
        vector< Var > auxVariablesSupportedByThis[ 2 ];
        vector< Var > possiblySupportedByThis[ 2 ];

        vector< Clause* > definingRulesForNonHCFAtom;
        vector< Literal > definingLiteralsForNonHCFAtom;
        Var unfoundedVarForHCC; //For an atom p in a HCC it represents up
        Var headVarForHCC; //For an atom p in a HCC it represents hp

        /* CONJUNCTIVE */
        vector< Literal > literals;
        unsigned int numberOfSupporting;

        /* DISJUNCTIVE */
        Literal sourcePointer;

        inline void setAux() { assert( !aux ); aux = 1; }
        inline bool isAux() const { return aux; }

        inline void setInQueue() { assert( !inQueue ); inQueue = 1; }
        inline void setOutQueue() { assert( inQueue ); inQueue = 0; resetPropagated(); }
        inline bool isInQueue() const { return inQueue; }

        inline void setFounded() { assert( !founded ); founded = 1; }
        inline void setUnfounded() { assert( founded ); founded = 0; }
        inline bool isFounded() const { return founded; }
        
        inline void setInUnfoundedSet() { assert( !inUnfoundedSet ); inUnfoundedSet = 1; }
        inline void removeFromUnfoundedSet() { assert( inUnfoundedSet ); inUnfoundedSet = 0; }
        inline bool isInUnfoundedSet() const { return inUnfoundedSet; }
        
        inline void setPropagated() { assert( !propagated ); propagated = 1; }
        inline void resetPropagated() { propagated = 0; }
        inline bool isPropagated() const { return propagated; }

//        inline Var var() const { return variable; }
//        inline void setVariable( Var v ) { variable = v; }
        
    private:
        unsigned int aux : 1;
        unsigned int founded : 1;
        unsigned int inQueue : 1;
        unsigned int inUnfoundedSet : 1;
        unsigned int propagated : 28;
//        unsigned int variable : 27;
};

GUSData::GUSData() : numberOfSupporting( 0 ), sourcePointer( Literal::null ), aux( 0 ), founded( 1 ), inQueue( 0 ), inUnfoundedSet( 0 ), propagated( 0 )
{
    unfoundedVarForHCC = 0;
    headVarForHCC = 0;
}

#endif