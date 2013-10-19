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

// TODO: useless. Remove!

//#include "SATFormulaBuilder.h"
//#include "../Clause.h"
//#include "../Literal.h"
//#include "../Solver.h"
//#include "../WaspRule.h"
//#include "../ErrorMessage.h"
//
//#include <iostream>
//using namespace std;
//
//SATFormulaBuilder::SATFormulaBuilder(
//    Solver* s ) : solver( s )
//{
//}
//
//void
//SATFormulaBuilder::newVar()
//{
//    solver->addVariable();
//}
//
//void
//SATFormulaBuilder::newVar( 
//    const string& name )
//{
//    solver->addVariable( name );
//}
//
//Clause* 
//SATFormulaBuilder::startClause()
//{
//    return new Clause();
//}
//
//Clause* 
//SATFormulaBuilder::startClause( 
//    unsigned int size )
//{
//    return new Clause( size );
//}
//
//void 
//SATFormulaBuilder::addLiteralInClause( 
//    int lit, 
//    Clause* clause )
//{
//    Literal literal = solver->getLiteral( lit );
//    addLiteralInClause( literal, clause );
//}
//
//void 
//SATFormulaBuilder::addLiteralInClause( 
//    Literal literal,
//    Clause* clause )
//{
//    assert( literal != NULL );
//    clause->addLiteral( literal );
//}
//
//void
//SATFormulaBuilder::addAuxLiteralInClause(
//    AuxLiteral*,
//    Clause* )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}
//
//void 
//SATFormulaBuilder::endClause(
//    Clause* clause )
//{
//    assert( "Clauses must contain at least two literals." && clause->size() > 1 );
//    solver->addClause( clause );
//    clause->attachClause();
//}
//
//WaspRule*
//SATFormulaBuilder::startWaspRule(
//    unsigned int )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//    return NULL;
//}
//
//void
//SATFormulaBuilder::addSupportedVariable(
//    unsigned int )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}
//
//void
//SATFormulaBuilder::addLiteralInWaspRule(
//    int,
//    WaspRule* )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}
//
//AuxLiteral*
//SATFormulaBuilder::endWaspRule(
//    WaspRule* )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//    return NULL;
//}
//
//void 
//SATFormulaBuilder::addTrueLiteral(
//    int lit )
//{
//    solver->addTrueLiteral( lit );
//}
//
//void
//SATFormulaBuilder::startBuilding()
//{
//}
//
//void
//SATFormulaBuilder::endBuilding()
//{
//}
//
//void
//SATFormulaBuilder::addSupportingLiteralForHeadAtom( 
//    unsigned int,
//    int )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}
//        
//void
//SATFormulaBuilder::addSupportingAuxLiteralForHeadAtom( 
//    unsigned int,
//    AuxLiteral* )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}
//
//void
//SATFormulaBuilder::setAtomName(
//    unsigned int,
//    string& )
//{
//    ErrorMessage::errorGeneric( "This method should be invoked only by the ASPSolver. Maybe you are using a wrong builder.");
//}