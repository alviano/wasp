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

#include "Solver.h"

#include "../inputBuilders/AbstractBuilder.h"
#include "../AuxLiteral.h"
#include "../NegativeLiteral.h"
#include "../PositiveLiteral.h"

Solver::~Solver()
{
    assignedLiterals.clear();
    undefinedLiterals.clear();
    literalsToPropagate.clear();
    
    for( unsigned int i = 1; i < positiveLiterals.size(); i++ )
    {
        delete positiveLiterals[ i ];
    }
    
    for( unsigned int i = 0; i < auxLiterals.size(); i++ )
    {
        delete auxLiterals[ i ];
    }
    
    while( !clauses.empty() )
    {
        delete clauses.back();
        clauses.pop_back();
    }
    
    while( !learnedClauses.empty() )
    {
        delete learnedClauses.back();
        learnedClauses.pop_back();
    }
        
    if( learningStrategy )
        delete learningStrategy;
    
    if( deletionStrategy )
        delete deletionStrategy;
    
    if( heuristicCounterFactoryForLiteral )
        delete heuristicCounterFactoryForLiteral;
    
    if( decisionHeuristic )
        delete decisionHeuristic;
    
    if( outputBuilder )
        delete outputBuilder;
}

void
Solver::addVariable( 
    const string& name )
{    
    PositiveLiteral* positiveLiteral = new PositiveLiteral( name );
    addVariableInternal( positiveLiteral );
}

void
Solver::addVariable()
{
    PositiveLiteral* positiveLiteral = new PositiveLiteral();
    addVariableInternal( positiveLiteral );
}

AuxLiteral*
Solver::addAuxVariable()
{
    AuxLiteral* auxLiteral = new AuxLiteral();
    NegativeLiteral* negativeLiteral = new NegativeLiteral();
    
    negativeLiteral->setOppositeLiteral( auxLiteral );
    auxLiteral->setOppositeLiteral( negativeLiteral );
    
    auxLiterals.push_back( auxLiteral );
    undefinedLiterals.insert( auxLiteral );
    
    auxLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
    negativeLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
    
    return auxLiteral;
}

void
Solver::addVariableInternal(
    PositiveLiteral* positiveLiteral )
{
    NegativeLiteral* negativeLiteral = new NegativeLiteral();
    
    negativeLiteral->setOppositeLiteral( positiveLiteral );
    positiveLiteral->setOppositeLiteral( negativeLiteral );
    
    positiveLiterals.push_back( positiveLiteral );
    undefinedLiterals.insert( positiveLiteral );
    
    positiveLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
    negativeLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
}

void 
Solver::onLiteralAssigned(
    Literal* literal,
    TruthValue truthValue,
    Clause* implicant )
{
    assert( "Assigned literal is NULL." && literal != NULL );
    assert( "TruthValue has an invalid value." && ( truthValue == TRUE || truthValue == FALSE ) );
    
    PositiveLiteral* positiveLiteral = literal->getPositiveLiteral();

    assert( positiveLiteral != NULL );
    if( undefinedLiterals.erase( positiveLiteral ) )
    {
        assignedLiterals.push_back( positiveLiteral );
        literalsToPropagate.push_back( literal );
        literal->setDecisionLevel( currentDecisionLevel );
        literal->setImplicant( implicant );
        truthValue == TRUE ? !literal->setTrue() : !literal->setFalse();
    }
    else
    {
        conflict = truthValue == TRUE ? !literal->setTrue() : !literal->setFalse();
        if( conflict )
        {
            conflictLiteral = literal;
            conflictClause = implicant;
        }
    }
}

void
Solver::unroll(
    unsigned int level )
{
    assert( "Level is not valid." && level < unrollVector.size() && currentDecisionLevel >= level );
    assert( "Vector for unroll is inconsistent" && assignedLiterals.size() >= unrollVector[ level ] );    
    unsigned int toUnroll = assignedLiterals.size() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        PositiveLiteral* tmp = assignedLiterals.back();
        assignedLiterals.pop_back();
        tmp->setUndefined();        
        this->undefinedLiterals.insert( tmp );
        toUnroll--;
    }
    
    while( toPop > 0 )
    {
        unrollVector.pop_back();
        toPop--;
    }
    literalsToPropagate.clear();
}

