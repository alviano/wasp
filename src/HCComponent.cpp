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
    Solver& s ) : PostPropagator(), gusData( gusData_ ), solver( s ), numberOfCalling( 0 ), first( true )
{   
    unsigned int i = 0;    
    while( i++ < s.numberOfVariables() )
    {
        checker.addVariable();
        inUnfoundedSet.push_back( 0 );
    }
    assert( checker.numberOfVariables() == s.numberOfVariables() );
    assert( checker.numberOfVariables() == inUnfoundedSet.size() );
    
    checker.setOutputBuilder( new WaspOutputBuilder() );
    checker.initFrom( solver );
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
    trace_msg( modelchecker, 1, "Check component " << *this );
    
    vector< Literal > assumptionsAND;
    vector< Literal > assumptionsOR;    

    computeAssumptions( assumptionsAND, assumptionsOR );
    if( assumptionsOR.empty() )
    {
        trace_msg( modelchecker, 2, "Nothing to check here!" );
        return;
    }
    
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
    
    if( checker.numberOfAssignedLiterals() > 1 )
        checker.doRestart();
    if( checker.solve( assumptionsAND, assumptionsOR ) )
    {
        numberOfCalling++;
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
    Clause* c,
    Var headAtom )
{
    assert( c != NULL );
    trace_msg( modelchecker, 2, "Adding clause " << *c );
    checker.addClause( c );
    
    Clause* clause = new Clause( c->size() );
    clause->copyLiterals( *c );
    getGUSData( headAtom ).definingRulesForNonHCFAtom.push_back( clause );
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
        Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet( unfoundedSet );
        
        trace_msg( modelchecker, 1, "Adding loop formula: " << *loopFormula );        
        unfoundedSet.clear();
        return loopFormula;            
    }    
}

void
HCComponent::computeReasonForUnfoundedAtom(
    Var v,
    Learning& learning )
{
    vector< Clause* >& definingRules = getGUSData( v ).definingRulesForNonHCFAtom;        
    for( unsigned int i = 0; i < definingRules.size(); i++ )
    {
        Clause* rule = definingRules[ i ];
        
        bool skipRule = false;
        
        unsigned int min = UINT_MAX;
        unsigned int pos = UINT_MAX;
        for( unsigned int j = 0; j < rule->size(); j++ )
        {
            Literal lit = rule->getAt( j );
            if( !solver.isTrue( lit ) )
                continue;

            Var curr = lit.getVariable();            
            if( isInUnfoundedSet( curr ) )
            {
                if( lit.isHeadAtom() )
                    continue;
                else if( lit.isPositiveBodyLiteral() )
                {
                    skipRule = true;
                    break;
                }
            }
            
            unsigned int dl = solver.getDecisionLevel( lit );            
            if( dl == 0 )
            {
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
            assert( pos < rule->size() );
            learning.onNavigatingLiteralForUnfoundedSetLearning( rule->getAt( pos ).getOppositeLiteral() );
        }
    }
}