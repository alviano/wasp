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
 * File:   SATFormulaBuilder.h
 * Author: Carmine Dodaro
 *
 * Created on 23 July 2013, 13.39
 */

// TODO: useless. Remove!

//#ifndef SATFORMULABUILDER_H
//#define	SATFORMULABUILDER_H
//
//#include "AbstractBuilder.h"
//#include "../stl/Trie.h"
//
//class Solver;
//class Literal;
//class AuxLiteral;
//class WaspRule;
//
//class SATFormulaBuilder : public AbstractBuilder
//{
//    public:        
//        SATFormulaBuilder( Solver* s );
//        inline virtual ~SATFormulaBuilder() {}
//        virtual void newVar();
//        virtual void newVar( const string& name );
//
//        virtual Clause* startClause();
//        virtual Clause* startClause( unsigned int size );
//        virtual void addLiteralInClause( int literal, Clause* clause );
//        virtual void addAuxLiteralInClause( AuxLiteral* auxLiteral, Clause* clause );
//        virtual void endClause( Clause* clause );
//
//        virtual WaspRule* startWaspRule( unsigned int size );
//        virtual void addLiteralInWaspRule( int lit, WaspRule* waspRule );
//        virtual AuxLiteral* endWaspRule( WaspRule* waspRule );
//
//        virtual void addTrueLiteral( int lit );
//
//        virtual void startBuilding();
//        virtual void endBuilding();
//        
//        virtual void addSupportingLiteralForHeadAtom( unsigned int variable, int literal );
//        virtual void addSupportingAuxLiteralForHeadAtom( unsigned int variable, AuxLiteral* auxLiteral );
//        virtual void addSupportedVariable( unsigned int variable );
//        
//        virtual void setAtomName( unsigned int variable, string& name );
//
//    protected:
//        Solver* solver;
//        virtual void addLiteralInClause( Literal literal, Clause* clause );        
//};
//
//#endif	/* SATFORMULABUILDER_H */