Literal*
Solver::getLiteral(
    int lit )
{
    assert( "Lit is out of range." && static_cast< unsigned >( abs( lit ) ) < positiveLiterals.size() );
    Literal* literal;
    if( lit > 0 )
        literal = positiveLiterals[ lit ];
    else
        literal = positiveLiterals[ -lit ]->getNegativeLiteral();
    
    assert( literal != NULL );
    return literal;
}

void
Solver::onDeletingLearnedClausesThresholdBased()
{
    for( List< LearnedClause* >::iterator it = learnedClauses.begin(); it != learnedClauses.end(); )
    {
        List< LearnedClause* >::iterator tmp_it = it++;
        LearnedClause* currentClause = *tmp_it;
        if( deletionStrategy->hasToDeleteClauseThreshold( currentClause ) )
        {
            deleteLearnedClause( currentClause, tmp_it );
        }
    }
}

void
Solver::onDeletingLearnedClausesAvgBased()
{
    for( List< LearnedClause* >::iterator it = learnedClauses.begin(); it != learnedClauses.end() && deletionStrategy->continueIterationAvg(); )
    {
        List< LearnedClause* >::iterator tmp_it = it++;
        LearnedClause* currentClause = *tmp_it;
        if( deletionStrategy->hasToDeleteClauseAvg( currentClause ) )
        {
            deleteLearnedClause( currentClause, tmp_it );
        }
    }
}

void
Solver::decreaseLearnedClausesActivity()
{
    for( List< LearnedClause* >::iterator it = learnedClauses.begin(); it != learnedClauses.end(); ++it )
    {
        LearnedClause* currentClause = *it;
        currentClause->decreaseActivity();
    }
}

bool
Solver::preprocessing()
{
    for( list< Literal* >::iterator it = trueLiterals.begin(); it != trueLiterals.end(); ++it )
    {
        Literal* literal = *it;
        onLiteralAssigned( literal, TRUE, NULL );
        
        if( conflictDetected() )
        {
            return false;
        }
        
        while( hasNextLiteralToPropagate() )
        {
            Literal* literalToPropagate = getNextLiteralToPropagate();
            literalToPropagate->setOrderInThePropagation( numberOfAssignedLiterals() );           
            propagate( literalToPropagate );
            
            if( conflictDetected() )
            {
                return false;
            }
        }
    }
    assert( !conflictDetected() );
    return true;
}

bool 
Solver::solve()
{
    init();
    if( !preprocessing() )
    {
        foundIncoherence();
        return false;
    }
    
    while( hasUndefinedLiterals() )
    {
        /*
        static unsigned int PROVA = 0;
        static time_t PROVA_TIME = time( 0 );
        
        if( ++PROVA > 2000000 ) {
            cerr << "PROVA END!" << endl;
            return false;
        }
        else if( ++PROVA % 10000 == 0 )
        {
            cout << PROVA << " " << learnedClauses.size() <<  " " << ( time( 0 ) - PROVA_TIME ) << endl;
        }
        //*/
        
        assert( !conflictDetected() );
        chooseLiteral();

        while( hasNextLiteralToPropagate() )
        {            
            Literal* literalToPropagate = getNextLiteralToPropagate();
            
            literalToPropagate->setOrderInThePropagation( numberOfAssignedLiterals() );
            propagate( literalToPropagate );
            
            if( conflictDetected() )
            {
                if( getCurrentDecisionLevel() == 0 )
                {
                    foundIncoherence();
                    return false;
                }

                analyzeConflict();
                assert( hasNextLiteralToPropagate() || getCurrentDecisionLevel() == 0 );
            }
        }
    }
    
    printAnswerSet();
    return true;
}