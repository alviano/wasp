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
#define SOLVER_H

#include <cassert>
#include <vector>
using namespace std;

#include "Clause.h"
#include "Variables.h"
#include "Literal.h"
#include "util/Options.h"
#include "util/Trace.h"
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
#include "Aggregate.h"
#include "util/Constants.h"
#include "WatchedList.h"

struct DataStructures
{    
    WatchedList< Clause* > variableWatchedLists;
    Vector< Clause* > variableAllOccurrences;
    Vector< pair< PostPropagator*, int > > variablePostPropagators;
    Vector< pair< Propagator*, int > > variablePropagators;
    Vector< Literal > variableBinaryClauses;
};

class Solver
{
    public:
        inline Solver();
        ~Solver();
        
        inline void greetings(){ outputBuilder->greetings(); }
        
        inline void init();
        bool solve();
        bool solvePropagators();
        inline void propagate( Var v );
        inline void propagateWithPropagators( Var variable );                
        void propagateAtLevelZero( Var variable );
        void propagateAtLevelZeroSatelite( Var variable );
        void unitPropagation( Var v );
        void shortPropagation( Var v );
        void propagation( Var v );
        void postPropagation( Var v );        

        inline bool preprocessing();
        inline void attachWatches();
        inline void clearVariableOccurrences();
        
//        inline void addVariable( const string& name );
        inline void addVariable();        
        
        inline bool cleanAndAddClause( Clause* clause );
        inline bool addClause( Literal literal );
        inline bool addClause( Clause* clause );
        inline void addClause( Literal lit1, Literal lit2 );
        
        inline bool addClauseFromModel( Clause* clause );
        inline void addLearnedClause( Clause* learnedClause );
        bool addClauseFromModelAndRestart();
        
        inline Literal getLiteral( int lit );
//        inline Var getVariable( unsigned int var );
        
        inline Var getNextVariableToPropagate();
        inline bool hasNextVariableToPropagate() const;        
        
        inline Literal getOppositeLiteralFromLastAssignedVariable();
        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();

        inline unsigned int getCurrentDecisionLevel();
        inline void incrementCurrentDecisionLevel();
        
        inline void assignLiteral( Literal literal );
        inline void assignLiteral( Clause* implicant );
        inline void assignLiteral( Literal literal, Reason* implicant );
        
        inline bool propagateLiteralAsDeterministicConsequence( Literal literal );
        inline bool propagateLiteralAsDeterministicConsequenceSatelite( Literal literal );
        
        inline void onLearningClause( Literal literalToPropagate, Clause* learnedClause, unsigned int backjumpingLevel );
        inline void onLearningUnaryClause( Literal literalToPropagate, Clause* learnedClause );        
        inline void doRestart();        
        
        inline unsigned int numberOfClauses() const { return clauses.size(); }
        inline unsigned int numberOfLearnedClauses() const;         
        inline unsigned int numberOfAssignedLiterals() const;
        inline unsigned int numberOfVariables() const;
        
        inline Var getAssignedVariable( unsigned idx ) { return variables.getAssignedVariable( idx ); }
        
        inline void setAChoice( Literal choice );        
        
        inline bool analyzeConflict();
        inline void clearConflictStatus();
        inline void chooseLiteral();
        inline bool conflictDetected();
        inline void foundIncoherence();
        inline bool hasUndefinedLiterals();
        inline void printAnswerSet();
        inline void printOptimizationValue( unsigned int cost );
        inline void optimumFound();
        
        void unroll( unsigned int level );
        inline void unrollOne();
        inline void unrollLastVariable();
        
        /* OPTIONS */
        inline void setOutputBuilder( OutputBuilder* value );        
        
        typedef vector< Clause* >::iterator ClauseIterator;
        typedef vector< Clause* >::reverse_iterator ClauseReverseIterator;
        typedef vector< Clause* >::const_iterator ConstClauseIterator;
        typedef vector< Clause* >::const_reverse_iterator ConstClauseReverseIterator;
        
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
        void printDimacs() const;
        
//        inline void initClauseData( Clause* clause ) { assert( heuristic != NULL ); heuristic->initClauseData( clause ); }
//        inline Heuristic* getHeuristic() { return heuristic; }
        inline void onLiteralInvolvedInConflict( Literal l ) { minisatHeuristic->onLiteralInvolvedInConflict( l ); }
        inline void finalizeDeletion( unsigned int newVectorSize ) { learnedClauses.resize( newVectorSize ); }        
        
        inline void setRestart( Restart* r );
        
        void simplifyOnRestart();
        void removeSatisfied( vector< Clause* >& clauses );

        inline void onEliminatingVariable( Var variable, unsigned int sign, Clause* definition );
        inline void completeModel();
        
        inline Clause* newClause( unsigned reserve = 8 );
        inline void releaseClause( Clause* clause );
        
        inline void addPostPropagator( PostPropagator* postPropagator );
        inline void resetPostPropagators();
        
        inline void addEdgeInDependencyGraph( unsigned int v1, unsigned int v2 ){ trace_msg( parser, 10, "Add arc " << v1 << " -> " << v2 ); dependencyGraph->addEdge( v1, v2 ); }
        inline void computeStrongConnectedComponents()
        {
            dependencyGraph->computeStrongConnectedComponents( gusDataVector );
            if( !dependencyGraph->tight() )
            {            
                for( unsigned int i = 0; i < dependencyGraph->numberOfCyclicComponents(); i++ )
                    cyclicComponents.push_back( dependencyGraph->getCyclicComponent( i ) );
            }
            
            delete dependencyGraph;
        }

