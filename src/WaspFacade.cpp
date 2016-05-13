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

#include "WaspFacade.h"

#include "Restart.h"

#include "input/GringoNumericFormat.h"

#include "outputBuilders/WaspOutputBuilder.h"
#include "outputBuilders/SilentOutputBuilder.h"
#include "outputBuilders/ThirdCompetitionOutputBuilder.h"
#include "outputBuilders/CompetitionOutputBuilder.h"
#include "outputBuilders/DimacsOutputBuilder.h"

#include "MinisatHeuristic.h"
#include "outputBuilders/MultiOutputBuilder.h"
#include "QueryInterface.h"
#include "outputBuilders/IdOutputBuilder.h"

void
WaspFacade::readInput()
{
    char tmp;
    cin >> tmp;

    if( !cin.good() && !cin.eof() )
    {   
        ErrorMessage::errorDuringParsing( "Unexpected symbol." );
    }    

    cin.putback( tmp );
    switch ( tmp )
    {
        case COMMENT_DIMACS:
        case FORMULA_INFO_DIMACS:
        {
            DimacsOutputBuilder* d = new DimacsOutputBuilder();
            solver.setOutputBuilder( d );
            Dimacs dimacs( solver );
            dimacs.parse();
            if( dimacs.isMaxsat() )
                d->setMaxsat();
            greetings();
            break;
        }

        default:
        {
            GringoNumericFormat gringo( solver );
            gringo.parse();
//            solver.setOutputBuilder( new WaspOutputBuilder() );
            greetings();
            break;
        }
    }
}

void
WaspFacade::solve()
{
    if( printProgram )
    {
        solver.printProgram();
        return;
    }
    
    if( solver.preprocessing() )
    {
        if( printDimacs )
        {
            solver.printDimacs();
            return;
        }
        
        if( queryAlgorithm != NO_QUERY )
        {
            QueryInterface queryInterface( solver );
            queryInterface.computeCautiousConsequences( queryAlgorithm );
            statistics( &solver, endSolving() );
            return;
        }        
        
        if( !solver.isOptimizationProblem() )
        {            
            enumerateModels();
        }
        else
        {
            unsigned int result = solveWithWeakConstraints();
            switch( result )
            {
                case COHERENT:
                    cerr << "ERROR";
                    break;
                
                case INCOHERENT:
                    solver.foundIncoherence();
                    break;
                    
                case OPTIMUM_FOUND:
                default:
                    solver.optimumFound();
                    if( maxModels > 1 )
                    {
                        solver.unrollToZero();
                        solver.clearConflictStatus();
                        enumerateModels();
                    }
                    break;
            }
            statistics( &solver, endSolving() );
            return;
        }
    }

    if( numberOfModels == 0 )
    {
        trace_msg( enumeration, 1, "No model found." );
        solver.foundIncoherence();
    }
    statistics( &solver, endSolving() );
    
//    solver.printLearnedClauses();
}

void
WaspFacade::setDeletionPolicy(
    DELETION_POLICY deletionPolicy,
    unsigned int /*deletionThreshold*/ )
{
    switch( deletionPolicy )
    {
//        case RESTARTS_BASED_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new RestartsBasedDeletionStrategy( solver ) );
//            break;
//            
//        case MINISAT_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new MinisatDeletionStrategy( solver ) );
//            break;
//            
//        case GLUCOSE_DELETION_POLICY:
//            heuristic->setDeletionStrategy( new GlueBasedDeletionStrategy( solver, deletionThreshold ) );
//            break;
//
//        case AGGRESSIVE_DELETION_POLICY:
//        default:
//            heuristic->setDeletionStrategy( new AggressiveDeletionStrategy( solver ) );
//            break;
    }
}

void
WaspFacade::setDecisionPolicy(
    DECISION_POLICY decisionPolicy,
    unsigned int /*threshold*/ )
{
    switch( decisionPolicy )
    {
//        case HEURISTIC_BERKMIN:
//            assert( threshold > 0 );
//            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, threshold ) );
//            break;
//            
//        case HEURISTIC_BERKMIN_CACHE:
//            assert( threshold > 0 );
//            heuristic->setDecisionStrategy( new BerkminHeuristicWithCache( solver, threshold ) );            
//            break;
//        
//        case HEURISTIC_FIRST_UNDEFINED:
//            heuristic->setDecisionStrategy( new FirstUndefinedHeuristic( solver ) );
//            break;
//            
        case HEURISTIC_MINISAT:
            solver.setMinisatHeuristic();
            break;
//    
//        default:
//            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, 512 ) );
//            break;
    }
}

