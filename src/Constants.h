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
//enum TruthValue { UNDEFINED = 0, FALSE = 1, TRUE = 2 };
#define UNDEFINED 0
#define FALSE 1
#define TRUE 2

#define AGGRESSIVE_DELETION_POLICY 0
#define RESTARTS_BASED_DELETION_POLICY 1
#define MINISAT_DELETION_POLICY 2

#define HEURISTIC_BERKMIN 0
#define HEURISTIC_FIRST_UNDEFINED 1

#define WASP_OUTPUT 0
#define COMPETITION_OUTPUT 1
#define DIMACS_OUTPUT 2
#define SILENT_OUTPUT 3
#define THIRD_COMPETITION_OUTPUT 4

#define SEQUENCE_BASED_RESTARTS_POLICY 0
#define GEOMETRIC_RESTARTS_POLICY 1

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
#define ERRORGENERIC WASP_STRING "\nGeneric error"
#define ERRORPARSINGCODE 100
#define ERRORGENERICCODE 110

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
 * Gringo Numeric Format
 */
#define GRINGO_NORMAL_RULE_ID 1
#define GRINGO_CONSTRAINT_RULE_ID 2
#define GRINGO_CHOICE_RULE_ID 3
#define GRINGO_DISJUNCTIVE_RULE_ID 8
#define GRINGO_LINE_SEPARATOR 0
#define GRINGO_BPLUS "B+"
#define GRINGO_BMINUS "B-"

/*
 * New types
 */
typedef double Activity;
typedef unsigned int BERKMIN_HEURISTIC_COUNTER;
typedef unsigned int TruthValue;
typedef unsigned int DELETION_POLICY;
typedef unsigned int HEURISTIC_POLICY;
typedef unsigned int OUTPUT_POLICY;
typedef unsigned int RESTARTS_POLICY;

#endif	/* CONSTANTS_H */

