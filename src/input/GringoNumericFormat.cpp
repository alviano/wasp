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

#include "GringoNumericFormat.h"
#include "../util/Constants.h"
#include "../util/ErrorMessage.h"
#include "../Clause.h"
//#include "../CRule.h"

#include <cassert>
#include <iostream>
#include <unordered_set>
using namespace std;


// TODO: revise

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
        
        if( !ok )
            return;
    }

    dealWithSupport();
    readAtomsTable( input );
    readTrueAtoms( input );
    readFalseAtoms( input );
    readErrorNumber( input );
    
    solver.computeStrongConnectedComponents();
    
    if( !solver.tight() )
    {
        programIsNotTight();
    }
}

void
GringoNumericFormat::readNormalRule(
    istream& input )
{
    trace_msg( parser, 1, "Reading normal rule.");
    unsigned int headAtom;

    input >> headAtom;
    trace_msg( parser, 1, "Head atom " << headAtom );

    assert_msg( headAtom > 0, "In the numeric format of gringo, the head atom is always greater than 0." );

    addNewVariable( headAtom );

    if( headAtom > 1 )
    {
        Literal bodyLiteral = readBody( input );
        if( !ok )
            return;
        if( bodyLiteral != Literal::null )
        {            
            assert_msg( headAtom < supportVector.size(), "Found value for head atom: " << headAtom << ", which is greater than the size of support vector: " << supportVector.size() );
            trace_msg( parser, 1, "Added body literal " << bodyLiteral << " in the support vector of head atom " << getLiteral( getIdInSolver( headAtom, false ) ) );
            supportVector[ headAtom ].push_back( bodyLiteral );
            
            Clause* c = solver.newClause();            
            
            if( !addLiteralInClause( getLiteral( getIdInSolver( headAtom, false ) ), c ) )
                return;            

            if( !addLiteralInClause( bodyLiteral.getOppositeLiteral(), c ) )
                return;
            
            trace_msg( parser, 1, "Added clause " << *c );
            ok = solver.addClause( c );
            if( !ok )
                return;

            if( bodyLiteral.isPositive() )
                solver.addEdgeInDependencyGraph( getIdInSolver( headAtom, false ), bodyLiteral.getVariable()->getId() );
        }
        else
        {
            trace_msg( parser, 1, "Rule is a fact: " << getIdInSolver( headAtom, false ) << " must be true.");
            /*
             * The body is empty: this rule is a fact.
             * Hence the head atom must be true.
             * 
             */
            addTrueLiteralInSolver( getIdInSolver( headAtom, false ) );
            supportedVariables.insert( getVariable( getIdInSolver( headAtom, false ) ) );
        }
    }
    else
    {
        assert( headAtom == 1 );
        readConstraint( input );
        if( !ok )
            return;
    }    
}

