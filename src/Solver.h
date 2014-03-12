/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
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

#ifndef SOLVER_H
#define	SOLVER_H

#include <cassert>
#include <vector>
using namespace std;

#include "Clause.h"
#include "Variable.h"
#include "Variables.h"
#include "Literal.h"
#include "util/Options.h"
#include "util/Trace.h"
#include "stl/List.h"
#include "stl/UnorderedSet.h"
#include "Learning.h"
#include "outputBuilders/OutputBuilder.h"
#include "util/Assert.h"
#include "Satelite.h"
#include "Restart.h"
#include "MinisatHeuristic.h"
#include "util/Statistics.h"
#include "PostPropagator.h"
#include "DependencyGraph.h"

class Solver
{
    public:
        inline Solver();
        ~Solver();
        
        inline void greetings(){ outputBuilder->greetings(); }
        
        inline void init();
        bool solve();
        bool solvePropagators();
        void unitPropagation( Variable* variable );
        void postPropagation( Variable* variable );
        inline void propagateWithPropagators( Variable* variable );
        void propagateAtLevelZero( Variable* variable );
        void propagateAtLevelZeroSatelite( Variable* variable );

        inline bool preprocessing();
        inline void attachWatches();
        
        inline void addVariable( const string& name );
        inline void addVariable();        
        
        inline bool addClause( Clause* clause );
        inline bool addClauseFromModel( Clause* clause );
        inline void addLearnedClause( Clause* learnedClause );
        bool addClauseFromModelAndRestart();
        
        inline Literal getLiteral( int lit );
        inline Variable* getVariable( unsigned int var );
        
        inline Variable* getNextVariableToPropagate();
        inline bool hasNextVariableToPropagate() const;        
        
        inline Literal getOppositeLiteralFromLastAssignedVariable();
        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();

        inline unsigned int getCurrentDecisionLevel();
        inline void incrementCurrentDecisionLevel();
        
        inline void assignLiteral( Literal literal );
        inline void assignLiteral( Clause* implicant );
        
        inline bool propagateLiteralAsDeterministicConsequence( Literal literal );
        inline bool propagateLiteralAsDeterministicConsequenceSatelite( Literal literal );
        
        inline void onLearningClause( Literal literalToPropagate, Clause* learnedClause, unsigned int backjumpingLevel );
        inline void onLearningUnaryClause( Literal literalToPropagate, Clause* learnedClause );        
        inline void doRestart();        
        
        inline unsigned int numberOfClauses();
        inline unsigned int numberOfLearnedClauses();        
        inline unsigned int numberOfAssignedLiterals();
        inline unsigned int numberOfVariables();
        inline unsigned int numberOfAuxVariables();
        
        inline void setAChoice( Literal choice );        
        
        inline void foundEmptyClause(){ conflictAtLevelZero = true; }
        inline bool analyzeConflict();
        inline void clearConflictStatus();
        inline void chooseLiteral();
        inline bool conflictDetected();
        inline void foundIncoherence();
        inline bool hasUndefinedLiterals();
        inline Variable* getFirstUndefined() { return variables.getFirstUndefined(); }
        inline Variable* getNextUndefined( Variable* v ) { return variables.getNextUndefined( v ); }
        inline void printAnswerSet();        
        
        void unroll( unsigned int level );
        inline void unrollOne();
        inline void unrollLastVariable();
        
        /* OPTIONS */
        inline void setOutputBuilder( OutputBuilder* value );        
        
        typedef vector< Clause* >::iterator ClauseIterator;
        typedef vector< Clause* >::reverse_iterator ClauseReverseIterator;
        typedef vector< Clause* >::const_iterator ConstClauseIterator;
        typedef vector< Clause* >::const_reverse_iterator ConstClauseReverseIterator;
        
        inline unsigned int numberOfClauses() const { return clauses.size(); }
        inline Clause* clauseAt( unsigned int i ) { assert( i < numberOfClauses() ); return clauses[ i ]; }
        
