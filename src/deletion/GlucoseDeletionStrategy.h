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

#ifndef GLUCOSEDELETIONSTRATEGY_H
#define	GLUCOSEDELETIONSTRATEGY_H

#include "DeletionStrategy.h"
#include <vector>
using namespace std;

class GlucoseDeletionStrategy : public DeletionStrategy
{
    public:
        inline GlucoseDeletionStrategy( Solver& solver );

        virtual void onLearning( LearnedClause* clause );
        virtual void onRestart() {}
        virtual void onUnitPropagation( Clause* clause );
        virtual void onNewVariable( Variable& variable );
        
    private:
        void deleteClauses();
        bool hasToDelete() const;
        int toDelete;
        Solver& solver;
        unsigned int countOfDeletion;
        vector< unsigned int > levelsVector;
        unsigned int numberOfUnitPropagations;
};

GlucoseDeletionStrategy::GlucoseDeletionStrategy(
    Solver& s ) : solver( s ), countOfDeletion( 0 ), numberOfUnitPropagations( 1 )
{
}

#endif	/* GLUCOSEDELETIONSTRATEGY_H */