        inline bool tight() const { return cyclicComponents.empty(); }
        inline unsigned int getNumberOfCyclicComponents() const { return cyclicComponents.size(); }
        inline Component* getCyclicComponent( unsigned int position ) { return cyclicComponents[ position ]; }
        
        inline void addGUSData( GUSData* gd ) { gusDataVector.push_back( gd ); }        
        
        inline void onStrengtheningClause( Clause* clause ) { satelite->onStrengtheningClause( clause ); }
        
        inline Satelite* getSatelite() { return satelite; }
        
        inline void addAggregate( Aggregate* aggr ) { assert( aggr != NULL ); aggregates.push_back( aggr ); }
        inline bool hasPropagators() const { return ( !tight() || !aggregates.empty() || hasOptimizationAggregate() ); }
        
        inline void turnOffSimplifications() { callSimplifications_ = false; }
        inline bool callSimplifications() const { return callSimplifications_; }
        
        inline void setOptimizationAggregate( Aggregate* optAggregate ) { assert( optAggregate != NULL ); optimizationAggregate = optAggregate; }
        inline void addOptimizationLiteral( Literal lit, unsigned int weight, unsigned int level );
        inline unsigned int computeCostOfModel();
        inline bool updateOptimizationAggregate( unsigned int modelCost );
        inline bool hasOptimizationAggregate() const { return optimizationAggregate != NULL; }
        
        inline unsigned int getCostOfLevel( unsigned int levelIndex, unsigned int totalCost ) const;
        inline void setMaxCostOfLevelOfOptimizationRules( vector< unsigned int >& m ) { maxCostOfLevelOfOptimizationRules.swap( m ); }
        inline void setNumberOfOptimizationLevels( unsigned int n ) { numberOfOptimizationLevels = n; }
        inline void setPrecomputedCost( unsigned int c ) { precomputedCost = c; }
        inline void addPreferredChoicesFromOptimizationLiterals();
        inline void removePrefChoices() { minisatHeuristic->removePrefChoices(); }
        
        inline bool isTrue( Var v ) const { return variables.isTrue( v ); }
        inline bool isFalse( Var v ) const { return variables.isFalse( v ); }        
        inline bool isUndefined( Var v ) const { return variables.isUndefined( v ); }
        
        inline bool isTrue( Literal lit ) const { return variables.isTrue( lit ); }
        inline bool isFalse( Literal lit ) const { return variables.isFalse( lit ); }
        inline bool isUndefined( Literal lit ) const { return variables.isUndefined( lit ); }
        
        inline bool setTruthValue( Var v, TruthValue truth ) { return variables.setTruthValue( v, truth ); }
        inline void setUndefined( Var v ) { variables.setUndefined( v ); }
        inline void setUndefinedBrutal( Var v ) { variables.setUndefinedBrutal( v ); }
        
        inline bool setTrue( Literal lit ) { return variables.setTrue( lit ); }

        inline bool isImplicant( Var v, const Clause* clause ) const { return variables.isImplicant( v, clause ); }
        inline bool hasImplicant( Var v ) const { return variables.hasImplicant( v ); }
        inline void setImplicant( Var v, Reason* clause ) { variables.setImplicant( v, clause ); }
        inline Reason* getImplicant( Var v ) { return variables.getImplicant( v ); }

        inline unsigned int getDecisionLevel( Var v ) const { return variables.getDecisionLevel( v ); }
        inline unsigned int getDecisionLevel( Literal lit ) const { return getDecisionLevel( lit.getVariable() ); }
        inline void setDecisionLevel( Var v, unsigned int decisionLevel ) { variables.setDecisionLevel( v, decisionLevel ); }

        inline TruthValue getTruthValue( Var v ) const { return variables.getTruthValue( v ); }
        inline TruthValue getCachedTruthValue( Var v ) const { return variables.getCachedTruthValue( v ); }
        
        inline void addWatchedClause( Literal lit, Clause* clause ) { getDataStructure( lit ).variableWatchedLists.add( clause ); }
        inline void findAndEraseWatchedClause( Literal lit, Clause* clause ) { getDataStructure( lit ).variableWatchedLists.findAndRemove( clause ); }
        
        inline void addClause( Literal lit, Clause* clause ) { getDataStructure( lit ).variableAllOccurrences.push_back( clause ); }
        inline void findAndEraseClause( Literal lit, Clause* clause ) { getDataStructure( lit ).variableAllOccurrences.findAndRemove( clause ); }

        inline Clause* getOccurrence( Literal lit, unsigned index ) { return getDataStructure( lit ).variableAllOccurrences[ index ]; }
        inline unsigned int numberOfOccurrences( Literal lit ) const { return getDataStructure( lit ).variableAllOccurrences.size(); }
        inline unsigned int numberOfOccurrences( Var v ) const { Literal pos( v, POSITIVE ); return getDataStructure( pos ).variableAllOccurrences.size() + getDataStructure( pos.getOppositeLiteral() ).variableAllOccurrences.size(); }
        
