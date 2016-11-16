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

#include "Learning.h"
#include "Clause.h"
#include "input/Rule.h"

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
            trace_msg( modelchecker, 4, "The reason is: " << rule->getAt( pos ) );
            learning.onNavigatingLiteralForUnfoundedSetLearning( rule->getAt( pos ).getOppositeLiteral() );
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
}

void
HCComponent::createDefiningRules(
    Rule* rule,
    Clause* clause )
{
    for( unsigned int k = 0; k < rule->size(); k++ )
    {
        Literal lit = rule->literals[ k ];
        if( !sameComponent( lit.getVariable() ) )
            continue;
        
        if( lit.isHeadAtom() )
            getGUSData( lit.getVariable() ).definingRulesForNonHCFAtom.push_back( clause );         
    }
    toDelete.push_back( clause );
}