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

#ifndef WASP_HELP_H
#define WASP_HELP_H

#include <iostream>
using namespace std;
#include "WaspConstants.h"

class WaspHelp
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
            cout << "--trace-multiaggregates=<verbosity>    - Trace multi aggregates propagation." << endl;
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
            cout << "--trace-predmin=<verbosity>            - Trace minimization of predicate." << endl;
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
            cout << "--minisat-policy                       - Enable minisat policy for deletion" << endl;
            cout << "--init-varinc=N                        - Initial value of variable increment [default: 1.0]" << endl;
            cout << "--init-vardecay=N                      - Initial value of variable decay 1/0.N [default: 95]" << endl;
            cout << "--init-strategy                        - Set the strategy to initialize minisat variable activities. N is the value specified by option --init-value" << endl;
            cout << "       =all-equals                     - All activities are equal to N" << endl;
            cout << "       =moms                           - Use moms (N is added to computed values)" << endl;
            cout << "       =binary                         - Use binary clauses (N is added to computed values)" << endl;
            cout << "       =watches                        - Use watches (N is added to computed values)" << endl;
            cout << "       =propagators                    - Use propagators (N is added to computed values)" << endl;
            cout << "       =visible-atoms                  - The activities of visible atoms are equal to N" << endl;
            cout << "       =hidden-atoms                   - The activities of hidden atoms are equal to N" << endl;
            cout << "       =combination                    - Use a combination of moms, watches and propagators (N is added to computed values)" << endl;             
            cout << "--init-value=N                         - Initial value of init-strategy [default: 0]" << endl;
            cout << "--init-sign                            - Set the polarity of selected literals" << endl;
            cout << "       =false                          - Choose always false" << endl;
            cout << "       =true                           - Choose always true" << endl;
            cout << "       =mixed                          - Choose always mixed" << endl;
            cout << separator << endl;
            cout << "Glucose options                        " << endl << endl;                        
            cout << "--size-lbdqueue                        - Initial size of LBD queue [default: 50]" << endl;
            cout << "--size-trailqueue                      - Initial size of trail queue [default: 5000]" << endl;
            cout << "--K                                    - Value used for restarts, greater values increment restarts [default: 0.8]" << endl;
            cout << "--R                                    - Value used for restarts, greater values reduce restarts [default: 1.4]" << endl;
            cout << "--clauses-beforereduce                 - Number of clauses needed for deletion [default: 2000]" << endl;
            cout << "--inc-reduceDB                         - Increment of the previous value after each deletion [default: 300]" << endl;
            cout << "--specialinc-reduceDB                  - Special increment performed when clauses have low lbd [default: 1000]" << endl;
            cout << "--lbd-frozenclause                     - Keep clauses with LBD lower than this value [default: 30]" << endl;
            cout << "--lbd-minclause                        - Learned clauses with LBD lower than this value are minimized [default: 6]" << endl;                        

            #if defined(ENABLE_PERL) || defined(ENABLE_PYTHON)
            cout << separator << endl;
            
            cout << "External scripts options                     " << endl << endl;
            cout << "--interpreter                          - Set the interpreter for heuristics and plugins" << endl;
            #ifdef ENABLE_PERL
            cout << "       =perl                           - Use perl as interpreter" << endl;
            #endif
            #ifdef ENABLE_PYTHON
            cout << "       =python                         - Use python as interpreter" << endl;            
            #endif
            cout << "--heuristic-scriptname                 - Specify the name of the script" << endl;            
            cout << "--plugins-files=file1,file2,...,filen  - Specify a list of plugins" << endl;
            cout << "--script-directory=absolutepath        - Specify a different folder for retrieving the python modules (default is .)" << endl;
            cout << "--lazy-weakconstraints                 - Enable the algorithm to lazily add weak constraints" << endl;
            #endif
            cout << separator << endl;

            cout << "Model Checker options                  " << endl << endl;
            cout << "--modelchecker-algorithm               - Compute the unfounded check for HC components using the specified algorithm" << endl;
            cout << "       =reduct                         - Algorithm based on the GL-reduct" << endl;
            cout << "       =unfounded                      - Algorithm based on the unfounded sets conditions" << endl;
            cout << "--modelchecker-compactreasons          - Enable a different strategy to compute reasons. (Note: it enables the option --disjunction=auto)" << endl;            
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
            cout << "       =one-multi                      - Core-guided algorithm one implemented with multi aggregates" << endl;
            cout << "       =pmres                          - Core-guided algorithm pmres" << endl;
            cout << "       =k                              - Core-guided algorithm k" << endl;
            cout << "       =interleaving-restarts          - Mixed approach between one and basic - restarts based" << endl;
            cout << "       =interleaving-choices           - Mixed approach between one and basic - choices based" << endl;
            cout << "--k-threshold                          - Initialization for algorithm k. Must be >= 0" << endl;
            cout << "       =0                              - Special case for a dynamic choice of k threshold" << endl;            
            cout << "       =1                              - Special case for algorithm one with cardinality constraints" << endl;
            cout << "--one-multi-threshold                  - Use one-multi only for cores whose size is greater than or equal to this threshold (default 0), for smaller cores use one. Must be >= 0" << endl;
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
            cout << "       =cb                             - Enable core-based algorithm (chunk size or percentage can be also specified)" << endl;
            cout << "       =chunk-static                   - Enable algorithm based on static chunks (Default size=2)" << endl;
            cout << "       =chunk-dynamic                  - Enable algorithm based on dynamic chunks (Default size=2)" << endl;
            cout << "       =preferences                    - Enable algorithm based on preferences" << endl;
            cout << "       =one                            - Enable algorithm based on algorithm one" << endl;
            cout << "       =kdyn                           - Enable algorithm based on algorithm kdyn" << endl;
            cout << "       =pmres                          - Enable algorithm based on algorithm pmres" << endl;
            cout << "       =pi                             - Enable algorithm based on prime implicate" << endl;
            cout << "       =ict-preferences                - Combines algorithm iterative coherence testing with algorithm based on preferences" << endl;
            cout << "--query-verbosity=(0,...,4)            - Enable verbosity of queries algorithms" << endl;
            cout << "--query-chunk-size=n                   - Specify the size of each chunk" << endl;
            cout << "--query-chunk-percentage=(1,...,100)   - The size of each chunk is a percentage of the number of candidates" << endl;
            cout << "--query-enable-core-cache              - Enable caching of cores for algorithms one, kdyn and pmres" << endl;
            
            cout << separator << endl;
            cout << "Predicates minimization                " << endl << endl;
            cout << "--minimize-predicates=pred1;...;predn  - Find the answer set subset-minimal w.r.t. atoms over specified predicates" << endl;
            cout << "--minimization-algorithm               - Specify the algorithm to use for the minimization" << endl;            
            cout << "       =enumeration                    - Enumerate all answer sets and select the cardinality minimum" << endl;
            cout << "       =guess-check                    - Guess an answer set and check the minimality" << endl;
            cout << "       =guess-check-minimize           - Guess an answer set and check the minimality. In case of failure reiterate the check" << endl;
            cout << "       =guess-check-split              - Guess an answer set and check the minimality of each atom" << endl;
            cout << "       =preferences                    - Use the always-false heuristic" << endl;
            cout << "       =core-based                     - Use algorithm based on unsatisfiable cores" << endl;
            cout << "--min-chunk-percentage=(1,...,100)     - Core based algorithm can use chunks. The size of each chunk is a percentage of the number of candidates" << endl;

            cout << separator << endl;
            cout << "Disjunction options                    " << endl << endl;
            cout << "--disjunction                          - Enable options for handling disjunctive rules" << endl;
            cout << "       =shift                          - Perform the shift of disjunctive rules" << endl;
            cout << "       =propagator                     - Use propagator for inferences" << endl;
