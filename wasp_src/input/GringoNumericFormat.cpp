/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

#include "GringoNumericFormat.h"
#include "../Constants.h"
#include "../ErrorMessage.h"
#include "../Clause.h"

#include <cassert>
#include <iostream>
#include <unordered_set>
using namespace std;

GringoNumericFormat::~GringoNumericFormat()
{    
}

void
GringoNumericFormat::parse()
{
    this->parse( cin );
}

void
GringoNumericFormat::parse(
    istream& input )
{
    builder->startBuilding();
    bool loop = true;
    
    while( loop )
    {
        unsigned int type;
        input >> type;
    
        switch( type )
        {
            case GRINGO_NORMAL_RULE_ID:
                readNormalRule( input );
                break;
                
            case GRINGO_CHOICE_RULE_ID:
                readChoiceRule( input );
                break;
                
            case GRINGO_CONSTRAINT_RULE_ID:
                readConstraintRule( input );
                break;
                
            case GRINGO_DISJUNCTIVE_RULE_ID:
                readDisjunctiveRule( input );
                break;

            case GRINGO_LINE_SEPARATOR:
                loop = false;
                break;
        }
    }
    
    readAtomsTable( input );
    readTrueAtoms( input );
    readFalseAtoms( input );
    readErrorNumber( input );
    builder->endBuilding();
}

void
GringoNumericFormat::readNormalRule(
    istream& input )
{
    unsigned int headAtom;

    input >> headAtom;

    assert( headAtom > 0 );    

    addNewVariable( headAtom );
    
    if( headAtom > 0 )
    {
        unsigned int bodySize;
        
        input >> bodySize;
        if( bodySize != 0 )
        {
            Clause* clause = builder->startClause( 2 );

            builder->addLiteralInClause( headAtom, clause );
            if( bodySize == 1 )
            {
                unsigned int negativeSize;
                input >> negativeSize;
                if( negativeSize > 1 )
                {
                    ErrorMessage::errorDuringParsing( "Negative size must be equal or less than total size." );
                }
                
                unsigned int next;
                input >> next;                                
                
                addNewVariable( next );
                
                if( negativeSize == 1 )
                {
                    //Switch the polarity of the literal.
                    builder->addLiteralInClause( next, clause );
                    builder->addSupportingLiteralForHeadAtom( headAtom, -next );
                }
                else
                {
                    assert( negativeSize == 0 );
                    //Switch the polarity of the literal.
                    builder->addLiteralInClause( -next, clause );
                    builder->addSupportingLiteralForHeadAtom( headAtom, next );
                }
            }
            else
            {
                unsigned int negativeSize;
                input >> negativeSize;

                if( negativeSize > bodySize )
                {
                    ErrorMessage::errorDuringParsing( "Negative size must be equal or less than total size." );
                }

                WaspRule* waspRule = builder->startWaspRule( bodySize + 1 );

                unordered_set< unsigned int > negativeLiteralsInserted;
                unordered_set< unsigned int > positiveLiteralsInserted;    
                
                unsigned int next;
                unsigned int i = 0;
                
                while( i < negativeSize )
                {                    
                    input >> next;
                    
                    //Avoid duplicate literals.
                    if( negativeLiteralsInserted.insert( next ).second )
                    {
                        addNewVariable( next );
                        //Switch the polarity of the literal.
                        builder->addLiteralInWaspRule( next, waspRule );
                    }
                    i++;
                }

                while( i < bodySize )
                {
                    input >> next;
                    
                    //Avoid duplicate literals.
                    if( positiveLiteralsInserted.insert( next ).second )
                    {
                        addNewVariable( next );
                        //Switch the polarity of the literal.
                        builder->addLiteralInWaspRule( -next, waspRule );
                    }
                    i++;
                }

                AuxLiteral* auxLiteral = builder->endWaspRule( waspRule );
                builder->addAuxLiteralInClause( auxLiteral, clause );
                builder->addSupportingAuxLiteralForHeadAtom( headAtom, auxLiteral );
            }
            
            builder->endClause( clause );
        }
        else
        {
            assert( bodySize == 0 );
            unsigned int negativeSize;
            input >> negativeSize;
            if( negativeSize != 0 )
            {
                ErrorMessage::errorDuringParsing( "Negative size must be equal or less than total size." );
            }
            
            builder->addSupportedVariable( headAtom );
            builder->addTrueLiteral( headAtom );
        }
    }
}

