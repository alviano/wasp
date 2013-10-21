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
 * File:   RestartsBasedDeletionStrategy.h
 * Author: carmine
 *
 * Created on 8 agosto 2013, 17.02
 */

#ifndef RESTARTSBASEDDELETIONSTRATEGY_H
#define	RESTARTSBASEDDELETIONSTRATEGY_H

#include "ActivityBasedDeletionStrategy.h"

class RestartsBasedDeletionStrategy : public ActivityBasedDeletionStrategy
{
    public:
        inline RestartsBasedDeletionStrategy( Solver& solver );
        
        virtual void onLearning( LearnedClause* clause );
        virtual void onRestart();
        
    private:
        bool hasToDelete();
    
        unsigned int countRestarts;
};

RestartsBasedDeletionStrategy::RestartsBasedDeletionStrategy(
     Solver& solver ) : ActivityBasedDeletionStrategy( solver ), countRestarts( 1 )
{
}


#endif	/* RESTARTSBASEDDELETIONSTRATEGY_H */

