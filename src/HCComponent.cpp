#include "HCComponent.h"
#include "Solver.h"
#include "input/Dimacs.h"
#include "outputBuilders/DimacsOutputBuilder.h"
#include "outputBuilders/WaspOutputBuilder.h"
#include "util/Trace.h"
#include "util/Options.h"

#include <vector>

ostream& operator<<( ostream& out, const HCComponent& component )
{
    out << "[ ";
    for( unsigned i = 0; i < component.hcVariables.size(); ++i )
        out << component.solver.getLiteral( component.hcVariables[ i ] ) << " ";
    return out << "]";
}

HCComponent::HCComponent(
    vector< GUSData* >& gusData_,
    Solver& s,
    unsigned numberOfInputAtoms ) : PostPropagator(), gusData( gusData_ ), solver( s ), numberOfCalls( 0 ), hasToTestModel( false ), numberOfAtoms( numberOfInputAtoms ), id( 0 )
{   
    inUnfoundedSet.push_back( 0 );
    generatorToCheckerId.push_back( UINT_MAX );
    checkerToGeneratorId.push_back( 0 );
    while( checker.numberOfVariables() < numberOfInputAtoms )
    {
        inUnfoundedSet.push_back( 0 );
        checker.addVariable();
        generatorToCheckerId.push_back( UINT_MAX );
        checkerToGeneratorId.push_back( 0 );
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

HCComponent::~HCComponent()
{
    checker.enableStatistics();
    statistics( &checker, onDeletingChecker( id ) );
}

bool
HCComponent::addExternalLiteral(
    Literal lit ) 
{    
    if( lit.isPositive() )
    {
        if( inUnfoundedSet[ lit.getVariable() ] & 1 )
            return false;
        inUnfoundedSet[ lit.getVariable() ] |= 1;
    }
    else
    {
        assert( lit.isNegative() );
        if( inUnfoundedSet[ lit.getVariable() ] & 2 )
            return false;
        inUnfoundedSet[ lit.getVariable() ] |= 2;
        checker.addVariable();
        generatorToCheckerId[ lit.getVariable() ] = checker.numberOfVariables();
        checkerToGeneratorId.push_back( lit.getVariable() );    
    }    
    externalLiterals.push_back( lit );    
    return true;
}

bool
HCComponent::addExternalLiteralForInternalVariable(
    Literal lit )
{
    assert( lit.isPositive() );
    if( inUnfoundedSet[ lit.getVariable() ] & 2 )
        return false;
    inUnfoundedSet[ lit.getVariable() ] |= 2;
    checker.addVariable();
    generatorToCheckerId[ lit.getVariable() ] = checker.numberOfVariables();
    checkerToGeneratorId.push_back( lit.getVariable() );
    externalLiterals.push_back( lit );
    return true;
}

void
HCComponent::reset()
{    
    while( !trail.empty() && solver.isUndefined( trail.back() ) )
    {
        hasToTestModel = false;
        trail.pop_back();
    }

    unfoundedSet.clear();
}

bool
HCComponent::onLiteralFalse(
    Literal literal )
{
    trail.push_back( literal );    
    if( trail.size() == ( hcVariables.size() + externalLiterals.size() ) )
    {
        //testModel();
        hasToTestModel = true;
        return true;
    }
    
    return false;
}

void
HCComponent::testModel()
{
    trace_msg( modelchecker, 1, "Check component " << *this );
    
    vector< Literal > assumptionsAND;
    vector< Literal > assumptionsOR;    

    computeAssumptions( assumptionsAND, assumptionsOR );
    if( assumptionsOR.empty() )
    {
        trace_msg( modelchecker, 2, "Nothing to check here!" );
        return;
    }
    
    if( numberOfCalls++ == 0 )
    {
        trace_msg( modelchecker, 2, "First call. Removing unused variables" );
        for( unsigned i = 1; i < inUnfoundedSet.size(); ++i )
        {
            if( inUnfoundedSet[ i ] == 0 )
                checker.addClause( Literal( i, POSITIVE ) );                
            else
                inUnfoundedSet[ i ] = 0;        
        }
        #ifndef NDEBUG
        bool result = 
        #endif
        checker.preprocessing();
        assert( result );
    }

    trace_action( modelchecker, 2,
    {
        trace_tag( cerr, modelchecker, 2 );
        cerr << "Assumptions AND: ";
        if( !assumptionsAND.empty() )
        {
            cerr << assumptionsAND[ 0 ];
            for( unsigned int i = 1; i < assumptionsAND.size(); i++ )
                cerr << ", " << assumptionsAND[ i ];
        }
        else
            cerr << "empty";
        cerr << endl;
        trace_tag( cerr, modelchecker, 2 );
        cerr << "Assumptions OR: ";
        if( !assumptionsOR.empty() )
        {
            cerr << assumptionsOR[ 0 ];
            for( unsigned int i = 1; i < assumptionsOR.size(); i++ )
                cerr << ", " << assumptionsOR[ i ];
        }
        else
            cerr << "empty";
        cerr << endl;        
    } );
    
    if( checker.getCurrentDecisionLevel() > 0 )
        checker.doRestart();
    statistics( &checker, startCheckerInvokation( trail.size() != ( hcVariables.size() + externalLiterals.size() ), time( 0 ) ) );    
    if( checker.solve( assumptionsAND, assumptionsOR ) )
    {        
        trace_msg( modelchecker, 1, "SATISFIABLE: the model is not stable." );                
        for( unsigned int i = 0; i < assumptionsOR.size(); i++ )
            if( checker.isTrue( assumptionsOR[ i ] ) )
            {
                Var curr = assumptionsOR[ i ].getVariable();
                unfoundedSet.push_back( curr );
                setInUnfoundedSet( curr );
            }
        trace_action( modelchecker, 2,
        {            
            trace_tag( cerr, modelchecker, 2 );
            cerr << "The unfounded set is:";
            for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
                cerr << " " << Literal( unfoundedSet[ i ], POSITIVE );
            cerr << endl;
        } );
        statistics( &checker, foundUS( trail.size() != ( hcVariables.size() + externalLiterals.size() ), unfoundedSet.size() ) );
        assert( !unfoundedSet.empty() );
    }
    else
    {
        trace_msg( modelchecker, 1, "UNSATISFIABLE: the model is stable." );    
    }
    statistics( &checker, endCheckerInvokation( time( 0 ) ) );
    
    if( solver.exchangeClauses() )
    {
        while( !learnedClausesFromChecker.empty() )
        {            
            Clause* c = learnedClausesFromChecker.back();
            learnedClausesFromChecker.pop_back();
            assert( c->size() <= 8 );            
            solver.addClauseInLearnedFromAllSolvers( c );
        }        
    }
}

void
HCComponent::computeAssumptions(
    vector< Literal >& assumptionsAND,
    vector< Literal >& assumptionsOR )
{    
    for( unsigned int i = 0; i < hcVariables.size(); i++ )
    {        
        Var v = hcVariables[ i ];
        if( !solver.isFalse( v ) )
            assumptionsOR.push_back( Literal( v, NEGATIVE ) );
        else
            assumptionsAND.push_back( Literal( v, NEGATIVE ) );        
    }        

    for( unsigned int i = 0; i < externalLiterals.size(); i++ )
    {
        Literal lit = externalLiterals[ i ];
        assert( getCheckerVarFromExternalLiteral( lit ) != UINT_MAX );        
        assumptionsAND.push_back( Literal( getCheckerVarFromExternalLiteral( lit ), solver.isTrue( lit ) ? POSITIVE : NEGATIVE ) );        
    }

    statistics( &checker, assumptionsAND( assumptionsAND.size() ) );
    statistics( &checker, assumptionsOR( assumptionsOR.size() ) );
}

void
HCComponent::addClauseToChecker(
    Clause* c,
    Var headAtom )
{
    assert( c != NULL );
    trace_msg( modelchecker, 2, "Adding clause " << *c );
    Clause& orig = *c;
    Clause* clause = new Clause( c->size() );    
    for( unsigned int i = 0; i < orig.size(); i++ )
    {        
        clause->addLiteral( orig[ i ] );        
        Var v = orig[ i ].getVariable();
        if( !sameComponent( v ) )
        {
            Var newVar = getCheckerVarFromExternalLiteral( orig[ i ] );            
            if( newVar != v )
            {
                orig[ i ].setVariable( newVar );
                orig[ i ].setPositive();
            }
        }
        else
        {
            if( orig[ i ].isNegativeBodyLiteral() )
            {
                Var newVar = getCheckerVarFromExternalLiteral( orig[ i ] );
                if( newVar != v )
                {
                    orig[ i ].setVariable( newVar );
                    orig[ i ].setPositive();
                }
            }
        }
    }    
    getGUSData( headAtom ).definingRulesForNonHCFAtom.push_back( clause );    
    checker.addClause( c );    
}

Clause*
HCComponent::getClauseToPropagate(
    Learning& learning )
{
    assert( unfoundedSet.empty() );
    if( hasToTestModel )
        testModel();
    if( unfoundedSet.empty() )
    {
        return NULL;
    }
    else
    {
        trace_msg( modelchecker, 1, "Learning unfounded set rule for component " << *this );
        Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet( unfoundedSet );
        trace_msg( modelchecker, 1, "Adding loop formula: " << *loopFormula );        
        unfoundedSet.clear();        
        solver.setAfterConflictPropagator( this ); 
        solver.onLearningALoopFormulaFromModelChecker();
        return loopFormula;
    }        
}

void
HCComponent::computeReasonForUnfoundedAtom(
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
            if( isInUnfoundedSet( lit.getVariable() ) )
            {
                trace_msg( modelchecker, 4, "Literal " << lit << " is in the unfounded set" );
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
            
            //This should be not true anymore.
//            assert( !isInUnfoundedSet( lit.getVariable() ) );
            //If the variable is in the HCC component and it is undefined can be a reason during partial checks
            if( solver.isUndefined( lit ) && solver.getHCComponent( lit.getVariable() ) == this )
            {
                if( pos == UINT_MAX )
                    pos = j;
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
HCComponent::addLearnedClausesFromChecker(
    Clause* learnedClause )
{
    assert( solver.exchangeClauses() );
    if( learnedClause->size() > 8 )
        return;        
    Clause* c = new Clause( learnedClause->size() );
    for( unsigned int i = 0; i < learnedClause->size(); i++ )
    {
        Literal current = getGeneratorLiteralFromCheckerLiteral( learnedClause->getAt( i ) );
        c->addLiteral( current );
    }
    learnedClausesFromChecker.push_back( c );
}

bool
HCComponent::sameComponent(
    Var v ) const
{
    return solver.getHCComponent( v ) == this;
}