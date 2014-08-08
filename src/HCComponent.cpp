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
    while( checker.numberOfVariables() < numberOfInputAtoms )
    {
        inUnfoundedSet.push_back( 0 );
        checker.addVariable();
    }
        
    assert_msg( checker.numberOfVariables() == numberOfInputAtoms, checker.numberOfVariables() << " != " << numberOfInputAtoms );
    assert( checker.numberOfVariables() == inUnfoundedSet.size() - 1 );
    
    checker.setOutputBuilder( new WaspOutputBuilder() );
    checker.initFrom( solver );
    checker.setGenerator( false ); 
    checker.disableStatistics();
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
    }
    checker.addVariable();
    externalLiterals.push_back( lit );
    assert( checker.numberOfVariables() == numberOfAtoms + externalLiterals.size() );
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
        addClausesForPartialModelChecks();
        trace_msg( modelchecker, 2, "First call. Removing unused variables" );
//        for( unsigned i = 1; i <= checker.numberOfVariables(); ++i )        
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
        cerr << "Assumptions AND:";
        for( unsigned int i = 0; i < assumptionsAND.size(); i++ )
            cerr << " " << assumptionsAND[ i ];
        cerr << endl;
        trace_tag( cerr, modelchecker, 2 );
        cerr << "Assumptions OR:";
        for( unsigned int i = 0; i < assumptionsOR.size(); i++ )
            cerr << " " << assumptionsOR[ i ];
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
        statistics( &checker, foundUS( unfoundedSet.size() ) );
        assert( !unfoundedSet.empty() );
    }
    statistics( &checker, endCheckerInvokation( time( 0 ) ) );
    
    if( solver.exchangeClauses() )
    {
//        for( Solver::ClauseIterator it = checker.learnedClauses_begin(); it != checker.learnedClauses_end(); it++ )
//        {            
//            Clause* learnedClause = *it;
//            if( learnedClause->addedInSolver() || learnedClause->size() > 8 )
//                continue;
//            Clause* c = new Clause( learnedClause->size() );
//            
//            for( unsigned int i = 0; i < learnedClause->size(); i++ )
//            {
//                Literal current = getGeneratorLiteralFromCheckerLiteral( learnedClause->getAt( i ) );
//                c->addLiteral( current );
//            }
//            
//            learnedClause->setAddedInSolver( true );
//            solver.addClauseInLearnedFromAllSolvers( c );
//        }
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
        Var v = lit.getVariable();        
        if( solver.isTrue( v ) )
        {
            assumptionsAND.push_back( Literal( getCheckerTrueVar( v, i ), POSITIVE ) );
            assumptionsAND.push_back( Literal( getCheckerFalseVar( v, i ), NEGATIVE ) );            
        }
        else if( solver.isFalse( v ) )
        {
            assumptionsAND.push_back( Literal( getCheckerTrueVar( v, i ), NEGATIVE ) );
            assumptionsAND.push_back( Literal( getCheckerFalseVar( v, i ), POSITIVE ) );            
        }
        else
        {
            assumptionsAND.push_back( Literal( getCheckerTrueVar( v, i ), NEGATIVE ) );
            assumptionsAND.push_back( Literal( getCheckerFalseVar( v, i ), NEGATIVE ) );
        }
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
        if( isExternal( v ) )
        {
            Var newVar = getCheckerVarFromExternalLiteral( orig[ i ] );
            if( newVar != v )
            {
                orig[ i ].setVariable( newVar );
                orig[ i ].setPositive();
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
HCComponent::addClausesForPartialModelChecks()
{
    return;
//    for( unsigned int i = 0; i < hcVariables.size(); i++ )
//    {
//        Var v = hcVariables[ i ];
//        Clause* c = new Clause( 3 );
//        c->addLiteral( Literal( v, POSITIVE ) );
//        c->addLiteral( Literal( getCheckerTrueVar( v ), NEGATIVE ) );
//        c->addLiteral( Literal( getCheckerFalseVar( v ), NEGATIVE ) );
//        checker.addClause( c );
//        
//        c = new Clause( 3 );
//        c->addLiteral( Literal( v, POSITIVE ) );
//        c->addLiteral( Literal( getCheckerTrueVar( v ), POSITIVE ) );
//        c->addLiteral( Literal( getCheckerFalseVar( v ), POSITIVE ) );
//        checker.addClause( c );
//        
////        checker.addClause( Literal( getCheckerTrueVar( v ), POSITIVE ), Literal( getCheckerFalseVar( v ), POSITIVE ) );
//    }
    
//    for( unsigned int i = 0; i < externalLiterals.size(); i++ )
//    {
//        Var v = externalLiterals[ i ].getVariable();
//        Clause* c = new Clause( 3 );
//        c->addLiteral( Literal( v, POSITIVE ) );
//        c->addLiteral( Literal( getCheckerTrueVar( v ), NEGATIVE ) );
//        c->addLiteral( Literal( getCheckerFalseVar( v ), NEGATIVE ) );
//        checker.addClause( c );
//        
//        checker.addClause( Literal( getCheckerTrueVar( v ), POSITIVE ), Literal( getCheckerFalseVar( v ), POSITIVE ) );
//    }
}