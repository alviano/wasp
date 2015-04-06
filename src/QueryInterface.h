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

#ifndef QUERYINTERFACE_H
#define	QUERYINTERFACE_H

#include "stl/Vector.h"

class Clause;
class Solver;

class QueryInterface
{
    public:
        QueryInterface( Solver& solver );
        void computeCautiousConsequences( unsigned int ALGORITHM );

    private:
        QueryInterface( const QueryInterface& orig );
        
        bool computeFirstModel();
        
        void enumerationOfModels();
        void iterativeCoherenceTesting();
        void overestimateReduction();
        void computeCandidates();
        void reduceCandidates();
        
        Clause* computeClauseFromCandidates();
        
        void addAnswer( Var v );
        
        void printCandidates();
        
        Vector< Var > candidates;
        Vector< Var > answers;
        
        Solver& solver;
};

#endif