void
WaspFacade::setOutputPolicy(
    OUTPUT_POLICY outputPolicy )
{
    switch( outputPolicy )
    {
        case COMPETITION_OUTPUT:
            solver.setOutputBuilder( new CompetitionOutputBuilder() );
            break;
            
        case DIMACS_OUTPUT:
            solver.setOutputBuilder( new DimacsOutputBuilder() );
            break;
            
        case SILENT_OUTPUT:
            solver.setOutputBuilder( new SilentOutputBuilder() );
            break;
            
        case THIRD_COMPETITION_OUTPUT:
            solver.setOutputBuilder( new ThirdCompetitionOutputBuilder() );
            break;
            
        case MULTI:
            solver.setOutputBuilder( new MultiOutputBuilder() );
            break;
            
        case ID_OUTPUT:
            solver.setOutputBuilder( new IdOutputBuilder() );
            break;
            
        case WASP_OUTPUT:
        default:
            solver.setOutputBuilder( new WaspOutputBuilder() );
            break;
    }
}

void
WaspFacade::setRestartsPolicy(
    RESTARTS_POLICY restartsPolicy,
    unsigned int threshold )
{
    assert( threshold > 0 );
    Restart* restart;
    switch( restartsPolicy )
    {
        case GEOMETRIC_RESTARTS_POLICY:
            restart = new Restart( threshold, false );
            solver.setRestart( restart );
            break;            

        case SEQUENCE_BASED_RESTARTS_POLICY:
        default:
            restart = new Restart( threshold, true );
            solver.setRestart( restart );
            break;
    }
}

void
WaspFacade::enumerateModels()
{
    assert( wasp::Options::enumerationStrategy == ENUMERATION_BC || wasp::Options::enumerationStrategy == ENUMERATION_BT );
    if( wasp::Options::enumerationStrategy == ENUMERATION_BC )
        enumerationBlockingClause();
    else
        enumerationBacktracking();    
}

void
WaspFacade::enumerationBlockingClause()
{
    while( solver.solve() == COHERENT )
    {
        solver.printAnswerSet();
        trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
        if( ++numberOfModels >= maxModels )
        {
            trace_msg( enumeration, 1, "Enumerated " << maxModels << "." );
            break;
        }
        else if( !solver.addClauseFromModelAndRestart() )
        {
            trace_msg( enumeration, 1, "All models have been found." );
            break;
        }
    }
}

void
WaspFacade::enumerationBacktracking()
{
    vector< bool > checked;
    while( checked.size() <= solver.numberOfVariables() )
        checked.push_back( false );
    unsigned int result = solver.solve();
    if( result == INCOHERENT )
        return;
        
    vector< Literal > assums;
    if( !foundModel( assums ) )
        return;
    
    flipLatestChoice( assums, checked );
    if( assums.empty() )
        return;
    solver.setComputeUnsatCores( true );
    begin:;
    solver.unrollToZero();
    solver.clearConflictStatus();
    result = solver.solve( assums );
    if( result == INCOHERENT )
    {
        const Clause* core = solver.getUnsatCore();
        assert( core != NULL );
        if( core->size() == 0 )
            assums.clear();
        
        unsigned int bl = solver.getMaxLevelOfClause( core );
        if( bl == 0 )
        {
            unsigned int k = 0;
            for( unsigned int i = 0; i < assums.size(); i++ )
            {
                assums[ k ] = assums[ i ];
                if( solver.getDecisionLevel( assums[ i ] ) != 0 )
                    k++;
            }
            assums.resize( k );
        }
        else
        {
            while( !assums.empty() && solver.getDecisionLevel( assums.back() ) > bl )
                assums.pop_back();
        }
    }
    else if( !foundModel( assums ) )
        return;
    
    flipLatestChoice( assums, checked );
    if( assums.empty() )
        return;        
    goto begin;
}

void
WaspFacade::flipLatestChoice(
    vector< Literal >& choices,
    vector< bool >& checked )
{
    unsigned int size;
    while( true )
    {
        size = choices.size();
        if( size == 0 )
            return;
        if( checked[ size ] )
            choices.pop_back();
        else
            break;
    }
    
    choices[ size - 1 ] = choices[ size - 1 ].getOppositeLiteral();    
    checked[ size ] = true;
    for( unsigned int i = size + 1; i < checked.size(); i++ )
        checked[ i ] = false;
}

bool
WaspFacade::foundModel(
    vector< Literal >& assums )
{
    solver.printAnswerSet();
    trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
    if( ++numberOfModels >= maxModels )
    {
        trace_msg( enumeration, 1, "Enumerated " << maxModels << "." );
        return false;
    }    
    solver.getChoicesWithoutAssumptions( assums );
    return true;
}