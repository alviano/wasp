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
            cout << "--trace-aggregates=<verbosity>         - Trace aggregates propagation." << endl;
            cout << "--trace-disjunction=<verbosity>        - Trace disjunction propagator." << endl;
            cout << "--trace-enumeration=<verbosity>        - Trace enumeration." << endl;
            cout << "--trace-heuristic=<verbosity>          - Trace heuristic." << endl;
            cout << "--trace-learning=<verbosity>           - Trace learning." << endl;
            cout << "--trace-modelchecker=<verbosity>       - Trace model checker." << endl;
            cout << "--trace-parser=<verbosity>             - Trace parser." << endl;
            cout << "--trace-satelite=<verbosity>           - Trace satelite simplifications." << endl;
            cout << "--trace-solver=<verbosity>             - Trace solver." << endl;
            cout << "--trace-unfoundedset=<verbosity>       - Trace computation of unfounded sets." << endl;
            cout << "--trace-weakconstraints=<verbosity>    - Trace weak constraints algorithms." << endl;
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
            cout << "--silent                               - Set verbosity of output" << endl;
            cout << "       <1                              - Print models with no spaces and lines" << endl;
            cout << "       =1                              - Print only the number of each model" << endl;
            cout << "       >1                              - Print only the number of models" << endl;
            cout << "--third-competition-output             - Print models according to the third competition output" << endl;
            cout << "--competition-output                   - Print models according to the fourth competition output" << endl;   
            cout << "--id-output                            - Print models using the internal id of atoms" << endl;
            cout << "--printlatestmodel                     - Print the latest computed model" << endl;   
            cout << "--printonlyoptimum                     - Print only optimum answer sets" << endl;
            cout << "--printbounds                          - Print lower and upper bound during optimization of answer sets" << endl;
            cout << "--printatomstable                      - Print internal dictionary for atoms used in wasp" << endl;            

            cout << separator << endl;
            
            cout << "Heuristics options                     " << endl << endl;
//            cout << "--heuristic-berkmin                    - Berkmin-like heuristic with look ahead for choosing polarity." << endl;
//            cout << "--heuristic-berkmin-cache              - Berkmin-like heuristic with cache for choosing polarity." << endl;
//            cout << "--heuristic-firstundefined             - Choose always the first undefined literal." << endl;
            cout << "--heuristic-minisat                    - Enable minisat heuristic" << endl;
            cout << separator << endl;

            cout << "Model Checker options                  " << endl << endl;
            cout << "--forward-partialchecks                - Enable forward partial checks" << endl;
            cout << "--heuristic-partialchecks              - Enable heuristic to activate or disactivate partial checks during the search" << endl;            
                
            cout << separator << endl;
            cout << "Weak Constraints options               " << endl << endl;
            cout << "--weakconstraints-algorithm            - Compute optimization problems using the specified algorithm" << endl;
            cout << "       =basic                          - Model-guided algorithm basic" << endl;
            cout << "       =basic-bt                       - Model-guided algorithm basic with classical backtracking after first model" << endl;
            cout << "       =mgd                            - Model-guided algorithm mgd" << endl;
            cout << "       =opt                            - Model-guided algorithm opt" << endl;
            cout << "       =one                            - Core-guided algorithm one (default)" << endl;
            cout << "       =pmres                          - Core-guided algorithm pmres" << endl;
            cout << "       =k                              - Core-guided algorithm k" << endl;
            cout << "       =interleaving-restarts          - Mixed approach between one and basic - restarts based" << endl;
            cout << "       =interleaving-choices           - Mixed approach between one and basic - choices based" << endl;
            cout << "--k-threshold                          - Initialization for algorithm k. Must be >= 0" << endl;
            cout << "       =0                              - Special case for a dynamic choice of k threshold" << endl;            
            cout << "       =1                              - Special case for algorithm one with cardinality constraints" << endl;            
            cout << "--enable-disjcores                     - Enable disjoint cores preprocessing (only for core-guided)" << endl;
            cout << "--trim-core                            - Enable polynomial trim of unsat cores (only for core-guided)" << endl;
            cout << "--disable-stratification               - Disable stratification (only for core-guided)" << endl;
            cout << "--compute-firstmodel=budget            - Compute a model before starting the algorithm. In addition, budget may specify the maximum number of seconds allowed" << endl;
            cout << "--shrinking-strategy                   - Expensive minimization of unsat cores (only for core-guided)" << endl;
            cout << "       =progression                    - Use progression" << endl;
            cout << "       =linearsearch                   - Use linear search" << endl;        
            cout << "--shrinking-budget=budget              - Limit the running time (in seconds) of the solver during the minimization" << endl;
            
            cout << separator << endl;
            cout << "Query options                          " << endl << endl;
            cout << "--query-algorithm                      - Compute cautious consequences according to the specified algorithm" << endl;
            cout << "       =ict                            - Enable iterative coherence testing" << endl;
            cout << "       =or                             - Enable overestimate reduction" << endl;
            cout << "       =chunk-static                   - Enable algorithm based on static chunks. (Default size=2)" << endl;
            cout << "       =chunk-dynamic                  - Enable algorithm based on dynamic chunks. (Default size=2)" << endl;
            cout << "--query-verbosity=(0,...,3)            - Enable verbosity of queries algorithms" << endl;
            cout << "--query-chunk-size=n                   - Specify the size of each chunk" << endl;
            cout << "--query-chunk-percentage=(1,...,100)   - The size of each chunk is a percentage of the number of candidates" << endl;

            cout << separator << endl;
            cout << "Disjunction options                    " << endl << endl;
            cout << "--disjunction                          - Enable options for handling disjunctive rules" << endl;
            cout << "       =shift                          - Perform the shift of disjunctive rules" << endl;
            cout << "       =propagator                     - Use propagator for inferences" << endl;
//            cout << "       =lr                             - Enable left/right strategy" << endl;
            cout << "       =compv                          - Perform the disjunctive completion" << endl;
//            cout << "       =quadratic                      - Enable quadratic strategy" << endl;
//            cout << "       =quadratic-aggregate            - Enable quadratic strategy with aggregate" << endl;
            cout << "--enable-shift-onedef                  - Enable shift also for atoms with one defition" << endl;
            
            cout << separator << endl;
            cout << "General options                        " << endl << endl;
            cout << "-n                                     - Specify the answers to compute " << endl;
            cout << "       <=0                             - To compute all answers" << endl;
            cout << "       =k                              - To compute at most k answers" << endl;
            cout << "--dimacs                               - Enable dimacs format as input and outputs" << endl;
            cout << "--help                                 - Print this guide and exit" << endl;
            cout << "--disable-simplifications              - Disable the simplifications of satelite" << endl;
            cout << "--enumeration-strategy                 - Select different strategy for enumeration of answer sets" << endl;
            cout << "       =bt                             - Use backtracking for enumeration" << endl;
            cout << "       =bc                             - Use blocking clause" << endl;
//            cout << "--stdin                                - Write a description." << endl;
//            cout << "--time-limit                           - Write a description." << endl;
        }
};

#endif
