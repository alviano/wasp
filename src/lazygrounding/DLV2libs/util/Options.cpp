/*
 *
 *  Copyright 2014 Mario Alviano, Carmine Dodaro, Francesco Ricca and
 *                 Pierfrancesco Veltri.
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

#include <getopt.h>
#include <cstdlib>

namespace DLV2 
{
// Options with a short alias.
#define OPTIONID_help 'h'

// "Only-long" options with their code.

/* INPUT OPTIONS */
#define OPTIONID_aspcore2strict ( 'z' + 1 )
#define OPTIONID_inmemory ( 'z' + 2 )
#define OPTIONID_dlvdb ( 'z' + 3 )
#define OPTIONID_testparser ( 'z' + 4 )

/* OUTPUT OPTIONS */
#define OPTIONID_silent ( 'z' + 10 )
#define OPTIONID_printprogram ( 'z' + 11 )
#define OPTIONID_selector ( 'z' + 12 )
#define OPTIONID_printdepgraph ( 'z' + 13 )
#define OPTIONID_printstatistics ( 'z' + 14 )

/* GENERIC OPTIONS */
#define OPTIONID_stdin ( 'z' + 20 )

/* DB OPTIONS */
#define OPTIONID_datasource ('z' + 30 )
#define OPTIONID_username ('z' + 31 )
#define OPTIONID_password ('z' + 32 )
};

using namespace std; 
using namespace DLV2;
    
// Initialize singleton
Options* Options::instance = NULL;

Options* 
Options::globalOptions() 
{
    if( instance == NULL )
    {
        instance = new Options();
    }
    return instance;
}

void
Options::finalizeGlobalOptions() 
{
    if( instance != NULL )
    {
        delete instance;
        instance = NULL;
    }
}


Options::Options(): 
        aspCore2Strict(false), 
        printProgram(false),
        printDepGraph(false),
        printStatistics(false),
        inputPolicy(BUILDER_IN_MEMORY),
        outputPolicy(OUTPUT_ASPCORE2) 
{

}

Options::Options(
    const Options& o):
        aspCore2Strict(o.aspCore2Strict),
        printProgram(o.printProgram),
        printDepGraph(o.printDepGraph),
        printStatistics(o.printStatistics),
        inputPolicy(o.inputPolicy),
        outputPolicy(o.outputPolicy) 
{

}

void
Options::init(
    int argc,
    char* const* argv )
{
    // It will store the option code.
    int code;
    
    // A string listing valid short options letters. 
    const char* const shortOptions = "h";
    
    // An array describing valid long options. 
    const struct option longOptions[] = 
    {    
        /* INPUT OPTIONS */
        { "aspcore2strict", no_argument, NULL, OPTIONID_aspcore2strict },
        { "inmemory", no_argument, NULL, OPTIONID_inmemory },
        { "dlvdb", no_argument, NULL, OPTIONID_dlvdb },
        { "test-parser", no_argument, NULL, OPTIONID_testparser },

        /* OUTPUT OPTIONS */
        { "silent", no_argument, NULL, OPTIONID_silent },
        { "printprogram", no_argument, NULL, OPTIONID_printprogram },
        { "selector", required_argument, NULL, OPTIONID_selector },
        { "printdepgraph", no_argument, NULL, OPTIONID_printdepgraph },
        { "stats", no_argument, NULL, OPTIONID_printstatistics },

        /* GENERIC OPTIONS */
        { "help", no_argument, NULL, OPTIONID_help },
        { "stdin", no_argument, NULL, OPTIONID_stdin },
        
        /* DB OPTIONS */
        { "db-source", required_argument, NULL, OPTIONID_datasource },
        { "db-user", required_argument, NULL, OPTIONID_username },
        { "db-pwd", required_argument, NULL, OPTIONID_password },

        // Required at end of array. 
        { NULL, 0, NULL, 0 }
    };
  
    do{
        // The function getopt_long() stores the option index here.
        int optionIndex = 0;
        
        code = getopt_long (argc, argv, shortOptions, longOptions, &optionIndex);
        
        switch (code)
        {
            case -1:
                // All the lines were parsed
                break;
            case OPTIONID_aspcore2strict:
                aspCore2Strict = true;
                break;
            
            case OPTIONID_inmemory:
                inputPolicy = BUILDER_IN_MEMORY;
                break;
                
            case OPTIONID_dlvdb:     
                inputPolicy = BUILDER_DLV_DB;
                break;
                
            case OPTIONID_testparser:
                inputPolicy = BUILDER_EMPTY;
                printStatistics = true;
                break;
                
            case OPTIONID_silent:
                outputPolicy = OUTPUT_SILENT;
                break;
                
            case OPTIONID_printprogram:
                inputPolicy = BUILDER_MOCK_OBJECTS;
                printProgram = true;
                break;
                
            case OPTIONID_selector:
                inputPolicy = BUILDER_SELECTOR;
                printProgram = true;
                namedpipe = atoi( optarg );
                break;
                
            case OPTIONID_printdepgraph:
                inputPolicy = BUILDER_DEPGRAPH;
                printDepGraph = true;
                break;

            case OPTIONID_printstatistics:
                printStatistics = true;
                break;
                
            case OPTIONID_help:
                Help::printHelp(argv[0]);
                exit(0);
                break;
                
            case OPTIONID_stdin:
                inputFiles.push_back("--");
                break;
            
            case OPTIONID_datasource:
                dbDataSource.append(optarg);
                break;
                
            case OPTIONID_username:
                dbUsername.append(optarg);
                break;
                
            case OPTIONID_password:
                dbPassword.append(optarg);
                break;
                
            default:
                ErrorMessage::errorGeneric( "This option is not supported." );
                break;
        }
    }while( code != -1 );
    
    // Manage the reminder options (probably, they are all input files).
    for( int i = optind; i < argc; i++ )
    {
        inputFiles.push_back( argv[ i ] );
    }
}