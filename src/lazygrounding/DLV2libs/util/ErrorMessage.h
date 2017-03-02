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

#ifndef ERRORMESSAGE_H
#define	ERRORMESSAGE_H

#include <string>
#include <iostream>
#include <cstdlib>

#include "Constants.h"

namespace DLV2
{

    class ErrorMessage
    {
        public:
            static void errorDuringParsing( const std::string& errorMessage )
            {
                errorDuringParsing( errorMessage.c_str() );
            }

            static void errorDuringParsing( const char* errorMessage )
            {
                std::cerr << ERROR_PARSING << ": " << errorMessage << std::endl;
                exit( ERROR_PARSING_CODE );
            }

            static void errorGeneric( const std::string& errorMessage )
            {
                errorGeneric( errorMessage.c_str() );
            }

            static void errorGeneric( const char* errorMessage )
            {
                std::cerr << ERROR_GENERIC << ": " << errorMessage << std::endl;
                exit( ERROR_GENERIC_CODE );
            }
            
            static void errorDBConnection( const std::string& errorMessage )
            {
                errorGeneric( errorMessage.c_str() );
            }

            static void errorDBConnection( const char* errorMessage )
            {
                std::cerr << ERROR_DB << ": " << errorMessage << std::endl;
                exit( ERROR_DB_CODE );
            }
            
            static void errorDBConnection( unsigned char* errorMessage )
            {
                std::cerr << ERROR_DB << ": " << errorMessage << std::endl;
                exit( ERROR_DB_CODE );
            }
    };

};

#endif	/* ERRORMESSAGE_H */
