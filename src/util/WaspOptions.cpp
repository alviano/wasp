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

#include "WaspOptions.h"

#include "WaspErrorMessage.h"
#include "WaspHelp.h"
#include "../WaspFacade.h"

#include <getopt.h>
#include <cstdlib>
#include <map>

namespace wasp
{

// Options with a short alias must use ascii codes.
// Here are all the short/long options ordered by code.
#define OPTIONID_help 'h'
#define OPTIONID_models 'n'

// Options without a short alias must use non-ascii codes.
// Here are all the "only-long" options ordered by code.

/* TRACE OPTIONS */
#define OPTIONID_trace_heuristic ( 'z' + 1 )
#define OPTIONID_trace_parser ( 'z' + 2 )
#define OPTIONID_trace_checker ( 'z' + 3 )
#define OPTIONID_trace_solving ( 'z' + 4 )
#define OPTIONID_trace_us ( 'z' + 5 )
#define OPTIONID_trace_learning ( 'z' + 6 )
#define OPTIONID_trace_enumeration ( 'z' + 7 )
#define OPTIONID_trace_satelite ( 'z' + 8 )
#define OPTIONID_trace_aggregates ( 'z' + 9 )
#define OPTIONID_trace_weakconstraints ( 'z' + 10 )
#define OPTIONID_trace_disjunction ( 'z' + 11 )
#define OPTIONID_trace_multiaggregates ( 'z' + 12 )
#define OPTIONID_trace_predmin ( 'z' + 13 )

/* OUTPUT OPTIONS */
#define OPTIONID_silent ( 'z' + 20 )
#define OPTIONID_competition_output ( 'z' + 21 )
#define OPTIONID_third_competition_output ( 'z' + 22 )
#define OPTIONID_multi ( 'z' + 25 )
#define OPTIONID_lastModel ( 'z' + 26 )
#define OPTIONID_printbounds ( 'z' + 27 )
#define OPTIONID_printatomtable ( 'z' + 28 )
#define OPTIONID_idOutput ( 'z' + 29 )
#define OPTIONID_onlyOptimum ( 'z' + 30 )
#define OPTIONID_stats ( 'z' + 31 )

#define OPTIONID_minisatheuristic ( 'z' + 40 )
#define OPTIONID_initVariableIncrement ( 'z' + 41 )
#define OPTIONID_initVariableDecay ( 'z' + 42 )
#define OPTIONID_initStrategy ( 'z' + 43 )
#define OPTIONID_initValue ( 'z' + 44 )
#define OPTIONID_initSign ( 'z' + 45 )    

/* RESTART OPTIONS */
#define OPTIONID_geometric_restarts ( 'z' + 50 )
#define OPTIONID_sequence_based_restarts ( 'z' + 51 )
#define OPTIONID_minisat_restarts ( 'z' + 52 )
#define OPTIONID_disable_restarts ( 'z' + 53 )

/* HEURISTIC OPTIONS */
#define OPTIONID_heuristic_interpreter ( 'z' + 60 )
#define OPTIONID_heuristic_scriptname ( 'z' + 61 )
#define OPTIONID_heuristic_plugins ( 'z' + 62 )
#define OPTIONID_heuristic_setscriptdirectory ( 'z' + 63 )
#define OPTIONID_heuristic_lazyweakconstraints ( 'z' + 64 )
    
    
/* GLUCOSE OPTIONS */
#define OPTIONID_glucose_sizeLBDQueue ( 'z' + 80 )
#define OPTIONID_glucose_sizeTrailQueue ( 'z' + 81 )
#define OPTIONID_glucose_K ( 'z' + 82 )
#define OPTIONID_glucose_R ( 'z' + 83 )
#define OPTIONID_glucose_nbclausesBeforeReduce ( 'z' + 84 )
#define OPTIONID_glucose_incReduceDB ( 'z' + 85 )
#define OPTIONID_glucose_specialIncReduceDB ( 'z' + 86 )
#define OPTIONID_glucose_lbLBDFrozenClause ( 'z' + 87 )
#define OPTIONID_glucose_lbLBDMinimizingClause ( 'z' + 88 )

/* INPUT OPTIONS */
#define OPTIONID_dimacs ( 'z' + 90 )

/* GENERIC OPTIONS */
#define OPTIONID_stdin ( 'z' + 100 )
#define OPTIONID_time_limit ( 'z' + 101 )
#define OPTIONID_max_cost ( 'z' + 102 )

#define OPTIONID_modelchecker_algorithm ( 'z' + 104 )
#define OPTIONID_forward_partialchecks ( 'z' + 105 )
#define OPTIONID_heuristic_partialchecks ( 'z' + 106 )
#define OPTIONID_shift_strategy ( 'z' + 107 )
#define OPTIONID_shift_onedef ( 'z' + 108 )
#define OPTIONID_simplifications ( 'z' + 109 )
#define OPTIONID_enumeration ( 'z' + 110 )
#define OPTIONID_modelchecker_compactreasons ( 'z' + 111 )
#define OPTIONID_multiaggregates ( 'z' + 112 ) 
    
/* WEAK CONSTRAINTS OPTIONS */
#define OPTIONID_weakconstraintsalgorithm ( 'z' + 200 )
#define OPTIONID_kthreshold ( 'z' + 201 )
#define OPTIONID_multithreshold ( 'z' + 202 )

#define OPTIONID_disjcores ( 'z' + 215 )
#define OPTIONID_minimize ( 'z' + 216 )
#define OPTIONID_stratification ( 'z' + 217 )
#define OPTIONID_firstmodel ( 'z' + 218 )
#define OPTIONID_minimizationstrategy ( 'z' + 219 )
#define OPTIONID_minimizationbudget ( 'z' + 220 )
    
/* QUERY OPTIONS */
#define OPTIONID_queryalgorithm ( 'z' + 300 )
#define OPTIONID_queryverbosity ( 'z' + 301 )
#define OPTIONID_querychunksize ( 'z' + 302 )
#define OPTIONID_querychunkpercentage ( 'z' + 303 )
#define OPTIONID_querycorecache ( 'z' + 304 )
    
/* PREDMIN OPTIONS */
#define OPTIONID_predminimizationalgorithm ( 'z' + 350 )
#define OPTIONID_predminimizationpredicate ( 'z' + 351 )
#define OPTIONID_predminimizationchunkpercentage ( 'z' + 352 )
    
#ifdef TRACE_ON
TraceLevels Options::traceLevels;
#endif

vector< const char* > Options::inputFiles;

OUTPUT_POLICY Options::outputPolicy = WASP_OUTPUT;

bool Options::printLastModelOnly = false;
bool Options::printBounds = false;
bool Options::printAtomTable = false;
bool Options::printOnlyOptimum = false;

bool Options::useLazyWeakConstraints = false;

bool Options::minisatPolicy = false;

unsigned int Options::interpreter = NO_INTERPRETER;
char* Options::heuristic_scriptname = NULL;

RESTARTS_POLICY Options::restartsPolicy = SEQUENCE_BASED_RESTARTS_POLICY;

unsigned int Options::restartsThreshold = 100;

unsigned int Options::timeLimit = 0;

unsigned int Options::maxModels = 1;

unsigned int Options::maxCost = MAXUNSIGNEDINT;

bool Options::forwardPartialChecks = false;

bool Options::heuristicPartialChecks = false;

WEAK_CONSTRAINTS_ALG Options::weakConstraintsAlg = ONE;

unsigned int Options::kthreshold = 0;

bool Options::disjCoresPreprocessing = false;
bool Options::minimizeUnsatCore = false;
bool Options::stratification = true;
bool Options::computeFirstModel = false;
unsigned Options::budget = UINT_MAX;

unsigned int Options::queryAlgorithm = NO_QUERY;
unsigned int Options::queryVerbosity = 0;

bool Options::callPyFinalize = true;

map< string, WEAK_CONSTRAINTS_ALG > Options::stringToWeak;

vector< string > Options::pluginsFilenames;

string Options::scriptDirectory = "";

unsigned int Options::predMinimizationAlgorithm = NO_PREDMINIMIZATION;
vector< string > Options::predicatesToMinimize;

bool Options::multiAggregates = false;
bool Options::queryCoreCache = false;

void split( const string &s, char delim, vector< string >& output )
{
    stringstream ss( s );
    string item;
    while( getline( ss, item, delim ) )
        output.push_back( item );    
}
SHIFT_STRATEGY Options::shiftStrategy = SHIFT_NAIVE;

bool Options::oneDefShift = false;

map< string, SHIFT_STRATEGY > Options::stringToShift;

map< string, unsigned int > Options::stringToMinimization;

map< string, unsigned int > Options::stringToQueryAlgorithms;

map< string, unsigned int > Options::stringToInitMinisatHeuristic;

map< string, unsigned int > Options::stringToPredMinimization;

bool Options::simplifications = true;

unsigned Options::silent = 0;

unsigned int Options::minimizationStrategy = MINIMIZATION_OFF;

unsigned int Options::minimizationBudget = UINT_MAX;

unsigned int Options::enumerationStrategy = ENUMERATION_BT;

unsigned int Options::chunkSize = UINT_MAX;

unsigned int Options::chunkPercentage = UINT_MAX;

unsigned int Options::modelcheckerAlgorithm = REDUCT_BASED;

unsigned int Options::minimizePredicateChunkPercentage = UINT_MAX;

bool Options::compactReasonsForHCC = false;

double Options::sizeLBDQueue = 50;
double Options::sizeTrailQueue = 5000;
double Options::K = 0.8;
double Options::R = 1.4;

int Options::nbclausesBeforeReduce = 2000;
int Options::incReduceDB = 300;
int Options::specialIncReduceDB = 1000;
unsigned int Options::lbLBDFrozenClause = 30;

unsigned int Options::lbLBDMinimizingClause = 6;

double Options::initVariableIncrement = 1.0;
double Options::initVariableDecay = ( 1 / 0.95 );

bool Options::stats = false;

unsigned Options::statsVerbosity = 0;

unsigned int Options::initMinisatHeuristic = INIT_MINISAT_ALL_EQUALS;
unsigned int Options::initValue = 0;
unsigned int Options::initSign = INIT_SIGN_MINISAT_ALLFALSE;

unsigned int Options::multiThreshold = 0;

void
Options::parse(
    int argc,
    char* const* argv )
{
    // It will store the option code.
    int code;
    initMap();
    
    do{
        // For each option there is an instance of option.
        // Each option specifies
        // - the long name;
        // - if it requires an argument (required_argument), accept an argument
        //   (optional_argument), or does not allow arguments (no_argument);
        // - the address of an integer flag to be setted followed by the value
        //   to use, or NULL followed by the code of the option.
        //
        // Here are all the long options ordered by long name.
        static struct option longOptions[] =
            {
                /* TRACE OPTIONS */
                { "trace-parser", required_argument, NULL, OPTIONID_trace_parser },
                { "trace-solver", required_argument, NULL, OPTIONID_trace_solving },
                { "trace-modelchecker", required_argument, NULL, OPTIONID_trace_checker },
                { "trace-unfoundedset", required_argument, NULL, OPTIONID_trace_us },
                { "trace-heuristic", required_argument, NULL, OPTIONID_trace_heuristic },
                { "trace-learning", required_argument, NULL, OPTIONID_trace_learning },
                { "trace-enumeration", required_argument, NULL, OPTIONID_trace_enumeration },
                { "trace-satelite", required_argument, NULL, OPTIONID_trace_satelite },
                { "trace-aggregates", required_argument, NULL, OPTIONID_trace_aggregates },
                { "trace-weakconstraints", required_argument, NULL, OPTIONID_trace_weakconstraints },
                { "trace-disjunction", required_argument, NULL, OPTIONID_trace_disjunction },
                { "trace-multiaggregates", required_argument, NULL, OPTIONID_trace_multiaggregates },
                { "trace-predmin", required_argument, NULL, OPTIONID_trace_predmin },

                /* OUTPUT OPTIONS */
                { "competition-output", no_argument, NULL, OPTIONID_competition_output },
                { "silent", optional_argument, NULL, OPTIONID_silent },                
                { "third-competition-output", no_argument, NULL, OPTIONID_third_competition_output },
                { "multi", no_argument, NULL, OPTIONID_multi },
                { "printlatestmodel", no_argument, NULL, OPTIONID_lastModel },
                { "printonlyoptimum", no_argument, NULL, OPTIONID_onlyOptimum },
                { "printbounds", no_argument, NULL, OPTIONID_printbounds },
                { "printatomstable", no_argument, NULL, OPTIONID_printatomtable },
                { "id-output", no_argument, NULL, OPTIONID_idOutput },
                { "stats", optional_argument, NULL, OPTIONID_stats },

                /* MINISAT POLICY */
                { "minisat-policy", no_argument, NULL, OPTIONID_minisatheuristic },
                { "init-varinc", required_argument, NULL, OPTIONID_initVariableIncrement },
                { "init-vardecay", required_argument, NULL, OPTIONID_initVariableDecay },
                { "init-strategy", required_argument, NULL, OPTIONID_initStrategy },
                { "init-value", required_argument, NULL, OPTIONID_initValue },
                { "init-sign", required_argument, NULL, OPTIONID_initSign },
                
                
                { "minimize-predicates", required_argument, NULL, OPTIONID_predminimizationpredicate },
                { "minimization-algorithm", required_argument, NULL, OPTIONID_predminimizationalgorithm },
                { "min-chunk-percentage", required_argument, NULL, OPTIONID_predminimizationchunkpercentage },
                
                #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
                /* HEURISTIC */
                { "interpreter", required_argument, NULL, OPTIONID_heuristic_interpreter },
                { "heuristic-scriptname", required_argument, NULL, OPTIONID_heuristic_scriptname },
                { "plugins-files", required_argument, NULL, OPTIONID_heuristic_plugins },
                { "script-directory", required_argument, NULL, OPTIONID_heuristic_setscriptdirectory },
                { "lazy-weakconstraints", no_argument, NULL, OPTIONID_heuristic_lazyweakconstraints },                
                #endif
                /* RESTART OPTIONS */                
//                { "geometric-restarts", optional_argument, NULL, OPTIONID_geometric_restarts },
//                { "minisat-restarts", optional_argument, NULL, OPTIONID_minisat_restarts },
//                { "disable-restarts", no_argument, NULL, OPTIONID_disable_restarts },
//                { "sequence-based-restarts", optional_argument, NULL, OPTIONID_sequence_based_restarts },                                
                
                /* GLUCOSE OPTIONS */                
                { "size-lbdqueue", required_argument, NULL, OPTIONID_glucose_sizeLBDQueue },
                { "size-trailqueue", required_argument, NULL, OPTIONID_glucose_sizeTrailQueue },
                { "K", required_argument, NULL, OPTIONID_glucose_K },
                { "R", required_argument, NULL, OPTIONID_glucose_R },
                { "clauses-beforereduce", required_argument, NULL, OPTIONID_glucose_nbclausesBeforeReduce },
                { "inc-reduceDB", required_argument, NULL, OPTIONID_glucose_incReduceDB },
                { "specialinc-reduceDB", required_argument, NULL, OPTIONID_glucose_specialIncReduceDB },
                { "lbd-frozenclause", required_argument, NULL, OPTIONID_glucose_lbLBDFrozenClause },
                { "lbd-minclause", required_argument, NULL, OPTIONID_glucose_lbLBDMinimizingClause },                

                /* INPUT OPTIONS */
                { "dimacs", no_argument, NULL, OPTIONID_dimacs },                
                
                /* GENERIC OPTIONS*/
                { "help", no_argument, NULL, OPTIONID_help },
                { "stdin", no_argument, NULL, OPTIONID_stdin },
                { "time-limit", required_argument, NULL, OPTIONID_time_limit },
                { "max-cost", required_argument, NULL, OPTIONID_max_cost },
                { "disable-simplifications", no_argument, NULL, OPTIONID_simplifications },
                { "enable-multiaggregates", no_argument, NULL, OPTIONID_multiaggregates },
                { "enumeration-strategy", required_argument, NULL, OPTIONID_enumeration },                
                
                { "modelchecker-algorithm", required_argument, NULL, OPTIONID_modelchecker_algorithm },  
                { "forward-partialchecks", no_argument, NULL, OPTIONID_forward_partialchecks },  
                { "heuristic-partialchecks", no_argument, NULL, OPTIONID_heuristic_partialchecks },
                { "modelchecker-compactreasons", no_argument, NULL, OPTIONID_modelchecker_compactreasons },                
                
                /* WEAK CONSTRAINTS */
                { "weakconstraints-algorithm", required_argument, NULL, OPTIONID_weakconstraintsalgorithm },
                { "k-threshold", required_argument, NULL, OPTIONID_kthreshold },
                { "one-multi-threshold", required_argument, NULL, OPTIONID_multithreshold },
                { "enable-disjcores", no_argument, NULL, OPTIONID_disjcores },
                { "trim-core", no_argument, NULL, OPTIONID_minimize },
                { "disable-stratification", no_argument, NULL, OPTIONID_stratification },
                { "compute-firstmodel", optional_argument, NULL, OPTIONID_firstmodel },
                { "shrinking-strategy", required_argument, NULL, OPTIONID_minimizationstrategy },
                { "shrinking-budget", required_argument, NULL, OPTIONID_minimizationbudget },
                
                /* SHIFT STRATEGY */
                { "disjunction", required_argument, NULL, OPTIONID_shift_strategy },
                { "enable-shift-onedef", no_argument, NULL, OPTIONID_shift_onedef },

                /* QUERY */
                { "query-algorithm", optional_argument, NULL, OPTIONID_queryalgorithm },
                { "query-verbosity", required_argument, NULL, OPTIONID_queryverbosity },
                { "query-chunk-size", required_argument, NULL, OPTIONID_querychunksize },
                { "query-chunk-percentage", required_argument, NULL, OPTIONID_querychunkpercentage },
                { "query-enable-core-cache", no_argument, NULL, OPTIONID_querycorecache },
                
                // The NULL-option indicates the end of the array.
                { NULL, 0, NULL, 0 }
            };

        // The function getopt_long() stores the option index here.
        int optionIndex = 0;

        // The third argument specifies all the short options.
        code = getopt_long( argc, argv, "hn:01", longOptions, &optionIndex );

        switch( code )
        {
            case -1:
                // All the command line was parsed.
                break;

            case OPTIONID_models:
                if( optarg )
                {
                    maxModels = atoi( optarg );
                    if( maxModels == 0 )
                        maxModels = UINT_MAX;
                }
                break;

            case OPTIONID_trace_parser:
                setTraceLevel( parser, atoi( optarg ) );
                break;

            case OPTIONID_trace_checker:
                setTraceLevel( modelchecker, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_heuristic:
                setTraceLevel( heuristic, atoi( optarg ) );
                break;

            case OPTIONID_trace_solving:
                setTraceLevel( solving, atoi( optarg ) );
                break;

            case OPTIONID_trace_us:
                setTraceLevel( unfoundedset, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_learning:
                setTraceLevel( learning, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_enumeration:
                setTraceLevel( enumeration, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_satelite:
                setTraceLevel( satelite, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_aggregates:
                setTraceLevel( aggregates, atoi( optarg ) );
                break;
            
            case OPTIONID_trace_weakconstraints:
                setTraceLevel( weakconstraints, atoi( optarg ) );
                break;
                
            case OPTIONID_trace_disjunction:
                setTraceLevel( disjunction, atoi( optarg ) );
                break;

            case OPTIONID_trace_multiaggregates:
                setTraceLevel( multiaggregates, atoi( optarg ) );
                break;    

            case OPTIONID_trace_predmin:
                setTraceLevel( predmin, atoi( optarg ) );
                break;    
                
            case OPTIONID_competition_output:
                outputPolicy = COMPETITION_OUTPUT;
                break;

            case OPTIONID_silent:
                if( optarg )
                {
                    silent = atoi( optarg );
                    if( silent < 1 )
                        silent = 0;
                    else if( silent > 1 )
                        silent = 2;                    
                }
                outputPolicy = SILENT_OUTPUT;
                break;

            case OPTIONID_third_competition_output:
                outputPolicy = THIRD_COMPETITION_OUTPUT;
                break;

            case OPTIONID_onlyOptimum:
                printOnlyOptimum = true;
                break;
                
            case OPTIONID_stats:
                stats = true;
                if( optarg )
                {
                    statsVerbosity = atoi( optarg );
                }
                break;
                
            case OPTIONID_multi:
                outputPolicy = MULTI;
                break;
                
            case OPTIONID_lastModel:
                outputPolicy = MULTI;
                printLastModelOnly = true;
                break;

            case OPTIONID_minisatheuristic:
                minisatPolicy = true;
                break;

            case OPTIONID_heuristic_interpreter:
                if( optarg )
                {
                    #ifdef ENABLE_PERL
                    if( !strcmp( optarg, "perl" ) )
                        interpreter = PERL_INTERPRETER;
                    else
                    #endif
                    #ifdef ENABLE_PYTHON
                    if( !strcmp( optarg, "python" ) )
                        interpreter = PYTHON_INTERPRETER;
                    else
                    #endif                    
                        WaspErrorMessage::errorGeneric( "Unkwown interpreter." );
                }
                break;

            case OPTIONID_printbounds:
                printBounds = true;
                break;
                
            case OPTIONID_printatomtable:
                printAtomTable = true;
                break;
                
            case OPTIONID_idOutput:
                outputPolicy = ID_OUTPUT;
                break;                
                                
            case OPTIONID_heuristic_scriptname:
                if( optarg )
                    heuristic_scriptname = optarg;
                break;
                
            case OPTIONID_heuristic_plugins:
                if( optarg )
                    split( string( optarg ), ',', pluginsFilenames );
                break;
                
            case OPTIONID_heuristic_setscriptdirectory:
                if( optarg )
                    scriptDirectory = string( optarg );
                break;
                
            case OPTIONID_heuristic_lazyweakconstraints:
                useLazyWeakConstraints = true;
                weakConstraintsAlg = LAZY;
                break;

            case OPTIONID_sequence_based_restarts:
                restartsPolicy = SEQUENCE_BASED_RESTARTS_POLICY;
                if( optarg )
                {
                    restartsThreshold = atoi( optarg );
                    if( restartsThreshold < 32 )
                        restartsThreshold = 32;
                }
                break;

            case OPTIONID_geometric_restarts:
                restartsPolicy = GEOMETRIC_RESTARTS_POLICY;
                if( optarg )
                {
                    restartsThreshold = atoi( optarg );
                    if( restartsThreshold < 100 )
                        restartsThreshold = 100;
                }
                break;
                
            case OPTIONID_minisat_restarts:
                restartsPolicy = MINISAT_RESTARTS_POLICY;
                if( optarg )
                {
                    restartsThreshold = atoi( optarg );
                    if( restartsThreshold < 100 )
                        restartsThreshold = 100;
                }
                break;
                
            case OPTIONID_disable_restarts:
                restartsPolicy = NO_RESTARTS_POLICY;
                break;                            
                            
            case OPTIONID_dimacs:
                outputPolicy = DIMACS_OUTPUT;
                break; 
                
            case OPTIONID_simplifications:
                simplifications = false;
                break;
                
            case OPTIONID_modelchecker_algorithm:
                modelcheckerAlgorithm = REDUCT_BASED;
                if( optarg )
                {
                    if( !strcmp( optarg, "reduct" ) )
                        modelcheckerAlgorithm = REDUCT_BASED;
                    else if( !strcmp( optarg, "unfounded" ) )
                        modelcheckerAlgorithm = UNFOUNDED_BASED;
                    else
                        WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for model checker." );
                }
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for model checker." );
                break;
                
            case OPTIONID_modelchecker_compactreasons:
                compactReasonsForHCC = true;
                shiftStrategy = SHIFT_AUTO;
                break;
                
            case OPTIONID_multiaggregates:
                multiAggregates = true;
                break;
                
            case OPTIONID_help:
                WaspHelp::printHelp();
                exit( 0 );
                break;

            case OPTIONID_stdin:
                inputFiles.push_back( "--" );
                break;

            case OPTIONID_time_limit:
                if( optarg )
                    timeLimit = atoi( optarg );
                break;
                
            case OPTIONID_max_cost:
                if( optarg )
                    maxCost = atoi( optarg );
                break;
                
            case OPTIONID_glucose_sizeLBDQueue:
                if( optarg )
                    sizeLBDQueue = atof( optarg );
                break;
             
            case OPTIONID_glucose_sizeTrailQueue:
                if( optarg )
                    sizeTrailQueue = atof( optarg );
                break;
                
            case OPTIONID_glucose_K:
                if( optarg )
                    K = atof( optarg );
                break;
                
            case OPTIONID_glucose_R:
                if( optarg )
                    R = atof( optarg );
                break;
                
            case OPTIONID_glucose_nbclausesBeforeReduce:
                if( optarg )
                    nbclausesBeforeReduce = atoi( optarg );
                break;
                
            case OPTIONID_glucose_incReduceDB:
                if( optarg )
                    incReduceDB = atoi( optarg );
                break;
            
            case OPTIONID_glucose_specialIncReduceDB:
                if( optarg )
                    specialIncReduceDB = atoi( optarg );
                break;
                
            case OPTIONID_glucose_lbLBDFrozenClause:
                if( optarg )
                    lbLBDFrozenClause = atoi( optarg );
                break;
                
            case OPTIONID_glucose_lbLBDMinimizingClause:
                if( optarg )
                    lbLBDMinimizingClause = atoi( optarg );
                break;
                
            case OPTIONID_initVariableIncrement:
                if( optarg )
                    initVariableIncrement = atof( optarg );
                break;
                
            case OPTIONID_initVariableDecay:
                if( optarg )                    
                    initVariableDecay = 1 / ( double ) ( atoi( optarg ) / 100 );                
                break;
                
            case OPTIONID_initStrategy:
                if( optarg )
                    initMinisatHeuristic = getInitMinisatHeuristic( optarg );
                break;
                
            case OPTIONID_initValue:
                if( optarg )
                    initValue = atoi( optarg );
                break;
                
            case OPTIONID_initSign:
                if( optarg )
                {
                    if( !strcmp( optarg, "false" ) )
                        initValue = INIT_SIGN_MINISAT_ALLFALSE;
                    else if( !strcmp( optarg, "true" ) )
                        initValue = INIT_SIGN_MINISAT_ALLTRUE;
                    else if( !strcmp( optarg, "mixed" ) )
                        initValue = INIT_SIGN_MINISAT_MIXED;
                }
                break;
    
            case OPTIONID_forward_partialchecks:
                forwardPartialChecks = true;
                break;
                
            case OPTIONID_heuristic_partialchecks:
                heuristicPartialChecks = true;
                break;                         

            case OPTIONID_weakconstraintsalgorithm:
                if( optarg )
                    weakConstraintsAlg = getAlgorithm( string( optarg ) );
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );
                break;
                
            case OPTIONID_kthreshold:
                if( optarg )
                {
                    int kthreshold_ = atoi( optarg );
                    if( kthreshold_ < 0 )
                        WaspErrorMessage::errorGeneric( "Threshold for k must be >= 0." );
                    kthreshold = kthreshold_;
                }
                break;
                
            case OPTIONID_multithreshold:
                if( optarg )
                {
                    int multiThreshold_ = atoi( optarg );
                    if( multiThreshold_ < 0 )
                        WaspErrorMessage::errorGeneric( "Threshold for multi must be >= 0." );
                    multiThreshold = multiThreshold_;
                }
                break;    
                
            case OPTIONID_shift_strategy:
                if( optarg )
                    shiftStrategy = getShiftStrategy( string( optarg ) );
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid strategy for shift." );
                break;
                
            case OPTIONID_enumeration:
                if( optarg )
                    enumerationStrategy = getEnumerationStrategy( string( optarg ) );
                else 
                    WaspErrorMessage::errorGeneric( "Inserted invalid strategy for enumeration." );
                break;

            case OPTIONID_shift_onedef:
                oneDefShift = true;
                break;
                
            case OPTIONID_disjcores:
                disjCoresPreprocessing = true;
                break;
                
            case OPTIONID_minimize:
                minimizeUnsatCore = true;
                break;
                
            case OPTIONID_stratification:
                stratification = false;
                break;
                
            case OPTIONID_firstmodel:
                computeFirstModel = true;
                if( optarg )
                    budget = atoi( optarg );                    
                break;
                
            case OPTIONID_minimizationstrategy:
                if( optarg )
                    minimizationStrategy = getMinimizationStrategy( string( optarg ) );
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid strategy for minimization." );                
                break;
                
            case OPTIONID_minimizationbudget:
                if( optarg )
                    minimizationBudget = atoi( optarg );                    
                break;
                
            case OPTIONID_queryalgorithm:
                queryAlgorithm = ITERATIVE_COHERENCE_TESTING;
                if( optarg )
                    queryAlgorithm = getQueryAlgorithm( optarg );                    
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for query answering." );
                break;
                
            case OPTIONID_queryverbosity:
                queryVerbosity = 0;
                if( optarg )
                {
                    unsigned int value = atoi( optarg );
                    if( value <= 4 )
                        queryVerbosity = value;
                    else
                        WaspErrorMessage::errorGeneric( "Inserted invalid value for query verbosity." );
                }
                else
                        WaspErrorMessage::errorGeneric( "Inserted invalid value for query verbosity." );
                break;
                
            case OPTIONID_querycorecache:
                queryCoreCache = true;
                break;
            
            case OPTIONID_querychunksize:
                if( optarg )
                {
                    chunkSize = atoi( optarg );
                    if( chunkPercentage != UINT_MAX )
                        WaspErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                }
                else
                    WaspErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                break;
                
            case OPTIONID_querychunkpercentage:
                if( optarg )
                {
                    chunkPercentage = atoi( optarg );
                    if( chunkPercentage == 0 || chunkPercentage > 100 )
                        WaspErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                    if( chunkSize != UINT_MAX )
                        WaspErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                }
                else
                    WaspErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                break;
                
            case OPTIONID_predminimizationalgorithm:
                if( optarg )
                {
                    map< string, unsigned int >::iterator it = stringToPredMinimization.find( optarg );
                    if( it == stringToPredMinimization.end() )
                        WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for pred minimization." );

                    predMinimizationAlgorithm = it->second;    
                }
                break;
            
            case OPTIONID_predminimizationpredicate:
                if( optarg )
                {
                    string s( optarg );                    
                    split( s, ';', predicatesToMinimize );                    
                }
                break;    
            
            case OPTIONID_predminimizationchunkpercentage:
                if( optarg )
                {
                    minimizePredicateChunkPercentage = atoi( optarg );
                    if( minimizePredicateChunkPercentage == 0 || minimizePredicateChunkPercentage > 100 )
                        WaspErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                }
                break;                    
                
            default:
                WaspErrorMessage::errorGeneric( "This option is not supported." );
                break;
        }
    } while( code != -1 );

    // TODO: Manage the reminder options (probably, they are all input
    //       files).
    for( int i = optind; i < argc; i++ )
    {
        inputFiles.push_back( argv[ i ] );
    }
    checkOptions();
}

void
Options::setOptions(
    WaspFacade& waspFacade )
{
    if( minisatPolicy ) waspFacade.setMinisatPolicy();
    waspFacade.setOutputPolicy( outputPolicy );
    waspFacade.setRestartsPolicy( restartsPolicy, restartsThreshold );    
    waspFacade.setMinimizeUnsatCore( minimizeUnsatCore );    
}

WEAK_CONSTRAINTS_ALG
Options::getAlgorithm(
    const string& s )
{   
    map< string, WEAK_CONSTRAINTS_ALG >::iterator it = stringToWeak.find( s );
    if( it == stringToWeak.end() )
        WaspErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );
    
    return it->second;    
}

SHIFT_STRATEGY
Options::getShiftStrategy(
    const string& s )
{   
    map< string, SHIFT_STRATEGY >::iterator it = stringToShift.find( s );
    if( it == stringToShift.end() )
        WaspErrorMessage::errorGeneric( "Inserted invalid strategy for shift." );
    
    return it->second;    
}

SHIFT_STRATEGY
Options::getMinimizationStrategy(
    const string& s )
{   
    map< string, unsigned int >::iterator it = stringToMinimization.find( s );
    if( it == stringToMinimization.end() )
        WaspErrorMessage::errorGeneric( "Inserted invalid strategy for minimization." );
    
    return it->second;    
}

unsigned int
Options::getQueryAlgorithm(
    const string& s )
{
    map< string, unsigned int >::iterator it = stringToQueryAlgorithms.find( s );
    if( it == stringToQueryAlgorithms.end() )
        WaspErrorMessage::errorGeneric( "Inserted invalid strategy for query algorithm." );
    
    return it->second; 
}

unsigned int
Options::getInitMinisatHeuristic(
    const string& s )
{
    map< string, unsigned int >::iterator it = stringToInitMinisatHeuristic.find( s );
    if( it == stringToInitMinisatHeuristic.end() )
        WaspErrorMessage::errorGeneric( "Inserted invalid strategy for initialization of minisat heuristic." );
    
    return it->second; 
}

unsigned int
Options::getEnumerationStrategy(
    const string& s )
{
    if( s == "bt" )
        return ENUMERATION_BT;
    else if( s == "bc" )
        return ENUMERATION_BC;
    else if(s=="bt-reorder")
        return ENUMERATION_BTO;
    WaspErrorMessage::errorGeneric( "Inserted invalid strategy for enumeration." );
    return ENUMERATION_BT;
}

void
Options::initMap()
{
    stringToWeak[ "one" ] = ONE;
    stringToWeak[ "opt" ] = OPT;
    stringToWeak[ "mgd" ] = MGD;
    stringToWeak[ "pmres" ] = PMRES;
    stringToWeak[ "basic" ] = BB;
    stringToWeak[ "interleaving-restarts" ] = ONEBBREST;
    stringToWeak[ "interleaving-choices" ] = ONEBB;
    stringToWeak[ "basic-bt" ] = BBBT;
    stringToWeak[ "k" ] = KALG;
    stringToWeak[ "one-multi" ] = ONEMULTI;

    stringToShift[ "shift" ] = SHIFT_NAIVE;
    stringToShift[ "propagator" ] = SHIFT_PROPAGATOR;
//    stringToShift[ "lr" ] = SHIFT_LEFT_RIGHT;
    stringToShift[ "compv" ] = SHIFT_NORMALIZE;
    stringToShift[ "auto" ] = SHIFT_AUTO;
//    stringToShift[ "quadratic" ] = SHIFT_QUADRATIC;
//    stringToShift[ "quadratic-aggregate" ] = SHIFT_QUADRATIC_AGGREGATE;
    
    stringToMinimization[ "progression" ] = MINIMIZATION_PROGRESSION;
    stringToMinimization[ "linearsearch" ] = MINIMIZATION_LINEARSEARCH;
    
    stringToQueryAlgorithms[ "or" ] = OVERESTIMATE_REDUCTION;
    stringToQueryAlgorithms[ "ict" ] = ITERATIVE_COHERENCE_TESTING;
    stringToQueryAlgorithms[ "cb" ] = COREBASED_QUERIES;
    stringToQueryAlgorithms[ "chunk-static" ] = CHUNK_STATIC;
    stringToQueryAlgorithms[ "chunk-dynamic" ] = CHUNK_DYNAMIC;
    stringToQueryAlgorithms[ "preferences" ] = PREFERENCE_QUERIES;
    stringToQueryAlgorithms[ "one" ] = ONE_QUERIES;
    stringToQueryAlgorithms[ "kdyn" ] = KDYN_QUERIES;
    stringToQueryAlgorithms[ "pmres" ] = PMRES_QUERIES;
    stringToQueryAlgorithms[ "ict-preferences" ] = ITERATIVE_COHERENCE_TESTING_PREFERENCES;
    stringToQueryAlgorithms[ "pi" ] = PRIME_IMPLICATE;
    
    stringToPredMinimization[ "enumeration" ] = PREDMIN_ENUMERATION;
    stringToPredMinimization[ "guess-check" ] = PREDMIN_GUESS_AND_CHECK;
    stringToPredMinimization[ "guess-check-minimize" ] = PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE;
    stringToPredMinimization[ "guess-check-split" ] = PREDMIN_GUESS_AND_CHECK_AND_SPLIT;
    stringToPredMinimization[ "preferences" ] = PREDMIN_PREFERENCES;
    stringToPredMinimization[ "core-based"] = PREDMIN_CORE;
    
    stringToInitMinisatHeuristic[ "all-equals" ] = INIT_MINISAT_ALL_EQUALS;
    stringToInitMinisatHeuristic[ "moms" ] = INIT_MINISAT_MOMS;
    stringToInitMinisatHeuristic[ "binary" ] = INIT_MINISAT_BINARY;
    stringToInitMinisatHeuristic[ "watches" ] = INIT_MINISAT_WATCHES;
    stringToInitMinisatHeuristic[ "propagators" ] = INIT_MINISAT_PROPAGATORS;
    stringToInitMinisatHeuristic[ "visible-atoms" ] = INIT_MINISAT_VISIBLE_ATOMS;
    stringToInitMinisatHeuristic[ "hidden-atoms" ] = INIT_MINISAT_HIDDEN_ATOMS;
    stringToInitMinisatHeuristic[ "combination" ] = INIT_MINISAT_COMBINATION;                    
}

void
Options::checkOptions()
{
    if( compactReasonsForHCC && shiftStrategy == SHIFT_NAIVE )
        WaspErrorMessage::errorGeneric( "Compact reasons canno be combined with shift" );
    
    if( modelcheckerAlgorithm == UNFOUNDED_BASED && heuristicPartialChecks )
        WaspErrorMessage::errorGeneric( "Partial checks are not available for the unfounded based approach" );
    
    if( modelcheckerAlgorithm == UNFOUNDED_BASED && forwardPartialChecks )
        WaspErrorMessage::errorGeneric( "Partial checks are not available for the unfounded based approach" );
}

};
