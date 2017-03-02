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

#ifndef WASP_LAZYINSTANTIATOR_H
#define WASP_LAZYINSTANTIATOR_H

#include "ExternalPropagator.h"

#include <string>
#include "../Literal.h"
#include "../util/WaspErrorMessage.h"
#include <iostream>
using namespace std;
class LazyConstraint;
class Clause;
class Solver;

class LazyInstantiator : public ExternalPropagator
{
    public:
        LazyInstantiator( const string& filename, Solver& solver );
        virtual ~LazyInstantiator();
        
        virtual void endParsing( Solver& );
        bool checkAnswerSet( Solver& solver );
        Clause* getReasonForCheckFailure( Solver& solver );

        //override of useless methods.
        virtual bool onLiteralFalse( Solver&, Literal, int ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); return false; }
        virtual void reset( const Solver& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); }
        virtual void simplifyAtLevelZero( Solver& ) {}        
        virtual void addedVarName( Var, const string& ) {}
        virtual void onAtomElimination( Var ) {}
        virtual void onAnswerSet( const Solver& ) {}
        virtual void foundLowerBound( uint64_t ) {}
        virtual void foundUpperBound( uint64_t ) {}
        virtual bool checkPartialInterpretation( Solver& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); return true; }
        virtual void endUnitPropagation( Solver& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); }
        virtual void endPropagation( Solver& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); }
        virtual void onStartingSolver() {}
        
        virtual bool hasWeakConstraintsToAdd( const Solver& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); return false; }
        virtual uint64_t addWeakConstraints( const Solver&, vector< Clause* >&, vector< uint64_t >& ) { WaspErrorMessage::errorGeneric("Lazy instantiator used as propagator"); return 0; }
        virtual bool isForLazyWeakConstraints() const { return false; }
        
    private:
        inline LazyInstantiator( const LazyInstantiator& orig );
        LazyConstraint* constraint;
};

#endif

