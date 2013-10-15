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
 * File:   BerkminHeuristic.h
 * Author: Carmine Dodaro
 *
 * Created on 02 September 2013, 16.36
 */

#ifndef BERKMINHEURISTIC_H
#define	BERKMINHEURISTIC_H

#include "DecisionHeuristic.h"

//#include <vector>
//using namespace std;

class BerkminHeuristic : public DecisionHeuristic
{
    public:
        inline BerkminHeuristic( unsigned int maxBerkminNumber );
        virtual ~BerkminHeuristic();
        virtual Literal makeAChoice( Solver& solver );
        virtual void onLearning( Solver& solver );
        virtual void onRestarting( Solver& solver );
        
    protected:
        virtual Literal pickLiteralUsingLearnedClauses( Solver& solver );
        virtual Literal pickLiteralUsingActivity( Solver& solver );
//        virtual bool tieBreak( vector< Literal* >& );
        
    private:
        Literal topMostUndefinedLearnedClause( Solver& solver );
        unsigned int maxBerkminNumber;
        unsigned int numberOfConflicts;
};

BerkminHeuristic::BerkminHeuristic( 
    unsigned int max ) : maxBerkminNumber( max ), numberOfConflicts( 0 )
{
}

#endif	/* BERKMINHEURISTIC_H */