Literal
GringoNumericFormat::readBody(
    istream& input )
{
    unsigned int bodySize;
    input >> bodySize;

    unsigned int negativeSize;
    input >> negativeSize;

    if( negativeSize > bodySize )
        ErrorMessage::errorDuringParsing( "Negative size must be equal or less than total size." );

    trace_msg( parser, 2, "Reading body. Positive literals: " << ( bodySize - negativeSize ) << " - Negative literals: " << negativeSize );
    
    if( bodySize == 0 )
        return Literal::null;

    if( bodySize == 1 )
    {
        unsigned int next;
        input >> next;

        addNewVariable( next );

        unsigned int idInSolver = getIdInSolver( next, false );
        return negativeSize == 1 ? getLiteral( -idInSolver ) : getLiteral( idInSolver );
    }
    else
    {
        unordered_set< unsigned int > negativeLiteralsInserted;
        unordered_set< unsigned int > positiveLiteralsInserted;

        unsigned int next;
        unsigned int i = 0;

        Clause* clause = solver.newClause();

        bodiesDictionary.startInsertion();
        bool trivialClause = false;
        
        while( i < negativeSize )
        {
            input >> next;
            
            //Avoid duplicate literals.
            if( !trivialClause && negativeLiteralsInserted.insert( next ).second )
            {
                addNewVariable( next );
                bodiesDictionary.addElement( -next );

                trace_msg( parser, 4, "Reading negative literal with id " << ( int ) -next << " in body" );
                unsigned int idInSolver = getIdInSolver( next, false );                
                
                //Switch the polarity of the literal.
                if( !addLiteralInClause( getLiteral( idInSolver ), clause ) )
                    trivialClause = true;
            }
            i++;
        }

        while( i < bodySize )
        {
            input >> next;

            //Avoid duplicate literals.
            if( !trivialClause && positiveLiteralsInserted.insert( next ).second )
            {
                addNewVariable( next );
                bodiesDictionary.addElement( next );

                trace_msg( parser, 4, "Reading positive literal with id " << next << " in body" );
                unsigned int idInSolver = getIdInSolver( next, false );
                //Switch the polarity of the literal.
                if( !addLiteralInClause( getLiteral( -idInSolver ), clause ) )
                    trivialClause = true;
            }
            i++;
        }

        unsigned int idAuxLiteral = bodiesDictionary.endInsertion();
        
        if( existsAuxVariable( idAuxLiteral ) )
        {
            unsigned int idInSolver = getIdInSolver( idAuxLiteral, true );
            trace_msg( parser, 4, "Body with id " << idInSolver << " has been already inserted: skip it" );
            
            if( trivialClause )
                addTrueLiteralInSolver( -idInSolver );
            else
                solver.releaseClause( clause );
            return getLiteral( idInSolver );
        }
        else
        {
            this->addNewAuxVariable( idAuxLiteral );
            
            unsigned int idInSolver = getIdInSolver( idAuxLiteral, true );

            trace_msg( parser, 4, "Inserted body with id " << idInSolver );
            Literal lit = getLiteral( idInSolver );
            if( !trivialClause )
            {
                if( !lit.isFalse() )
                {
                    Literal oppLit = lit.getOppositeLiteral();                    
                    
                    for( unsigned int i = 0; i < clause->size(); i++ )
                    {
                        Clause* c = solver.newClause();
                        addLiteralInClause( oppLit, c );

                        assert( clause->getAt( i ).getOppositeLiteral().isUndefined() );
                        c->addLiteral( clause->getAt( i ).getOppositeLiteral() );

                        trace_msg( parser, 1, "Adding supporting rule " << *c << " for variable " << lit );
                        ok = solver.addClause( c );
                        if( !ok )
                            return Literal::null;

                        if( !clause->getAt( i ).isPositive() )
                            solver.addEdgeInDependencyGraph( oppLit.getVariable()->getId(), clause->getAt( i ).getVariable()->getId() );

                        assert_msg( idAuxLiteral < supportVectorForAuxVariables.size(), "Id is " << idAuxLiteral << " while the size of the vector is " << supportVectorForAuxVariables.size() );
                        supportVectorForAuxVariables[ idAuxLiteral ].push_back( clause->getAt( i ).getOppositeLiteral() );
                    }

                    addLiteralInClause( lit, clause );

                    trace_msg( parser, 1, "Adding clause " << *clause );
                    ok = solver.addClause( clause );
                    if( !ok )
                        return Literal::null;
                }
                else
                {
                    solver.releaseClause( clause );
                }
            }
            else
            {
                addTrueLiteralInSolver( -idInSolver );
            }
            return lit;
        }
    }

    assert( 0 );
    return Literal::null;
}

void
GringoNumericFormat::readConstraint(
    istream& input )
{
    unsigned int bodySize;
    input >> bodySize;

    unsigned int negativeSize;
    input >> negativeSize;

    if( negativeSize > bodySize )
        ErrorMessage::errorDuringParsing( "Negative size must be equal or less than total size." );

    trace_msg( parser, 2, "Reading body. Positive literals: " << ( bodySize - negativeSize ) << " - Negative literals: " << negativeSize );
    if( bodySize == 0 )
    {
        //Program is incoherent, so adding the false literal 1 as true.
        addTrueLiteralInSolver( 1 );
    }
    else if( bodySize == 1 )
    {
        unsigned int next;
        input >> next;

        addNewVariable( next );

        unsigned int idInSolver = getIdInSolver( next, false );
        //switch the polarity
        negativeSize == 1 ? addTrueLiteralInSolver( idInSolver ) : addTrueLiteralInSolver( -idInSolver );;        
    }
    else
    {
        unordered_set< unsigned int > negativeLiteralsInserted;
        unordered_set< unsigned int > positiveLiteralsInserted;

        unsigned int next;
        unsigned int i = 0;

        Clause* clause = solver.newClause();
        bool trivialClause = false;

        while( i < negativeSize )
        {
            input >> next;
            
            //Avoid duplicate literals.
            if( !trivialClause && negativeLiteralsInserted.insert( next ).second )
            {
                addNewVariable( next );
        
                trace_msg( parser, 4, "Reading negative literal with id " << ( int ) -next << " in body" );
                unsigned int idInSolver = getIdInSolver( next, false );                
                
                //Switch the polarity of the literal.
                if( !addLiteralInClause( getLiteral( idInSolver ), clause ) )
                    trivialClause = true;
            }
            i++;
        }

        while( i < bodySize )
        {
            input >> next;

            //Avoid duplicate literals.
            if( !trivialClause && positiveLiteralsInserted.insert( next ).second )
            {
                addNewVariable( next );
        
                trace_msg( parser, 4, "Reading positive literal with id " << next << " in body" );
                unsigned int idInSolver = getIdInSolver( next, false );
                //Switch the polarity of the literal.
                if( !addLiteralInClause( getLiteral( -idInSolver ), clause ) )
                    trivialClause = true;
            }
            i++;
        }

        if( trivialClause )
            return;

        trace_msg( parser, 1, "Adding clause " << *clause << " which represent a constraint" );        
        ok = solver.addClause( clause );
        if( !ok )
            return;
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

        VariableNames::setName( getVariable( getIdInSolver( nextAtom, false ) ), name );
        //getVariable( getIdInSolver( nextAtom, false ) )->setName( name );
        input >> nextAtom;
    }
    
    #ifdef TRACE_ON
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        if( VariableNames::isHidden( getVariable( i ) ) )
        {
            stringstream s;
            s << "#hidden" << i;
            string name = s.str();
            VariableNames::setName( getVariable( i ), name );
        }
    }
    #endif
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
        addNewVariable( nextAtom );
        trace_msg( parser, 2, "Adding " << nextAtom << " as true" );
        addTrueLiteralInSolver( getIdInSolver( nextAtom, false ) );
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
        addNewVariable( nextAtom );
        trace_msg( parser, 2, "Adding " << nextAtom << " as false" );
        addTrueLiteralInSolver( -getIdInSolver( nextAtom, false ) );
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
    while( variable > numberOfAddedVariables )
    {
        solver.addVariable();
        solver.addEdgeInDependencyGraph( solver.numberOfVariables(), 0 );
        ++numberOfAddedVariables;
        trace_msg( parser, 4, "Adding a new variable id " << numberOfAddedVariables << ". Id in solver: " << solver.numberOfVariables() );
        gringoIdToSolverId[ numberOfAddedVariables ] = solver.numberOfVariables();
        supportVector.push_back( vector< Literal >() );
    }
}