        inline const Clause* getDefinition( Var v ) const { return variables.getDefinition( v ); }
//        inline void setEliminated( Var v, Clause* definition ) { variablesData[ v ].definition = definition; }
        inline void setEliminated( Var v, unsigned int value, Clause* definition ) { variables.setEliminated( v, value, definition ); }
        inline unsigned int getSignOfEliminatedVariable( Var v ) const { return variables.getSignOfEliminatedVariable( v ); }
        inline bool hasBeenEliminated( Var v ) const { return variables.hasBeenEliminated( v ); }
        
        inline bool inTheSameComponent( Var v1, Var v2 ) const { return variables.inTheSameComponent( v1, v2 ); } 
        inline bool isInCyclicComponent( Var v ) const { return variables.isInCyclicComponent( v ); }
        inline void setComponent( Var v, Component* c ){ variables.setComponent( v, c ); }
        inline Component* getComponent( Var v ) { return variables.getComponent( v ); }
        
        inline void addPropagator( Literal lit, Propagator* p, int position ) { getDataStructure( lit ).variablePropagators.push_back( pair< Propagator*, int >( p, position ) ); }
        inline void addPostPropagator( Literal lit, PostPropagator* p, int position ) { getDataStructure( lit ).variablePostPropagators.push_back( pair< PostPropagator*, int >( p, position ) ); }
        inline void addLiteralInShortClause( Literal firstLiteral, Literal secondLiteral ) { getDataStructure( firstLiteral ).variableBinaryClauses.push_back( secondLiteral ); }
                
        bool isFrozen( Var v ) const { return variables.isFrozen( v ); }
        void setFrozen( Var v ) { variables.setFrozen( v ); }
        
        inline unsigned int cost( Var v ) const { Literal pos( v, POSITIVE ); return numberOfOccurrences( pos ) * numberOfOccurrences( pos.getOppositeLiteral() ); }
        
        void checkSubsumptionForClause( Clause* clause, Literal lit );
        
        Literal createFromAssignedVariable( Var v ) const { assert( TRUE == 2 && FALSE == 1 ); return Literal( v, getTruthValue( v ) & 1 ); }
        Literal createOppositeFromAssignedVariable( Var v ) const { assert( TRUE == 2 && FALSE == 1 ); return Literal( v, ~( getTruthValue( v ) ) & 1 ); }
        
        inline void clearOccurrences( Var v );     
        
        inline void attachClause( Clause& );
        inline void attachSecondWatch( Clause& );
//        inline void attachClause( unsigned int firstWatch, unsigned int secondWatch );
        inline void attachClauseToAllLiterals( Clause& );
        inline void detachClause( Clause& );
        inline void detachClauseFromAllLiterals( Clause& );
        inline void detachClauseFromAllLiterals( Clause&, Literal literal );
        
        inline void attachAggregate( Aggregate& );
        
        inline bool isSatisfied( const Clause& clause ) const;
        inline bool allUndefined( const Clause& clause ) const;
        
        inline bool isLocked( const Clause& clause ) const;
        
        inline void markAllClauses( Var v );
        inline void removeAllClauses( Var v );
        inline void removeLiteralAndMarkClause( Clause& clause, Literal literal );
        inline bool removeSatisfiedLiterals( Clause& clause );
        
        inline Literal getLiteralWithMinOccurrences( const Clause& clause ) const;
        inline Var getVariableWithMinOccurrences( const Clause& clause ) const;
        
        inline bool onLiteralFalse( Clause& clause, Literal literal );
        
        inline const DataStructures& getDataStructure( Literal lit ) const { return *variableDataStructures[ lit.getIndex() ]; }
        inline DataStructures& getDataStructure( Literal lit ) { return *variableDataStructures[ lit.getIndex() ]; }        

    private:
        inline void addVariableInternal();
        
        bool checkVariablesState();
        
        Solver( const Solver& ) : learning( *this ), dependencyGraph( NULL )
        {
//            dependencyGraph = new DependencyGraph( *this );
            assert( "The copy constructor has been disabled." && 0 );
        }

        unsigned int currentDecisionLevel;
        Variables variables;
        
        vector< Clause* > clauses;
        vector< Clause* > learnedClauses;
        
        vector< unsigned int > unrollVector;
        
        Literal conflictLiteral;        
        Reason* conflictClause;
        
        Learning learning;
        OutputBuilder* outputBuilder;        
        
        MinisatHeuristic* minisatHeuristic;
        Restart* restart;
        Satelite* satelite;                
        
        unsigned int getNumberOfUndefined() const;
        bool allClausesSatisfied() const;
        
        unsigned int assignedVariablesAtLevelZero;
        int64_t nextValueOfPropagation;
        
        uint64_t literalsInClauses;
        uint64_t literalsInLearnedClauses;
        
        vector< Var > eliminatedVariables;
        vector< Clause* > poolOfClauses;

        DependencyGraph* dependencyGraph;
        Vector< PostPropagator* > postPropagators;
        
        vector< GUSData* > gusDataVector;
        vector< Aggregate* > aggregates;
        
        Aggregate* optimizationAggregate;
        unsigned int numberOfOptimizationLevels;
        unsigned int precomputedCost;                
        
        bool callSimplifications_;
        struct OptimizationLiteralData
        {
            Literal lit;
            unsigned int weight;
            unsigned int level;
        };
        
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
        
        vector< OptimizationLiteralData > optimizationLiterals;
        vector< unsigned int > maxCostOfLevelOfOptimizationRules;        
        
