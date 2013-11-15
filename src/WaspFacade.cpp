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

#include "heuristic/restart/NoRestartStrategy.h"
#include "heuristic/restart/MinisatRestartStrategy.h"
#include "heuristic/restart/GeometricRestartStrategy.h"
#include "heuristic/restart/SequenceBasedRestartStrategy.h"

#include "heuristic/deletion/AggressiveDeletionStrategy.h"
#include "heuristic/deletion/RestartsBasedDeletionStrategy.h"
#include "heuristic/deletion/MinisatDeletionStrategy.h"
#include "heuristic/deletion/GlueBasedDeletionStrategy.h"

#include "outputBuilders/WaspOutputBuilder.h"
#include "outputBuilders/SilentOutputBuilder.h"
#include "outputBuilders/ThirdCompetitionOutputBuilder.h"
#include "outputBuilders/CompetitionOutputBuilder.h"
#include "outputBuilders/DimacsOutputBuilder.h"

#include "heuristic/decision/BerkminHeuristic.h"
#include "heuristic/decision/BerkminHeuristicWithCache.h"
#include "heuristic/decision/FirstUndefinedHeuristic.h"
#include "heuristic/decision/MinisatHeuristic.h"

void
WaspFacade::readInput()
{
    Dimacs dimacs( solver );
    dimacs.parse();
}

void
WaspFacade::solve()
{
    if( printProgram )
    {
        solver.printProgram();
        return;
    }
    
    solver.init();
    if( !solver.preprocessing() )
    {
        solver.foundIncoherence();
        return;
    }
    
    while( solver.solve() )
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
    
    if( numberOfModels == 0 )
    {
        trace_msg( enumeration, 1, "No model found." );
        solver.foundIncoherence();
    }
}

void
WaspFacade::setDeletionPolicy(
    DELETION_POLICY deletionPolicy,
    unsigned int deletionThreshold )
{
    switch( deletionPolicy )
    {
        case RESTARTS_BASED_DELETION_POLICY:
            heuristic->setDeletionStrategy( new RestartsBasedDeletionStrategy( solver ) );
            break;
            
        case MINISAT_DELETION_POLICY:
            heuristic->setDeletionStrategy( new MinisatDeletionStrategy( solver ) );
            break;
            
        case GLUCOSE_DELETION_POLICY:
            heuristic->setDeletionStrategy( new GlueBasedDeletionStrategy( solver, deletionThreshold ) );
            break;

        case AGGRESSIVE_DELETION_POLICY:
        default:
            heuristic->setDeletionStrategy( new AggressiveDeletionStrategy( solver ) );
            break;
    }
}

void
WaspFacade::setDecisionPolicy(
    DECISION_POLICY decisionPolicy,
    unsigned int threshold )
{
    switch( decisionPolicy )
    {
        case HEURISTIC_BERKMIN:
            assert( threshold > 0 );
            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, threshold ) );
            break;
        
        case HEURISTIC_FIRST_UNDEFINED:
            heuristic->setDecisionStrategy( new FirstUndefinedHeuristic( solver ) );
            break;
            
        case HEURISTIC_MINISAT:
            heuristic->setDecisionStrategy( new MinisatHeuristic( solver ) );
            break;
    
        default:
            heuristic->setDecisionStrategy( new BerkminHeuristic( solver, 512 ) );
            break;
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
    switch( restartsPolicy )
    {
        case GEOMETRIC_RESTARTS_POLICY:
            heuristic->setRestartStrategy( new GeometricRestartStrategy( threshold ) );
            break;
            
        case MINISAT_RESTARTS_POLICY:
            heuristic->setRestartStrategy( new MinisatRestartStrategy( threshold ) );
            break;
            
        case NO_RESTARTS_POLICY:
            heuristic->setRestartStrategy( new NoRestartStrategy() );
            break;
            
        case SEQUENCE_BASED_RESTARTS_POLICY:
        default:
            heuristic->setRestartStrategy( new SequenceBasedRestartStrategy( threshold ) );
            break;
    }
}
