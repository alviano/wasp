/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   Options.h
 * Author: Mario Alviano
 *
 * Created on 15 October 2013, 08.52
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
        
        private:

            static DELETION_POLICY deletionPolicy;

            static HEURISTIC_POLICY heuristicPolicy;

            static vector< const char* > inputFiles;

            static unsigned int maxBerkminNumber;
            
            static unsigned int maxModels;

            static OUTPUT_POLICY outputPolicy;

            static bool printProgram;

            static RESTARTS_POLICY restartsPolicy;
            
            static unsigned int restartsThreshold;            

            static unsigned int timeLimit;
    };
};

#endif /* OPTIONS_H */
