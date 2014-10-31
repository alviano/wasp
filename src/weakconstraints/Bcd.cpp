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

#include "Bcd.h"

//unsigned int
//Bcd::run()
//{
//    trace_msg( weakconstraints, 1, "Starting algorithm BCD" );
//    vector< Literal > assumptionsAND;
//    vector< Literal > assumptionsOR;    
//    computeAssumptionsAND( assumptionsAND );
// 
//    vector< BcdData* > disjCores;    
//    vector< BcdData* > auxVarsToBcdData;
//    
//    vector< unsigned int > inUnsatCore;
//    unsigned int numberOfCalls = 0;
//    
//    for( unsigned int i = 0; i <= solver.numberOfVariables(); i++ )
//    {
//        inUnsatCore.push_back( numberOfCalls );
//        auxVarsToBcdData.push_back( NULL );
//    }
//    
//    unsigned int originalNumberOfVariables = solver.numberOfVariables();
//    solver.setComputeUnsatCores( true );
//    solver.turnOffSimplifications();
//    unsigned int numberOfModels = 0;
//    Vector< Literal > assumptionsToAdd;
//    do
//    {
//        trace_msg( weakconstraints, 1, "Calling solver..." );
//        numberOfCalls++;
//        if( solver.solve( assumptionsAND, assumptionsOR ) )
//        {
//            trace_msg( weakconstraints, 1, "Model Found" );
//            numberOfModels++;
//            solver.printAnswerSet();
//            solver.printOptimizationValue( solver.computeCostOfModel() );
//            
//            vector< BcdData* > toUpdate;
//            toUpdate.reserve( disjCores.size() );
//            bool exit = false;
//            for( unsigned int i = 0; i < disjCores.size(); i++ )
//            {
//                BcdData* data = disjCores[ i ];                
//                unsigned int costOfBcdData = computeCostOfBcdData( data );
//                assert( costOfBcdData <= data->ub() );
//                trace_msg( weakconstraints, 3, "Processing BCD data " << *data << " - Cost: " << costOfBcdData << " - UB: " << data->ub() );
//                if( costOfBcdData < data->ub() )
//                {
//                    data->setUb( computeCostOfBcdData( data ) );
//                    data->computeMiddle();
//                    trace_msg( weakconstraints, 4, "Middle " << data->middle() );
//                    toUpdate.push_back( data );
//                }
//            }
//            
//            solver.clearConflictStatus();
//            if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
//                break;
//            
//            for( unsigned int i = 0; i < toUpdate.size(); i++ )
//            {
//                BcdData* data = toUpdate[ i ];                
//                trace_msg( weakconstraints, 3, "Processing BCD data " << *data );                
//                /*
//                 * Removing previous aggregate
//                 */
//                if( data->getAggregate() != UINT_MAX && !solver.addClause( Literal( data->getAggregate(), POSITIVE ) ) )
//                {
//                    cout << "QUI" << endl;
//                    exit = true;
//                    break;
//                }
//                if( !addAggregateBcd( data, inUnsatCore, auxVarsToBcdData ) )
//                {
//                    cout << "QUI2" << endl;
//                    exit = true;
//                    break;
//                }
//                if( data->getAggregate() != UINT_MAX )
//                    assumptionsToAdd.push_back( Literal( data->getAggregate(), NEGATIVE ) );
//            }
//            
//            if( exit )
//                break;
//            
//        }
//        else
//        {
//            assert( solver.getUnsatCore() != NULL );
//            const Clause& unsatCore = *( solver.getUnsatCore() );
//            //The incoherence does not depend on weak constraints
//            if( unsatCore.size() == 0 )
//                return INCOHERENT;
//            
//            solver.clearConflictStatus();
//            if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
//                break;
//            
//            bool newVars = false;
//            for( unsigned int i = 0; i < unsatCore.size(); i++ )
//            {                
//                Var v = unsatCore[ i ].getVariable();
//                if( v <= originalNumberOfVariables )
//                {
//                    if( visited( v, inUnsatCore, 0 ) )
//                        newVars = true;
//                    visit( v, inUnsatCore, numberOfCalls );
//                }
//                else
//                {
//                    assert( v < auxVarsToBcdData.size() );
//                    BcdData* b = auxVarsToBcdData[ v ];
//                    assert( b != NULL );
//                    assert( b->numberOfLiterals() >= 1 );
//                    visit( b->getLiteral( 0 ).getVariable(), inUnsatCore, numberOfCalls );
//                }                
//            }
//
//            vector< BcdData* > intersectingCores;
//            computeIntersectingCores( intersectingCores, disjCores, inUnsatCore, numberOfCalls );
//            
//            BcdData* bcdDataToAdd = NULL;
//            if( !newVars && intersectingCores.size() == 1 )
//            {
//                trace_msg( weakconstraints, 2, "Updating lower bound for core " << *intersectingCores[ 0 ] << ". New value: " << intersectingCores[ 0 ]->middle() );
//                bcdDataToAdd = intersectingCores[ 0 ];                
//                bcdDataToAdd->setLb( intersectingCores[ 0 ]->middle() );
//                bcdDataToAdd->computeMiddle();
//                assert( bcdDataToAdd->isRemoved() );
//                bcdDataToAdd->restore();
//                disjCores.push_back( bcdDataToAdd );
//                if( bcdDataToAdd->getAggregate() != UINT_MAX && !solver.addClause( Literal( bcdDataToAdd->getAggregate(), POSITIVE ) ) )
//                    break;                
//            }
//            else
//            {
//                bcdDataToAdd = createBcdData( intersectingCores, inUnsatCore, numberOfCalls );
//                trace_msg( weakconstraints, 2, "Added new BCD data " << *bcdDataToAdd << ". Upper bound: " << bcdDataToAdd->ub() << " - Lower bound: " << bcdDataToAdd->lb() );
//                disjCores.push_back( bcdDataToAdd );
//            }
//            
//            trace_msg( weakconstraints, 2, "Adding aggregate from BCD data" );            
//            if( !addAggregateBcd( bcdDataToAdd, inUnsatCore, auxVarsToBcdData ) )
//                break;         
//            
//            if( bcdDataToAdd->getAggregate() != UINT_MAX )
//                assumptionsToAdd.push_back( Literal( bcdDataToAdd->getAggregate(), NEGATIVE ) );        
//        }
//        
//        assumptionsAND.clear();
//        computeAssumptionsAND( assumptionsAND );
//        
//        unsigned int k = 0;
//        for( unsigned int i = 0; i < assumptionsToAdd.size(); i++ )
//        {
//            assumptionsToAdd[ k ] = assumptionsToAdd[ i ];
//            if( solver.isUndefined( assumptionsToAdd[ i ] ) )
//                assumptionsAND.push_back( assumptionsToAdd[ k++ ] );
//        }
//        assumptionsToAdd.shrink( k );
//        trace_action( weakconstraints, 2, 
//        {
//            trace_tag( cerr, weakconstraints, 2 );
//            cerr << "AssumptionsAND: [";
//            for( unsigned int i = 0; i < assumptionsAND.size(); i++ )
//                cerr << " " << assumptionsAND[ i ];
//            cerr << " ]" << endl;
//        });
//        
////        static int loop = 0;
////        if( ++loop >= 40 )
////        {
////            cerr << "BYe!" << endl;
////            exit( 0 );
////        }
//        
//    } while( !hasToFinishBcd( disjCores ) );
//    if( numberOfModels > 0 )
//        return OPTIMUM_FOUND;
//    
//    return INCOHERENT;
//}