bool
GringoNumericFormat::existsAuxVariable(
    unsigned int auxVariable ) const
{
    return auxVariable <= numberOfAddedAuxVariables;
}

void
GringoNumericFormat::addNewAuxVariable(
    unsigned int auxVariable )
{
    assert_msg( !existsAuxVariable( auxVariable ), "Aux variable " << auxVariable << " already exists in the solver" );
    #ifdef TRACE_ON
    stringstream s;
    s << "#aux" << auxVariable;    
    solver.addVariable( s.str() );
    #else
    solver.addVariable();
    #endif
    solver.addEdgeInDependencyGraph( solver.numberOfVariables(), 0 );    
    trace_msg( parser, 4, "Adding aux variable id " << auxVariable << ". Id in solver: " << solver.numberOfVariables() );
    gringoIdToSolverId[ -auxVariable ] = solver.numberOfVariables();
    ++numberOfAddedAuxVariables;
    supportVectorForAuxVariables.push_back( vector< Literal >() );
    
    assert_msg( auxVariable == numberOfAddedAuxVariables, "Aux variable id " << auxVariable << " must be equal to the number of added variable " << numberOfAddedAuxVariables );    
}

void
GringoNumericFormat::dealWithSupport()
{
    assert_msg( supportVector[ 0 ].empty(), "Support vector in position 0 contains " << supportVector[ 0 ].size() << " elements" );
    assert_msg( supportVector.size() == 1 || supportVector[ 1 ].empty(), "Support vector in position 1 contains " << supportVector[ 1 ].size() << " elements" );
    
    trace_msg( parser, 1, "Starting iteration on normal variables for dealing with support. Number of variables: " << supportVector.size() - 1 );
    assert_msg( supportVector.size() - 1 == numberOfAddedVariables, "Support vector contains " << supportVector.size() << " elements and there are " << numberOfAddedVariables << " variables" );
    for( unsigned int i = 2; i < supportVector.size(); i++ )
    {
        vector< Literal >& lits = supportVector[ i ];
        unsigned int atomId = getIdInSolver( i, false );

        if( supportedVariables.find( getVariable( atomId ) ) != supportedVariables.end() )
        {
            trace_msg( parser, 2, "Atom " << atomId << " is supported: no operations required" );
            continue;
        }

        if( lits.empty() )
        {
            trace_msg( parser, 2, "Atom " << atomId << " has no supporting rules. It must be inferred false" );
            addTrueLiteralInSolver( -atomId );
            continue;
        }

        trace_msg( parser, 2, "Creating supporting clause for atom " << atomId << " which has " << lits.size() << " supporting rules" );
        //CRule* cRule = new CRule( lits.size() + 1 );
        
        bool trivial = false;
        Clause* clause = solver.newClause();
        for( unsigned int j = 0; j < lits.size(); j++ )
        {
            trace_msg( parser, 3, "Adding literal " << lits[ j ] << " id(" << lits[ j ].getVariable()->getId() << ") in clause");
            if( !addLiteralInClause( lits[ j ], clause ) )
            {
                trivial = true;
                break;
            }            
        }
        
        if( !addLiteralInClause( getLiteral( -atomId ), clause ) )
            trivial = true;
        
        if( trivial )
            continue;

        trace_msg( parser, 1, "Added supporting rule " << *clause << " for variable " << *getVariable( atomId ) );
        ok = solver.addClause( clause );
        if( !ok )
            return;
    }    
}

