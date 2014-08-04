#include "HCComponent.h"
#include "Solver.h"
#include "input/Dimacs.h"
#include "outputBuilders/DimacsOutputBuilder.h"
#include "outputBuilders/WaspOutputBuilder.h"
#include "util/Trace.h"
#include "util/Options.h"

#include <vector>

HCComponent::HCComponent(
    Solver& s ) : PostPropagator(), solver( s ), first( true )
{   
    unsigned int i = 0;
    while( i++ < s.numberOfVariables() )
        checker.addVariable();    
    assert( checker.numberOfVariables() == s.numberOfVariables() );
    
    checker.setOutputBuilder( new WaspOutputBuilder() );
    checker.init();    
}

HCComponent::~HCComponent()
{
}

void
HCComponent::reset()
{    
    while( !trail.empty() && solver.isUndefined( trail.back() ) )
        trail.pop_back();
    
    unfoundedSet.clear();
}

bool
HCComponent::onLiteralFalse(
    Literal literal )
{
    trail.push_back( literal );    
    if( trail.size() == ( hcVariables.size() + externalLiterals.size() ) )
    {
        testModel();
        return true;
    }
    
    return false;
}

void
HCComponent::testModel()
{
    vector< Literal > assumptionsAND;
    vector< Literal > assumptionsOR;    

    computeAssumptions( assumptionsAND, assumptionsOR );
    if( assumptionsOR.empty() )
        return;
    
    if( first )
    {
        first = false;
        bool result = checker.preprocessing();
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
    
    if( checker.solve( assumptionsAND, assumptionsOR ) )
    {
        trace_msg( modelchecker, 1, "The SAT formula is satisfiable: the model is not stable." );                
        for( unsigned int i = 0; i < assumptionsOR.size(); i++ )
            if( checker.isTrue( assumptionsOR[ i ] ) )
                unfoundedSet.push_back( assumptionsOR[ i ].getVariable() );

        trace_action( modelchecker, 2,
        {            
            trace_tag( cerr, modelchecker, 2 );
            cerr << "The unfounded set is: ";
            for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
                cerr << " " << Literal( unfoundedSet[ i ], POSITIVE );
            cerr << endl;
        } );
        assert( !unfoundedSet.empty() );
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
        if( solver.isTrue( v ) )
            assumptionsOR.push_back( Literal( v, NEGATIVE ) );
        else
            assumptionsAND.push_back( Literal( v, NEGATIVE ) );
    }

    for( unsigned int i = 0; i < externalLiterals.size(); i++ )
    {
        Literal lit = externalLiterals[ i ];
        if( solver.isTrue( lit ) )
            assumptionsAND.push_back( lit );
        else
            assumptionsAND.push_back( lit.getOppositeLiteral() );
    }
}

void
HCComponent::addClauseToChecker(
    Clause* c )
{
    assert( c != NULL );
    trace_msg( modelchecker, 2, "Adding clause " << *c );
    checker.addClause( c );
}

Clause*
HCComponent::getClauseToPropagate(
    Learning& learning )
{   
    if( unfoundedSet.empty() )
    {        
        return NULL;
    }
    else
    {
        Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet( unfoundedSet, externalLiterals );
        
        trace_msg( modelchecker, 1, "Adding loop formula: " << *loopFormula );        
        unfoundedSet.clear();
        return loopFormula;            
    }    
}