unsigned int
Bcd::run()
{
    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    initInUnsatCore();    
    for( unsigned int i = 0; i <= solver.numberOfVariables(); i++ )
    {
        toKeepInAssumptions.push_back( true );
        auxVarsToBcdData.push_back( NULL );
    }
    originalNumberOfVariables = solver.numberOfVariables();
    do
    {
        assert( solver.getCurrentDecisionLevel() == 0 );
        if( !processModifiedCores() )
            break;
        
        computeAssumptions();
        trace_msg( weakconstraints, 1, "Calling solver..." );
        if( solver.solve( assumptionsAND, assumptionsOR ) == COHERENT )
        {
            foundModel();
            solver.unrollToZero();
        }
        else
        {
            assert_msg( solver.getUnsatCore() != NULL, "Unsat core has not been computed" );
            const Clause& unsatCore = *( solver.getUnsatCore() );
            //The incoherence does not depend on weak constraints
            if( unsatCore.size() == 0 )
                return INCOHERENT;
            solver.clearConflictStatus();
            solver.unrollToZero();
            foundCore( unsatCore );            
        }        
    } while( !hasToFinishBcd() );
    
    if( numberOfModels > 0 )
        return OPTIMUM_FOUND;
    
    return INCOHERENT;
}

//bool
//Bcd::createFalseAggregate(
//    BcdData* bcd )
//{    
//    Aggregate* aggregate = solver.createAggregate( bcd->getLiterals(), bcd->getWeights() );
//    
//    assert( bcd->numberOfLiterals() >= 1 );
//    solver.addClause( bcd->getLiteral( 0 ) );
//    solver.attachAggregate( *aggregate );
//    if( !aggregate->updateBound( solver, bcd->middle() ) )
//        return false;
//
//    assert( solver.isFalse( bcd->getLiteral( 0 ).getOppositeLiteral() ) );
//    aggregate->onLiteralFalse( solver, bcd->getLiteral( 0 ).getOppositeLiteral(), -1 );    
//
//    solver.addAggregate( aggregate );
//    
//    if( solver.conflictDetected() )
//        return false;
//    
//    return true;
//}

