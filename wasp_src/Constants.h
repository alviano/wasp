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
 * File:   Constants.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 17.29
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <climits>
//#define UNDEFINED 1
//#define TRUE 2
//#define FALSE 4
//#define TRUE_UNDEFINED 3
//#define FALSE_UNDEFINED 5
//#define INTERPRETATION_MASK 7

enum TruthValue { UNDEFINED = 1, TRUE = 2, FALSE = 4 };

/*
 * Wasp constants
 */
#define WASP_STRING "WASP 2.0\n"
#define NOMODEL "INCOHERENT"
#define NOMODEL_COMPETITION_OUTPUT "INCONSISTENT"
#define ANSWER "ANSWER"
#define ANSWER_THIRD_COMPETITION "ANSWER SET FOUND"
#define WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR "@"
#define COST "COST"
#define OPTIMUM "OPTIMUM"
#define QUERY_FALSE_OUTPUT "no."
#define QUERY_TRUE_OUTPUT "yes."
#define MAXUNSIGNEDINT UINT_MAX

/* 
 * Error messages
 */
#define ERRORPARSING WASP_STRING "\nError during parsing"
#define ERRORPARSINGCODE 100

/*
 * DIMACS Format
 */
#define COMMENT_DIMACS 'c'
#define FORMULA_INFO_DIMACS 'p'
#define SOLUTION_DIMACS 's'
#define VALUE_DIMACS 'v'
#define UNSAT "UNSATISFIABLE"
#define SAT "SATISFIABLE"


/*
 * New types
 */
typedef double Activity;
typedef unsigned int BERKMIN_HEURISTIC_COUNTER;


#endif	/* CONSTANTS_H */

