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

#include "BerkminHeuristic.h"

#include <cassert>
#include "../Variable.h"
#include "../LearnedClause.h"
#include "../Literal.h"
#include "../solvers/Solver.h"
#include "visitors/TopMostUndefinedClauseVisitor.h"
#include "visitors/HigherGlobalCounterVisitor.h"
#include "visitors/MostOccurrencesVisitor.h"

BerkminHeuristic::~BerkminHeuristic()
{
}

Literal
BerkminHeuristic::makeAChoice(
    Solver& solver )
{
    Literal chosenLiteral;
    chosenLiteral = pickLiteralUsingLearnedClauses( solver );
    
    if( chosenLiteral == NULL )
        chosenLiteral = pickLiteralUsingActivity( solver );
    
    assert( chosenLiteral != NULL );    
    assert( chosenLiteral.isUndefined() );
    return chosenLiteral;
}

Literal
BerkminHeuristic::pickLiteralUsingLearnedClauses(
    Solver& solver )
{
    return topMostUndefinedLearnedClause( solver );
}

Literal
BerkminHeuristic::topMostUndefinedLearnedClause(
   Solver& solver )
{
    const List< LearnedClause* >& learnedClauses = solver.getLearnedClauses();
    TopMostUndefinedClauseVisitor visitor;
    unsigned int count = 0;
    for( List< LearnedClause* >::const_reverse_iterator it = learnedClauses.rbegin(); it != learnedClauses.rend(); ++it )
    {
        if( count++ > maxBerkminNumber )
            break;
        
        assert( *it != NULL );
        LearnedClause& learnedClause = *( *it );        
        learnedClause.visitForHeuristic( &visitor );
        
        if( visitor.hasChosenLiteral() )
        {
            Literal chosenLiteral = visitor.getChosenLiteral();        
            assert( chosenLiteral.isUndefined() );
            return chosenLiteral;
        }
    }
    
    return NULL;
}

Literal
BerkminHeuristic::pickLiteralUsingActivity(
    Solver& solver )
{
    const UnorderedSet< Variable* >& undefinedVariable = solver.getUndefinedVariables();
    HigherGlobalCounterVisitor higherGlobalCounterVisitor( &solver );
    MostOccurrencesVisitor mostOccurrencesVisitor;
    for( unsigned int i = 0; i < undefinedVariable.size(); ++i )
    {
        Variable* variable = undefinedVariable.at( i );
        
        assert( "The literal must be undefined." && variable->isUndefined() );
        variable->visitForHeuristic( &higherGlobalCounterVisitor );
        variable->visitForHeuristic( &mostOccurrencesVisitor );
    }
        
    if( higherGlobalCounterVisitor.hasChosenLiteral() )
    {
        Literal chosenLiteral = higherGlobalCounterVisitor.getChosenLiteral();
        assert( chosenLiteral.isUndefined() );
        return chosenLiteral;
    }
    
    if( mostOccurrencesVisitor.hasChosenLiteral() )
    {
        Literal chosenLiteral = mostOccurrencesVisitor.getChosenLiteral();
        assert( chosenLiteral.isUndefined() );
        return chosenLiteral;
    }
    
    assert( "The literal must be undefined." && undefinedVariable.at( 0 )->isUndefined() );
    return Literal( undefinedVariable.at( 0 ), false );
}

void
BerkminHeuristic::onLearning(
    Solver& )
{
    numberOfConflicts++;    
}

void
BerkminHeuristic::onRestarting(
    Solver& )
{
}