void
Bcd::createBcdData(    
    vector< BcdData* >& intersectingCores )
{
    trace_msg( weakconstraints, 4, "Creating new BCD data" );
    BcdData* corePointer = new BcdData();
    BcdData& core = *corePointer;    
    //Weak constraints fresh 
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        OptimizationLiteralData& opt = solver.getOptimizationLiteral( i );
        if( opt.isRemoved() )
            continue;

        if( visited( opt.lit.getVariable() ) )
        {
            opt.remove();
            core.addElement( opt.lit, opt.weight );
        }
    }

    //Weak constraints in previous cores
    for( unsigned int i = 0; i < intersectingCores.size(); i++ )
        core.copyFrom( intersectingCores[ i ] );    

    core.sort();
    disjCores.push_back( corePointer );
    modifiedCores.push_back( corePointer );
    trace_msg( weakconstraints, 5, "New BCD data: " << core << " - (LB,UB) = (" << core.lb() << "," << core.ub() << ")" );
}

bool
Bcd::addAggregateBcd(
    BcdData* bcdData )
{
    Var aggrId = addAuxVariableBcd();
    cout << "VARIABILE CREATA " << aggrId << endl;
    Aggregate* aggregate = createAggregate( aggrId, bcdData->getLiterals(), bcdData->getWeights() );    
    
    assert( solver.isUndefined( aggrId ) );
    if( !processAndAddAggregate( aggregate, bcdData->middle() + 1 ) )
        return false;
    
    if( solver.isFalse( aggrId ) )
    {
        bcdData->setAggregateAux( UINT_MAX );
        return true;
    }
    
    Var guardId = addBinaryClauseForAggregateBcd( aggrId );
    bcdData->setAggregateAux( guardId );
    auxVarsToBcdData[ guardId ] = bcdData;
    trace_msg( weakconstraints, 4, "Adding clause: (" << Literal( guardId, POSITIVE ) << ", " << Literal( aggrId, NEGATIVE ) << ")"  );

    return true;
}

