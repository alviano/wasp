/*
 *
 *  Copyright 2014 Mario Alviano, Carmine Dodaro, Francesco Ricca and
 *                 Pierfrancesco Veltri.
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

/* 
 * File:   InputBuilder.h
 * Author: cesco
 *
 * Created on 6 marzo 2014, 23.03
 */

#ifndef INPUTBUILDER_H
#define	INPUTBUILDER_H

namespace DLV2
{

    class InputBuilder {
    public:
        InputBuilder() { }
        virtual ~InputBuilder() { }

        virtual void onDirective( char* directiveName, char* directiveValue ) = 0;
        virtual void onRule() = 0;
        virtual void onConstraint() = 0;
        virtual void onWeakConstraint() = 0;
        virtual void onQuery() = 0;
        virtual void onHeadAtom() = 0;
        virtual void onHead() = 0;
        virtual void onBodyLiteral() = 0;
        virtual void onBody() = 0;
        virtual void onNafLiteral( bool naf = false ) = 0;
        virtual void onAtom( bool isStrongNeg = false ) = 0;
        virtual void onExistentialAtom() = 0;
        virtual void onPredicateName( char* name ) = 0;
        virtual void onExistentialVariable( char* var ) = 0;
        virtual void onEqualOperator() = 0;
        virtual void onUnequalOperator() = 0;
        virtual void onLessOperator() = 0;
        virtual void onLessOrEqualOperator() = 0;
        virtual void onGreaterOperator() = 0;
        virtual void onGreaterOrEqualOperator() = 0;
        virtual void onTerm( char* value ) = 0;
        virtual void onTerm( int value ) = 0;
        virtual void onUnknownVariable() = 0;
        virtual void onFunction( char* functionSymbol, int nTerms ) = 0;
        virtual void onTermDash() = 0;
        virtual void onTermParams() = 0;
        virtual void onTermRange( char* lowerBound, char* upperBound ) = 0;
        virtual void onArithmeticOperation( char arithOperator ) = 0;
        virtual void onWeightAtLevels( int nWeight, int nLevel, int nTerm ) = 0;
        virtual void onChoiceLowerGuard() = 0;
        virtual void onChoiceUpperGuard() = 0;
        virtual void onChoiceElementAtom() = 0;
        virtual void onChoiceElementLiteral() = 0;
        virtual void onChoiceElement() = 0;
        virtual void onChoiceAtom() = 0;
        virtual void onBuiltinAtom() = 0;
        virtual void onAggregateLowerGuard() = 0;
        virtual void onAggregateUpperGuard() = 0;
        virtual void onAggregateFunction( char* functionSymbol ) = 0;
        virtual void onAggregateGroundTerm( char* value, bool dash = false ) = 0;
        virtual void onAggregateVariableTerm( char* value ) = 0;
        virtual void onAggregateUnknownVariable() = 0;
        virtual void onAggregateNafLiteral() = 0;
        virtual void onAggregateElement() = 0;
        virtual void onAggregate( bool naf = false ) = 0;
    };
    
};

#endif	/* INPUTBUILDER_H */