        Vector< DataStructures* > variableDataStructures;
        
        vector< Component* > cyclicComponents;
        
        #ifndef NDEBUG
        bool checkStatusBeforePropagation( Var variable )
        {
            unsigned int sign = ( getTruthValue( variable ) >> 1 );
            assert_msg( sign <= 1, "Sign " << sign << " should be less or equal to 1" );
            assert_msg( getTruthValue( variable ) == TRUE ? sign == NEGATIVE : sign == POSITIVE, "TruthValue of variable does not correspond to the sign" );
            assert_msg( !isUndefined( variable ), "Propagating variable " << this << ", the truth value is Undefined." );
            assert( FALSE == 1 && TRUE == 2 );
            
            return true;
        }
        #endif
};

Solver::Solver() 
: 
    currentDecisionLevel( 0 ),
    conflictLiteral( Literal::null ),
    conflictClause( NULL ),
    learning( *this ),        
    outputBuilder( NULL ),
    restart( NULL ),
    assignedVariablesAtLevelZero( MAXUNSIGNEDINT ),
    nextValueOfPropagation( 0 ),
    literalsInClauses( 0 ),
    literalsInLearnedClauses( 0 ),
    optimizationAggregate( NULL ),
    numberOfOptimizationLevels( 0 ),
    precomputedCost( 0 ),
    callSimplifications_( true )
{
    dependencyGraph = new DependencyGraph( *this );
    satelite = new Satelite( *this );
    minisatHeuristic = new MinisatHeuristic( *this );
    deletionCounters.init();
    VariableNames::addVariable();
    variableDataStructures.push_back( NULL );
    variableDataStructures.push_back( NULL );
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
Solver::addVariableInternal()
{
    VariableNames::addVariable();
    variables.push_back();    
    minisatHeuristic->onNewVariable( variables.numberOfVariables() );
    learning.onNewVariable();
    
    variableDataStructures.push_back( new DataStructures() );
    variableDataStructures.push_back( new DataStructures() );

//    return variables.numberOfVariables();
}

//void
//Solver::addVariable( 
//    const string& name )
//{    
//    Var variable = addVariableInternal();
//    VariableNames::setName( variable, name );    
//}

void
Solver::addVariable()
{
    addVariableInternal();
}

Literal
Solver::getLiteral(
    int lit )
{
//    assert( "Lit is out of range." && static_cast< unsigned >( abs( lit ) ) <= variables.numberOfVariables() && abs( lit ) > 0);
    Var var = lit > 0 ? lit : -lit;
    return lit > 0 ? Literal( var, POSITIVE ) : Literal( var, NEGATIVE );
}

//Var
//Solver::getVariable(
//    unsigned int var )
//{
//    assert_msg( ( var > 0 && var <= variables.numberOfVariables() ), "Variable id " << var << " is greater than the number of variables: " << numberOfVariables() );
//    return variables[ var ];
//}

void
Solver::init()
{
    variables.init();    
}

void
Solver::assignLiteral(
    Literal literal )
{
    assert( !conflictDetected() );
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
    assert( !conflictDetected() );
    if( !variables.assign( currentDecisionLevel, implicant ) )
    {
        conflictLiteral = implicant->getAt( 0 );
        conflictClause = implicant;        
    }
}

void
Solver::assignLiteral(
    Literal lit,
    Reason* implicant )
{
    assert( implicant != NULL );
    
    assert( !conflictDetected() );
    if( !variables.assign( currentDecisionLevel, lit, implicant ) )
    {
        conflictLiteral = lit;
        conflictClause = implicant;        
    }
}

bool
Solver::cleanAndAddClause(
    Clause* clause )
{
    assert( clause != NULL );
    
    if( clause->removeDuplicatesAndFalseAndCheckIfTautological( *this ) )
    {
        releaseClause( clause );
        return true;
    }
    
    if( clause->size() == 0 )
    {
        conflictLiteral = Literal::conflict;
        releaseClause( clause );
        return false;
    }
    
    assert( allUndefined( *clause ) );
    return addClause( clause );
}

bool
Solver::addClause(
    Literal literal )
{
    if( isTrue( literal ) || propagateLiteralAsDeterministicConsequence( literal ) )
        return true;
    
    conflictLiteral = literal;
    return false;
}

void
Solver::addClause(
    Literal lit1,
    Literal lit2 )
{
    addLiteralInShortClause( lit1, lit2 );
    addLiteralInShortClause( lit2, lit1 );        
}

bool
Solver::addClause(
    Clause* clause )
{
    assert( clause != NULL );
    assert( allUndefined( *clause ) );
    assert( !clause->isTautology() );
    
    unsigned int size = clause->size();    
    if( size > 1 )
    {
        statistics( onAddingClause( size ) );
        if( callSimplifications() )
            attachClauseToAllLiterals( *clause );
        clause->setPositionInSolver( clauses.size() );
        clauses.push_back( clause );
        return true;
    }        

    if( size == 1 )
    {
        if( addClause( clause->getAt( 0 ) ) )
        {
            releaseClause( clause );
            return true;
        }
    }

    conflictLiteral = Literal::conflict;
    releaseClause( clause );
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
        attachClause( *clause );
        clause->setPositionInSolver( clauses.size() );
        clauses.push_back( clause );
        return true;
    }
    else
    {
        assert( !conflictDetected() );
        assignLiteral( clause->getAt( 0 ) );
        releaseClause( clause );
    }

    return true;
}

