#include "UnfoundedFreeHC.h"
#include "Solver.h"
#include "outputBuilders/WaspOutputBuilder.h"
#include "util/Trace.h"
#include "util/Options.h"
#include "input/Rule.h"

ostream& operator<<( ostream& out, const UnfoundedFreeHC& component )
{
    out << "[ ";
    for( unsigned i = 0; i < component.hcVariables.size(); ++i )
        out << component.solver.getLiteral( component.hcVariables[ i ] ) << " ";
    return out << "]";
}

UnfoundedFreeHC::UnfoundedFreeHC(
    vector< GUSData* >& gusData_,
    Solver& s,
    unsigned numberOfInputAtoms ) : PostPropagator(), gusData( gusData_ ), solver( s )
{   
    hasToTestModel = false; isConflictual = false; low = 0; high = solver.numberOfVariables();
    id = 0; varsAtLevelZero = 0; numberOfCalls = 0; numberOfAttachedVars = 0;
    inUnfoundedSet.push_back( 0 );
    while( checker.numberOfVariables() < numberOfInputAtoms )
    {
        checker.addVariable();
        inUnfoundedSet.push_back( 0 );
    }
    assert_msg( checker.numberOfVariables() == numberOfInputAtoms, checker.numberOfVariables() << " != " << numberOfInputAtoms );
    assert( checker.numberOfVariables() == inUnfoundedSet.size() - 1 );
    
    checker.setOutputBuilder( new WaspOutputBuilder() );
    checker.initFrom( solver );
    checker.setGenerator( false ); 
    checker.disableStatistics();
    checker.setHCComponentForChecker( this );
    checker.disableVariableElimination();
}

UnfoundedFreeHC::~UnfoundedFreeHC()
{
    checker.enableStatistics();
    statistics( &checker, onDeletingChecker( id ) ); 
    for( unsigned int i = 0; i < toDelete.size(); i++ )
        delete toDelete[ i ];
    toDelete.clear();
}

void
UnfoundedFreeHC::reset()
{    
    while( !trail.empty() && solver.isUndefined( trail.back() ) )
    {
        hasToTestModel = false;
        trail.pop_back();
    }

    unfoundedSet.clear();
}

bool
UnfoundedFreeHC::onLiteralFalse(
    Literal literal )
{    
    cout << "ON LITERAL FALSE " << literal << " " << solver.getCurrentDecisionLevel() << endl;
    if( solver.getCurrentDecisionLevel() > 0 )
        trail.push_back( literal );
    else
        varsAtLevelZero++;

    if( wasp::Options::forwardPartialChecks /*&& ( low + high ) / 2 <= solver.getCurrentDecisionLevel()*/ )
    {
        hasToTestModel = true;
        return true;
    }
    
    cout << "TRAIL SIZE " << trail.size() << " " << varsAtLevelZero << " " << numberOfAttachedVars << endl;
    if( trail.size() + varsAtLevelZero == ( numberOfAttachedVars ) )
    {
        hasToTestModel = true;
        return true;
    }
    
    return false;
}

void
UnfoundedFreeHC::testModel()
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
    statistics( &checker, endCheckerInvocation( time( 0 ) ) );
}

void
UnfoundedFreeHC::computeAssumptions(
    vector< Literal >& assumptions )
{
    trace_msg( modelchecker, 1, "Computing assumptions" );
    iterationInternalLiterals( assumptions );
    iterationExternalLiterals( assumptions );
    statistics( &checker, assumptions( assumptions.size() ) );
}

