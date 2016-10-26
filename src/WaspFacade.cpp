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

#include "outputBuilders/MultiOutputBuilder.h"
#include "QueryInterface.h"
#include "outputBuilders/IdOutputBuilder.h"
#include "outputBuilders/NoopOutputBuilder.h"

void
WaspFacade::readInput()
{
    char tmp;
    cin >> tmp;

    if( !cin.good() && !cin.eof() )
        ErrorMessage::errorDuringParsing( "Unexpected symbol." );
    
    solver.onStartingParsing();
    cin.putback( tmp );    
    switch ( tmp )
    {
        case COMMENT_DIMACS:
        case FORMULA_INFO_DIMACS:
        {
            DimacsOutputBuilder* d = new DimacsOutputBuilder();            
            solver.setOutputBuilder( d );
            greetings();
            Dimacs dimacs( solver );
            dimacs.parse();
            if( dimacs.isMaxsat() )
                d->setMaxsat();
            delete outputBuilder;
            outputBuilder = d;
            greetings();
            break;
        }

        default:
        {
            greetings();            
            GringoNumericFormat gringo( solver );
            gringo.parse();
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
            estatistics( &solver, endSolving() );
            return;
        }
        solver.onStartingSolver( solver.numberOfVariables(), solver.numberOfClauses() );

        if( !solver.isOptimizationProblem() )
        {            
            enumerateModels();
        }
        else
        {
            NoopOutputBuilder* tmp = new NoopOutputBuilder();
            if( wasp::Options::printOnlyOptimum && maxModels > 1 )
                solver.setOutputBuilder( tmp );            
            unsigned int result = solveWithWeakConstraints();
            switch( result )
            {
                case COHERENT:
                    cerr << "ERROR";
                    break;
                
                case INCOHERENT:
                    if( wasp::Options::printOnlyOptimum && maxModels > 1 )
                        solver.setOutputBuilder( outputBuilder );
                    solver.foundIncoherence();
                    break;
                    
                case OPTIMUM_FOUND:
                default:
                    if( maxModels > 1 )
                    {
                        solver.unrollToZero();
                        solver.clearConflictStatus();
                        if( wasp::Options::printOnlyOptimum )
                            solver.setOutputBuilder( outputBuilder );
                        enumerateModels();
                        if( wasp::Options::printOnlyOptimum )
                            tmp->print();
                        solver.optimumFound();                        
                    }
                    else
                        solver.optimumFound();
                    break;
            }
            delete tmp;
            statistics( &solver, endSolving() );
            estatistics( &solver, endSolving() );
            return;
        }
    }

    if( numberOfModels == 0 )
    {
        trace_msg( enumeration, 1, "No model found." );
        solver.foundIncoherence();
    }
    statistics( &solver, endSolving() );
    estatistics( &solver, endSolving() );
    
//    solver.printLearnedClauses();
}

void
WaspFacade::setMinisatPolicy()
{
    solver.setMinisatHeuristic();
}

void
WaspFacade::setOutputPolicy(
    OUTPUT_POLICY outputPolicy )
{
    switch( outputPolicy )
    {
        case COMPETITION_OUTPUT:
            outputBuilder = new CompetitionOutputBuilder();
            break;
            
        case DIMACS_OUTPUT:
            outputBuilder = new DimacsOutputBuilder();
            break;
            
        case SILENT_OUTPUT:
            outputBuilder = new SilentOutputBuilder();
            break;
            
        case THIRD_COMPETITION_OUTPUT:
            outputBuilder = new ThirdCompetitionOutputBuilder();
            break;
            
        case MULTI:
            outputBuilder = new MultiOutputBuilder();
            break;
            
        case ID_OUTPUT:
            outputBuilder = new IdOutputBuilder();
            break;
            
        case WASP_OUTPUT:
        default:
            outputBuilder = new WaspOutputBuilder();
            break;
    }
    solver.setOutputBuilder( outputBuilder );
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
    begin:;
    if( solver.getDecisionLevel( assums.back() ) - 1 < solver.getCurrentDecisionLevel() )
        solver.unroll( solver.getDecisionLevel( assums.back() ) - 1 );
    solver.clearConflictStatus();
    result = solver.solve( assums );
    if( result == INCOHERENT )
    {
        unsigned int bl = solver.getCurrentDecisionLevel();
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
