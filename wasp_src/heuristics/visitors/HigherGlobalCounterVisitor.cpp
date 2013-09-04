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

#include "HigherGlobalCounterVisitor.h"
#include <cassert>

#include "../../Solver.h"

void
HigherGlobalCounterVisitor::choosePolarity(
    Literal* literal,
    Literal* oppositeLiteral )
{
//    assert( "IMPLEMENT LOOKAHEAD" && 0 );
    ( getLiteralCounter( literal ) > getLiteralCounter( oppositeLiteral ) ) ? chosenLiteral = literal : chosenLiteral = oppositeLiteral;
    
    assert( "Solver has not been set." && solver != NULL );
    
    solver->incrementCurrentDecisionLevel();
    solver->setAChoice( literal );
    
    while( solver->hasNextLiteralToPropagate() )
    {
        Literal* literalToPropagate = solver->getNextLiteralToPropagate();
        literalToPropagate->setOrderInThePropagation( solver->numberOfAssignedLiterals() );
        solver->propagate( literalToPropagate );        
    }
    
    //void
//BerkminHeuristic::choosePolarity(
//    Atom& a )
//{
//    startTimer( lookahead );
//    statistics( incrementChoosePolarityInvocationsNumber() );
//    AtomId atomId = a.getId();
//    
//    TruthValue atomTruthValue;
//    TruthValue otherAtomTruthValue;
//    
//    if( isMoreNegativeOccurrences( atomId ) )
//    {
//        atomTruthValue = TRUE;
//        otherAtomTruthValue = FALSE;
//    }
//    else
//    {
//        atomTruthValue = FALSE;
//        otherAtomTruthValue = TRUE;
//    }
//
//    AtomTruthValue literal;
//    literal.first = atomId;    
//    literal.second = atomTruthValue;
//
//    nextLevel();
//
//    Stack< unsigned int >& backtrackingStack = getBacktrackingStack();
//    stack< AtomTruthValue >& propagationStack = getPropagationStack();
//    backtrackingStack.push( &( a.getChoiceAddress() ) );
//    a.setChoice();
//    propagationStack.push( literal );
//    if( !propagate() )
//    {
//        unrollOne();
////        currentChoice.second = otherAtomTruthValue;
//        chosenPolarity = atomTruthValue;
//    }
//    else
//    {
//        unsigned int diff = backtrackingStack.size();
//        unrollOne();
//        nextLevel();
//
//        backtrackingStack.push( &( a.getChoiceAddress() ) );
//        a.setChoice();
//        
//        literal.second = otherAtomTruthValue;
//        propagationStack.push( literal );
//        if( !propagate() )
//        {
//            unrollOne();            
////            currentChoice.second = atomTruthValue;
//            chosenPolarity = otherAtomTruthValue;
//        }
//        else
//        {
//            unrollOne();
//            if( diff <= backtrackingStack.size() )
//                chosenPolarity = otherAtomTruthValue;
//            else
//                chosenPolarity = atomTruthValue;
//        }
//    }
//    
//    stopTimer( lookahead );
//}

}