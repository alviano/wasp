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
            return;
        }
        solver.onStartingSolver( solver.numberOfVariables(), solver.numberOfClauses() );
        if( !solver.isOptimizationProblem() )
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
                    break;
            }
            return;
        }
    }

    if( numberOfModels == 0 )
    {
        trace_msg( enumeration, 1, "No model found." );
        solver.foundIncoherence();
    }
    
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
