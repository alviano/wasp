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

#ifndef ASPPROGRAMBUILDER_H
#define	ASPPROGRAMBUILDER_H

#include "../stl/Trie.h"
#include "SATFormulaBuilder.h"

#include <unordered_set>
#include <unordered_map>
using namespace std;

class AuxLiteral;
class Solver;
class WaspRule;

// TODO: move what is useful in GringoNumericFormat. Then, remove this file
/*
class ASPProgramBuilder : public SATFormulaBuilder
{
    public:        
        ASPProgramBuilder( Solver* s );
        inline virtual ~ASPProgramBuilder() {}
        
        virtual WaspRule* startWaspRule( unsigned int size );
        virtual void addLiteralInWaspRule( int lit, WaspRule* waspRule );
        virtual void addAuxLiteralInClause( AuxLiteral* auxLiteral, Clause* clause );
        virtual AuxLiteral* endWaspRule( WaspRule* waspRule );

        virtual void startBuilding();
        virtual void endBuilding();
        
        virtual void addSupportingLiteralForHeadAtom( unsigned int variable, int literal );
        virtual void addSupportingAuxLiteralForHeadAtom( unsigned int variable, AuxLiteral* auxLiteral );
        virtual void addSupportedVariable( unsigned int variable );
        
        virtual void setAtomName( unsigned int variable, string& name );

    private:
        virtual AuxLiteral* newAuxVar();
        virtual void setAuxLiteralInWaspRule( AuxLiteral* auxLiteral, WaspRule* waspRule );
        
        Trie trie;
        unordered_map< unsigned int, vector< Literal* > > mapForSupport;
        unordered_set< unsigned int > supportedVariables;
};
*/

#endif	/* ASPPROGRAMBUILDER_H */

