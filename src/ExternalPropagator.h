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

#ifndef EXTERNALPROPAGATOR_H
#define EXTERNALPROPAGATOR_H

#include "Propagator.h"
#include "./stl/Vector.h"
class Clause;
class Interpreter;

#define method_plugins_addedVarName "addedVarName"
#define method_plugins_onAtomElimination "onAtomElimination" 
#define method_plugins_getLiterals "getLiterals"
#define method_plugins_getAtomsToFreeze "getAtomsToFreeze"
#define method_onStartingSolver "onStartingSolver"
#define method_plugins_onLiteralTrue "onLiteralTrue"
#define method_plugins_onLiteralsUndefined "onLiteralsUndefined"
#define method_plugins_onLitAtLevelZero "onLitAtLevelZero"
#define method_plugins_getReason "getReason"
#define method_plugins_isProgramIncoherent "isProgramIncoherent"

class ExternalPropagator : public Propagator
{
    public:
        ExternalPropagator( const string& filename, unsigned int interpr );
        virtual ~ExternalPropagator();
        
        virtual bool onLiteralFalse( Solver& solver, Literal literal, int pos );
        virtual void reset( const Solver& solver );
        
        void addedVarName( Var var, const string& name );
        void onAtomElimination( Var var );
        void endParsing( Solver& solver ) { attachWatches( solver ); }
        void onLitAtLevelZero( Literal lit );
        bool isProgramIncoherent();
        
    private:
        inline ExternalPropagator( const ExternalPropagator& orig );
        void checkWellFormed();
        Clause* getReason( Solver& solver ) const;
        void clearClausesToDelete();        
        void attachWatches( Solver& solver );
        
        Vector< Literal > trail;
        Interpreter* interpreter;
        Vector< Clause* > clausesToDelete;
        char* fn;
        bool check_addedVarName;
        bool check_onAtomElimination;
        bool check_onLitAtLevelZero;
};

#endif

