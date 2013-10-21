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
using namespace std;

#include "Clause.h"
#include "LearnedClause.h"
#include "Variable.h"
#include "Variables.h"
#include "Literal.h"
#include "Options.h"
#include "Trace.h"
#include "WaspRule.h"
#include "stl/List.h"
#include "stl/UnorderedSet.h"
#include "Learning.h"
#include "heuristics/DecisionHeuristic.h"
#include "outputBuilders/OutputBuilder.h"
#include "restart/RestartStrategy.h"
#include "deletion/DeletionStrategy.h"

class Solver
{
    public:
        inline Solver();
        ~Solver();
        
        inline void init();
        bool preprocessing();
        bool solve();
        inline void propagate( Literal literalToPropagate );
        
        inline void addVariable( const string& name );
        inline void addVariable();
        
//        AuxLiteral* addAuxVariable();
//        inline bool existsAuxLiteral( unsigned int id ) const;
//        inline AuxLiteral* getAuxLiteral( unsigned int id );
        
        inline void addClause( Clause* clause );
        inline void addLearnedClause( LearnedClause* learnedClause );
        bool addClauseFromModelAndRestart();
        
        inline Literal getLiteral( int lit );
        inline void addTrueLiteral( Literal lit );
        
        inline Literal getNextLiteralToPropagate();
        inline bool hasNextLiteralToPropagate() const;        
        
//        inline const Variable* getNextAssignedVariable();
        inline Literal getOppositeLiteralFromLastAssignedVariable();
        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();

        inline unsigned int getCurrentDecisionLevel();
        inline void incrementCurrentDecisionLevel();
        
        inline void onLiteralAssigned( Literal literal, Clause* implicant );
        
        inline bool propagateLiteralAsDeterministicConsequence( Literal literal );
        
        void decreaseLearnedClausesActivity();
        inline void onLearningClause( Literal literalToPropagate, LearnedClause* learnedClause, unsigned int backjumpingLevel );
        inline void onLearningUnaryClause( Literal literalToPropagate, LearnedClause* learnedClause );        
        inline void onRestart();        
        
        inline unsigned int numberOfClauses();
        inline unsigned int numberOfLearnedClauses();        
        inline unsigned int numberOfAssignedLiterals();
        inline unsigned int numberOfVariables();
        inline unsigned int numberOfAuxVariables();
        
        inline void setAChoice( Literal choice );        
        
        inline void analyzeConflict();
        inline void clearConflictStatus();
        inline void chooseLiteral();
        inline bool conflictDetected();
        inline void foundIncoherence();
        inline bool hasUndefinedLiterals();
//        inline void startIterationOnUndefinedVariables() { variables.startIterationOnUndefinedVariables(); }
//        inline bool hasNextUndefinedVariable() { return variables.hasNextUndefinedVariable(); }
//        inline Variable* getNextUndefinedVariable() { return variables.getNextUndefinedVariable(); }
        inline Variable* getFirstUndefined() { return variables.getFirstUndefined(); }
        inline Variable* getNextUndefined( Variable* v ) { return variables.getNextUndefined( v ); }
        inline void printAnswerSet();        
        
        void unroll( unsigned int level );
        inline void unrollOne();
        inline void unrollLastVariable();
        
        /* OPTIONS */
        inline void setHeuristic( DecisionHeuristic* value );
        inline void setOutputBuilder( OutputBuilder* value );
        inline void setRestartStrategy( RestartStrategy* value );
        inline void setDeletionStrategy( DeletionStrategy* value );
        
        typedef List< LearnedClause* >::iterator LearnedClausesIterator;
        typedef List< LearnedClause* >::reverse_iterator LearnedClausesReverseIterator;
        inline LearnedClausesIterator learnedClauses_begin() { return learnedClauses.begin(); }
        inline LearnedClausesIterator learnedClauses_end() { return learnedClauses.end(); }
        inline LearnedClausesReverseIterator learnedClauses_rbegin() { return learnedClauses.rbegin(); }
        inline LearnedClausesReverseIterator learnedClauses_rend() { return learnedClauses.rend(); }

        inline void deleteLearnedClause( LearnedClause* learnedClause, List< LearnedClause* >::iterator iterator );
        
        void printProgram()
        {
            for( List< Clause* >::const_iterator it = clauses.begin(); it != clauses.end(); ++it )
            {
                cout << *( *it ) << endl;
            }
        }
        
        inline void onLiteralInvolvedInConflict( Literal l ) { decisionHeuristic->onLiteralInvolvedInConflict( l ); }
        
