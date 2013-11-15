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

#ifndef HELP_H
#define HELP_H

#include <iostream>
using namespace std;
#include "Constants.h"

class Help
{
    public:
        static void printHelp()
        {
            string separator = "\n---------------------------\n";

            cout << WASP_STRING << endl;

            cout << separator << endl;

            #ifdef TRACE_ON
            cout << "Trace options                          " << endl << endl;
            cout << "--trace-parser=<verbosity>             - Trace parser." << endl;
            cout << "--trace-solving=<verbosity>            - Trace solver." << endl;
            cout << "--trace-learning=<verbosity>           - Trace learning." << endl;
            cout << "--trace-heuristic=<verbosity>          - Trace heuristic." << endl;
            cout << "--trace-enumeration=<verbosity>        - Trace enumeration." << endl;
            cout << "--trace-mc=<verbosity>                 - Trace model checker." << endl;
            cout << "--trace-us=<verbosity>                 - Trace unfounded set function." << endl;
            cout << separator << endl;
            #endif

            cout << "Restart options                       " << endl << endl;
            cout << "--sequence-based-restarts=<N>          - Luby et al. sequence using N as number of conflicts." << endl;
            cout << "--geometric-restarts=<N>               - Geometric restarts." << endl;
            cout << "--minisat-restarts=<N>                 - Minisat restarts." << endl;
            cout << "--disable-restarts                     - Disable restarts." << endl;
            cout << separator << endl;
            
            cout << "Deletion options                       " << endl << endl;            
            cout << "--aggressive-deletion                  - Aggressive variant of minisat deletion policy." << endl;
            cout << "--restarts-based-deletion              - Restarts-based deletion policy." << endl;
            cout << "--minisat-deletion                     - Minisat deletion policy." << endl;
            cout << "--glucose-deletion=<N>                 - Glucose deletion policy. Keep all the clauses with a LBD less or equal than N." << endl;
            cout << separator << endl;

            cout << "Output options                         " << endl << endl;
            cout << "--printprogram                         - Print the input program and exit." << endl;
            cout << "--silent                               - Print models in the wasp output." << endl;
            cout << "--third-competition-output             - Print models according to the third competition output." << endl;
            cout << "--competition-output                   - Print models according to the fourth competition output." << endl;
            cout << separator << endl;

            cout << "Heuristics options                     " << endl << endl;
            cout << "--heuristic-berkmin                    - Berkmin-like heuristic with look ahead for choosing polarity." << endl;
            cout << "--heuristic-firstundefined             - Choose always the first undefined literal." << endl;
            cout << "--heuristic-minisat                    - Enable minisat heuristic." << endl;
            cout << separator << endl;

            cout << "General options                     " << endl << endl;
            cout << "--dimacs                               - Enable dimacs format as input and outputs." << endl;
            cout << "--help                                 - Print this guide and exit." << endl;
//            cout << "--stdin                                - Write a description." << endl;
//            cout << "--time-limit                           - Write a description." << endl;
        }
};

#endif