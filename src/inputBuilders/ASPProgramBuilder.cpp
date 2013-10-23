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

#include "ASPProgramBuilder.h"
#include "../Clause.h"
#include "../Literal.h"
#include "../Solver.h"
#include "../WaspRule.h"

#include <iostream>
#include <sstream>
using namespace std;

// TODO: remove
//ASPProgramBuilder::ASPProgramBuilder(
//    Solver* s ) : SATFormulaBuilder( s )
//{
//}
//
//AuxLiteral*
//ASPProgramBuilder::newAuxVar()
//{
//    #ifdef NDEBUG    
////        return solver->addAuxVariable();
//    #else
////        AuxLiteral* tmp = solver->addAuxVariable();
////        stringstream s;
////        s << "#aux" << solver->numberOfAuxVariables();
////        string n = s.str();
////        tmp->setName( n );
////        return tmp;
//    #endif
//
//    assert( 0 );
//    return NULL;
//}
//
//WaspRule*
//ASPProgramBuilder::startWaspRule(
//    unsigned int size )
//{
//    trie.startInsertion();
//    return new WaspRule( size );
//}
//
//void
//ASPProgramBuilder::addLiteralInWaspRule(
//    int lit,
//    WaspRule* waspRule )
//{
//    trie.addElement( -lit );
////    Literal* literal = solver->getLiteral( lit );
////    waspRule->addLiteral( literal );
//}
//
//void
//ASPProgramBuilder::setAuxLiteralInWaspRule(
//    AuxLiteral* auxLiteral,
//    WaspRule* waspRule )
//{    
////    waspRule->addLiteral( auxLiteral );
////    waspRule->setAuxLiteral( auxLiteral );
////    auxLiteral->setWaspRule( waspRule );
//}
//
//AuxLiteral*
//ASPProgramBuilder::endWaspRule(
//    WaspRule* waspRule )
//{
////    unsigned int id = trie.endInsertion();
////    if( solver->existsAuxLiteral( id ) )
////    {
////        delete waspRule;
////        return solver->getAuxLiteral( id );
////    }
////    else
////    {
////        AuxLiteral* auxLiteral = this->newAuxVar();
////        this->setAuxLiteralInWaspRule( auxLiteral, waspRule );
////
////        solver->addClause( waspRule );
////        waspRule->attachClause();
////        waspRule->attachWaspRule();
////
////        return auxLiteral;
////    }
//    assert( 0 );
//    return NULL;
//}
//
//void
//ASPProgramBuilder::startBuilding()
//{
//    newVar();
//}
//
//void
//ASPProgramBuilder::endBuilding()
//{
//    /*
//     * Modelling support for all atoms.
//     * 
//     */
//    unsigned int numberOfVariables = solver->numberOfVariables();
//    
//    for( unsigned int i = 2; i < numberOfVariables; i++ )
//    {
//        if( supportedVariables.find( i ) != supportedVariables.end() )
//            continue;
//        
//        vector< Literal* >& literalsToAdd = mapForSupport[ i ];
//        unsigned int size = literalsToAdd.size();
//        
//        if( size > 0 )
//        {
//            Clause* clause = this->startClause( size + 1 );
//            this->addLiteralInClause( -i, clause );
//
//            for( unsigned int j = 0; j < size; j++ )
//            {
////                clause->addLiteral( literalsToAdd[ j ] );
//            }
//
//            this->endClause( clause );
//        }
//        else
//        {
//            this->addTrueLiteral( -i );
//        }
//    }
//}
//
//void
//ASPProgramBuilder::addSupportingLiteralForHeadAtom( 
//    unsigned int variable,
//    int lit )
//{
////    Literal* literal = solver->getLiteral( lit );
////    mapForSupport[ variable ].push_back( literal );
//}
//        
//void
//ASPProgramBuilder::addSupportingAuxLiteralForHeadAtom( 
//    unsigned int variable,
//    AuxLiteral* auxLiteral )
//{
////    mapForSupport[ variable ].push_back( auxLiteral );
//}
//
//void
//ASPProgramBuilder::setAtomName(
//    unsigned int variable,
//    string& name )
//{
////    Variable* literal = static_cast< Variable* >( solver->getVariable( variable ) );
////    literal->setName( name );
//}
//
//void
//ASPProgramBuilder::addAuxLiteralInClause(
//    AuxLiteral* auxLiteral,
//    Clause* clause )
//{
////    clause->addLiteral( auxLiteral->getOppositeLiteral() );
//}
//
//void
//ASPProgramBuilder::addSupportedVariable(
//    unsigned int variable )
//{
//    supportedVariables.insert( variable );
//}