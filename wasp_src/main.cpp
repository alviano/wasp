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
 * File:   main.cpp
 * Author: Carmine Dodaro
 *
 * Created on 23 July 2013, 17.50
 */

#include <cstdlib>

#include "input/Dimacs.h"
#include "inputBuilders/SATFormulaBuilder.h"
#include "solvers/Solver.h"
#include "solvers/SATSolver.h"
#include "solvers/ASPSolver.h"
#include "inputBuilders/ASPProgramBuilder.h"
#include "input/GringoNumericFormat.h"
using namespace std;

int EXIT_CODE = 0;

/*
 * 
 */
int main( int argc, char** argv )
{    
//    SATSolver solver;
//    
//    SATFormulaBuilder* satFormulaBuilder = new SATFormulaBuilder( &solver );
//    
//    Dimacs dimacs( satFormulaBuilder );
//    dimacs.parse();
//    
//    solver.solve();    
//    
//    delete satFormulaBuilder;
    
    ASPSolver solver;
    ASPProgramBuilder* aspProgramBuilder = new ASPProgramBuilder( &solver );
    
    GringoNumericFormat gringoNumericFormat( aspProgramBuilder );
    gringoNumericFormat.parse();
    
    solver.solve();
    
    delete aspProgramBuilder;
    
    return EXIT_CODE;
}

