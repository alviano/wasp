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
            cout << "--trace-satelite=<verbosity>           - Trace satelite simplifications." << endl;
            cout << "--trace-modelchecker=<verbosity>       - Trace model checker." << endl;
            cout << "--trace-unfoundedset=<verbosity>       - Trace unfounded set function." << endl;            
            cout << separator << endl;
            #endif

//            cout << "Restart options                       " << endl << endl;
//            cout << "--sequence-based-restarts=<N>          - Luby et al. sequence using N as number of conflicts." << endl;
//            cout << "--geometric-restarts=<N>               - Geometric restarts." << endl;
//            cout << "--minisat-restarts=<N>                 - Minisat restarts." << endl;
//            cout << "--disable-restarts                     - Disable restarts." << endl;
//            cout << separator << endl;
//            
//            cout << "Deletion options                       " << endl << endl;            
//            cout << "--aggressive-deletion                  - Aggressive variant of minisat deletion policy." << endl;
//            cout << "--restarts-based-deletion              - Restarts-based deletion policy." << endl;
//            cout << "--minisat-deletion                     - Minisat deletion policy." << endl;
//            cout << "--glucose-deletion=<N>                 - Glucose deletion policy. Keep all the clauses with a LBD less or equal than N." << endl;
//            cout << separator << endl;

            cout << "Output options                         " << endl << endl;
            cout << "--printprogram                         - Print the input program and exit" << endl;
            cout << "--printdimacs                          - Print the input program in the dimacs format and exit" << endl;
            cout << "--silent                               - Print models with no spaces and lines" << endl;
            cout << "--third-competition-output             - Print models according to the third competition output" << endl;
            cout << "--competition-output                   - Print models according to the fourth competition output" << endl;   
            cout << "--printlatestmodel                     - Print the latest computed model" << endl;   
            cout << separator << endl;
            
            cout << "Heuristics options                     " << endl << endl;
//            cout << "--heuristic-berkmin                    - Berkmin-like heuristic with look ahead for choosing polarity." << endl;
//            cout << "--heuristic-berkmin-cache              - Berkmin-like heuristic with cache for choosing polarity." << endl;
//            cout << "--heuristic-firstundefined             - Choose always the first undefined literal." << endl;
            cout << "--heuristic-minisat                    - Enable minisat heuristic" << endl;
            cout << separator << endl;

            cout << "Model Checker options                  " << endl << endl;
            cout << "--exchange-clauses                     - Exchange clauses from checker to generator" << endl;
            cout << "--forward-partialchecks                - Enable forward partial checks" << endl;
            cout << "--backward-partialchecks               - Enable backward partial checks" << endl;
            cout << "--bump-activity-partialchecks          - Bump activity after partial checks" << endl;
                
            cout << separator << endl;
            cout << "Weak Constraints options               " << endl << endl;
            cout << "--weakconstraints-algorithm            - Compute optimization problems using the specified algorithm" << endl;
            cout << "       =basic                          - Model-guided algorithm basic" << endl;
            cout << "       =mgd                            - Model-guided algorithm mgd" << endl;
            cout << "       =opt                            - Model-guided algorithm opt" << endl;
            cout << "       =oll                            - Core-guided algorithm oll" << endl;
            cout << "       =pmres                          - Core-guided algorithm pmres" << endl;
            cout << "       =interleaving-restarts          - Mixed approach between oll and basic - restarts based" << endl;
            cout << "       =interleaving-choices           - Mixed approach between oll and basic - choices based" << endl;
            cout << "--enable-disjcores                     - Enable disjoint cores preprocessing (only for core-guided)" << endl;
            cout << "--minimize-unsatcore                   - Enable minimization of unsat cores (only for core-guided)" << endl;
            cout << "--disable-stratification               - Disable stratification (only for core-guided)" << endl;
            cout << "--compute-firstmodel=budget            - Compute a model before starting the algorithm. In addition, budget may specify the maximum number of seconds allowed." << endl;

            cout << separator << endl;
            cout << "Query options                          " << endl << endl;
            cout << "--query-algorithm                      - Compute cautious consequences according to the specified algorithm" << endl;
            cout << "       =ict                            - Enable iterative coherence testing" << endl;
            cout << "       =or                             - Enable overestimate reduction" << endl;
            cout << "--query-verbosity=(0,1,2)              - Enable verbosity of queries algorithms" << endl;

            cout << separator << endl;
            cout << "General options                        " << endl << endl;
            cout << "--dimacs                               - Enable dimacs format as input and outputs" << endl;
            cout << "--help                                 - Print this guide and exit" << endl;
//            cout << "--stdin                                - Write a description." << endl;
//            cout << "--time-limit                           - Write a description." << endl;
        }
};

#endif