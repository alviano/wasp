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
#define OPTIONID_heuristic_interpreter ( 'z' + 30 )
#define OPTIONID_heuristic_scriptname ( 'z' + 31 )
#define OPTIONID_heuristic_pluginsinterpreter ( 'z' + 32 )
#define OPTIONID_heuristic_plugins ( 'z' + 33 )

#define OPTIONID_minisatheuristic ( 'z' + 40 )

/* RESTART OPTIONS */
#define OPTIONID_geometric_restarts ( 'z' + 50 )
#define OPTIONID_sequence_based_restarts ( 'z' + 51 )
#define OPTIONID_minisat_restarts ( 'z' + 52 )
#define OPTIONID_disable_restarts ( 'z' + 53 )

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

vector< const char* > Options::inputFiles;

OUTPUT_POLICY Options::outputPolicy = WASP_OUTPUT;

bool Options::printProgram = false;
bool Options::printDimacs = false;
bool Options::printLastModelOnly = false;

bool Options::minisatPolicy = false;

unsigned int Options::interpreter = NO_INTERPRETER;
unsigned int Options::plugins_interpreter = NO_INTERPRETER;
char* Options::heuristic_scriptname = NULL;

RESTARTS_POLICY Options::restartsPolicy = SEQUENCE_BASED_RESTARTS_POLICY;

unsigned int Options::restartsThreshold = 100;

unsigned int Options::timeLimit = 0;

unsigned int Options::maxModels = 1;

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

vector< string > Options::pluginsFilenames;

void split( const string &s, char delim, vector< string >& output )
{
    stringstream ss( s );
    string item;
    while( getline( ss, item, delim ) )
        output.push_back( item );    
}

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

                /* MINISAT POLICY */
                { "minisat-policy", no_argument, NULL, OPTIONID_minisatheuristic },                
                
                #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
                /* HEURISTIC */
                { "heuristic-interpreter", required_argument, NULL, OPTIONID_heuristic_interpreter },
                { "heuristic-scriptname", required_argument, NULL, OPTIONID_heuristic_scriptname },
                { "plugins-interpreter", required_argument, NULL, OPTIONID_heuristic_pluginsinterpreter },
                { "plugins-files", required_argument, NULL, OPTIONID_heuristic_plugins },
                #endif
                /* RESTART OPTIONS */                
//                { "geometric-restarts", optional_argument, NULL, OPTIONID_geometric_restarts },
//                { "minisat-restarts", optional_argument, NULL, OPTIONID_minisat_restarts },
//                { "disable-restarts", no_argument, NULL, OPTIONID_disable_restarts },
//                { "sequence-based-restarts", optional_argument, NULL, OPTIONID_sequence_based_restarts },                                
                
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
                        ErrorMessage::errorGeneric( "Unkwown interpreter." );
                }
                break;
                
            case OPTIONID_heuristic_scriptname:
                if( optarg )
                    heuristic_scriptname = optarg;
                break;
                
            case OPTIONID_heuristic_pluginsinterpreter:
                if( optarg )
                {
                    #ifdef ENABLE_PERL
                    if( !strcmp( optarg, "perl" ) )
                        plugins_interpreter = PERL_INTERPRETER;
                    else
                    #endif
                    #ifdef ENABLE_PYTHON
                    if( !strcmp( optarg, "python" ) )
                        plugins_interpreter = PYTHON_INTERPRETER;
                    else
                    #endif                    
                        ErrorMessage::errorGeneric( "Unkwown interpreter." );
                }
                break;
                
            case OPTIONID_heuristic_plugins:
                if( optarg )
                    split( string( optarg ), ',', pluginsFilenames );
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
    if( minisatPolicy )
        waspFacade.setMinisatPolicy();
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

