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

/* 
 * File:   GringoNumericFormat.h
 * Author: Carmine Dodaro
 *
 * Created on 9 September 2013, 11.36
 */


#ifndef _GRINGONUMERICFORMAT_H
#define	_GRINGONUMERICFORMAT_H

#include "../Solver.h"

// TODO: the program has to be built in this class! No need of a separate builder. Revise this class.
class GringoNumericFormat
{
    public:

        inline GringoNumericFormat( Solver& s );
        ~GringoNumericFormat();

        /**
        * This function read instruction from standard input and
        * build the program.
        */
        void parse();

        /**
        * This function read instruction from input and
        * build the program.
        *
        * @param input The istream input.
        */
        void parse( istream& input );

    private:    

        void readAtomsTable( istream& input );
        void readNormalRule( istream& input );
        void readDisjunctiveRule( istream& input );
        void readChoiceRule( istream& input );
        void readConstraintRule( istream& input );

        void readTrueAtoms( istream& input );
        void readFalseAtoms( istream& input );

        void readErrorNumber( istream& input );

        void postprocessing();

        void addNewVariable( unsigned int variable );

        Solver& solver;

        unsigned int max;              
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), max( 1 )
{
}

#endif	/* _GRINGONUMERICFORMAT_H */
