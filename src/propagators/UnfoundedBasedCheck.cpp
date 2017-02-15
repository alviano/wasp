#include "UnfoundedBasedCheck.h"
#include "../Solver.h"
#include "../util/WaspTrace.h"
#include "../util/WaspOptions.h"
#include "../input/Rule.h"

UnfoundedBasedCheck::UnfoundedBasedCheck(
    vector< GUSData* >& gusData_,
    Solver& s,
    unsigned numberOfInputAtoms ) : HCComponent( gusData_, s )
{   
    varsAtLevelZero = 0; numberOfAttachedVars = 0; inUnfoundedSet.push_back( 0 );
    while( checker.numberOfVariables() < numberOfInputAtoms )
    {
        checker.addVariable();
        inUnfoundedSet.push_back( 0 );
    }
    assert_msg( checker.numberOfVariables() == numberOfInputAtoms, checker.numberOfVariables() << " != " << numberOfInputAtoms );
    assert( checker.numberOfVariables() == inUnfoundedSet.size() - 1 );
    
    initChecker();
}

bool
UnfoundedBasedCheck::onLiteralFalse(
    Literal literal )
{    
    if( solver.getCurrentDecisionLevel() > 0 )
        trail.push_back( literal );
    else
        varsAtLevelZero++;

    //disabled partial checks
//    if( wasp::Options::forwardPartialChecks )
//    {
//        hasToTestModel = true;
//        return true;
//    }
    
    if( trail.size() + varsAtLevelZero == ( numberOfAttachedVars ) )
    {
        hasToTestModel = true;
        return true;
    }
    
    return false;
}

void
UnfoundedBasedCheck::testModel()
{
    trace_msg( modelchecker, 1, "Check component " << *this );
    if( numberOfCalls++ == 0 )
        initDataStructures();    
    
    //The checker will return always unsat
    if( isConflictual )
        return;

    vector< Literal > assumptions;
    computeAssumptions( assumptions );
    //The checker will return always unsat
    if( isConflictual )
        return;

    checkModel( assumptions );

    assert( !checker.conflictDetected() );
}

void
UnfoundedBasedCheck::computeAssumptions(
    vector< Literal >& assumptions )
{
    trace_msg( modelchecker, 1, "Computing assumptions" );
    iterationInternalLiterals( assumptions );
    iterationExternalLiterals( assumptions );
    statistics( &checker, assumptions( assumptions.size() ) );
}

void
UnfoundedBasedCheck::iterationInternalLiterals(
    vector< Literal >& assumptions )
{
    trace_msg( modelchecker, 2, "Iteration on " << hcVariablesNotTrueAtLevelZero.size() << " internal variables" );
    for( unsigned int i = 0; i < hcVariablesNotTrueAtLevelZero.size(); i++ )
    {
        Var v = hcVariablesNotTrueAtLevelZero[ i ];
        if( solver.isUndefined( v ) )
            continue;
        
        if( solver.isTrue( v ) )
            assumptions.push_back( Literal( v, POSITIVE ) );
        else if( solver.isFalse( v ) )
        {
            assumptions.push_back( Literal( v, NEGATIVE ) );
            assumptions.push_back( Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ) );
        }
    }
}
void

UnfoundedBasedCheck::iterationExternalLiterals(
    vector< Literal >& assumptions )
{
    trace_msg( modelchecker, 2, "Iteration on " << externalVars.size() << " external variables" );
    int j = 0;
    for( unsigned int i = 0; i < externalVars.size(); i++ )
    {
        Var v = externalVars[ j ] = externalVars[ i ];
        if( solver.isUndefined( v ) )
        {
            j++;
            continue;
        }

        if( solver.getDecisionLevel( v ) > 0 )
        {
            assumptions.push_back( Literal( v, solver.isTrue( v ) ? POSITIVE : NEGATIVE ) );
            j++;
        }
        else
        {
            isConflictual = !checker.addClauseRuntime( Literal( v, solver.isTrue( v ) ? POSITIVE : NEGATIVE ) );
            if( isConflictual )
                return;
        }
    }
    externalVars.resize( j );
}

