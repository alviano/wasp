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

#include "Solver.h"

Solver::~Solver()
{
//    for( unsigned int i = 0; i < auxLiterals.size(); i++ )
//    {
//        delete auxLiterals[ i ];
//    }
    
    while( !clauses.empty() )
    {
        delete clauses.back();
        clauses.pop_back();
    }
    
    while( !learnedClauses.empty() )
    {
        delete learnedClauses.back();
        learnedClauses.pop_back();
    }
        
    if( outputBuilder != NULL )
        delete outputBuilder;
        
    if( heuristic != NULL )
        delete heuristic;
}

void
Solver::unroll(
    unsigned int level )
{
    assert( "Level is not valid." && level < unrollVector.size() && currentDecisionLevel >= level );
    assert( "Vector for unroll is inconsistent" && variables.numberOfAssignedLiterals() >= unrollVector[ level ] );    
    unsigned int toUnroll = variables.numberOfAssignedLiterals() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        unrollLastVariable();
        toUnroll--;
    }
    
    while( toPop > 0 )
    {
        unrollVector.pop_back();
        toPop--;
    }
    
    variables.onUnroll();
}

void
Solver::decreaseLearnedClausesActivity()
{
    for( List< Clause* >::iterator it = learnedClauses.begin(); it != learnedClauses.end(); ++it )
    {
        Clause* currentClause = *it;
        currentClause->decreaseActivity();
    }
}

bool
Solver::addClauseFromModelAndRestart()
{
    assert( variables.numberOfAssignedLiterals() > 0 );
    
    Clause* clause = new Clause();
    
    Literal lastInsertedLiteral;
    
    for( unsigned int i = 1; i < variables.numberOfVariables(); i++ )
    {
        Variable* v = variables[ i ];
        assert( !v->isUndefined() );
        if( !v->hasImplicant() && v->getDecisionLevel() != 0 )
        {
            if( v->isTrue() )
            {
                Literal lit( v, false );
                lastInsertedLiteral = lit;
                clause->addLiteral( lit );
            }
            else
            {
                Literal lit( v );
                lastInsertedLiteral = lit;
                clause->addLiteral( lit );
            }
        }
    }
    
    if( clause->size() == 0 )
    {
        delete clause;
        return false;
    }
    
    this->doRestart();
    if( clause->size() > 1 )
    {
        clause->attachClause();
        addClause( clause );
    }
    else
    {        
        delete clause;        
        if( !propagateLiteralAsDeterministicConsequence( lastInsertedLiteral ) )
            return false;
        
    }
    
    return true;
}

bool
Solver::preprocessing()
{
    unsigned size = trueLiterals.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        Literal literal = trueLiterals[ i ];
        if( !propagateLiteralAsDeterministicConsequence( literal ) )
            return false;
    }
    assert( !conflictDetected() );
    return true;
}

bool 
Solver::solve()
{
    trace( solving, 1, "Starting solving.\n" );
    while( hasUndefinedLiterals() )
    {
        /*
        static unsigned int PROVA = 0;
        static time_t PROVA_TIME = time( 0 );


        unsigned int end = 3000000;
        unsigned int printValue = 10000;

        if( ++PROVA > end ) {
            cerr << "PROVA END!" << endl;
            return false;
        }
        else if( ++PROVA % printValue == 0 )
        {
            cout << PROVA << " " << learnedClauses.size() <<  " " << ( time( 0 ) - PROVA_TIME ) << endl;
        }
        //*/

        assert( !conflictDetected() );
        chooseLiteral();
        
        while( hasNextVariableToPropagate() )
        {
            Variable* variableToPropagate = getNextVariableToPropagate();            
            propagate( variableToPropagate );

            if( conflictDetected() )
            {
                trace( solving, 1, "Conflict detected.\n" );
                if( getCurrentDecisionLevel() == 0 )
                {
                    trace( solving, 1, "Conflict at level 0: return. \n");
                    return false;
                }

                analyzeConflict();
                assert( hasNextVariableToPropagate() || getCurrentDecisionLevel() == 0 );
            }
        }
    }
    
    return true;
}

void
Solver::propagate(
    Variable* variable )
{
    assert( "Variable to propagate has not been set." && variable != NULL );
    trace( solving, 2, "Propagating: %s.\n", toString( *variable ).c_str() );
    
    Literal complement = Literal::createOppositeFromAssignedVariable( variable );
    
    variable->unitPropagationStart();
    assert( !conflictDetected() );
    while( variable->unitPropagationHasNext() && !conflictDetected() )
    {
        Clause* clause = variable->unitPropagationNext();
        assert( "Next clause to propagate is null." && clause != NULL );
        trace( solving, 5, "Considering clause %s.\n", toString( *clause ).c_str() );
        if( clause->onLiteralFalse( complement ) )
            assignLiteral( clause );
        else
            assert( !conflictDetected() );
    }
}

void
Solver::printProgram() const
{
    for( List< Clause* >::const_iterator it = clauses.begin(); it != clauses.end(); ++it )
    {
        cout << *( *it ) << endl;
    }
}
