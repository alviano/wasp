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

#include "Options.h"

#include "ErrorMessage.h"
#include "Help.h"
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

/* OUTPUT OPTIONS */
#define OPTIONID_silent ( 'z' + 20 )
#define OPTIONID_competition_output ( 'z' + 21 )
#define OPTIONID_third_competition_output ( 'z' + 22 )
#define OPTIONID_printprogram ( 'z' + 23 )
#define OPTIONID_printdimacs ( 'z' + 24 )
#define OPTIONID_multi ( 'z' + 25 )
#define OPTIONID_lastModel ( 'z' + 26 )

/* HEURISTIC OPTIONS */
#define OPTIONID_fuheuristic ( 'z' + 30 )
#define OPTIONID_berkminheuristic ( 'z' + 31 )
#define OPTIONID_minisatheuristic ( 'z' + 32 )
#define OPTIONID_berkminheuristiccache ( 'z' + 33 )

/* RESTART OPTIONS */
#define OPTIONID_geometric_restarts ( 'z' + 50 )
#define OPTIONID_sequence_based_restarts ( 'z' + 51 )
#define OPTIONID_minisat_restarts ( 'z' + 52 )
#define OPTIONID_disable_restarts ( 'z' + 53 )

/* DELETION OPTIONS */
#define OPTIONID_aggressive_deletion ( 'z' + 70 )
#define OPTIONID_restarts_based_deletion ( 'z' + 71 )
#define OPTIONID_minisat_deletion ( 'z' + 72 )
#define OPTIONID_glucose_deletion ( 'z' + 73 )

/* INPUT OPTIONS */
#define OPTIONID_dimacs ( 'z' + 90 )

/* GENERIC OPTIONS */
#define OPTIONID_stdin ( 'z' + 100 )
#define OPTIONID_time_limit ( 'z' + 101 )
#define OPTIONID_max_cost ( 'z' + 102 )
#define OPTIONID_exchange_clauses ( 'z' + 103 )
#define OPTIONID_forward_partialchecks ( 'z' + 104 )
#define OPTIONID_bumpactivityafterpartialchecks ( 'z' + 105 )
#define OPTIONID_backward_partialchecks ( 'z' + 106 )
    
/* WEAK CONSTRAINTS OPTIONS */
#define OPTIONID_weakconstraintsalgorithm ( 'z' + 200 )

#define OPTIONID_disjcores ( 'z' + 215 )
#define OPTIONID_minimize ( 'z' + 216 )
#define OPTIONID_stratification ( 'z' + 217 )
#define OPTIONID_firstmodel ( 'z' + 218 )
    
/* QUERY OPTIONS */
#define OPTIONID_queryalgorithm ( 'z' + 300 )
#define OPTIONID_queryverbosity ( 'z' + 301 )
    
#ifdef TRACE_ON
TraceLevels Options::traceLevels;
#endif

//DELETION_POLICY Options::deletionPolicy = AGGRESSIVE_DELETION_POLICY;
DELETION_POLICY Options::deletionPolicy = RESTARTS_BASED_DELETION_POLICY;

DECISION_POLICY Options::decisionPolicy = HEURISTIC_BERKMIN;

vector< const char* > Options::inputFiles;

//unsigned int Options::decisionThreshold = UINT_MAX;
unsigned int Options::decisionThreshold = 512;

OUTPUT_POLICY Options::outputPolicy = WASP_OUTPUT;
//OUTPUT_POLICY Options::outputPolicy = DIMACS_OUTPUT;

bool Options::printProgram = false;
bool Options::printDimacs = false;
bool Options::printLastModelOnly = false;

RESTARTS_POLICY Options::restartsPolicy = SEQUENCE_BASED_RESTARTS_POLICY;

//unsigned int Options::restartsThreshold = 32;
//unsigned int Options::restartsThreshold = 100000;
unsigned int Options::restartsThreshold = 100;

unsigned int Options::timeLimit = 0;

unsigned int Options::maxModels = 1;

unsigned int Options::deletionThreshold = 8;

unsigned int Options::maxCost = MAXUNSIGNEDINT;

bool Options::exchangeClauses = false;

bool Options::forwardPartialChecks = false;

bool Options::backwardPartialChecks = false;

bool Options::bumpActivityAfterPartialCheck = false;

WEAK_CONSTRAINTS_ALG Options::weakConstraintsAlg = OLL;

bool Options::disjCoresPreprocessing = false;
bool Options::minimizeUnsatCore = false;
bool Options::stratification = true;
bool Options::computeFirstModel = false;
unsigned Options::budget = UINT_MAX;

unsigned int Options::queryAlgorithm = NO_QUERY;
unsigned int Options::queryVerbosity = 0;

