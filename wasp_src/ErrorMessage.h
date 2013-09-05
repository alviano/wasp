/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   ErrorMessage.h
 * Author: Carmine Dodaro
 *
 * Created on 05 September 2013, 10.26
 */

#ifndef ERRORMESSAGE_H
#define	ERRORMESSAGE_H

#include <string>
using namespace std;

#include "Constants.h"

class ErrorMessage
{
    public:
        static void errorDuringParsing( const string& errorMessage )
        {
            errorDuringParsing( errorMessage.c_str() );
        }
        
        static void errorDuringParsing( const char* errorMessage )
        {
            cerr << ERRORPARSING << ": " << errorMessage << endl;
            exit( ERRORPARSINGCODE );
        }
};

#endif	/* ERRORMESSAGE_H */

