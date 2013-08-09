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

/* 
 * File:   LearnedClause.h
 * Author: Carmine Dodaro
 *
 * Created on 8 August 2013, 15.43
 */

#ifndef LEARNEDCLAUSE_H
#define	LEARNEDCLAUSE_H

#include "Clause.h"
#include "Constants.h"

class LearnedClause : public Clause
{
    public:
        inline LearnedClause();
        inline LearnedClause( unsigned int size );
        inline LearnedClause( unsigned int size, unsigned int firstWatch, unsigned int secondWatch );
        
        inline void decreaseActivity();
        inline Activity getActivity() const;
        inline void incrementActivity( Activity increment );
        
        inline bool isLocked() const;        

    private:
        
        /**
        * The activity of this learned clause.
        
        * This number computes how often this clause is used in the unit propagation.
        */
        Activity activity;
            
};

LearnedClause::LearnedClause() : Clause(), activity( 0 )
{
}

LearnedClause::LearnedClause(
    unsigned int size ) : Clause( size ), activity( 0 )
{
}

LearnedClause::LearnedClause(
    unsigned int size,
    unsigned int firstWatch,
    unsigned int secondWatch ) : Clause( size, firstWatch, secondWatch ), activity( 0 )
{
}

bool
LearnedClause::isLocked() const
{
    return isImplicantOfALiteral();
}

void
LearnedClause::decreaseActivity()
{
    activity *= 1e-20;
}

Activity
LearnedClause::getActivity() const
{
    return activity;
}

void
LearnedClause::incrementActivity( 
    Activity increment )
{
    activity += increment;
}

#endif	/* LEARNEDCLAUSE_H */