        inline ClauseIterator clauses_begin() { return clauses.begin(); }
        inline ClauseIterator clauses_end() { return clauses.end(); }
        inline ClauseReverseIterator clauses_rbegin() { return clauses.rbegin(); }
        inline ClauseReverseIterator clauses_rend() { return clauses.rend(); }
        inline ClauseIterator learnedClauses_begin() { return learnedClauses.begin(); }
        inline ClauseIterator learnedClauses_end() { return learnedClauses.end(); }
        inline ClauseReverseIterator learnedClauses_rbegin() { return learnedClauses.rbegin(); }
        inline ClauseReverseIterator learnedClauses_rend() { return learnedClauses.rend(); }
        inline ConstClauseIterator clauses_begin() const { return clauses.begin(); }
        inline ConstClauseIterator clauses_end() const { return clauses.end(); }
        inline ConstClauseReverseIterator clauses_rbegin() const { return clauses.rbegin(); }
        inline ConstClauseReverseIterator clauses_rend() const { return clauses.rend(); }
        inline ConstClauseIterator learnedClauses_begin() const { return learnedClauses.begin(); }
        inline ConstClauseIterator learnedClauses_end() const { return learnedClauses.end(); }
        inline ConstClauseReverseIterator learnedClauses_rbegin() const { return learnedClauses.rbegin(); }
        inline ConstClauseReverseIterator learnedClauses_rend() const { return learnedClauses.rend(); }

        inline void deleteLearnedClause( ClauseIterator iterator );
        inline void deleteClause( Clause* clause );
        inline void removeClauseNoDeletion( Clause* clause );
        void deleteClauses();
        void updateActivity( Clause* learnedClause );
        inline void decrementActivity(){ deletionCounters.increment *= deletionCounters.decrement; }
        inline void onLearning( Clause* learnedClause );
        inline bool hasToDelete();
        inline void markClauseForDeletion( Clause* clause ){ satelite->onDeletingClause( clause ); clause->markAsDeleted(); }
        
        void printProgram() const;
        
//        inline void initClauseData( Clause* clause ) { assert( heuristic != NULL ); heuristic->initClauseData( clause ); }
//        inline Heuristic* getHeuristic() { return heuristic; }
        inline void onLiteralInvolvedInConflict( Literal l ) { minisatHeuristic.onLiteralInvolvedInConflict( l ); }
        inline void finalizeDeletion( unsigned int newVectorSize ) { learnedClauses.resize( newVectorSize ); }        
        
        inline void setRestart( Restart* r );
        
        void simplifyOnRestart();
        void removeSatisfied( vector< Clause* >& clauses );

        inline void onEliminatingVariable( Variable* variable, unsigned int sign, Clause* definition );
        inline void completeModel();
        
        inline Clause* newClause();
        inline void releaseClause( Clause* clause );
        
        inline void addPostPropagator( PostPropagator* postPropagator );
        inline void resetPostPropagators();
        
        inline void addEdgeInDependencyGraph( unsigned int v1, unsigned int v2 ){ dependencyGraph.addEdge( v1, v2 ); }
        inline void computeStrongConnectedComponents(){ dependencyGraph.computeStrongConnectedComponents( gusDataVector ); }
        inline bool tight() const { return dependencyGraph.tight(); }
        inline unsigned int getNumberOfCyclicComponents(){ return dependencyGraph.numberOfCyclicComponents(); }
        inline Component* getCyclicComponent( unsigned int position ){ return dependencyGraph.getCyclicComponent( position ); }
        
        inline void addGUSData( GUSData* gd ) { gusDataVector.push_back( gd ); }        
        
    private:
        inline Variable* addVariableInternal();
        
        Solver( const Solver& ) : learning( *this )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }

        unsigned int currentDecisionLevel;
        Variables variables;
        
        vector< Clause* > clauses;
        vector< Clause* > learnedClauses;
        
        vector< unsigned int > unrollVector;
        
        Literal conflictLiteral;
        Clause* conflictClause;
        
        Learning learning;
        OutputBuilder* outputBuilder;        
        
        MinisatHeuristic minisatHeuristic;
        Restart* restart;
        Satelite* satelite;
        
        bool conflictAtLevelZero;
        unsigned int getNumberOfUndefined() const;
        bool allClausesSatisfied() const;
        
        unsigned int assignedVariablesAtLevelZero;
        int64_t nextValueOfPropagation;
        
        uint64_t literalsInClauses;
        uint64_t literalsInLearnedClauses;
        
        vector< Variable* > eliminatedVariables;
        vector< Clause* > poolOfClauses;

        DependencyGraph dependencyGraph;
        Vector< PostPropagator* > postPropagators;
        
        vector< GUSData* > gusDataVector;

        struct DeletionCounters
        {
            Activity increment;
            Activity decrement;

            double learnedSizeFactor;
            double learnedSizeIncrement;
            double maxLearned;
            unsigned int learnedSizeAdjustStartConfl;
            double learnedSizeAdjustConfl;
            double learnedSizeAdjustIncrement;
            unsigned int learnedSizeAdjustCnt;
            
            void init()
            {
                increment = 1 ;
                decrement = 1/0.999;
                learnedSizeFactor = ( ( double ) 1 / ( double) 3 );
                learnedSizeIncrement = 1.1;
                maxLearned = 0.0;
                learnedSizeAdjustStartConfl = 100;
                learnedSizeAdjustConfl = 0.0;
                learnedSizeAdjustCnt = 0;
                learnedSizeAdjustIncrement = 1.5;
            }
        } deletionCounters; 
};

