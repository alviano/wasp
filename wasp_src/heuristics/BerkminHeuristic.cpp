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
#include "../LearnedClause.h"
#include "../Literal.h"
#include "../Solver.h"
#include "visitors/TopMostUndefinedClauseVisitor.h"
#include "visitors/HigherGlobalCounterVisitor.h"
#include "visitors/MostOccurrencesVisitor.h"

BerkminHeuristic::~BerkminHeuristic()
{
}

Literal*
BerkminHeuristic::makeAChoice(
    Solver& solver )
{
    Literal* chosenLiteral;
    chosenLiteral = pickLiteralUsingLearnedClauses( solver );
    
    if( chosenLiteral == NULL )
        chosenLiteral = pickLiteralUsingActivity( solver );
    
    assert( chosenLiteral != NULL );    
    assert( chosenLiteral->isUndefined() );
    return chosenLiteral;
}

Literal*
BerkminHeuristic::pickLiteralUsingLearnedClauses(
    Solver& solver )
{
    return topMostUndefinedLearnedClause( solver );
}

Literal*
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
        
        Literal* chosenLiteral = visitor.getChosenLiteral();
        if( chosenLiteral != NULL )
        {
            assert( chosenLiteral->isUndefined() );
            return chosenLiteral;
        }
    }
    
    return NULL;
}

Literal*
BerkminHeuristic::pickLiteralUsingActivity(
    Solver& solver )
{
    const UnorderedSet< PositiveLiteral* >& undefinedLiterals = solver.getUndefinedLiterals();
    HigherGlobalCounterVisitor higherGlobalCounterVisitor( &solver );
    MostOccurrencesVisitor mostOccurrencesVisitor;
    for( unsigned int i = 0; i < undefinedLiterals.size(); ++i )
    {
        PositiveLiteral* positiveLiteral = undefinedLiterals.at( i );
        
        assert( "The literal must be undefined." && positiveLiteral->isUndefined() );
        positiveLiteral->visitForHeuristic( &higherGlobalCounterVisitor );
        positiveLiteral->visitForHeuristic( &mostOccurrencesVisitor );
    }
    
    Literal* chosenLiteral = higherGlobalCounterVisitor.getChosenLiteral();
    if( chosenLiteral != NULL )
    {
        assert( chosenLiteral->isUndefined() );
        return chosenLiteral;
    }
    
    chosenLiteral = mostOccurrencesVisitor.getChosenLiteral();
    if( chosenLiteral != NULL )
    {
        assert( chosenLiteral->isUndefined() );
        return chosenLiteral;
    }
    
    assert( "The literal must be undefined." && undefinedLiterals.at( 0 )->isUndefined() );
    return undefinedLiterals.at( 0 );
}
