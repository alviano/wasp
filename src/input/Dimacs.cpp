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

#include "Dimacs.h"

#include "../Solver.h"
#include "../Clause.h"
#include "../util/WaspConstants.h"
#include "../util/WaspTrace.h"
#include "../util/WaspOptions.h"

#include <cassert>
#include <sstream>
#include <unordered_set>
using namespace std;

Dimacs::Dimacs(
    Solver& s ) : solver( s ), numberOfClauses( 0 ), numberOfVariables( 0 ), maxWeight( 0 ), maxsat( false )
{
}

void
Dimacs::parse()
{
    Istream input( cin );
    this->parse( input );
}

void
Dimacs::parse(
    Istream& input )
{    
    if( !input.readInfoDimacs( numberOfVariables, numberOfClauses, maxWeight ) )
        WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );

    maxsat = maxWeight != 0;
    //insertVariables( numberOfVariables );
    readAllClauses( input );    
}

void
Dimacs::readAllClauses(
    Istream& input )
{
    trace_msg( parser, 1, "Starting iteration on " << numberOfClauses << " clauses.");
    unsigned int numOfClauses = 0;    
    if( !maxsat )
    {
        while( readClause( input ) )
            numOfClauses++;
    }
    else
    {
        while( solver.numberOfVariables() < numberOfVariables )
        {
            solver.addVariable();
            VariableNames::setToBePrinted( solver.numberOfVariables() );
        }
        
        solver.setLevels( 1 );
        while( readWeightedClause( input ) )
            numOfClauses++;
    }
    
    if( input.eof() && numOfClauses != numberOfClauses )
        cerr << "Warning: Read " << numOfClauses << " clauses, expected " << numberOfClauses << " clauses" << endl;
    
    if( input.eof() && !maxsat && solver.numberOfVariables() != numberOfVariables )
        cerr << "Warning: Read " << solver.numberOfVariables() << " variables, expected " << numberOfVariables << " variables" << endl;
}

bool
Dimacs::readClause(
    Istream& input )
{
    //a variable containing the literal to read
    int next;

    //read the next literal
    bool result = readNextLiteral( input, next );
    if( input.eof() )
        return false;
    
    if( !result )
        WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );

    if( next == 0 )
    {
        solver.addClause( solver.newClause() );
        return false;
    }
//        ErrorMessage::errorDuringParsing( "Empty clause are not allowed." );

    unordered_set< int > tempSet;    
    bool trivial = false;
    Clause* clause = solver.newClause();

    do
    {
        while( solver.numberOfVariables() < static_cast< unsigned int > ( abs( next ) ) )
        {            
            solver.addVariable();
            VariableNames::setToBePrinted( solver.numberOfVariables() );
        }

        //insert the current literal in the set
        bool inserted = !tempSet.insert( next ).second;
        trace_msg( parser, 2, "Reading " << next );        
        
        Literal literal = solver.getLiteral( next );

        //if a literal appears in a clause C with both polarities then C is a tautology
        //if a literal is true the clause is satisfied
        if( tempSet.find( -next ) != tempSet.end() || solver.isTrue( literal ) )
            trivial = true;
        else if( solver.isUndefined( literal ) && !inserted ) //skip false literals and already inserted literals
            clause->addLiteral( literal );

        //read the next literal
        bool result = readNextLiteral( input, next );
        if( !result )
            WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );
    } while( next != 0 );
    
    if( !trivial )
    {
        trace_msg( parser, 1, "Adding clause " << *clause << " to the solver" );
        return solver.addClause( clause );
    }
    else
    {
        trace_msg( parser, 1, "Deleting clause " << *clause << " because is trivial" );
        solver.releaseClause( clause );
    }
    
    return true;
}

bool
Dimacs::readWeightedClause(
    Istream& input )
{
    uint64_t weight;
    input.read( weight );
    if( weight == maxWeight )
        return readClause( input );    
    
    //a variable containing the literal to read
    int next;

    //read the next literal
    bool result = readNextLiteral( input, next );
    if( input.eof() )
        return false;
    
    if( !result )
        WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );

    if( next == 0 )
    {
        solver.addClause( solver.newClause() );
        return false;
    }
//        ErrorMessage::errorDuringParsing( "Empty clause are not allowed." );

    unordered_set< int > tempSet;  
    bool trivial = false;
    Clause* clause = solver.newClause();

    do
    {
        if( static_cast< unsigned int > ( abs( next ) ) > numberOfVariables )
            WaspErrorMessage::errorDuringParsing( "Wrong number of variables." );

        //insert the current literal in the set
        bool inserted = !tempSet.insert( next ).second;
        trace_msg( parser, 2, "Reading " << next );

        Literal literal = solver.getLiteral( next );

        //if a literal appears in a clause C with both polarities then C is a tautology
        //if a literal is true the clause is satisfied
        if( tempSet.find( -next ) != tempSet.end() || solver.isTrue( literal ) )
            trivial = true;
        else if( solver.isUndefined( literal ) && !inserted ) //skip false literals and already inserted literals
            clause->addLiteral( literal );

        //read the next literal
        bool result = readNextLiteral( input, next );
        if( !result )
            WaspErrorMessage::errorDuringParsing( "Unexpected symbol." );
    } while( next != 0 );

    if( !trivial )
    {
        solver.addVariable();
        Var newVar = solver.numberOfVariables();
        clause->addLiteral( Literal( newVar, NEGATIVE ) );
        relaxationVars.push_back( newVar );
        relaxationVarsWeights.push_back( weight );
        trace_msg( parser, 1, "Adding weighted clause " << *clause << " to the solver. Weight: " << weight );        
        solver.setFrozen( newVar );
        solver.addOptimizationLiteral( Literal( newVar, NEGATIVE ), weight, 0, false );        
        return solver.addClause( clause );
    }
    else
    {
        trace_msg( parser, 1, "Deleting clause " << *clause << " because is trivial" );
        solver.releaseClause( clause );
    }

    return true;
}