Solver::Solver() 
: 
    currentDecisionLevel( 0 ),
    conflictLiteral( NULL ),
    conflictClause( NULL ),
    learning( *this ),
    outputBuilder( NULL ),
    restart( NULL ),
    conflictAtLevelZero( false ),
    assignedVariablesAtLevelZero( MAXUNSIGNEDINT ),
    nextValueOfPropagation( 0 ),
    literalsInClauses( 0 ),
    literalsInLearnedClauses( 0 )    
{
    satelite = new Satelite( *this );
    deletionCounters.init();
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

Variable*
Solver::addVariableInternal()
{
    Variable* variable = new Variable( variables.numberOfVariables() + 1 );
    variables.push_back( variable );
    assert( variables.numberOfVariables() == variable->getId() );
    minisatHeuristic.onNewVariable( variable );
    learning.onNewVariable();
    satelite->onAddingVariable( variable );
    return variable;
}

void
Solver::addVariable( 
    const string& name )
{    
    Variable* variable = addVariableInternal();
    VariableNames::setName( variable, name );    
}

void
Solver::addVariable()
{
    addVariableInternal();
}

Literal
Solver::getLiteral(
    int lit )
{
    assert( "Lit is out of range." && static_cast< unsigned >( abs( lit ) ) <= variables.numberOfVariables() && abs( lit ) > 0);
    return lit > 0 ? Literal( variables[ lit ], POSITIVE ) : Literal( variables[ -lit ], NEGATIVE );
}

Variable*
Solver::getVariable(
    unsigned int var )
{
    assert_msg( ( var > 0 && var <= variables.numberOfVariables() ), "Variable id " << var << " is greater than the number of variables: " << numberOfVariables() );
    return variables[ var ];
}

void
Solver::init()
{
    variables.init();    
}

void
Solver::assignLiteral(
    Literal literal )
{
    if( !variables.assign( currentDecisionLevel, literal ) )
    {
        conflictLiteral = literal;
        conflictClause = NULL; 
    }
}

void
Solver::assignLiteral(
    Clause* implicant )
{
    assert( implicant != NULL );
    if( !variables.assign( currentDecisionLevel, implicant ) )
    {
        conflictLiteral = implicant->getAt( 0 );
        conflictClause = implicant;        
    }
}

bool
Solver::addClause(
    Clause* clause )
{
    assert( clause != NULL );
    unsigned int size = clause->size();    
    if( size > 1 )
    {
        statistics( onAddingClause( size ) );
//        clause->attachClause();
        clause->attachClauseToAllLiterals();
        clause->setPositionInSolver( clauses.size() );
        clauses.push_back( clause );
        return true;
    }

    if( size == 1 )
    {
        Literal literal = clause->getAt( 0 );
        if( !literal.isTrue() && !propagateLiteralAsDeterministicConsequence( literal ) )
        {
            conflictLiteral = literal;
        }
        else
        {
            releaseClause( clause );
//            delete clause;
            return true;
        }
    }

    conflictAtLevelZero = true;
    releaseClause( clause );
//    delete clause;
    return false;
}

bool
Solver::addClauseFromModel(
    Clause* clause )
{
    assert( clause != NULL );
    unsigned int size = clause->size();
    assert( size != 0 );
    
    if( size > 1 )
    {
        statistics( onAddingClause( size ) );
        clause->attachClause();
        clause->setPositionInSolver( clauses.size() );
        clauses.push_back( clause );
        return true;
    }
    else
    {
        assignLiteral( clause->getAt( 0 ) );
        releaseClause( clause );

//        clearConflictStatus();
//        while( hasNextVariableToPropagate() )
//        {
//            nextValueOfPropagation--;
//            Variable* variableToPropagate = getNextVariableToPropagate();
//            propagateWithPropagators( variableToPropagate );
//
//            if( conflictDetected() )            
//                return false;
//        }
    }

    return true;
}

void
Solver::addLearnedClause( 
    Clause* learnedClause )
{
    assert( learnedClause != NULL );
    learnedClause->attachClause();
    learnedClauses.push_back( learnedClause );
    literalsInLearnedClauses += learnedClause->size();
}

Variable*
Solver::getNextVariableToPropagate()
{
    assert( variables.hasNextVariableToPropagate() );
    return variables.getNextVariableToPropagate();
}
        
bool
Solver::hasNextVariableToPropagate() const
{
    return variables.hasNextVariableToPropagate();
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
    minisatHeuristic.onUnrollingVariable( variables.unrollLastVariable() );
}

void
Solver::unrollOne()
{
    unroll( currentDecisionLevel - 1 );
}

void
Solver::doRestart()
{
    statistics( onRestart() );
    trace( solving, 2, "Performing restart.\n" );
    restart->onRestart();
    unroll( 0 );
}

void
Solver::deleteLearnedClause( 
    ClauseIterator iterator )
{
    Clause* learnedClause = *iterator;
    trace_msg( solving, 4, "Deleting learned clause " << *learnedClause );
    learnedClause->detachClause();
    literalsInLearnedClauses -= learnedClause->size();
    releaseClause( learnedClause );
//    delete learnedClause;
//    learnedClauses.erase( iterator );
}

void
Solver::deleteClause( 
    Clause* clause )
{
    unsigned int position = clause->getPositionInSolver();
    assert_msg( position < clauses.size(), "Position " << position << " is greater than size of the clauses vector " << clauses.size() );
    assert_msg( clause == clauses[ position ], "The clause to delete " << *clause << " is not equal to the clause " << clauses[ position ] << " in position " << position  );
    trace_msg( solving, 4, "Deleting clause " << *clause );

    clauses[ position ] = clauses.back();
    trace_msg( solving, 6, "Swapping clause " << *clause << " and " << *clauses[ position ] );
    clauses[ position ]->setPositionInSolver( position );
    clauses.pop_back();
//    delete clause;
    releaseClause( clause );
}

void
Solver::removeClauseNoDeletion( 
    Clause* clause )
{
    satelite->onDeletingClause( clause );
    unsigned int position = clause->getPositionInSolver();
    assert_msg( position < clauses.size(), "Position " << position << " is greater than size of the clauses vector " << clauses.size() );
    assert_msg( clause == clauses[ position ], "The clause to delete " << *clause << " is not equal to the clause " << clauses[ position ] << " in position " << position  );
    trace_msg( solving, 4, "Deleting clause " << *clause );

    clauses[ position ] = clauses.back();
    trace_msg( solving, 6, "Swapping clause " << *clause << " and " << *clauses[ position ] );
    clauses[ position ]->setPositionInSolver( position );
    clauses.pop_back();
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
    return conflictLiteral != Literal::null;
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
    Literal choice = minisatHeuristic.makeAChoice();
    trace( solving, 1, "Choice: %s.\n", toString( choice ).c_str() );
    setAChoice( choice );    
    statistics( onChoice() );
}

bool
Solver::analyzeConflict()
{
    Clause* learnedClause = learning.onConflict( conflictLiteral, conflictClause );
    assert( "Learned clause has not been calculated." && learnedClause != NULL );
    statistics( onLearning( learnedClause->size() ) );
    
    if( learnedClause->size() == 1 )
    {
        doRestart();
        assignLiteral( learnedClause->getAt( 0 ) );
//        delete learnedClause;
        releaseClause( learnedClause );

        clearConflictStatus();
        while( hasNextVariableToPropagate() )
        {
            nextValueOfPropagation--;            
            Variable* variableToPropagate = getNextVariableToPropagate();
            propagateWithPropagators( variableToPropagate );

            if( conflictDetected() )
                return false;
        }
        
        simplifyOnRestart();
    }
    else
    {
        //Be careful. UIP should be always in position 0.
        assert( learnedClause->getAt( 0 ).getDecisionLevel() == currentDecisionLevel );
        assert( learnedClause->getAt( 1 ).getDecisionLevel() == learnedClause->getMaxDecisionLevel( 1, learnedClause->size() ) );
        addLearnedClause( learnedClause );

        if( restart->hasToRestart() )
        {
            doRestart();
            simplifyOnRestart();
        }
        else
        {
            assert( learnedClause->getAt( 1 ).getDecisionLevel() != 0 );
            assert( "Backjumping level is not valid." && learnedClause->getAt( 1 ).getDecisionLevel() < currentDecisionLevel );
            trace( solving, 2, "Learned clause and backjumping to level %d.\n", learnedClause->getAt( 1 ).getDecisionLevel() );
            unroll( learnedClause->getAt( 1 ).getDecisionLevel() );    
            
            assert( "Each learned clause has to be an asserting clause." && learnedClause->getAt( 0 ) != Literal::null );
            
            assignLiteral( learnedClause );
            
            onLearning( learnedClause );  // FIXME: this should be moved outside
        }
        
        clearConflictStatus();
    }

    if( --deletionCounters.learnedSizeAdjustCnt == 0 )
    {
        deletionCounters.learnedSizeAdjustConfl *= deletionCounters.learnedSizeAdjustIncrement;
        deletionCounters.learnedSizeAdjustCnt = ( unsigned int ) deletionCounters.learnedSizeAdjustConfl;
        deletionCounters.maxLearned *= deletionCounters.learnedSizeIncrement;
    }

    return true;
}

void
Solver::clearConflictStatus()
{
    resetPostPropagators();
    conflictLiteral = Literal::null;
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
    assert( choice != Literal::null );
    incrementCurrentDecisionLevel();
    assert( choice.isUndefined() );
    assignLiteral( choice );
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
    assignLiteral( literal );
    if( conflictDetected() )
        return false;

    while( hasNextVariableToPropagate() )
    {
        Variable* variableToPropagate = getNextVariableToPropagate();
        propagateAtLevelZero( variableToPropagate );

        if( conflictDetected() )
            return false;
    }    
    assert( !conflictDetected() );

    return true;
}

bool
Solver::propagateLiteralAsDeterministicConsequenceSatelite(
    Literal literal )
{
    assignLiteral( literal );
    if( conflictDetected() )
        return false;

    while( hasNextVariableToPropagate() )
    {
        Variable* variableToPropagate = getNextVariableToPropagate();        
        propagateAtLevelZeroSatelite( variableToPropagate );

        if( conflictDetected() )
            return false;
    }    
    assert( !conflictDetected() );

    return true;
}

void
Solver::attachWatches()
{
    for( unsigned int i = 0; i < clauses.size(); )
    {
        Clause* current = clauses[ i ];
        if( current->hasBeenDeleted() )
        {
            deleteClause( current );
        }
        else
        {
            literalsInClauses += current->size();
            current->attachClause();
            ++i;
        }
    }    
}

bool
Solver::preprocessing()
{
    if( conflictDetected() || conflictAtLevelZero )
    {
        trace( solving, 1, "Conflict at level 0.\n" );
        return false;
    }    

    statistics( beforePreprocessing( numberOfVariables() - numberOfAssignedLiterals(), numberOfClauses() ) );
    assert( satelite != NULL );        
    if( !satelite->simplify() )
        return false;

    minisatHeuristic.simplifyVariablesAtLevelZero();    
    attachWatches();    
    
    assignedVariablesAtLevelZero = numberOfAssignedLiterals();
    
    deletionCounters.maxLearned = numberOfClauses() * deletionCounters.learnedSizeFactor;
    deletionCounters.learnedSizeAdjustConfl = deletionCounters.learnedSizeAdjustStartConfl;
    deletionCounters.learnedSizeAdjustCnt = ( unsigned int ) deletionCounters.learnedSizeAdjustConfl;
    
    statistics( afterPreprocessing( numberOfVariables() - numberOfAssignedLiterals(), numberOfClauses() ) );

    return true;
}

void
Solver::onLearning(
    Clause* learnedClause )
{
    updateActivity( learnedClause );
    decrementActivity();    
}

bool
Solver::hasToDelete()
{
    return ( ( int ) ( numberOfLearnedClauses() - numberOfAssignedLiterals() ) >= deletionCounters.maxLearned );
}

void
Solver::setRestart(
    Restart* r )
{
    if( restart != NULL )
        delete restart;
    
    assert( r != NULL );    
    restart = r;
}

void
Solver::onEliminatingVariable(
    Variable* variable,
    unsigned int sign,
    Clause* definition )
{
    variables.onEliminatingVariable( variable, sign, definition );
    eliminatedVariables.push_back( variable );
}

void
Solver::completeModel()
{
    trace_msg( satelite, 5, "Completing the model for eliminated variables" );  
    for( int i = eliminatedVariables.size() - 1; i >= 0; i-- )    
    {
        Variable* back = eliminatedVariables[ i ];

        assert( back->hasBeenEliminated() );
        unsigned int sign = back->getSignOfEliminatedVariable();
    
        trace_msg( satelite, 5, "Processing variable " << *back );
        if( sign == ELIMINATED_BY_DISTRIBUTION )
        {
            trace_msg( satelite, 5, "Eliminated by distribution " << *back );
            bool found = false;            
            Literal positiveLiteral( back, POSITIVE );
            positiveLiteral.startIterationOverOccurrences();
            while( positiveLiteral.hasNextOccurrence() )
            {
                Clause* clause = positiveLiteral.nextOccurence();
                assert( clause->hasBeenDeleted() );
                if( !clause->isSatisfied() )
                {
                    back->setUndefinedBrutal();                    
                    #ifndef NDEBUG
                    bool result =
                    #endif
                    positiveLiteral.setTrue();
                    assert( result );
                    found = true;
                    
                    trace_msg( satelite, 5, "Clause " << *clause << " is not satisfied: inferring " << positiveLiteral );
                    break;
                }
            }            
            if( !found )
            {
                Literal negativeLiteral( back, NEGATIVE );
                negativeLiteral.startIterationOverOccurrences();
                while( negativeLiteral.hasNextOccurrence() )
                {
                    Clause* clause = negativeLiteral.nextOccurence();                    
                    assert( clause->hasBeenDeleted() );
                    if( !clause->isSatisfied() )                        
                    {
                        back->setUndefinedBrutal();

                        #ifndef NDEBUG
                        bool result =
                        #endif
                        negativeLiteral.setTrue();                            
                        assert( result );
                        
                        trace_msg( satelite, 5, "Clause " << *clause << " is not satisfied: inferring " << negativeLiteral );
                        break;
                    }
                }
            }
        }
        else
        {            
            assert( sign == POSITIVE || sign == NEGATIVE );
        
            Literal literal( back, sign );
            back->setUndefinedBrutal();
            const Clause* definition = back->getDefinition();
            trace_msg( satelite, 5, "Considering variable " << *back << " and its definition " << *definition << " which is " << ( definition->isSatisfied() ? "satisfied" : "unsatisfied" ) );
            #ifndef NDEBUG
            bool result =
            #endif
            definition->isSatisfied() ? literal.getOppositeLiteral().setTrue() : literal.setTrue();                

            assert( result );
            trace_msg( satelite, 5, "Inferring " << ( definition->isSatisfied() ? literal.getOppositeLiteral() : literal ) );            
        }
    }
}

Clause*
Solver::newClause()
{
    if( poolOfClauses.empty() )
    {
        unsigned int bufferSize = 20;
        for( unsigned int i = 0; i < bufferSize; i++ )
            poolOfClauses.push_back( new Clause() );       
    }
    
    Clause* back = poolOfClauses.back();
    poolOfClauses.pop_back();
    return back;
}

void
Solver::releaseClause(
    Clause* clause )
{
    clause->free();    
    poolOfClauses.push_back( clause );
}

void
Solver::addPostPropagator(
    PostPropagator* postPropagator )
{
    if( !postPropagator->hasBeenAdded() )
    {
        postPropagators.push_back( postPropagator );
        postPropagator->onAdding();
    }
}

void
Solver::resetPostPropagators()
{
    while( !postPropagators.empty() )
    {
        PostPropagator* postPropagator = postPropagators.back();        
        postPropagators.pop_back();        
        postPropagator->onRemoving();
    }
}

void
Solver::propagateWithPropagators(
    Variable* variable )
{
    unitPropagation( variable );
    postPropagation( variable );
}

#endif	/* SOLVER_H */