void
UnfoundedFreeHC::iterationInternalLiterals(
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

UnfoundedFreeHC::iterationExternalLiterals(
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

Clause*
UnfoundedFreeHC::getClauseToPropagate(
    Learning& learning )
{
    assert( unfoundedSet.empty() );
    if( hasToTestModel )
        testModel();
    
    hasToTestModel = false;
    if( unfoundedSet.empty() )
        return NULL;
    
    trace_msg( modelchecker, 1, "Learning unfounded set rule for component " << *this );
    Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet( unfoundedSet );
    trace_msg( modelchecker, 1, "Adding loop formula: " << *loopFormula );
    unfoundedSet.clear();
    if( !( wasp::Options::forwardPartialChecks ) )
        solver.setAfterConflictPropagator( this );        
    solver.onLearningALoopFormulaFromModelChecker();    
    return loopFormula;    
}

void
UnfoundedFreeHC::computeReasonForUnfoundedAtom(
    Var v,
    Learning& learning )
{
    trace_msg( modelchecker, 2, "Processing variable " << solver.getLiteral( v ) );
    vector< Clause* >& definingRules = getGUSData( v ).definingRulesForNonHCFAtom;        
    for( unsigned int i = 0; i < definingRules.size(); i++ )
    {
        Clause* rule = definingRules[ i ];
        trace_msg( modelchecker, 3, "Processing rule " << *rule );
        
        bool skipRule = false;
        
        unsigned int min = UINT_MAX;
        unsigned int pos = UINT_MAX;
        for( unsigned int j = 0; j < rule->size(); j++ )
        {
            Literal lit = rule->getAt( j );
            trace_msg( modelchecker, 4, "Processing literal " << lit << " that is " << ( isInUnfoundedSet( lit.getVariable() ) ? "unfounded" : "founded" ) );
            if( isInUnfoundedSet( lit.getVariable() ) )
            {
                if( lit.isHeadAtom() )
                {
                    trace_msg( modelchecker, 5, "Skip " << lit << " because it is in the head" );
                    continue;
                }
                else if( lit.isPositiveBodyLiteral() )
                {
                    trace_msg( modelchecker, 5, "Skip rule because of an unfounded positive body literal: " << lit );
                    skipRule = true;
                    break;
                }
            }
            
            if( solver.isUndefined( lit ) )
            {
                trace_msg( modelchecker, 5, lit << " is undefined" );
                cout << "POS " << pos << endl;
                if( pos == UINT_MAX )
                    pos = j;
                cout << "POS DOPO " << pos << endl;
                continue;
            }
            
            if( !solver.isTrue( lit ) )
            {
                trace_msg( modelchecker, 5, "Skip " << lit << " because it is not true" );
                continue;
            }
            
            unsigned int dl = solver.getDecisionLevel( lit );
            if( dl == 0 )
            {
                trace_msg( modelchecker, 5, "Skip rule because of a literal of level 0: " << lit );
                skipRule = true;
                break;
            }
            if( dl < min )
            {
                min = dl;
                pos = j;
            }
        }
        
        if( !skipRule )
        {
            assert_msg( pos < rule->size(), "Trying to access " << pos << " in " << *rule );
            trace_msg( modelchecker, 4, "The reason is: " << rule->getAt( pos ) );
            learning.onNavigatingLiteralForUnfoundedSetLearning( rule->getAt( pos ).getOppositeLiteral() );
        }
    }
}

void
UnfoundedFreeHC::addClausesForHCAtoms()
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
UnfoundedFreeHC::initDataStructures()
{
    trace_msg( modelchecker, 2, "First call. Removing unused variables" );    
    if( isConflictual )
        return;
    
    addClausesForHCAtoms();
    if( !isConflictual )
        isConflictual = !checker.preprocessing();
    checker.turnOffSimplifications();
}

void UnfoundedFreeHC::checkModel(
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
        high = solver.getCurrentDecisionLevel();        
    }
    else
    {
        trace_msg( modelchecker, 1, "UNSATISFIABLE: the model is stable." );
        low = solver.getCurrentDecisionLevel();        
        checker.clearConflictStatus();        
    }
    
    assert( !checker.conflictDetected() );
    checker.unrollToZero();    
}

void
UnfoundedFreeHC::processRule(
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
        {
            addExternalVariable( v  );            
        }
        else
        {
            if( lit.isHeadAtom() )
            {
                c->addLiteral( Literal( getGUSData( v ).headVarForHCC, POSITIVE ) );
                headAtoms.push_back( v );
            }
            else
            {
                if( lit.isPositiveBodyLiteral() )
                    c->addLiteral( Literal( getGUSData( v ).unfoundedVarForHCC, POSITIVE ) );
            }
        }
        
        //body literal with opposite polarity
        if( !lit.isHeadAtom() || !sameComponent( v ) )
            c->addLiteral( lit );
    }
    
    clause->removeDuplicates();
    c->removeDuplicates();
    checker.addVariable();
    Var freshVar = checker.numberOfVariables();
    c->addLiteral( Literal( freshVar, NEGATIVE ) );    
    checker.addClause( c );
    trace_msg( modelchecker, 2, "Adding clause " << *c );
    
    for( unsigned int k = 0; k < headAtoms.size(); k++ )
    {
        Var v = headAtoms[ k ];
        assert( sameComponent( v ) );
        getGUSData( v ).definingRulesForNonHCFAtom.push_back( clause );
        trace_msg( modelchecker, 2, "Adding clause: [ " << Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ) << " | " << Literal( freshVar, POSITIVE ) << " ]" );
        checker.addClause( Literal( getGUSData( v ).unfoundedVarForHCC, NEGATIVE ), Literal( freshVar, POSITIVE ) );
    }
    toDelete.push_back( clause );    
}

void
UnfoundedFreeHC::processBeforeStarting()
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