void
GringoNumericFormat::programIsNotTight()
{
    unordered_map< Variable*, Component* > variableComponent;
    
    //Add two fake positions
    solver.addGUSData( NULL );
    solver.addGUSData( NULL );
    for( unsigned int i = 2; i <= solver.numberOfVariables(); i++ )
    {
        GUSData* gd = new GUSData();
        gd->variable = solver.getVariable( i );
        solver.addGUSData( gd );        
    }
    
    trace_msg( parser, 2, "Program is not tight. Number of cyclic components " << solver.getNumberOfCyclicComponents() );
    for( unsigned int i = 0; i < solver.getNumberOfCyclicComponents(); i++ )
    {
        Component* component = solver.getCyclicComponent( i );
        
        for( unsigned int j = 0; j < component->size(); j++ )
        {
            unsigned int varId = component->getVariable( j );
            trace_msg( parser, 2, "Variable " << *solver.getVariable( varId ) << " is in the cyclic component " << i );
            Variable* currentVariable = solver.getVariable( varId );
            currentVariable->setComponent( component );
            variableComponent[ currentVariable ] = component;
            component->variableHasNoSourcePointer( currentVariable );
        }
        
        solver.addPostPropagator( component );
    }                
    
    unordered_map< Variable*, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];    
    
    for( unsigned int i = 2; i < supportVector.size(); i++ )
    {
        unsigned int atomId = getIdInSolver( i, false );
        Variable* variable = getVariable( atomId );

        if( !variable->isInCyclicComponent() || supportedVariables.find( variable ) != supportedVariables.end() )
            continue;
        
        vector< Literal >& lits = supportVector[ i ];
        if( lits.empty() )
            continue;

        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " which has " << lits.size() << " supporting rules" );
        
        Component* component = variableComponent[ variable ];
        assert( component != NULL );
        
        variable->setFrozen();
        
        unordered_set< Variable* > tmp;
        for( unsigned int j = 0; j < lits.size(); j++ )
        {
            if( !tmp.insert( lits[ j ].getVariable() ).second )
                continue;

            if( lits[ j ].isPositive() && variable->inTheSameComponent( lits[ j ].getVariable() ) )
            {
                trace_msg( parser, 3, "Adding " << lits[ j ] << " as internal rule for " << *variable );
                component->addInternalLiteralForVariable( variable->getId(), lits[ j ] );
            }
            else
            {
                trace_msg( parser, 3, "Adding " << lits[ j ] << " as external rule for " << *variable );
                component->addExternalLiteralForVariable( variable->getId(), lits[ j ] );
            }
            
            component->addVariablePossiblySupportedByLiteral( variable, lits[ j ] );
            
            unsigned int sign = lits[ j ].getSign();
            if( literalsPostPropagator[ sign ][ lits[ j ].getVariable() ].insert( component ).second )            
                lits[ j ].addPostPropagator( component );
            
            lits[ j ].getVariable()->setFrozen();
        }
    }    
    
    for( unsigned int i = 1; i < supportVectorForAuxVariables.size(); i++ )
    {
        unsigned int atomId = getIdInSolver( i, true );
        Variable* variable = getVariable( atomId );
        
        if( !variable->isInCyclicComponent() || supportedVariables.find( variable ) != supportedVariables.end() )
            continue;

        vector< Literal >& lits = supportVectorForAuxVariables[ i ];
        if( lits.empty() )
            continue;

        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " which has " << lits.size() << " supporting rules" );

        variable->setFrozen();
        
        Component* component = variableComponent[ variable ];
        component->setAuxVariable( variable->getId() );
        
        assert( component != NULL );

        unordered_set< Variable* > tmp;
        for( unsigned int j = 0; j < lits.size(); j++ )
        {
            if( !tmp.insert( lits[ j ].getVariable() ).second )
                continue;
            trace_msg( parser, 3, "Adding " << lits[ j ] << " as supporting rule for " << *variable );
            if( lits[ j ].isPositive() && lits[ j ].getVariable()->inTheSameComponent( variable ) )
            {
                component->addAuxVariableSupportedByLiteral( variable, lits[ j ] );
                component->addInternalLiteralForVariable( variable->getId(), lits[ j ] );
            }
            
            lits[ j ].getVariable()->setFrozen();
        }
    }    
}