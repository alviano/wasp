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
    
/* OUTPUT OPTIONS */
#define OPTIONID_silent ( 'z' + 20 )
#define OPTIONID_competition_output ( 'z' + 21 )
#define OPTIONID_third_competition_output ( 'z' + 22 )
#define OPTIONID_printprogram ( 'z' + 23 )

/* HEURISTIC OPTIONS */
#define OPTIONID_fuheuristic ( 'z' + 30 )
#define OPTIONID_berkminheuristic ( 'z' + 31 )
#define OPTIONID_minisatheuristic ( 'z' + 32 )

/* RESTART OPTIONS */
#define OPTIONID_geometric_restarts ( 'z' + 50 )
#define OPTIONID_sequence_based_restarts ( 'z' + 51 )
#define OPTIONID_minisat_restarts ( 'z' + 52 )
#define OPTIONID_disable_restarts ( 'z' + 53 )

/* DELETION OPTIONS */
#define OPTIONID_aggressive_deletion ( 'z' + 70 )
#define OPTIONID_restarts_based_deletion ( 'z' + 71 )
#define OPTIONID_minisat_deletion ( 'z' + 72 )

/* INPUT OPTIONS */
#define OPTIONID_dimacs ( 'z' + 90 )

/* GENERIC OPTIONS */
#define OPTIONID_stdin ( 'z' + 100 )
#define OPTIONID_time_limit ( 'z' + 101 )
    
#ifdef TRACE_ON
TraceLevels Options::traceLevels;
#endif

//DELETION_POLICY Options::deletionPolicy = AGGRESSIVE_DELETION_POLICY;
DELETION_POLICY Options::deletionPolicy = RESTARTS_BASED_DELETION_POLICY;

HEURISTIC_POLICY Options::heuristicPolicy = HEURISTIC_BERKMIN;

vector< const char* > Options::inputFiles;

//unsigned int Options::maxBerkminNumber = UINT_MAX;
unsigned int Options::maxBerkminNumber = 512;

//OUTPUT_POLICY Options::outputPolicy = WASP_OUTPUT;
OUTPUT_POLICY Options::outputPolicy = DIMACS_OUTPUT;

bool Options::printProgram = false;

RESTARTS_POLICY Options::restartsPolicy = SEQUENCE_BASED_RESTARTS_POLICY;

//unsigned int Options::restartsThreshold = 32;
unsigned int Options::restartsThreshold = 100000;

unsigned int Options::timeLimit = 0;

unsigned int Options::maxModels = 1;
    
void
Options::parse(
    int argc,
    char* const* argv )
{
    // It will store the option code.
    int code;
    
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
                { "trace-mc", required_argument, NULL, OPTIONID_trace_checker },
                { "trace-us", required_argument, NULL, OPTIONID_trace_us },
                { "trace-heuristic", required_argument, NULL, OPTIONID_trace_heuristic },
                { "trace-learning", required_argument, NULL, OPTIONID_trace_learning },
                { "trace-enumeration", required_argument, NULL, OPTIONID_trace_enumeration },
                
                /* OUTPUT OPTIONS */
                { "competition-output", no_argument, NULL, OPTIONID_competition_output },
                { "silent", no_argument, NULL, OPTIONID_silent },                
                { "third-competition-output", no_argument, NULL, OPTIONID_third_competition_output },
                { "printprogram", no_argument, NULL, OPTIONID_printprogram },

                /* HEURISTIC OPTIONS */
                { "heuristic-berkmin", optional_argument, NULL, OPTIONID_berkminheuristic },
                { "heuristic-firstundefined", no_argument, NULL, OPTIONID_fuheuristic },
                { "heuristic-minisat", no_argument, NULL, OPTIONID_minisatheuristic },
                
                /* RESTART OPTIONS */                
                { "geometric-restarts", optional_argument, NULL, OPTIONID_geometric_restarts },
                { "minisat-restarts", optional_argument, NULL, OPTIONID_minisat_restarts },
                { "disable-restarts", no_argument, NULL, OPTIONID_disable_restarts },
                { "sequence-based-restarts", optional_argument, NULL, OPTIONID_sequence_based_restarts },
                
                /* DELETION OPTIONS */
                { "aggressive-deletion", no_argument, NULL, OPTIONID_aggressive_deletion },
                { "restarts-based-deletion", no_argument, NULL, OPTIONID_restarts_based_deletion },
                { "minisat-deletion", no_argument, NULL, OPTIONID_minisat_deletion },
                
                /* INPUT OPTIONS */
                { "dimacs", no_argument, NULL, OPTIONID_dimacs },                
                
                /* GENERIC OPTIONS*/
                { "help", no_argument, NULL, OPTIONID_help },
                { "stdin", no_argument, NULL, OPTIONID_stdin },
                { "time-limit", required_argument, NULL, OPTIONID_time_limit },

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

            case OPTIONID_berkminheuristic:
                if( optarg )
                {
                    maxBerkminNumber = atoi( optarg );
                    if( maxBerkminNumber == 0 )
                        maxBerkminNumber = UINT_MAX;
                }
                heuristicPolicy = HEURISTIC_BERKMIN;
                break;
            
            case OPTIONID_fuheuristic:
                heuristicPolicy = HEURISTIC_FIRST_UNDEFINED;
                break;
            
            case OPTIONID_minisatheuristic:
                heuristicPolicy = HEURISTIC_MINISAT;
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
    waspFacade.setDeletionPolicy( deletionPolicy );
    waspFacade.setDecisionPolicy( heuristicPolicy, maxBerkminNumber );
    waspFacade.setOutputPolicy( outputPolicy );
    waspFacade.setRestartsPolicy( restartsPolicy, restartsThreshold );
    waspFacade.setMaxModels( maxModels );
    waspFacade.setPrintProgram( printProgram );
}

};
