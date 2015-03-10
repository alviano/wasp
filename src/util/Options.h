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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>

#include "Constants.h"
#include "Trace.h"
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
        
        private:

            static DELETION_POLICY deletionPolicy;

            static unsigned int deletionThreshold;
            
            static DECISION_POLICY decisionPolicy;

            static vector< const char* > inputFiles;

            static unsigned int decisionThreshold;
            
            static unsigned int maxModels;

            static OUTPUT_POLICY outputPolicy;

            static bool printProgram;
            static bool printDimacs;

            static RESTARTS_POLICY restartsPolicy;
            
            static unsigned int restartsThreshold;            

            static unsigned int timeLimit;
            
            static bool exchangeClauses;
            
            static WEAK_CONSTRAINTS_ALG weakConstraintsAlg;
            
            static bool disjCoresPreprocessing;
            
            static bool minimizeUnsatCore;
            
            static bool stratification;
    };
}

#endif /* OPTIONS_H */
