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

#include <vector>

#include "Solver.h"

#include "Clause.h"
#include "Literal.h"

ostream&
operator<<(
    ostream & out, 
    const Clause & clause )
{
    if( clause.literals.empty() )
        return out;

    out << *( clause.literals[ 0 ] );
    for( unsigned int i = 1; i < clause.literals.size(); i++ )
    {
        Literal* lit = clause.literals[ i ];
        out << " | " << *lit;
    }

    return out;
}

void
Clause::onLiteralFalse(
    Literal* literal,
    Solver& solver )
{
    assert( "The literal must be false." && literal->isFalse() );
    
    assert( "First watch is not in range." && firstWatch < literals.size() );
    assert( "First watch points to a NULL literal." && literals[ firstWatch ] != NULL );
    assert( "Second watch is not in range." && secondWatch < literals.size() );
    assert( "Second watch points to a NULL literal." && literals[ secondWatch ] != NULL );
    
    if( literal == literals[ firstWatch ] )
    {
        //if the clause is already satisfied do nothing.
        if( !literals[ secondWatch ]->isTrue() )
        {
            //update first watch
            updateWatch( firstWatch, secondWatch, iterator_firstWatch, solver );            
        }
    }
    else
    {
        assert( "Literal is not watched." && literal == literals[ secondWatch ] );
        //if the clause is already satisfied do nothing.
        if( !literals[ firstWatch ]->isTrue() )
        {
            //update second watch
            updateWatch( secondWatch, firstWatch, iterator_secondWatch, solver );
        }
    }
}

void
Clause::updateWatch(
    unsigned int& watchToUpdate,
    unsigned int& otherWatch,
    WatchedList< Clause* >::iterator iteratorWatchToUpdate,
    Solver& solver )
{    
    assert( "Clause is empty or contains only one literal." && literals.size() > 1 );
    
    assert( "The watchToUpdate is not in range." && watchToUpdate < literals.size() );
    assert( "The otherWatch is not in range." && otherWatch < literals.size() );

    assert( "The watchToUpdate points to a non false literal." && literals[ watchToUpdate ]->isFalse() );
    assert( "The otherWatch is true." && !literals[ otherWatch ]->isTrue() );
    
    //Save the oldPosition of the watch
    unsigned int oldPosition = watchToUpdate;    
    
    //Update the watchToUpdate until the old position is reached or the watchToUpdate points to a non false literal.
    do
    {
        watchToUpdate = ( watchToUpdate + 1 ) % literals.size();
        //Two watches cannot point to the same literal, hence the otherWatch is skipped.    
        if( watchToUpdate == otherWatch )
            watchToUpdate = ( watchToUpdate + 1 ) % literals.size();
    } while( watchToUpdate != oldPosition && literals[ watchToUpdate ]->isFalse() );
    assert( "The watchToUpdate is in a incosistent position." && watchToUpdate < literals.size() );
    
    //If the watchToUpdate has reached its previous position the only undefined
    //literal in the clause is pointed by the otherWatch. Thus, unit propagation!
    if( watchToUpdate == oldPosition )
    {
        assert( "The other literal cannot be true." && !literals[ otherWatch ]->isTrue() );
        //Propagate literals[ otherWatch ];
        solver.onLiteralAssigned( literals[ otherWatch ], TRUE, this );
        
        //Literals inferred by this clause are inserting in the position 0 by default.
//        moveWatchToFirstPosition( watchToUpdate, otherWatch );
        
//        cout << *this << endl;
//        cout << "watch: " << firstWatch << " " << secondWatch << endl;
//        moveWatchToFirstPosition( otherWatch, watchToUpdate );
//        cout << *this << endl;
//        cout << "watch: " << firstWatch << " " << secondWatch << endl;
    }
    else
    {
        //Detach the old watch
        detachWatch( oldPosition, iteratorWatchToUpdate );
        //Attach the watch in the new position
        attachWatch( watchToUpdate, iteratorWatchToUpdate );                
    }
}

//void
//Clause::moveWatchToFirstPosition(
//    unsigned int& watchToUpdate,
//    unsigned int& otherWatch )
//{
//    assert( "The watchToUpdate is not in range." && watchToUpdate < literals.size() );
//    assert( "The otherWatch is not in range." && otherWatch < literals.size() );
//    
//    //If the watch to update is equal to 0 than no operation needed.
//    if( watchToUpdate == 0 )
//        return;
//    
//    //Swap the literal pointed by the watchToUpdate with the one in the first position.
//    Literal* tmp = literals[ 0 ];
//    literals[ 0 ] = literals[ watchToUpdate ];
//    literals[ watchToUpdate ] = tmp;
//    
//    //If the other watch is equal to 0, we need to change the position of the watches.
//    if( otherWatch == 0 )
//    {
//        otherWatch = watchToUpdate;
//    }
//    
//    //In the end, the watchToUpdate is moved to the first position.
//    watchToUpdate = 0;
//}