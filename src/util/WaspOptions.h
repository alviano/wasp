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

#ifndef WASP_OPTIONS_H
#define WASP_OPTIONS_H

#include <vector>
#include <map>
#include "WaspConstants.h"
#include "WaspTrace.h"
using namespace std;

class WaspFacade;

namespace wasp
{
    /**
     * This class contains a field for each option of wasp.
     *
     */
    class Options
    {
        public:
            #ifdef TRACE_ON
            static TraceLevels traceLevels;
            #endif

            static void parse( int argc, char* const* argv );
            static void setOptions( WaspFacade& waspFacade );
            static unsigned int maxCost;        
            static bool forwardPartialChecks;
            static bool heuristicPartialChecks;
            
            static unsigned int queryAlgorithm;
            static unsigned int queryVerbosity;
            static bool computeFirstModel;
            static unsigned int budget;
            static bool printLastModelOnly;
            static bool printBounds;
            static bool printAtomTable;
            static bool stratification;
            
            static unsigned int interpreter;
            static char* heuristic_scriptname;
            static vector< string > pluginsFilenames;
            static SHIFT_STRATEGY shiftStrategy;
            static string scriptDirectory;
            
            static bool oneDefShift;
            
            static bool simplifications;            
            
            static unsigned int minimizationStrategy;
            static unsigned int minimizationBudget;
            
            static unsigned int enumerationStrategy;            
            static WEAK_CONSTRAINTS_ALG weakConstraintsAlg;
            
            static unsigned int kthreshold;
            
            static unsigned int silent;
            static bool printOnlyOptimum;
            
            static bool useLazyWeakConstraints;
            
            static unsigned int chunkPercentage;
            static unsigned int chunkSize;
            
            static unsigned int minimizePredicateChunkPercentage;
            
            static unsigned int modelcheckerAlgorithm;
            
            static bool compactReasonsForHCC;
            
            static DELETION_POLICY deletionPolicy;
            static unsigned int deletionThreshold;
            
            static vector< const char* > inputFiles;            

            static unsigned int maxModels;

            static OUTPUT_POLICY outputPolicy;

            
            static bool minisatPolicy;

            static RESTARTS_POLICY restartsPolicy;
            
            static unsigned int restartsThreshold;            

            static unsigned int timeLimit;                        
            
            static bool disjCoresPreprocessing;
            
            static bool minimizeUnsatCore;     
            
            static bool callPyFinalize;
            
            static double sizeLBDQueue;
            static double sizeTrailQueue;
            static double K;
            static double R;
            
            static int nbclausesBeforeReduce;
            static int incReduceDB;
            static int specialIncReduceDB;
            static unsigned int lbLBDFrozenClause;
            
            static unsigned int lbLBDMinimizingClause;                                    
            
            static bool stats;
            
            static unsigned int statsVerbosity;
            
            static double initVariableIncrement;
            static double initVariableDecay;
            static unsigned int initValue;
            static unsigned int initMinisatHeuristic;
            static unsigned int initSign;
            
            static bool multiAggregates;
            static bool queryCoreCache;
            
            static unsigned int predMinimizationAlgorithm;
            static vector< string > predicatesToMinimize;
            
            static unsigned int multiThreshold;
            
            static map< string, WEAK_CONSTRAINTS_ALG > stringToWeak;
            
            static map< string, SHIFT_STRATEGY > stringToShift;
            
            static map< string, unsigned int > stringToMinimization;
            
            static map< string, unsigned int > stringToQueryAlgorithms;
            
            static map< string, unsigned int > stringToInitMinisatHeuristic;
            
            static map< string, unsigned int > stringToPredMinimization;
            
            static WEAK_CONSTRAINTS_ALG getAlgorithm( const string& s );
            
            static SHIFT_STRATEGY getShiftStrategy( const string& s );
            
            static unsigned int getMinimizationStrategy( const string& s );
            
            static unsigned int getEnumerationStrategy( const string& s );
            
            static unsigned int getQueryAlgorithm( const string& s );
            
            static unsigned int getInitMinisatHeuristic( const string& s );
            
            static void initMap();
            
            static void checkOptions();
    };
}

#endif
