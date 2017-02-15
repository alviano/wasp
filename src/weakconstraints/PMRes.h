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

#ifndef WASP_PMRES_H
#define	WASP_PMRES_H

#include "WeakInterface.h"

class PMRes : public WeakInterface
{
    public:
        inline PMRes( Solver& solver ) : WeakInterface( solver ) {}
        unsigned int run();        

    protected:
        bool foundUnsat();
        
    private:
        unsigned int runWeighted();
        unsigned int runUnweighted();
                
        bool addAuxClauses( vector< Literal >& optLiterals );
        bool addAuxClausesCompressed( vector< Literal > &optLiterals, uint64_t minWeight );
        bool addClauseToSolver( Clause* clause );
//        bool addClauseToSolverAndCheckDuplicatesAndTautological( Clause* clause );
        Var relaxClause( Clause* clause );        
};

#endif