    private:
        Solver( const Solver& ) : learning( *this )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }
                
        vector< Literal > trueLiterals;

        unsigned int currentDecisionLevel;        
        
        Variables variables;
        
        List< Clause* > clauses;
        List< LearnedClause* > learnedClauses;
        
        vector< unsigned int > unrollVector;
        
        Literal conflictLiteral;
        Clause* conflictClause;
        
        Learning learning;
        DecisionHeuristic* decisionHeuristic;        
        DeletionStrategy* deletionStrategy;
        OutputBuilder* outputBuilder;
        RestartStrategy* restartStrategy;
};

Solver::Solver() 
: currentDecisionLevel( 0 ),
  conflictLiteral( NULL ),
  conflictClause( NULL ),
  learning( *this ),
  decisionHeuristic( NULL ),
  deletionStrategy( NULL ),
  outputBuilder( NULL ),
  restartStrategy( NULL )
{
}

void
Solver::setHeuristic(
    DecisionHeuristic* value )
{
    assert( value != NULL );
    if( decisionHeuristic != NULL )
        delete decisionHeuristic;
    decisionHeuristic = value;
}

void
Solver::setOutputBuilder(
    OutputBuilder* value )
{
    assert( value != NULL );
    if( outputBuilder != NULL )
        delete outputBuilder;
    outputBuilder = value;
}

void
Solver::setRestartStrategy(
    RestartStrategy* value )
{
    assert( value != NULL );
    if( restartStrategy != NULL )
        delete restartStrategy;
    restartStrategy = value;
}

void
Solver::setDeletionStrategy(
    DeletionStrategy* value )
{
    assert( value != NULL );
    if( deletionStrategy != NULL )
        delete deletionStrategy;
    deletionStrategy = value;
}

void
Solver::addVariable( 
    const string& name )
{    
    Variable* variable = new Variable( variables.numberOfVariables()+1, name );
    variables.push_back( variable );
    assert( variables.numberOfVariables() == variable->getId() );
    assert( decisionHeuristic != NULL );
    decisionHeuristic->onNewVariable( *variable );
    learning.onNewVariable();
}

void
Solver::addVariable()
{
    Variable* variable = new Variable( variables.numberOfVariables()+1 );
    variables.push_back( variable );
    assert( variables.numberOfVariables() == variable->getId() );
    assert( decisionHeuristic != NULL );
    decisionHeuristic->onNewVariable( *variable );
}

Literal
Solver::getLiteral(
    int lit )
{
    assert( "Lit is out of range." && static_cast< unsigned >( abs( lit ) ) <= variables.numberOfVariables() && abs( lit ) > 0);
    return lit > 0 ? Literal( variables[ lit ] ) : Literal( variables[ -lit ], false );
//    if( lit > 0 )
//    {
//        Literal literal( variables[ lit ] );
//        return literal;
//    }
//    else
//    {
//        Literal literal( variables[ -lit ], false );
//        return literal;
//    }    
}

void
Solver::init()
{
    variables.init();
    cout << COMMENT_DIMACS << " " << WASP_STRING << endl;
}

void 
Solver::onLiteralAssigned(
    Literal literal,
    Clause* implicant )
{
    if( !variables.assign( literal, currentDecisionLevel, implicant ) )
    {
        conflictLiteral = literal;
        conflictClause = implicant;        
    }
}

void
Solver::propagate(
    Literal literalToPropagate )
{
    trace( solving, 1, "Propagating: %s.\n", literalToPropagate.literalToCharStar() );
    literalToPropagate.unitPropagation( *this );
}

void
Solver::addClause(
    Clause* clause )
{
    clauses.push_back( clause );
}

void
Solver::addLearnedClause( 
    LearnedClause* learnedClause )
{    
    learnedClauses.push_back( learnedClause );
}

void
Solver::addTrueLiteral(
    Literal lit )
{
    trueLiterals.push_back( lit );
}

Literal
Solver::getNextLiteralToPropagate()
{
    assert( variables.hasNextLiteralToPropagate() );
    return variables.getNextLiteralToPropagate();
}
        
bool
Solver::hasNextLiteralToPropagate() const
{
    return variables.hasNextLiteralToPropagate();
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
    unrollVector.push_back( variables.numberOfAssignedLiterals() );
    
    assert( currentDecisionLevel == unrollVector.size() );
}

void
Solver::unrollLastVariable()
{
    variables.unrollLastVariable();
}

void
Solver::unrollOne()
{
    unroll( currentDecisionLevel - 1 );
}

void
Solver::onRestart()
{
    trace( solving, 1, "Performing restart.\n" );
    deletionStrategy->onRestart();
    decisionHeuristic->onRestart();
    unroll( 0 );
}

void
Solver::deleteLearnedClause( 
    LearnedClause* learnedClause,
    List< LearnedClause* >::iterator iterator )
{
    trace( solving, 4, "Deleting learned clause %s.", learnedClause->clauseToCharStar() );
    learnedClause->detachClause();
    delete learnedClause;    
    learnedClauses.erase( iterator );
}

