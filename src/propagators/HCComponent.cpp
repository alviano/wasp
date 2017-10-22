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

#include "HCComponent.h"

#include "../Learning.h"
#include "../Clause.h"
#include "../input/Rule.h"
#include <iostream>
using namespace std;

ostream& operator<<( ostream& out, const HCComponent& component )
{
    out << component.id << ": [ ";
    for( unsigned i = 0; i < component.hcVariables.size(); ++i )
        out << component.solver.getLiteral( component.hcVariables[ i ] ) << " ";
    return out << "]";
}

void
HCComponent::computeReasonForUnfoundedAtom(
    Var v,
    Learning& learning )
{
    if( wasp::Options::compactReasonsForHCC )
    {
        computeReasonForUnfoundedAtomCompactReasons( v, learning );
        return;
    }
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
            if( solver.isUndefined( lit ) && solver.getHCComponent( lit.getVariable() ) == this && ( lit.isNegativeBodyLiteral() || lit.isHeadAtom() ) )
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
            if( pos >= rule->size() )
                WaspErrorMessage::errorGeneric( "Error while computing reasons for check failure." );
            trace_msg( modelchecker, 4, "The reason is: " << rule->getAt( pos ) );
            learning.onNavigatingLiteralForUnfoundedSetLearning( rule->getAt( pos ).getOppositeLiteral() );
        }
    }
}

void
HCComponent::computeReasonForUnfoundedAtomCompactReasons(
    Var v,
    Learning& learning )
{
    trace_msg( modelchecker, 2, "Processing variable " << solver.getLiteral( v ) );
    vector< Clause* >& definingRules = getGUSData( v ).definingRulesForNonHCFAtom;
    vector< Literal >& definingLiterals = getGUSData( v ).definingLiteralsForNonHCFAtom;
    for( unsigned int i = 0; i < definingRules.size(); i++ )
    {
        Clause* rule = definingRules[ i ];
        bool skipRule = false;
        Literal l = Literal::null;
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
            
            if( solver.isUndefined( lit ) && sameComponent( lit.getVariable() ) && ( lit.isNegativeBodyLiteral() || lit.isHeadAtom() ) )
            {
                trace_msg( modelchecker, 4, "is undefined and in the same component" );
                if( l == Literal::null )
                    l = lit;
                continue;
            }
            
            if( !solver.isTrue( lit ) )
            {
                trace_msg( modelchecker, 5, "Skip " << lit << " because it is not true" );
                continue;
            }
            
            if( solver.getDecisionLevel( lit ) == 0 )
            {
                skipRule = true;
                break;
            }
            
            if( !lit.isHeadAtom() )
            {
                trace_msg( modelchecker, 5, "Skip " << lit << " because it is a body literal" );
                continue;
            }
            
            if( l == Literal::null || solver.isUndefined( l ) || solver.getDecisionLevel( lit ) < solver.getDecisionLevel( l ) )
                l = lit;
        }
        
        if( skipRule )
            continue;
        
        assert( i < definingLiterals.size() );
        Literal defLit = definingLiterals[ i ];
        trace_msg( modelchecker, 2, "Def rule " << *rule );
        trace_msg( modelchecker, 2, "Def lit " << defLit << " which is " << ( solver.isTrue( defLit ) ? "true" : "false/undefined" ) );
        if( defLit.getVariable() != 0 && solver.isTrue( defLit ) )
        {
            if( solver.getDecisionLevel( defLit ) != 0 )
            {
                if( l == Literal::null || solver.isUndefined( l ) || solver.getDecisionLevel( defLit ) < solver.getDecisionLevel( l ) )
                    learning.onNavigatingLiteralForUnfoundedSetLearning( defLit.getOppositeLiteral() );
                else
                    learning.onNavigatingLiteralForUnfoundedSetLearning( l.getOppositeLiteral() );                    
            }
        }
        else
        {
            assert( l != Literal::null );
            learning.onNavigatingLiteralForUnfoundedSetLearning( l.getOppositeLiteral() );
        }        
    }
}

HCComponent::~HCComponent()
{
    delete outputBuilder;
    while( !toDelete.empty() )
    {
        delete toDelete.back();
        toDelete.pop_back();
    }
    checker.enableStatistics();
    statistics( &checker, onDeletingChecker( id ) );
}

Clause*
HCComponent::getClauseToPropagate(
    Learning& learning )
{
    assert( unfoundedSet.empty() );
    if( hasToTestModel )
        testModel();

    hasToTestModel = false;
    if( !unfoundedSet.empty() )
    {
        trace_msg( modelchecker, 1, "Learning unfounded set rule for component " << *this );
        Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet( unfoundedSet );
        trace_msg( modelchecker, 1, "Adding loop formula: " << *loopFormula );
        unfoundedSet.clear();
        solver.onLearningALoopFormulaFromModelChecker();
        return loopFormula;
    }
    return NULL;
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