void
GringoNumericFormat::readChoiceRule(
    istream& input )
{
    ErrorMessage::errorDuringParsing( "Unsupported rule type." );
//    GringoRule* gRule = new GringoRule();
//
//    unsigned int size;
//    input >> size;
//
//    unsigned int i = 0;
//
//    gRule->reserveHeadsSize( size );
//
//    while( i < size )
//    {
//        unsigned int headAtom;
//
//        input >> headAtom;
//
//        assert( headAtom > 1 );
//
//        headAtom--;
//        checkMax( headAtom );
//
//        gRule->pushHeadAtom( headAtom );
//
//        i++;
//    }
//
//    gRule->setChoiceRule();
//
//    readBody( input, gRule );
}

void
GringoNumericFormat::readDisjunctiveRule(
    istream& input )
{
    ErrorMessage::errorDuringParsing( "Unsupported rule type." );
//    GringoRule* gRule = new GringoRule();
//
//    unsigned int size;
//    input >> size;
//
//    unsigned int i = 0;
//
//    gRule->reserveHeadsSize( size );
//
//    while( i < size )
//    {
//        unsigned int headAtom;
//
//        input >> headAtom;
//
//        assert( headAtom > 1 );
//
//        headAtom--;
//        checkMax( headAtom );
//
//        gRule->pushHeadAtom( headAtom );
//
//        i++;
//    }
//
//    readBody( input, gRule );
}

void
GringoNumericFormat::readAtomsTable(
    istream& input )
{
    unsigned int nextAtom;
    input >> nextAtom;

    while( nextAtom != 0 )
    {
        string name;
        input >> name;

        builder->setAtomName( nextAtom, name );       
        input >> nextAtom;
    }    
}

void
GringoNumericFormat::readTrueAtoms(
    istream& input )
{
    char b;
    
    input >> b;
    input >> b;
    
    unsigned int nextAtom;
    input >> nextAtom;
    
    while( nextAtom != 0 )
    {
        assert( nextAtom > 0 );
        builder->addSupportedVariable( nextAtom );
        builder->addTrueLiteral( nextAtom );
        
        input >> nextAtom;
    }
}

void
GringoNumericFormat::readFalseAtoms(
    istream& input )
{
    char b;
    
    input >> b;
    input >> b;
    
    unsigned int nextAtom;
    input >> nextAtom;
    
    while( nextAtom != 0 )
    {        
        if( nextAtom != 0 )
        {
            builder->addSupportedVariable( nextAtom );
            builder->addTrueLiteral( -nextAtom );
        }
        
        input >> nextAtom;
    }
}

void
GringoNumericFormat::readErrorNumber(
    istream& input )
{
    unsigned int errorNumber;
    input >> errorNumber;
    
    if( errorNumber != 1 )
    {
        cout << ERRORPARSING << endl;
        exit( 0 );
    }
    else
    {
    }
}

void 
GringoNumericFormat::readConstraintRule( 
    istream & input )
{
    ErrorMessage::errorDuringParsing("Unsupported rule type.");
//    unsigned int id;
//    input >> id;
//    
//    assert( id > 1 );
//    id--;
//    
//    checkMax( id );
//    
//    unsigned int literalsSize;
//    input >> literalsSize;
//    
//    unsigned int negLiteralsSize;
//    input >> negLiteralsSize;
//    
//    if( negLiteralsSize > 0 )
//    {
//        cerr << GRINGO_NEG_LIT_NOT_SUPPORTED << endl;
//        exit( 0 );
//    }
//    
//    unsigned int bound;
//    input >> bound;
//    
//    GringoAggregate* gAggregate = new GringoAggregate( id, bound, literalsSize );
//    
//    unsigned int i = 0;
//    while( i < literalsSize )
//    {    
//        unsigned int atom;
//
//        input >> atom;
//
//        assert( atom > 1 );
//        
//        atom--;
//        checkMax( atom );
//        
//        gAggregate->addAtom( atom );
//        
//        atomAggregateTypeSet[ atom ] = GRINGO_ATOM_IN_AGGREGATE_VALUE;
//        
//        i++;
//    }
//    
//    atomAggregateTypeSet[ id ] = GRINGO_AGGREGATE_GREATER_VALUE;
//    
//    gringoAggregates.push_back( gAggregate );
}

void
GringoNumericFormat::addNewVariable(
    unsigned int variable )
{
    while( variable > max )
    {
        builder->newVar();
        max++;
    }
}