map< string, WEAK_CONSTRAINTS_ALG > Options::stringToWeak;
    
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
                { "trace-solving", required_argument, NULL, OPTIONID_trace_solving },
                { "trace-modelchecker", required_argument, NULL, OPTIONID_trace_checker },
                { "trace-unfoundedset", required_argument, NULL, OPTIONID_trace_us },
                { "trace-heuristic", required_argument, NULL, OPTIONID_trace_heuristic },
                { "trace-learning", required_argument, NULL, OPTIONID_trace_learning },
                { "trace-enumeration", required_argument, NULL, OPTIONID_trace_enumeration },
                { "trace-satelite", required_argument, NULL, OPTIONID_trace_satelite },
                { "trace-aggregates", required_argument, NULL, OPTIONID_trace_aggregates },
                { "trace-weakconstraints", required_argument, NULL, OPTIONID_trace_weakconstraints },

                /* OUTPUT OPTIONS */
                { "competition-output", no_argument, NULL, OPTIONID_competition_output },
                { "silent", no_argument, NULL, OPTIONID_silent },                
                { "third-competition-output", no_argument, NULL, OPTIONID_third_competition_output },
                { "printprogram", no_argument, NULL, OPTIONID_printprogram },
                { "printdimacs", no_argument, NULL, OPTIONID_printdimacs },
                { "multi", no_argument, NULL, OPTIONID_multi },
                { "printlatestmodel", no_argument, NULL, OPTIONID_lastModel },

                /* HEURISTIC OPTIONS */
//                { "heuristic-berkmin", optional_argument, NULL, OPTIONID_berkminheuristic },
//                { "heuristic-berkmin-cache", optional_argument, NULL, OPTIONID_berkminheuristiccache },
//                { "heuristic-firstundefined", no_argument, NULL, OPTIONID_fuheuristic },
                { "heuristic-minisat", no_argument, NULL, OPTIONID_minisatheuristic },
                
                /* RESTART OPTIONS */                
//                { "geometric-restarts", optional_argument, NULL, OPTIONID_geometric_restarts },
//                { "minisat-restarts", optional_argument, NULL, OPTIONID_minisat_restarts },
//                { "disable-restarts", no_argument, NULL, OPTIONID_disable_restarts },
//                { "sequence-based-restarts", optional_argument, NULL, OPTIONID_sequence_based_restarts },
                
                /* DELETION OPTIONS */
