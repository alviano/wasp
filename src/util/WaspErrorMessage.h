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

#ifndef WASP_ERRORMESSAGE_H
#define WASP_ERRORMESSAGE_H

#include <string>
#include <cstdlib>
using namespace std;

#include "WaspConstants.h"

class WaspErrorMessage
{
    public:
        static void errorDuringParsing( const string& errorMessage )
        {
            errorDuringParsing( errorMessage.c_str() );
        }
        
        static void errorDuringParsing( const char* errorMessage )
        {
            printMessageAndExit( errorMessage, ERRORPARSING, ERRORPARSINGCODE );
        }
        
        static void errorGeneric( const string& errorMessage )
        {
            errorGeneric( errorMessage.c_str() );
        }
        
        static void errorGeneric( const char* errorMessage )
        {
            printMessageAndExit( errorMessage, ERRORGENERIC, ERRORGENERICCODE );            
        }
        
    private:
        static void printMessageAndExit( const char* errorMessage, const char* errorType, unsigned int errorExitCode )
        {
            fprintf( stderr, "%s: %s\n", errorType, errorMessage );
            exit( errorExitCode );
        }
};

#endif