//            cout << "       =lr                             - Enable left/right strategy" << endl;
            cout << "       =compv                          - Perform the disjunctive completion" << endl;
            cout << "       =auto                           - Perform an automatic choice between propagator and disjunctive completion" << endl;
//            cout << "       =quadratic                      - Enable quadratic strategy" << endl;
//            cout << "       =quadratic-aggregate            - Enable quadratic strategy with aggregate" << endl;
            cout << "--enable-shift-onedef                  - Enable shift also for atoms with one defition" << endl;
            
            cout << separator << endl;
            cout << "General options                        " << endl << endl;
            cout << "--stats=verbosity                      - Print statistics (verbosity is optional 0=basic stats, 1=more stats, 2=dynamic stats)" << endl;
            cout << "-n                                     - Specify the answers to compute" << endl;
            cout << "       <=0                             - To compute all answers" << endl;
            cout << "       =k                              - To compute at most k answers" << endl;
            cout << "--dimacs                               - Enable dimacs format as input and outputs" << endl;
            cout << "--help                                 - Print this guide and exit" << endl;
            cout << "--disable-simplifications              - Disable the simplifications of satelite" << endl;
            cout << "--enable-multiaggregates               - Share aggregate sets if possible" << endl;
            cout << "--enumeration-strategy                 - Select different strategy for enumeration of answer sets" << endl;
            cout << "       =bt                             - Use backtracking for enumeration" << endl;
            cout << "       =bt-reorder                     - Use backtracking for enumeration with reorder of assumptions" << endl;
            cout << "       =bc                             - Use blocking clause" << endl;
//            cout << "--stdin                                - Write a description." << endl;
//            cout << "--time-limit                           - Write a description." << endl;
        }
};

#endif
