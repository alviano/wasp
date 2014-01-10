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

#ifndef INPUTFACADE_H
#define	INPUTFACADE_H

#include <iostream>
using namespace std;

#include "../util/Constants.h"
#include "../util/ErrorMessage.h"
#include "Dimacs.h"
#include "GringoNumericFormat.h"

#define ISSAT 0
#define ISASP 1

class InputFacade
{
    public:

        inline InputFacade( Solver& solver );

        /**
        * This function read instruction from standard input and
        * build the program.
        */
        inline void parse();

//        /**
//        * This function read instruction from input and
//        * build the program.
//        *
//        * @param input The istream input.
//        */
//        inline void parse( istream& input );

        inline bool isInstanceOfSAT(){ return type == ISSAT; }

        inline bool isInstanceOfASP(){ return type == ISASP; }

    private:
        Solver& solver;
        unsigned int type;
};

InputFacade::InputFacade(
    Solver& s ) : solver( s )
{
}

void
InputFacade::parse()
{
    char tmp;
    cin >> tmp;

    cin.putback( tmp );
    switch ( tmp )
    {
        case COMMENT_DIMACS:
        {
            Dimacs dimacs( solver );
            dimacs.parse();
            type = ISSAT;
            break;
        }

        case FORMULA_INFO_DIMACS:
        {
            Dimacs dimacs( solver );
            dimacs.parse();
            type = ISSAT;
            break;
        }

        default:
        {
            GringoNumericFormat gringo( solver );
            gringo.parse( cin );
            type = ISASP;
            break;
        }
    }

    if( !cin.good() && !cin.eof() )
    {   
        ErrorMessage::errorDuringParsing( "Unexpected symbol." );
    }    
}


#endif	/* INPUTFACADE_H */

