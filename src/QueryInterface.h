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

#ifndef WASP_QUERYINTERFACE_H
#define	WASP_QUERYINTERFACE_H

#include "stl/Vector.h"
#include "Literal.h"
#include <vector>
using namespace std;

class Clause;
class Solver;

class QueryInterface
{
    public:
        QueryInterface( Solver& solver );
        void solve();

    private:
        QueryInterface( const QueryInterface& orig );
        
        bool computeFirstModel();
        
        void iterativeCoherenceTesting();
        void overestimateReduction();
        void chunkDynamicAlgorithm( unsigned int chunkSize );
        void chunkStaticAlgorithm( unsigned int chunkSize );
        void coreBasedAlgorithm( unsigned int chunkSize );
        void preferenceAlgorithm();
        
        void reset( vector< Literal >& assumptions );
        void processCore( unsigned int numberOfCalls, vector< unsigned int >& inUnsatCore );
        
        void computeCandidates();
        void reduceCandidates();
        void reduceCandidatesForChunk( unsigned int chunkSize );
        
        Clause* computeClauseFromCandidates();
        Clause* computeClauseForChunk( unsigned int chunkSize, Var& auxVar );        
        
        void addAnswer( Var v );
        
        void printCandidates();
        
        
        void computeCandidatesForChunk( Vector< Var >& candidatesForChunk, unsigned int chunkSize );
        Clause* computeClauseFromChunkCandidates( Vector< Var >& candidatesForChunk, Var& auxVar );

        Vector< Var > candidates;
        Vector< Var > answers;
        
        Solver& solver;        
};

#endif
