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

#include "WaspRule.h"
#include "AuxLiteral.h"
#include "solvers/Solver.h"
#include "inputBuilders/AbstractBuilder.h"

ostream&
WaspRule::print(
    ostream& out ) const
{
    out << "WaspRule: ";
    return Clause::print( out );
}

void
WaspRule::onLiteralTrue( 
    Literal* literal,
    Solver& solver )
{
    assert( firstTrueLiteral != NULL );    
    assert( literal != auxLiteral );
    if( literal == firstTrueLiteral || !firstTrueLiteral->isTrue() )
    {
        firstTrueLiteral = literal;
        solver.onLiteralAssigned( auxLiteral->getOppositeLiteral(), this );
    }
}

void
WaspRule::onAuxLiteralTrue(
    Solver& solver )
{
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        if( !literals[ i ]->isFalse() && literals[ i ] != auxLiteral )
        {
            solver.onLiteralAssigned( literals[ i ]->getOppositeLiteral(), this );
        }
    }
}

void
WaspRule::onLearning(
    LearningStrategy* strategy )
{
    assert( auxLiteral != NULL );
    if( auxLiteral->isTrue() )
    {
        strategy->onNavigatingLiteral( auxLiteral );
    }
    else
    {
        Clause::onLearning( strategy );
    }
}

void
WaspRule::onLearningForFalseAuxLiteral(
    LearningStrategy* strategy )
{
    assert( firstTrueLiteral != NULL );
    assert( firstTrueLiteral->isTrue() );

    strategy->onNavigatingLiteral( firstTrueLiteral );
}

void
WaspRule::onLearningForTrueAuxLiteral(
    LearningStrategy* strategy )
{
    Clause::onLearning( strategy );
}

void
WaspRule::attachWaspRule()
{
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        if( literals[ i ] != auxLiteral )
        {
            literals[ i ]->addWaspRule( this );
        }
    } 
}

void
WaspRule::setAuxLiteral(
    AuxLiteral* auxLit )
{
    auxLiteral = auxLit;
    
    //Set the first true literal in a random position
    unsigned int pos = rand() % size();
    if( literals[ pos ] == auxLiteral )
        pos = ( pos + 1 ) % size();
    
    firstTrueLiteral = literals[ pos ];
}