unsigned int
Solver::numberOfClauses()
{
    return clauses.size();
}

unsigned int
Solver::numberOfLearnedClauses()
{
    return learnedClauses.size();
}

bool
Solver::conflictDetected()
{
    return conflictLiteral != NULL;
}

bool
Solver::hasUndefinedLiterals()
{
    return variables.numberOfAssignedLiterals() < variables.numberOfVariables();
}

void
Solver::printAnswerSet()
{
    variables.printAnswerSet( outputBuilder );
}

void
Solver::foundIncoherence()
{
    outputBuilder->onProgramIncoherent();
}

void
Solver::chooseLiteral()
{
    Literal choice = decisionHeuristic->makeAChoice();
    trace( solving, 1, "Choice: %s.\n", choice.literalToCharStar() );
    setAChoice( choice );    
}

void
Solver::analyzeConflict()
{
    trace( solving, 1, "Analyzing conflict.\n" );
    LearnedClause* learnedClause = learning.onConflict( conflictLiteral, conflictClause );
    assert( "Learned clause has not been calculated." && learnedClause != NULL );
    
    if( learnedClause->size() == 1 )
    {
        onRestart();
        onLiteralAssigned( learnedClause->getAt( 0 ), NULL );
        delete learnedClause;
        
        assert( "The strategy for restarts must be initialized." && restartStrategy != NULL );
        restartStrategy->onLearningUnaryClause();
    }
    else
    {
        //Be careful. UIP should be always in position 0.
        assert( learnedClause->getAt( 0 ).getDecisionLevel() == currentDecisionLevel );
        assert( learnedClause->getAt( 1 ).getDecisionLevel() == learnedClause->getMaxDecisionLevel( 1, learnedClause->size() ) );
        learnedClause->attachClause();
        addLearnedClause( learnedClause );

        assert( "The strategy for restarts must be initialized." && restartStrategy != NULL );
        if( restartStrategy->onLearningClause() )
        {
            onRestart();
        }
        else
        {
            assert( learnedClause->getAt( 1 ).getDecisionLevel() != 0 );
            assert( "Backjumping level is not valid." && learnedClause->getAt( 1 ).getDecisionLevel() < currentDecisionLevel );
            trace( solving, 2, "Learned clause and backjumping to level %d.", learnedClause->getAt( 1 ).getDecisionLevel() );
            unroll( learnedClause->getAt( 1 ).getDecisionLevel() );    
            
            assert( "Each learned clause has to be an asserting clause." && learnedClause->getAt( 0 ) != NULL );
            
            onLiteralAssigned( learnedClause->getAt( 0 ), learnedClause );
            
            deletionStrategy->onLearning( learnedClause );
        }
    }
    
    decisionHeuristic->onLearning();
    clearConflictStatus();
}

void
Solver::clearConflictStatus()
{
    conflictLiteral = NULL;
    conflictClause = NULL;
}

unsigned int
Solver::numberOfAssignedLiterals()
{
    return variables.numberOfAssignedLiterals();
}

unsigned int
Solver::numberOfVariables()
{
    return variables.numberOfVariables();
}

void
Solver::setAChoice(
    Literal choice )
{
    assert( choice != NULL );
    incrementCurrentDecisionLevel();
    assert( choice.isUndefined() );
    onLiteralAssigned( choice, NULL );
}

Literal
Solver::getOppositeLiteralFromLastAssignedVariable()
{
    return variables.getOppositeLiteralFromLastAssignedVariable();
}

bool
Solver::hasNextAssignedVariable() const
{
    return variables.hasNextAssignedVariable();
}

void
Solver::startIterationOnAssignedVariable()
{
    variables.startIterationOnAssignedVariable();
}

bool
Solver::propagateLiteralAsDeterministicConsequence(
    Literal literal )
{
    onLiteralAssigned( literal, NULL );
    if( conflictDetected() )
    {
        return false;
    }

    while( hasNextLiteralToPropagate() )
    {
        Literal literalToPropagate = getNextLiteralToPropagate();
        propagate( literalToPropagate );

        if( conflictDetected() )
        {
            return false;
        }
    }    
    assert( !conflictDetected() );

    return true;
}

//bool
//Solver::existsAuxLiteral(
//    unsigned int id ) const
//{
//    return( id < auxLiterals.size() );
//}
//
//AuxLiteral*
//Solver::getAuxLiteral(
//    unsigned int id )
//{
//    assert( existsAuxLiteral( id ) );
//    return auxLiterals[ id ];
//}

//unsigned int
//Solver::numberOfAuxVariables()
//{
//    return auxLiterals.size();
//}

#endif	/* SOLVER_H */