void
UnfoundedBasedCheck::addClausesForHCAtoms()
{
    trace_msg( modelchecker, 1, "Simplifying Head Cycle variables" );
    if( isConflictual )
        return;
    
    Clause* clause = new Clause( hcVariables.size() );
    for( unsigned int i = 0; i < hcVariables.size(); i++ )
    {
        Var v = hcVariables[ i ]; 
        Var uv = getGUSData( v ).unfoundedVarForHCC;
        Var hv = getGUSData( v ).headVarForHCC;
        
        clause->addLiteral( Literal( uv, POSITIVE ) );
        
        Clause* c = new Clause( 3 );
        c->addLiteral( Literal( uv, POSITIVE ) );
        c->addLiteral( Literal( hv, POSITIVE ) );
        c->addLiteral( Literal( v, NEGATIVE ) );
        
        isConflictual = 
            !checker.addClause( Literal( uv, NEGATIVE ), Literal( hv, NEGATIVE ) )
        ||  !checker.addClause( Literal( v, POSITIVE ), Literal( hv, NEGATIVE ) )
        ||  !checker.addClause( c );        
        
        trace_msg( modelchecker, 2, "Adding clause: " << *c );
        trace_msg( modelchecker, 2, "Adding clause: [ " << Literal( uv, NEGATIVE ) << " | " << Literal( hv, NEGATIVE ) << " ]");
        trace_msg( modelchecker, 2, "Adding clause: [ " << Literal( v, POSITIVE ) << " | " << Literal( hv, NEGATIVE ) << " ]");
        
        if( isConflictual )
        {
            delete clause;
            return;
        }
        if( !solver.isUndefined( v ) && solver.getDecisionLevel( v ) == 0 )
        {
            if( solver.isTrue( v ) )
                isConflictual = !checker.addClause( Literal( v, POSITIVE ) );
            else
            {
                isConflictual = !checker.addClause( Literal( v, NEGATIVE ) )
                || !checker.addClause( Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ) );
            }
            if( isConflictual )
            {
                delete clause;
                return;
            }
        }
        else
        {
            hcVariablesNotTrueAtLevelZero.push_back( v );
        }
    }
    
    trace_action( modelchecker, 2, {
        trace_tag( cerr, modelchecker, 2 );
        if( hcVariables.size() == 0 )
            cerr << "Adding clause: []" << endl;
        else
        {
            cerr << "Adding clause: [ ";
            cerr << "u" << Literal( hcVariables[ 0 ], POSITIVE );
            for( unsigned int i = 1; i < hcVariables.size(); i++ )
                cerr << " | u" << Literal( hcVariables[ i ], POSITIVE );
            cerr << " ]" << endl;
        }
    } );
    isConflictual = !checker.cleanAndAddClause( clause );
}

void
UnfoundedBasedCheck::initDataStructures()
{
    trace_msg( modelchecker, 2, "First call. Removing unused variables" );    
    if( isConflictual )
        return;
    
    addClausesForHCAtoms();
    if( !isConflictual )
        isConflictual = !checker.preprocessing();
    checker.turnOffSimplifications();
}

void
UnfoundedBasedCheck::checkModel(
    vector< Literal >& assumptions )
{    
    assert( !checker.conflictDetected() );
    assert( checker.getCurrentDecisionLevel() == 0 );
    trace_action( modelchecker, 2, { printVector( assumptions, "Assumptions" ); } );
    statistics( &checker, startCheckerInvocation( trail.size() != ( hcVariables.size() + externalVars.size() ), time( 0 ) ) );
    checker.clearConflictStatus();
    
    if( checker.solve( assumptions ) == COHERENT )
    {
        trace_msg( modelchecker, 1, "SATISFIABLE: the model is not stable." );
        for( unsigned int i = 0; i < hcVariables.size(); i++ )
            if( checker.isTrue( getGUSData( hcVariables[ i ] ).unfoundedVarForHCC ) )
                setInUnfoundedSet( hcVariables[ i ] );
        trace_action( modelchecker, 2, { printVector( unfoundedSet, "Unfounded set" ); } );
        statistics( &checker, foundUS( trail.size() != ( hcVariables.size() + externalVars.size() ), unfoundedSet.size() ) );
        assert( !unfoundedSet.empty() );        
    }
    else
    {
        trace_msg( modelchecker, 1, "UNSATISFIABLE: the model is stable." );        
        checker.clearConflictStatus();        
    }
    
    assert( !checker.conflictDetected() );
    checker.unrollToZero();
    statistics( &checker, endCheckerInvocation( time( 0 ) ) );
}

