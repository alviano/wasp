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
#ifndef WASP_DISJUNCTIONPROPAGATOR_H
#define	WASP_DISJUNCTIONPROPAGATOR_H

#include "Propagator.h"
#include "../util/WaspAssert.h"
#include "../util/WaspOptions.h"
#include "../util/WaspTrace.h"
#include "../stl/Vector.h"
#include "../Literal.h"
#include "../ReasonForBinaryClauses.h"
#include "../Clause.h"

class DisjunctionPropagator : public Propagator
{
    public:
        inline DisjunctionPropagator( Literal bodyLiteral );        
        virtual ~DisjunctionPropagator();
        
        virtual bool onLiteralFalse( Solver& solver, Literal literal, PropagatorData p );        
        virtual void reset( const Solver& ) { assert( 0 ); }        
        virtual void simplifyAtLevelZero( Solver& ) {}        

        inline void addOriginalAndAuxLiterals( Literal origLit, Literal auxLit );
        void finalize( Solver& solver );        

    private:
        inline DisjunctionPropagator( const DisjunctionPropagator& orig );
        
        void origIsTrue( Solver& solver, Literal literal, unsigned int pos );
        void auxIsTrue( Solver& solver, Literal literal, unsigned int pos );
        
        void inferLiteral( Solver& solver, Literal lit, Clause* reason );
        
        inline Clause* createEmptyBinaryClause() const;
        
        //The first position is the body literal
        Vector< Literal > origLiterals;
        
        //The first position is Literal::null
        Vector< Literal > auxLiterals;
        unsigned int startPos;
        
        Clause* firstReason;
        Clause* secondReason;
        
        unsigned int firstPos;

        Clause* createReason( Clause* clauseReason, Literal reason );                
};

DisjunctionPropagator::DisjunctionPropagator(
    Literal bodyLiteral ) : Propagator()
{
    trace_msg( disjunction, 1, "Creating propagator, body literal is " << bodyLiteral );
    startPos = ( bodyLiteral != Literal::null ) ? 0 : 1;
    origLiterals.push_back( ( bodyLiteral != Literal::null ) ? bodyLiteral.getOppositeLiteral() : Literal::null );
    auxLiterals.push_back( Literal::null );
    firstReason = createEmptyBinaryClause();
    secondReason = createEmptyBinaryClause();    
}

void
DisjunctionPropagator::addOriginalAndAuxLiterals(
    Literal origLit,
    Literal auxLit )
{
    trace_msg( disjunction, 2, "Adding orig literal " << origLit << " and aux literal " << auxLit );
    origLiterals.push_back( origLit );
    auxLiterals.push_back( auxLit );
}

Clause*
DisjunctionPropagator::createEmptyBinaryClause() const
{
    Clause* clause = new Clause( 2 );
    clause->addLiteral( Literal( 1, NEGATIVE ) );
    clause->addLiteral( Literal::null );
    return clause;
}

#endif

