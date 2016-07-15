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

#ifndef WASPFACADE_H
#define WASPFACADE_H

#include <cassert>
#include <vector>
#include <unordered_map>
using namespace std;

#include "util/Constants.h"
#include "Solver.h"
#include "input/Dimacs.h"
#include "weakconstraints/WeakInterface.h"
#include "weakconstraints/Mgd.h"
#include "weakconstraints/One.h"
#include "weakconstraints/Opt.h"
#include "weakconstraints/PMRes.h"
#include "weakconstraints/OneBB.h"
#include "weakconstraints/K.h"
#include "heuristic/ExternalHeuristic.h"

class WaspFacade
{
    public:
        inline WaspFacade();
        inline ~WaspFacade(){ delete outputBuilder; }
        
        void readInput();
        void solve();
        inline void onFinish() { solver.onFinish(); }
        inline void onKill() { solver.onKill(); }
        
        inline void greetings(){ solver.greetings(); }
        
        void setMinisatPolicy();
        void setOutputPolicy( OUTPUT_POLICY );
        void setRestartsPolicy( RESTARTS_POLICY, unsigned int threshold );

        inline void setMaxModels( unsigned int max ) { maxModels = max; }
        inline void setPrintProgram( bool printProgram ) { this->printProgram = printProgram; }
        inline void setPrintDimacs( bool printDimacs ) { this->printDimacs = printDimacs; }
        void setExchangeClauses( bool exchangeClauses ) { solver.setExchangeClauses( exchangeClauses ); }                
        
        inline void setWeakConstraintsAlgorithm( WEAK_CONSTRAINTS_ALG alg ) { weakConstraintsAlg = alg; }
        inline void setDisjCoresPreprocessing( bool value ) { disjCoresPreprocessing = value; }
        inline void setMinimizeUnsatCore( bool value ) { solver.setMinimizeUnsatCore( value ); }        
        
        inline void setQueryAlgorithm( unsigned int value ) { queryAlgorithm = value; }
        
        inline unsigned int solveWithWeakConstraints();               

    private:
        Solver solver;
        
        unsigned int numberOfModels;
        unsigned int maxModels;
        bool printProgram;
        bool printDimacs;        

        WEAK_CONSTRAINTS_ALG weakConstraintsAlg;
        bool disjCoresPreprocessing;        
        
        unsigned int queryAlgorithm;
        OutputBuilder* outputBuilder;
        
        void enumerateModels();
        void enumerationBlockingClause();
        void enumerationBacktracking();
        void flipLatestChoice( vector< Literal >& choices, vector< bool >& checked );
        bool foundModel( vector< Literal >& assums );
        unsigned int getMaxLevelUnsatCore( const Clause* unsatCore );
};

WaspFacade::WaspFacade() : numberOfModels( 0 ), maxModels( 1 ), printProgram( false ), printDimacs( false ), weakConstraintsAlg( OPT ), disjCoresPreprocessing( false ), outputBuilder( NULL )
{   
    if( wasp::Options::interpreter != NO_INTERPRETER && wasp::Options::heuristic_scriptname != NULL )
        solver.setChoiceHeuristic( new ExternalHeuristic( solver, wasp::Options::heuristic_scriptname, wasp::Options::interpreter, wasp::Options::scriptDirectory ) );
}

unsigned int
WaspFacade::solveWithWeakConstraints()
{
    WeakInterface* w = NULL;    
    switch( weakConstraintsAlg )
    {
        case MGD:
            w = new Mgd( solver );
            break;

        case OPT:
            w = new Opt( solver );
            break;

        case BB:
            w = new Opt( solver, true );
            break;

        case PMRES:            
            w = new PMRes( solver );
            break;

        case ONEBB:
            w = new OneBB( solver );
            break;

        case ONEBBREST:
            w = new OneBB( solver, true );
            break;
            
        case BBBT:
            w = new Opt( solver, true );            
            break;
            
        case KALG:
            w = new K( solver );
            break;
            
        case ONE:
        default:            
            w = new One( solver );
            break;
    }
    
//    if( weakConstraintsAlg != OLLBB && weakConstraintsAlg != OLLBBREST )
//        solver.simplifyOptimizationLiteralsAndUpdateLowerBound( w );
    w->setDisjCoresPreprocessing( disjCoresPreprocessing );
    unsigned int res = w->solve();    
    delete w;
    return res;
}

#endif
