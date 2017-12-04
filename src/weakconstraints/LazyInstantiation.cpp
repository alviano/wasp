/* 
 * File:   LazyInstantiation.cpp
 * Author: carmine
 * 
 * Created on 5 dicembre 2016, 10.18
 */

#include "LazyInstantiation.h"

LazyInstantiation::LazyInstantiation(
    Solver& s,
    ExternalPropagator* ext ) : One( s ), externalPropagator( ext )
{
    if( solver.numberOfLevels() > 1 )
        WaspErrorMessage::errorDuringParsing( "Lazy weak constraints cannot be combined with multi-level weak constraints" );
    
    if( solver.numberOfLevels() == 0 )
        solver.setLevels( 1 );
    
    solver.attachAnswerSetListener( this );
}

unsigned int
LazyInstantiation::run()
{   
    trace_msg( weakconstraints, 1, "Starting algorithm One with lazy instantiation" );
    computeAssumptions();    
    initInUnsatCore();

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    unsigned int numberOfModels = 0;
    while( true )
    {
        if( solver.solve( assumptions ) == INCOHERENT )
        {
            trace_msg( weakconstraints, 2, "Incoherent: analyze unsatisfiable core" );
            if( !foundUnsat() )
                return numberOfModels == 0 ? INCOHERENT : OPTIMUM_FOUND;            
        }
        else
        {
            numberOfModels++;
            if( !handleAnswerSet() )
                break;
        }
        
        if( lb() == ub() )
            break;
        assumptions.clear();
        computeAssumptions();
        resetSolver();
    }    
        
    assert_msg( lb() == ub(), lb() << " != " << ub() );
    return OPTIMUM_FOUND;
}

bool
LazyInstantiation::handleAnswerSet()
{    
    assert( solver.numberOfLevels() == 1 );
    uint64_t cost = solver.computeCostOfModel( 0 );
    
    vector< Clause* > weakConstraints;
    vector< uint64_t > weights;
    if( externalPropagator->hasWeakConstraintsToAdd( solver ) )
        cost += externalPropagator->addWeakConstraints( solver, weakConstraints, weights );
    trace_msg( weakconstraints, 2, "Found answer set with cost " << cost  << " - upper bound " << ub() );
    if( cost < ub() )
    {
        solver.printAnswerSet();
        setUpperBound( cost );
        solver.foundUpperBound( cost );
        Vector< uint64_t > costs;
        costs.push_back( cost );
        solver.printOptimizationValue( costs );                
    }
    
    solver.unrollToZero();    
    
    for( unsigned int i = 0; i < weakConstraints.size(); i++ )
    {
        Clause* clause = weakConstraints[ i ];
        Var addedVar = 0;
        if( clause->size() > 1 )
        {
            addedVar = addAuxVariable();
            clause->addLiteral( Literal( addedVar, NEGATIVE ) );
            solver.addOptimizationLiteral( Literal( addedVar, NEGATIVE ), weights[ i ], 0, false );
            if( !solver.addClauseRuntime( clause ) )
                return false;
        }
        else if( clause->size() == 1 )
        {
            solver.addOptimizationLiteral( clause->getAt( 0 ).getOppositeLiteral(), weights[ i ], 0, false );
            delete clause;
        }
        else
        {
            addedVar = addAuxVariable();
            solver.addClauseRuntime( Literal( addedVar, NEGATIVE ) );
            solver.addOptimizationLiteral( Literal( addedVar, NEGATIVE ), weights[ i ], 0, false );
        }
    }
    
    return !weakConstraints.empty();        
}