void
UnfoundedBasedCheck::processRule(
    Rule* rule )
{
    Clause* clause = new Clause();
    Clause* c = new Clause();
    
    vector< Var > headAtoms;
    for( unsigned int k = 0; k < rule->size(); k++ )
    {
        Literal lit = rule->literals[ k ];
        Var v = lit.getVariable();
        clause->addLiteral( lit );
        if( !sameComponent( v ) )
            addExternalVariable( v  );            
        else if( lit.isHeadAtom() )
        {
            c->addLiteral( Literal( getGUSData( v ).headVarForHCC, POSITIVE ) );
            headAtoms.push_back( v );
        }
        else if( lit.isPositiveBodyLiteral() )
            c->addLiteral( Literal( getGUSData( v ).unfoundedVarForHCC, POSITIVE ) );        
        
        //body literal with opposite polarity
        if( !lit.isHeadAtom() || !sameComponent( v ) )
            c->addLiteral( lit );
    }
    
    clause->removeDuplicates();    
    
    assert( headAtoms.size() > 0 );
    if( headAtoms.size() == 1 )
    {
        c->addLiteral( Literal( headAtoms[ 0 ], NEGATIVE ) );
        getGUSData( headAtoms[ 0 ] ).definingRulesForNonHCFAtom.push_back( clause );
        Literal defLit = solver.getLiteral( rule->getBodyAux() );
        if( defLit.getVariable() != 0 )
            solver.setFrozen( defLit.getVariable() );
        getGUSData( headAtoms[ 0 ] ).definingLiteralsForNonHCFAtom.push_back( defLit );
    }
    else
    {
        checker.addVariable();
        Var freshVar = checker.numberOfVariables();
        c->addLiteral( Literal( freshVar, NEGATIVE ) );
        for( unsigned int k = 0; k < headAtoms.size(); k++ )
        {
            Var v = headAtoms[ k ];
            assert( sameComponent( v ) );
            getGUSData( v ).definingRulesForNonHCFAtom.push_back( clause );
            Literal defLit = solver.getLiteral( rule->getBodyAux() );
            if( defLit.getVariable() != 0 )
                solver.setFrozen( defLit.getVariable() );
            getGUSData( v ).definingLiteralsForNonHCFAtom.push_back( defLit );
            trace_msg( modelchecker, 2, "Adding clause: [ " << Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ) << " | " << Literal( freshVar, POSITIVE ) << " ]" );
            checker.addClause( Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ), Literal( freshVar, POSITIVE ) );
        }
    }
    c->removeDuplicates();
    checker.addClause( c );
    trace_msg( modelchecker, 2, "Adding clause " << *c );    
    toDelete.push_back( clause );
}

void
UnfoundedBasedCheck::processComponentBeforeStarting()
{
    for( unsigned int j = 0; j < hcVariables.size(); j++ )
    {
        Var v = hcVariables[ j ];
        if( solver.isTrue( v ) )
            onLiteralFalse( Literal( v, NEGATIVE ) );
        else if( solver.isFalse( v ) )
            onLiteralFalse( Literal( v, POSITIVE ) );
    }

    for( unsigned int j = 0; j < externalVars.size(); j++ )
    {
        Var v = externalVars[ j ];
        if( solver.isTrue( v ) )
            onLiteralFalse( Literal( v, NEGATIVE ) );
        else if( solver.isFalse( v ) )
            onLiteralFalse( Literal( v, POSITIVE ) );
    }
}