//                { "aggressive-deletion", no_argument, NULL, OPTIONID_aggressive_deletion },
//                { "restarts-based-deletion", no_argument, NULL, OPTIONID_restarts_based_deletion },
//                { "minisat-deletion", no_argument, NULL, OPTIONID_minisat_deletion },
//                { "glucose-deletion", optional_argument, NULL, OPTIONID_glucose_deletion },
                
                /* INPUT OPTIONS */
                { "dimacs", no_argument, NULL, OPTIONID_dimacs },                
                
                /* GENERIC OPTIONS*/
                { "help", no_argument, NULL, OPTIONID_help },
                { "stdin", no_argument, NULL, OPTIONID_stdin },
                { "time-limit", required_argument, NULL, OPTIONID_time_limit },
                { "max-cost", required_argument, NULL, OPTIONID_max_cost },
                
                { "exchange-clauses", no_argument, NULL, OPTIONID_exchange_clauses },
                { "forward-partialchecks", no_argument, NULL, OPTIONID_forward_partialchecks },  
                { "backward-partialchecks", no_argument, NULL, OPTIONID_backward_partialchecks },  
                { "bump-activity-partialchecks", no_argument, NULL, OPTIONID_bumpactivityafterpartialchecks },  
                
                /* WEAK CONSTRAINTS */
                { "weakconstraints-algorithm", required_argument, NULL, OPTIONID_weakconstraintsalgorithm },
                { "enable-disjcores", no_argument, NULL, OPTIONID_disjcores },
                { "minimize-unsatcore", no_argument, NULL, OPTIONID_minimize },
                { "disable-stratification", no_argument, NULL, OPTIONID_stratification },
                { "compute-firstmodel", optional_argument, NULL, OPTIONID_firstmodel },

                /* QUERY */
                { "query-algorithm", optional_argument, NULL, OPTIONID_queryalgorithm },
                { "query-verbosity", required_argument, NULL, OPTIONID_queryverbosity },
                
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

            case OPTIONID_competition_output:
                outputPolicy = COMPETITION_OUTPUT;
                break;

            case OPTIONID_silent:
                outputPolicy = SILENT_OUTPUT;
                break;

            case OPTIONID_third_competition_output:
                outputPolicy = THIRD_COMPETITION_OUTPUT;
                break;

            case OPTIONID_printprogram:
                printProgram = true;
                break;

            case OPTIONID_printdimacs:
                printDimacs = true;
                break;
                
            case OPTIONID_multi:
                outputPolicy = MULTI;
                break;
                
            case OPTIONID_lastModel:
                outputPolicy = MULTI;
                printLastModelOnly = true;
                break;

            case OPTIONID_berkminheuristic:
                if( optarg )
                {
                    decisionThreshold = atoi( optarg );
                    if( decisionThreshold == 0 )
                        decisionThreshold = UINT_MAX;
                }
                decisionPolicy = HEURISTIC_BERKMIN;
                break;
            
            case OPTIONID_berkminheuristiccache:
                if( optarg )
                {
                    decisionThreshold = atoi( optarg );
                    if( decisionThreshold == 0 )
                        decisionThreshold = UINT_MAX;
                }
                decisionPolicy = HEURISTIC_BERKMIN_CACHE;
                break;
                
            case OPTIONID_fuheuristic:
                decisionPolicy = HEURISTIC_FIRST_UNDEFINED;
                break;
            
            case OPTIONID_minisatheuristic:
                decisionPolicy = HEURISTIC_MINISAT;
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
                
            case OPTIONID_aggressive_deletion:
                deletionPolicy = AGGRESSIVE_DELETION_POLICY;
                break;
                
            case OPTIONID_restarts_based_deletion:
                deletionPolicy = RESTARTS_BASED_DELETION_POLICY;
                break;
                
            case OPTIONID_minisat_deletion:
                deletionPolicy = MINISAT_DELETION_POLICY;
                break;
                
            case OPTIONID_glucose_deletion:
                deletionPolicy = GLUCOSE_DELETION_POLICY;
                if( optarg )
                {
                    deletionThreshold = atoi( optarg );
                    if( deletionThreshold < 2 )
                        deletionThreshold = 2;
                }
                break;

            case OPTIONID_dimacs:
                outputPolicy = DIMACS_OUTPUT;
                break; 
                
            case OPTIONID_help:
                Help::printHelp();
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
                
            case OPTIONID_exchange_clauses:
                exchangeClauses = true;
                break;
                
            case OPTIONID_forward_partialchecks:
                forwardPartialChecks = true;
                break;
                
            case OPTIONID_backward_partialchecks:
                backwardPartialChecks = true;
                break;            
                
            case OPTIONID_bumpactivityafterpartialchecks:
                bumpActivityAfterPartialCheck = true;
                break;

            case OPTIONID_weakconstraintsalgorithm:
                if( optarg )
                    weakConstraintsAlg = getAlgorithm( string( optarg ) );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );
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
                
            case OPTIONID_queryalgorithm:
                queryAlgorithm = ITERATIVE_COHERENCE_TESTING;
                if( optarg )
                {
                    if( !strcmp( optarg, "or" ) )
                        queryAlgorithm = OVERESTIMATE_REDUCTION;
                    else if( !strcmp( optarg, "ict" ) )
                        queryAlgorithm = ITERATIVE_COHERENCE_TESTING;
                    else
                        ErrorMessage::errorGeneric( "Inserted invalid algorithm for query answering." );
                }
                break;
                
            case OPTIONID_queryverbosity:
                queryVerbosity = 0;
                if( optarg )
                {
                    unsigned int value = atoi( optarg );
                    if( value <= 2 )
                        queryVerbosity = value;
                    else
                        ErrorMessage::errorGeneric( "Inserted invalid value for query verbosity." );
                }
                break;
                
            default:
                ErrorMessage::errorGeneric( "This option is not supported." );
                break;
        }
    } while( code != -1 );

    // TODO: Manage the reminder options (probably, they are all input
    //       files).
    for( int i = optind; i < argc; i++ )
    {
        inputFiles.push_back( argv[ i ] );
    }
}

void
Options::setOptions(
    WaspFacade& waspFacade )
{
    waspFacade.setDeletionPolicy( deletionPolicy, deletionThreshold );
    waspFacade.setDecisionPolicy( decisionPolicy, decisionThreshold );
    waspFacade.setOutputPolicy( outputPolicy );
    waspFacade.setRestartsPolicy( restartsPolicy, restartsThreshold );
    waspFacade.setMaxModels( maxModels );
    waspFacade.setPrintProgram( printProgram );
    waspFacade.setPrintDimacs( printDimacs);
    waspFacade.setExchangeClauses( exchangeClauses );
    waspFacade.setWeakConstraintsAlgorithm( weakConstraintsAlg );
    waspFacade.setDisjCoresPreprocessing( disjCoresPreprocessing );
    waspFacade.setMinimizeUnsatCore( minimizeUnsatCore );
    waspFacade.setQueryAlgorithm( queryAlgorithm );
}

WEAK_CONSTRAINTS_ALG
Options::getAlgorithm(
    const string& s )
{   
    map< string, WEAK_CONSTRAINTS_ALG >::iterator it = stringToWeak.find( s );
    if( it == stringToWeak.end() )
        ErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );
    
    return it->second;    
}

void
Options::initMap()
{
    stringToWeak[ "oll" ] = OLL;
    stringToWeak[ "opt" ] = OPT;
    stringToWeak[ "mgd" ] = MGD;
    stringToWeak[ "pmres" ] = PMRES;
    stringToWeak[ "basic" ] = BB;
    stringToWeak[ "interleaving-restarts" ] = OLLBBREST;
    stringToWeak[ "interleaving-choices" ] = OLLBB;    
}

};

