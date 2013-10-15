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

#include "inputBuilders/AbstractBuilder.h"

Solver::~Solver()
{
//    for( unsigned int i = 0; i < auxLiterals.size(); i++ )
//    {
//        delete auxLiterals[ i ];
//    }
    
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
    Variable* variable = new Variable( name );
    addVariableInternal( variable );
}

void
Solver::addVariable()
{
    Variable* variable = new Variable();
    addVariableInternal( variable );
}

//AuxLiteral*
//Solver::addAuxVariable()
//{
//    AuxLiteral* auxLiteral = new AuxLiteral();
//    NegativeLiteral* negativeLiteral = new NegativeLiteral();
//    
//    negativeLiteral->setOppositeLiteral( auxLiteral );
//    auxLiteral->setOppositeLiteral( negativeLiteral );
//    
//    auxLiterals.push_back( auxLiteral );
//    undefinedLiterals.insert( auxLiteral );
//    
//    auxLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
//    negativeLiteral->setHeuristicCounter( heuristicCounterFactoryForLiteral );
//    
//    return auxLiteral;
//}

void
Solver::addVariableInternal(
    Variable* variable )
{
    variables.push_back( variable );
    
    variable->setHeuristicCounterForLiterals( heuristicCounterFactoryForLiteral );    
}

void 
Solver::onLiteralAssigned(
    Literal literal,
    Clause* implicant )
{
    if( !variables.assign( literal, currentDecisionLevel, implicant ) )
    {
        conflict = true;
        //conflict = !literal.setTrue();
        //if( conflict )
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
    assert( "Vector for unroll is inconsistent" && variables.numberOfAssignedLiterals() >= unrollVector[ level ] );    
    unsigned int toUnroll = variables.numberOfAssignedLiterals() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        unrollLastVariable();
        toUnroll--;
    }
    
    while( toPop > 0 )
    {
        unrollVector.pop_back();
        toPop--;
    }
    
    variables.resetLiteralsToPropagate();
}

Literal
Solver::getLiteral(
    int lit )
{
    assert( "Lit is out of range." && static_cast< unsigned >( abs( lit ) ) < variables.numberOfVariables() );
    if( lit > 0 )
    {
        Literal literal( variables[ lit ] );
        return literal;
    }
    else
    {
        Literal literal( variables[ -lit ], false );
        return literal;
    }    
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
Solver::addClauseFromModelAndRestart()
{
    assert( variables.numberOfAssignedLiterals() > 0 );
    
    Clause* clause = new Clause();
    
    Literal lastInsertedLiteral;
    
    for( unsigned int i = 1; i < variables.numberOfVariables(); i++ )
    {
        Variable* v = variables[ i ];
        assert( !v->isUndefined() );
        if( !v->hasImplicant() && v->getDecisionLevel() != 0 )
        {
            if( v->isTrue() )
            {
                Literal lit( v, false );
                lastInsertedLiteral = lit;
                clause->addLiteral( lit );
            }
            else
            {
                Literal lit( v );
                lastInsertedLiteral = lit;
                clause->addLiteral( lit );
            }
        }
    }
    
    if( clause->size() == 0 )
    {
        delete clause;
        return false;
    }
    
    this->onRestarting();
    if( clause->size() > 1 )
    {
        clause->attachClause();
        addClause( clause );
    }
    else
    {        
        delete clause;        
        if( !propagateLiteralAsDeterministicConsequence( lastInsertedLiteral ) )
            return false;
        
    }
    
    return true;
}

bool
Solver::preprocessing()
{
    unsigned size = trueLiterals.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        Literal literal = trueLiterals[ i ];
        if( !propagateLiteralAsDeterministicConsequence( literal ) )
            return false;
//        onLiteralAssigned( literal, NULL );
//        
//        if( conflictDetected() )
//        {
//            return false;
//        }
//        
//        while( hasNextLiteralToPropagate() )
//        {
//            Literal literalToPropagate = getNextLiteralToPropagate();
////            literalToPropagate.setOrderInThePropagation( numberOfAssignedLiterals() );
//            propagate( literalToPropagate );
//            
//            if( conflictDetected() )
//            {
//                return false;
//            }
//        }
    }
    assert( !conflictDetected() );
    return true;
}

bool 
Solver::solve()
{
    while( hasUndefinedLiterals() )
    {
        /*
        static unsigned int PROVA = 0;
        static time_t PROVA_TIME = time( 0 );


        unsigned int end = 3000000;
        unsigned int printValue = 10000;

        if( ++PROVA > end ) {
            cerr << "PROVA END!" << endl;
            return false;
        }
        else if( ++PROVA % printValue == 0 )
        {
            cout << PROVA << " " << learnedClauses.size() <<  " " << ( time( 0 ) - PROVA_TIME ) << endl;
        }
        //*/

        assert( !conflictDetected() );
        chooseLiteral();
        
        while( hasNextLiteralToPropagate() )
        {
            Literal literalToPropagate = getNextLiteralToPropagate();            
            propagate( literalToPropagate );

            if( conflictDetected() )
            {
                if( getCurrentDecisionLevel() == 0 )
                {                        
                    return false;
                }

                analyzeConflict();
                assert( hasNextLiteralToPropagate() || getCurrentDecisionLevel() == 0 );
            }
        }
    }    
    
    return true;
}
