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
 * File:   Solver.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 17.36
 */

#ifndef SOLVER_H
#define	SOLVER_H

#include <cassert>
#include <vector>
#include <list>
using namespace std;

#include "Clause.h"
#include "Literal.h"
#include "stl/List.h"
#include "stl/UnorderedSet.h"
#include "learning/LearningStrategy.h"
#include "learning/FirstUIPLearningStrategy.h"
#include "PositiveLiteral.h"

class Variable;

class Solver
{
    public:
        inline Solver();
        ~Solver();
        
        virtual void init() = 0;
        virtual void solve() = 0;        
        
        void addVariable( const string& name );
        void addVariable();
        
        inline void addClause( Clause* clause );
        Literal* getLiteral( int lit );
        
        inline pair< Literal*, TruthValue >& getNextLiteralToPropagate();
        inline bool hasNextLiteralToPropagate() const;        
        
        inline unsigned int getCurrentDecisionLevel();
        inline void incrementCurrentDecisionLevel();
        
        void onLiteralAssigned( Literal* literal, TruthValue truthValue, Clause* implicant );
        
        void unroll( unsigned int level );
        inline void unrollOne();
        
        void printProgram()
        {
            for( list< Clause* >::iterator it = clauses.begin(); it != clauses.end(); ++it )
            {
                cout << *( *it ) << endl;
            }
        }
        
    private:
        Solver( Solver& orig )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }
        
        void addVariableInternal( PositiveLiteral* posLiteral );
        
        list< pair< Literal*, TruthValue > > literalsToPropagate;
        unsigned int currentDecisionLevel;
        
    protected:
        LearningStrategy* learningStrategy;
        list< PositiveLiteral* > assignedLiterals;
        UnorderedSet< PositiveLiteral* > undefinedLiterals;
        vector< unsigned int > unrollVector;
        bool conflict;
        
        /* Data structures */
        vector< PositiveLiteral* > positiveLiterals;
        List< Clause* > clauses;
        List< Clause* > learnedClauses;
};

Solver::Solver() : currentDecisionLevel( 0 ), conflict( false )
{
    //Add a fake position.
    positiveLiterals.push_back( NULL );
    learningStrategy = new FirstUIPLearningStrategy();    
}

void
Solver::addClause(
    Clause* clause )
{
    clauses.push_back( clause );
}

pair< Literal*, TruthValue >&
Solver::getNextLiteralToPropagate()
{
    assert( !literalsToPropagate.empty() );
    pair< Literal*, TruthValue >& tmp = literalsToPropagate.back();
    literalsToPropagate.pop_back();
    return tmp;
}
        
bool
Solver::hasNextLiteralToPropagate() const
{
    return !literalsToPropagate.empty();
}

unsigned int
Solver::getCurrentDecisionLevel()
{
    return currentDecisionLevel;
}

void
Solver::incrementCurrentDecisionLevel()
{
    currentDecisionLevel++;
    unrollVector.push_back( assignedLiterals.size() );
    
    assert( currentDecisionLevel == unrollVector.size() );
}

void
Solver::unrollOne()
{
    unroll( currentDecisionLevel - 1 );
}

#endif	/* SOLVER_H */

