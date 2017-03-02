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

#include "LazyInstantiator.h"

#include "../Solver.h"
#ifdef WASP_LAZY_GROUNDING_ON
#include "../lazygrounding/LazyConstraint.h"
#include "../lazygrounding/LazyConstraintImpl.h"
#include "../util/VariableNames.h"
#include <fstream>
using namespace std;

LazyInstantiator::LazyInstantiator(
    const string& filename,
    Solver& solver ) : ExternalPropagator()
{
    constraint = new LazyConstraintImpl();
    ifstream f(filename.c_str());
    if( !f.good() )
        WaspErrorMessage::errorGeneric( "Lazy grounder cannot find file " + filename );
    
    constraint->setFilename( filename );
    solver.addPropagatorAttachedToCheckAnswerSet( this );    
}

LazyInstantiator::~LazyInstantiator()
{
    delete constraint;
}

bool
LazyInstantiator::checkAnswerSet(
    Solver& solver )
{    
    bool retValue = true;
    vector< unsigned int > answerSet;
    answerSet.push_back( 0 );
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        assert( !solver.isUndefined( i ) );
        if( solver.isTrue( i ) )
            answerSet.push_back( 1 );
        else
            answerSet.push_back( 0 );        
    }
    retValue = constraint->onAnswerSet( answerSet );
    return retValue;
}

Clause*
LazyInstantiator::getReasonForCheckFailure(
    Solver& solver )
{
    vector< vector< int > > failedConstraints;
    constraint->onCheckFail( failedConstraints );
    
    vector< Clause* > clauses;
    for( unsigned int i = 0; i < failedConstraints.size(); i++ )
    {
        vector< int >& constraint = failedConstraints[ i ];
        Clause* clause = new Clause();
        for( unsigned int j = 0; j < constraint.size(); j++)
        {
            Literal l = Literal::createLiteralFromInt( -constraint[ j ] );
            if( solver.isUndefined( l ) )
                WaspErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is undefined." );
            if( solver.isTrue( l ) )
                WaspErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is true." );
            if( solver.getDecisionLevel( l ) > 0 )
                clause->addLiteral( l );
        }
        clause->setLearned();        
        if( solver.glucoseHeuristic() && clause->size() > 2 )
            clause->setLbd( solver.computeLBD( *clause ) );
        clauses.push_back( clause );        
    }
    if( clauses.size() == 1 )
    {
        if( clauses[ 0 ]->size() >= 2 )
            Learning::sortClause( clauses[ 0 ], solver );
        return clauses[ 0 ];
    }

    solver.unrollToZero();
    unsigned int i = 0;
    for( ; i < clauses.size(); i++ )
    {
        unsigned int size = clauses[ i ]->size();
        if( size == 0 )
        {
            for( unsigned int j = i + 1; j < clauses.size(); j++ )
                delete clauses[ j ];
            return clauses[ i ];
        }
        else if( size == 1 )
        {
            if( !solver.addClauseRuntime( clauses[ i ] ) )
            {
                for( unsigned int j = i + 1; j < clauses.size(); j++ )
                    delete clauses[ j ];
                return NULL;
            }
        }
        else
            solver.addLearnedClause( clauses[ i ], true );
    }
    return NULL;    
}

void
LazyInstantiator::endParsing(
    Solver& solver )
{
    vector< unsigned int > atomsToFreeze;
    constraint->onAtoms( VariableNames::getVariables(), atomsToFreeze );
    for( unsigned int i = 0; i < atomsToFreeze.size(); i++ )
        solver.setFrozen( atomsToFreeze[ i ] );    
}
#else
LazyInstantiator::LazyInstantiator( const string&, Solver& ) : ExternalPropagator() { WaspErrorMessage::errorGeneric( "WASP is not compiled for including lazy grounding." ); }
LazyInstantiator::~LazyInstantiator() { WaspErrorMessage::errorGeneric( "WASP is not compiled for including lazy grounding." ); }
bool LazyInstantiator::checkAnswerSet( Solver& ) { WaspErrorMessage::errorGeneric( "WASP is not compiled for including lazy grounding." ); return true; }
Clause* LazyInstantiator::getReasonForCheckFailure( Solver& ) { WaspErrorMessage::errorGeneric( "WASP is not compiled for including lazy grounding." ); return NULL; }
void LazyInstantiator::endParsing( Solver& ) { WaspErrorMessage::errorGeneric( "WASP is not compiled for including lazy grounding." ); }
#endif