void
Bcd::foundModel()
{
    trace_msg( weakconstraints, 1, "Model Found" );
    numberOfModels++;
    solver.printAnswerSet();
    solver.printOptimizationValue( solver.computeCostOfModel() );

    for( unsigned int i = 0; i < disjCores.size(); i++ )
    {
        BcdData* data = disjCores[ i ];                
        unsigned int costOfBcdData = computeCostOfBcdData( data );
        assert_msg( costOfBcdData <= data->ub(), costOfBcdData << ">" << data->ub() );
        trace_msg( weakconstraints, 3, "Processing BCD data " << *data << " - Cost: " << costOfBcdData << " - UB: " << data->ub() );
        if( costOfBcdData < data->ub() )
        {
            data->setUb( costOfBcdData );            
            modifiedCores.push_back( data );
        }
    }
}

void
Bcd::foundCore(
    const Clause& unsatCore )
{
    numberOfCalls++;    

    vector< BcdData* > intersectingCores;
    bool newVars = false;
    for( unsigned int i = 0; i < unsatCore.size(); i++ )
    {        
        Var v = unsatCore[ i ].getVariable();
        if( v > originalNumberOfVariables )
        {
            assert( v < auxVarsToBcdData.size() );
            assert( auxVarsToBcdData[ v ] != NULL );
            assert( auxVarsToBcdData.size() == toKeepInAssumptions.size() );
            intersectingCores.push_back( auxVarsToBcdData[ v ] );
            auxVarsToBcdData[ v ]->remove();
            toKeepInAssumptions[ v ] = false;
            visit( v );
            bool res = solver.addClauseRuntime( Literal( v, POSITIVE ) );
            if( !res )
                cout << "probably return false" << endl;
            continue;
        }
        if( visited( v, 0 ) )
            newVars = true;
        visit( v );
    }

    computeIntersectingCores( intersectingCores );
    if( !newVars && intersectingCores.size() == 1 )
        updateLowerBound( intersectingCores[ 0 ] );
    else
        createBcdData( intersectingCores );
    deleteOldCores();
}

void
Bcd::updateLowerBound(
    BcdData* corePointer )
{
    BcdData& core = *corePointer;
    trace_msg( weakconstraints, 4, "Updating lower bound for core " << core << ". New value: " << core.middle() << ". Upper bound was " << core.ub() );
    core.setLb( core.middle() );
    assert( core.isRemoved() );
    core.restore();
    modifiedCores.push_back( corePointer );
}

bool
Bcd::processModifiedCores()
{
    while( !modifiedCores.empty() )
    {
        BcdData* data = modifiedCores.back();
        modifiedCores.pop_back();
        data->computeMiddle();
        Var aggrId = data->getAggregateAux();
        trace_msg( weakconstraints, 3, "Processing BCD data " << *data << " for adding aggregate - Middle value: " << data->middle() );
        trace_msg( weakconstraints, 4, "Previous aggregate has id " << aggrId );
        /*
         * Removing previous aggregate
         */        
        if( aggrId != UINT_MAX )
        {
            if( !solver.addClauseRuntime( Literal( aggrId, POSITIVE ) ) )
                return false;
            assert( aggrId < toKeepInAssumptions.size() );
            toKeepInAssumptions[ aggrId ] = false;            
        }
        
        if( !addAggregateBcd( data ) )
            return false;
        
        aggrId = data->getAggregateAux();
        if( aggrId != UINT_MAX )
        {
            assert_msg( aggrId < auxVarsToBcdData.size(), aggrId << " >= " << auxVarsToBcdData.size() );        
            assert_msg( auxVarsToBcdData[ aggrId ] == data, auxVarsToBcdData[ aggrId ] << "!=" << data );
            trace_msg( weakconstraints, 5, "Adding " << Literal( aggrId, NEGATIVE ) << " in the assumptions to add" );
            assumptionsToAdd.push_back( Literal( aggrId, NEGATIVE ) );
        }
    }

    return true;
}