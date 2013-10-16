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
#include "../Solver.h"
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
    trace( heuristic, 1, "Starting Berkmin Heuristic.\n" );
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
    trace( heuristic, 1, "Starting TopMostUndefinedLearnedClause.\n" );
    const List< LearnedClause* >& learnedClauses = solver.getLearnedClauses();
    
    trace( heuristic, 1, "Considering %d learned clauses.\n", maxBerkminNumber );
    unsigned int count = 0;
    for( List< LearnedClause* >::const_reverse_iterator it = learnedClauses.rbegin(); it != learnedClauses.rend(); ++it )
    {
        if( count++ > maxBerkminNumber )
            break;
        
        assert( *it != NULL );
        LearnedClause& learnedClause = **it;
        trace( heuristic, 1, "Learned clause %d: %s.\n", count, learnedClause.clauseToCharStar() );
        
        TopMostUndefinedClauseVisitor visitor;
        learnedClause.visitForHeuristic( &visitor );
        visitor.endIteration();

        if( visitor.hasChosenLiteral() )
        {
            Literal chosenLiteral = visitor.getChosenLiteral();
            trace( heuristic, 1, "Chosen literal %s.\n", chosenLiteral.literalToCharStar() );
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
    trace( heuristic, 1, "Starting pickLiteralUsingActivity.\n" );
    //const UnorderedSet< Variable* >& undefinedVariable = solver.getUndefinedVariables();
    HigherGlobalCounterVisitor higherGlobalCounterVisitor( &solver );
    MostOccurrencesVisitor mostOccurrencesVisitor;
    Variable* variable = NULL;

    //for( unsigned int i = 0; i < undefinedVariable.size(); ++i )
    do{
        variable = solver.getNextUndefinedVariable(); //undefinedVariable.at( i );
        
        assert( "Variable has not been set." && variable != NULL );
        assert( "The literal must be undefined." && variable->isUndefined() );
        variable->visitForHeuristic( &higherGlobalCounterVisitor );
        variable->visitForHeuristic( &mostOccurrencesVisitor );
    } while( solver.hasNextUndefinedVariable() );
    
    higherGlobalCounterVisitor.endIteration();
    mostOccurrencesVisitor.endIteration();

    if( higherGlobalCounterVisitor.hasChosenLiteral() )
    {
        Literal chosenLiteral = higherGlobalCounterVisitor.getChosenLiteral();
        trace( heuristic, 1, "Chosen literal using HigherGlobalCounter %s.\n", chosenLiteral.literalToCharStar() );
        assert( chosenLiteral.isUndefined() );
        return chosenLiteral;
    }
    
    if( mostOccurrencesVisitor.hasChosenLiteral() )
    {
        Literal chosenLiteral = mostOccurrencesVisitor.getChosenLiteral();
        trace( heuristic, 1, "Chosen literal using MostOccurrences %s.\n", chosenLiteral.literalToCharStar() );
        assert( chosenLiteral.isUndefined() );
        return chosenLiteral;
    }

    trace( heuristic, 1, "Choosing first undefined literal.\n" );

    assert( "The literal must be undefined." && variable->isUndefined() );
    return Literal( variable, false );
}

void
BerkminHeuristic::onLearning(
    Solver& )
{
    numberOfConflicts++;
    //TODO: Implement aging.
}

void
BerkminHeuristic::onRestarting(
    Solver& )
{
}
