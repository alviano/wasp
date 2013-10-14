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
 * File:   AbstractBuilder.h
 * Author: Carmine Dodaro
 *
 * Created on 23 July 2013, 13.34
 */

#ifndef ABSTRACTBUILDER_H
#define	ABSTRACTBUILDER_H

#include <string>
using namespace std;

class AuxLiteral;
class Clause;
class WaspRule;

class AbstractBuilder
{
    public:
        virtual void newVar() = 0;
        virtual void newVar( const string& name ) = 0;
        
        virtual Clause* startClause() = 0;
        virtual Clause* startClause( unsigned int size ) = 0;        
        virtual void addLiteralInClause( int lit, Clause* clause ) = 0;
        virtual void addAuxLiteralInClause( AuxLiteral*, Clause* clause ) = 0;
        virtual void endClause( Clause* clause ) = 0;        
        
        virtual WaspRule* startWaspRule( unsigned int size ) = 0;
        virtual void addLiteralInWaspRule( int lit, WaspRule* waspRule ) = 0;
        virtual AuxLiteral* endWaspRule( WaspRule* waspRule ) = 0;
        
        virtual void addSupportingLiteralForHeadAtom( unsigned int variable, int literal ) = 0;
        virtual void addSupportingAuxLiteralForHeadAtom( unsigned int variable, AuxLiteral* auxLiteral ) = 0;
        virtual void addSupportedVariable( unsigned int variable ) = 0;
        
        virtual void addTrueLiteral( int lit ) = 0;        
        
        virtual void startBuilding() = 0;
        virtual void endBuilding() = 0;
        
        virtual void setAtomName( unsigned int variable, string& name ) = 0;
};

#endif	/* ABSTRACTBUILDER_H */