void
Solver::addLearnedClause( 
    Clause* learnedClause )
{
    assert( learnedClause != NULL );
    attachClause( *learnedClause );
    learnedClauses.push_back( learnedClause );
    literalsInLearnedClauses += learnedClause->size();
}

Var
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
    minisatHeuristic->onUnrollingVariable( variables.unrollLastVariable() );
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
    detachClause( *learnedClause );
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
Solver::numberOfLearnedClauses() const
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
Solver::printOptimizationValue(
    unsigned int cost )
{
    outputBuilder->foundModelOptimization( *this, cost, numberOfOptimizationLevels );
}

void
Solver::optimumFound()
{
    outputBuilder->optimumFound();
}

void
Solver::foundIncoherence()
{
    outputBuilder->onProgramIncoherent();
}

void
Solver::chooseLiteral()
{
    Literal choice = minisatHeuristic->makeAChoice();
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
        clearConflictStatus();
        assignLiteral( learnedClause->getAt( 0 ) );
        assert( isTrue( learnedClause->getAt( 0 ) ) );
        assert( !conflictDetected() );
//        delete learnedClause;
        releaseClause( learnedClause );

        while( hasNextVariableToPropagate() )
        {
            nextValueOfPropagation--;            
            Var variableToPropagate = getNextVariableToPropagate();
            if( hasPropagators() )
                propagateWithPropagators( variableToPropagate );
            else
                propagate( variableToPropagate );

            if( conflictDetected() )
                return false;
        }
        
        simplifyOnRestart();
    }
    else
    {
        //Be careful. UIP should be always in position 0.
        assert( getDecisionLevel( learnedClause->getAt( 0 ) ) == currentDecisionLevel );
        assert( getDecisionLevel( learnedClause->getAt( 1 ) ) == learnedClause->getMaxDecisionLevel( *this, 1, learnedClause->size() ) );
        addLearnedClause( learnedClause );

        if( restart->hasToRestart() )
        {
            doRestart();
            simplifyOnRestart();
        }
        else
        {
            assert( getDecisionLevel( learnedClause->getAt( 1 ) ) != 0 );
            assert( "Backjumping level is not valid." && getDecisionLevel( learnedClause->getAt( 1 ) ) < currentDecisionLevel );
            trace( solving, 2, "Learned clause and backjumping to level %d.\n", getDecisionLevel( learnedClause->getAt( 1 ) ) );
            unroll( getDecisionLevel( learnedClause->getAt( 1 ) ) );    
            
            assert( "Each learned clause has to be an asserting clause." && learnedClause->getAt( 0 ) != Literal::null );
            
            clearConflictStatus();
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
Solver::numberOfAssignedLiterals() const
{
    return variables.numberOfAssignedLiterals();
}

unsigned int
Solver::numberOfVariables() const
{
    return variables.numberOfVariables();
}

void
Solver::setAChoice(
    Literal choice )
{
    assert( choice != Literal::null );
    incrementCurrentDecisionLevel();
    assert( isUndefined( choice ) );
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
        Var variableToPropagate = getNextVariableToPropagate();
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
        Var variableToPropagate = getNextVariableToPropagate();        
        propagateAtLevelZeroSatelite( variableToPropagate );

        if( conflictDetected() )
            return false;
    }    
    assert( !conflictDetected() );

    return true;
}

void
Solver::clearVariableOccurrences()
{
    if( callSimplifications() )
    {
        for( unsigned int i = 1; i <= numberOfVariables(); i++ )
            clearOccurrences( i );
    }
//    #ifndef NDEBUG    
//    for( unsigned int i = 1; i <= numberOfVariables(); i++ )
//        assert( getVariable( i )->hasBeenEliminated() || getVariable( i )->numberOfOccurrences() == 0 );    
//    #endif
}

void
Solver::attachWatches()
{
    for( unsigned int i = 0; i < clauses.size(); )
    {
        Clause* currentPointer = clauses[ i ];
        Clause& current = *currentPointer;
        if( current.hasBeenDeleted() )
        {
            deleteClause( currentPointer );
        }
        else
        {                    
            if( current.size() == 2 )
            {     
                addLiteralInShortClause( current[ 0 ], current[ 1 ] );
                addLiteralInShortClause( current[ 1 ], current[ 0 ] );
        
                deleteClause( currentPointer );
            }
            else
            {
                literalsInClauses += current.size();
                attachClause( current );
                ++i;
            }
        }
    }    
}

bool
Solver::preprocessing()
{
    if( conflictDetected() )
    {
        trace( solving, 1, "Conflict at level 0.\n" );
        return false;
    }    

    statistics( beforePreprocessing( numberOfVariables() - numberOfAssignedLiterals(), numberOfClauses() ) );
    assert( satelite != NULL );
    assert( checkVariablesState() );    
    if( callSimplifications() && !satelite->simplify() )
        return false;

    minisatHeuristic->simplifyVariablesAtLevelZero();
    clearVariableOccurrences();
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
    Var variable,
    unsigned int sign,
    Clause* definition )
{
    variables.onEliminatingVariable( variable );
    eliminatedVariables.push_back( variable );
    setEliminated( variable, sign, definition );
}

void
Solver::completeModel()
{
    trace_msg( satelite, 5, "Completing the model for eliminated variables" );  
    for( int i = eliminatedVariables.size() - 1; i >= 0; i-- )    
    {
        Var back = eliminatedVariables[ i ];

        assert( hasBeenEliminated( back ) );
        unsigned int sign = getSignOfEliminatedVariable( back );
    
        trace_msg( satelite, 5, "Processing variable " << back );
        if( sign == ELIMINATED_BY_DISTRIBUTION )
        {
            trace_msg( satelite, 5, "Eliminated by distribution " << back );
            bool found = false;            
            Literal positiveLiteral( back, POSITIVE );
            for( unsigned j = 0; j < numberOfOccurrences( positiveLiteral ); ++j )
            {
                Clause* clause = getOccurrence( positiveLiteral, j );
                assert( clause->hasBeenDeleted() );
                if( !isSatisfied( *clause ) )
                {
                    setUndefinedBrutal( back );
                    #ifndef NDEBUG
                    bool result =
                    #endif
                    setTrue( positiveLiteral );
                    assert( result );
                    found = true;
                    
                    trace_msg( satelite, 5, "Clause " << *clause << " is not satisfied: inferring " << positiveLiteral );
                    break;
                }
            }            
            if( !found )
            {
                Literal negativeLiteral( back, NEGATIVE );
                for( unsigned j = 0; j < numberOfOccurrences( negativeLiteral ); ++j )
                {
                    Clause* clause = getOccurrence( negativeLiteral, j );                    
                    assert( clause->hasBeenDeleted() );
                    if( !isSatisfied( *clause ) )
                    {
                        setUndefinedBrutal( back );

                        #ifndef NDEBUG
                        bool result =
                        #endif
                        setTrue( negativeLiteral );                            
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
            setUndefinedBrutal( back );
            const Clause* definition = getDefinition( back );
            trace_msg( satelite, 5, "Considering variable " << back << " and its definition " << *definition << " which is " << ( isSatisfied( *definition ) ? "satisfied" : "unsatisfied" ) );
            #ifndef NDEBUG
            bool result =
            #endif
            isSatisfied( *definition ) ? setTrue( literal.getOppositeLiteral() ) : setTrue( literal );                

            assert( result );
            trace_msg( satelite, 5, "Inferring " << ( isSatisfied( *definition ) ? literal.getOppositeLiteral() : literal ) );            
        }
    }
}

Clause*
Solver::newClause(
    unsigned reserve )
{
    if( poolOfClauses.empty() )
    {
        unsigned int bufferSize = 20;
        for( unsigned int i = 0; i < bufferSize; i++ )
            poolOfClauses.push_back( new Clause( reserve ) );       
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
        postPropagator->reset();
    }
}

void
Solver::propagate(
    Var variable )
{
    assert( checkStatusBeforePropagation( variable ) );
    trace_msg( solving, 5, "Propagating variable " << variable );
    
    shortPropagation( variable );
    if( conflictDetected() )
        return;
    unitPropagation( variable );
}

void
Solver::propagateWithPropagators(
    Var variable )
{
    propagate( variable );
    if( conflictDetected() )
        return;
    propagation( variable );
    if( conflictDetected() )
        return;
    postPropagation( variable );
    assert( !conflictDetected() );
}

unsigned int
Solver::computeCostOfModel()
{
    unsigned int cost = 0;
    
    for( unsigned int i = 0; i < optimizationLiterals.size(); i++ )
    {
        assert( optimizationLiterals[ i ].lit != Literal::null );        
        if( isTrue( optimizationLiterals[ i ].lit ) )
            cost += optimizationLiterals[ i ].weight;        
    }
    
    return cost;
}

bool
Solver::updateOptimizationAggregate(
    unsigned int modelCost )
{   
    assert( optimizationAggregate != NULL );
    if( precomputedCost >= modelCost )
        return false;
    
    unsigned int backjumpingLevel = optimizationAggregate->getLevelOfBackjump( *this, modelCost - precomputedCost );
    unroll( backjumpingLevel );
    clearConflictStatus();
    
    if( !optimizationAggregate->updateBound( *this, modelCost - precomputedCost ) )
        return false;
    
    assert( isFalse( optimizationAggregate->getLiteral( 1 ).getOppositeLiteral() ) );
    optimizationAggregate->onLiteralFalse( *this, optimizationAggregate->getLiteral( 1 ).getOppositeLiteral(), -1 );
    
    if( conflictDetected() )
        return false;
//        addPostPropagator( optimizationAggregate ); 
    
    return true;
}

void
Solver::addOptimizationLiteral(
    Literal lit,
    unsigned int weight,
    unsigned int level )
{
    OptimizationLiteralData opt;
    opt.lit = lit;
    opt.weight = weight;
    opt.level = level;
    optimizationLiterals.push_back( opt );
}

unsigned int
Solver::getCostOfLevel(
    unsigned int levelIndex,
    unsigned int totalCost ) const
{    
    assert_msg( levelIndex + 1 < maxCostOfLevelOfOptimizationRules.size(), "Index is " << levelIndex << " - Elements " << totalCost );
    return ( totalCost % maxCostOfLevelOfOptimizationRules[ levelIndex + 1 ] ) / maxCostOfLevelOfOptimizationRules[ levelIndex ];
}

void
Solver::addPreferredChoicesFromOptimizationLiterals()
{
    assert( currentDecisionLevel == 0 );
    for( unsigned int i = 0; i < optimizationLiterals.size(); i++ )
    {
        if( isUndefined( optimizationLiterals[ i ].lit ) )
            minisatHeuristic->addPreferredChoice( optimizationLiterals[ i ].lit.getOppositeLiteral() );
    }
}

void
Solver::clearOccurrences(
    Var v )
{
    if( variables.hasBeenEliminatedByDistribution( v ) )
        return;
    
    Literal lit( v, POSITIVE );
    getDataStructure( lit ).variableAllOccurrences.clearAndDelete();
    getDataStructure( lit.getOppositeLiteral() ).variableAllOccurrences.clearAndDelete();    
}

//void
//Solver::attachFirstWatch()
//{
//    assert( "Unary clause must be removed." && literals.size() > 1 );
//    literals[ 0 ].addWatchedClause( this );
//}
//
//void
//Solver::attachSecondWatch()
//{
//    assert( "Unary clause must be removed." && literals.size() > 1 );
//    literals[ 1 ].addWatchedClause( this );
//}

void
Solver::attachClause(
    Clause& clause )
{
    assert_msg( clause.size() > 1, "Clause " << clause << " is unary and it must be removed" );
    addWatchedClause( clause[ 0 ], &clause );
    addWatchedClause( clause[ 1 ], &clause );    
}

void
Solver::attachSecondWatch(
    Clause& clause )
{
    assert_msg( clause.size() > 1, "Clause " << clause << " is unary and it must be removed" );
    addWatchedClause( clause[ 1 ], &clause );    
}

void
Solver::attachClauseToAllLiterals(
    Clause& clause )
{
    unsigned int size = clause.size();
    for( unsigned int i = 0; i < size; i++ )
    {
        addClause( clause[ i ], &clause );
    }
}
//
//void
//Clause::attachClause( 
//    unsigned int first,
//    unsigned int second )
//{
//    assert( "First watch is out of range." && first < literals.size() );
//    assert( "Second watch is out of range." && second < literals.size() );
//    assert( "First watch and second watch point to the same literal." && first != second );   
//    
//    #ifndef NDEBUG
//    Literal tmp1 = literals[ first ];
//    Literal tmp2 = literals[ second ];
//    #endif
//
//    swapLiterals( 0, first );
//    second == 0 ? swapLiterals( 1, first ) : swapLiterals( 1, second );    
//
//    assert( literals[ 0 ] == tmp1 );
//    assert( literals[ 1 ] == tmp2 );
//    
//    attachFirstWatch();
//    attachSecondWatch();
//}

void
Solver::detachClause(
    Clause& clause )
{
    findAndEraseWatchedClause( clause[ 0 ], &clause );
    findAndEraseWatchedClause( clause[ 1 ], &clause );
}

void
Solver::detachClauseFromAllLiterals(
    Clause& clause,
    Literal literal )
{
    for( unsigned int i = 0; i < clause.size(); ++i )
    {
        if( clause[ i ] != literal )
            findAndEraseClause( clause[ i ], &clause );
    }    
}

void
Solver::detachClauseFromAllLiterals(
    Clause& clause )
{
    for( unsigned int i = 0; i < clause.size(); ++i )
        findAndEraseClause( clause[ i ], &clause );    
}

void
Solver::attachAggregate(
    Aggregate& aggregate )
{    
    assert( aggregate.size() > 0 );
    Literal aggregateLiteral = aggregate[ 1 ].getOppositeLiteral();
    addPropagator( aggregateLiteral, &aggregate, -1 );
    addPropagator( aggregateLiteral.getOppositeLiteral(), &aggregate, 1 );    
    for( unsigned int j = 2; j <= aggregate.size(); j++ )
    {
        Literal lit = aggregate[ j ];
        if( !isTrue( aggregateLiteral ) )
            addPropagator( lit.getOppositeLiteral(), &aggregate, -j );
        if( !isFalse( aggregateLiteral ) )
            addPropagator( lit, &aggregate, j );
    }
}

bool
Solver::isSatisfied(
    const Clause& clause ) const
{
    assert( clause.size() > 0 );
    
    unsigned i = 0;
    if( clause.hasBeenDeleted() )
        i = 1;
    
    for( ; i < clause.size(); ++i )
        if( isTrue( clause[ i ] ) )
            return true;
    return false;
}

bool
Solver::allUndefined(
    const Clause& clause ) const
{
    for( unsigned i = 0; i < clause.size(); ++i )
        if( !isUndefined( clause[ i ] ) )
            return false;
    return true;
}

bool
Solver::isLocked(
    const Clause& clause ) const
{
    assert_msg( clause.size() > 1, "Clause " << clause << " is unary and it must be removed" );

    //We assume that the literal inferred is always in the first position.
    return isImplicant( clause[ 0 ].getVariable(), &clause );
}

void 
Solver::markAllClauses(
    Var variable )
{
    Literal pos = Literal( variable, POSITIVE );
    Literal neg = Literal( variable, NEGATIVE );
    
    Vector< Clause* >& posAllOccurrences = getDataStructure( pos ).variableAllOccurrences;
    
    for( unsigned i = 0; i < posAllOccurrences.size(); ++i )
    {
        Clause* clause = posAllOccurrences[ i ];
        detachClauseFromAllLiterals( *clause, pos );        
        markClauseForDeletion( clause );
    }
    posAllOccurrences.clearAndDelete();

    Vector< Clause* >& negAllOccurrences = getDataStructure( neg ).variableAllOccurrences;
    for( unsigned i = 0; i < negAllOccurrences.size(); ++i )
    {
        Clause* clause = negAllOccurrences[ i ];
        detachClauseFromAllLiterals( *clause, neg );
        markClauseForDeletion( clause );
    }
    negAllOccurrences.clearAndDelete();
}

void
Solver::removeAllClauses(
    Var v )
{
    Literal pos( v, POSITIVE );
    Literal neg( v, NEGATIVE );    
    
    Vector< Clause* >& posAllOccurrences = getDataStructure( pos ).variableAllOccurrences;
    for( unsigned i = 0; i < posAllOccurrences.size(); ++i )
    {
        Clause* clause = posAllOccurrences[ i ];
        removeClauseNoDeletion( clause );
        removeLiteralAndMarkClause( *clause, pos );
    }
    posAllOccurrences.clearAndDelete();

    Vector< Clause* >& negAllOccurrences = getDataStructure( neg ).variableAllOccurrences;
    for( unsigned i = 0; i < negAllOccurrences.size(); ++i )
    {
        Clause* clause = negAllOccurrences[ i ];
        removeClauseNoDeletion( clause );
        removeLiteralAndMarkClause( *clause, neg );
    }
    negAllOccurrences.clearAndDelete();
}

void
Solver::removeLiteralAndMarkClause(
    Clause& clause,
    Literal literal )
{
    for( unsigned int i = 0; i < clause.size(); ++i )
    {
        if( clause[ i ] != literal )
            findAndEraseClause( clause[ i ], &clause );
    }
    
    assert( clause.size() != 0 );
    clause.markAsDeleted();
}

Literal
Solver::getLiteralWithMinOccurrences(
    const Clause& clause ) const
{
    assert( clause.size() > 1 );
    Literal minLiteral = clause[ 0 ];
    assert( numberOfOccurrences( minLiteral ) > 0 );

    unsigned int i = 1;
    do
    {
        assert( numberOfOccurrences( clause[ i ] ) > 0 );
        if( numberOfOccurrences( clause[ i ] ) < numberOfOccurrences( minLiteral ) )
        {
            minLiteral = clause[ i ];
        }
    } while( ++i < clause.size() );    
        
    return minLiteral;
}

Var
Solver::getVariableWithMinOccurrences(
    const Clause& clause ) const
{
    assert( clause.size() > 1 );
    assert( !clause.hasBeenDeleted() );
    Var minVariable = clause[ 0 ].getVariable();
    assert_msg( numberOfOccurrences( minVariable ) > 0, "Variable " << minVariable << " does not know to occur in " << clause );

    unsigned int i = 1;
    do
    {
        assert_msg( numberOfOccurrences( clause[ i ].getVariable() ) > 0, "Variable " << clause[ i ].getVariable() << " does not know to occur in " << clause );
        if( numberOfOccurrences( clause[ i ].getVariable() ) < numberOfOccurrences( minVariable ) )
        {
            minVariable = clause[ i ].getVariable();
        }
    } while( ++i < clause.size() );    
        
    return minVariable;
}

bool
Solver::removeSatisfiedLiterals(
    Clause& clause )
{
    if( isTrue( clause[ 0 ] ) )
    {        
        if( isLocked( clause ) )
            setImplicant( clause[ 0 ].getVariable(), NULL );
        return true;        
    }
    
    assert_msg( !isFalse( clause[ 0 ] ), "Literal " << clause[ 0 ] <<  " in clause " << clause << " is false" );
    assert_msg( !isFalse( clause[ 1 ] ), "Literal " << clause[ 1 ] <<  " in clause " << clause << " is false" );

    if( isTrue( clause[ 1 ] ) )
        return true;        
    
    for( unsigned int i = 2; i < clause.size(); )
    {
        if( isTrue( clause[ i ] ) )
            return true;

        if( isFalse( clause[ i ] ) )
        {
            clause[ i ] = clause[ clause.size() - 1 ];
            clause.removeLastLiteralNoWatches();            
        }
        else
        {
            i++;
        }        
    }
        
    return false;
}

bool
Solver::onLiteralFalse(
    Clause& clause,
    Literal literal )
{
    assert_msg( isFalse( literal ), "Literal " << literal << " is not false");
    assert_msg( clause.size() > 1, "Clause " << clause << " is unary and it must be removed" );

    if( literal == clause[ 0 ] )
    {
        //The watch to update should be always in position 1.
        clause[ 0 ] = clause[ 1 ];
        clause[ 1 ] = literal;
    }

    assert_msg( literal == clause[ 1 ], "Literal " << literal << " should be in position 1, but literal " << clause[ 1 ] << "is in position 1" );
    //if the clause is already satisfied do nothing.
    if( isTrue( clause[ 0 ] ) )
        return false;
    
    for( unsigned i = 2; i < clause.size(); ++i )
    {
        if( !isFalse( clause[ i ] ) )
        {
            //Swap the two literals
            clause.swapLiteralsNoWatches( 1, i );            

            //Attach the watch in the new position
            attachSecondWatch( clause );
            return false;
        }
    }

    assert( "The other watched literal cannot be true." && !isTrue( clause[ 0 ] ) );    
    //Propagate clause[ 0 ];
    return true;
}

#endif

