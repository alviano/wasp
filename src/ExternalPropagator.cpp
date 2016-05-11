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

#include "ExternalPropagator.h"
#include "heuristic/interpreters/Interpreter.h"
#include "Literal.h"
#include "stl/Vector.h"
#include "Solver.h"
#include "Clause.h"
#include "heuristic/interpreters/MyPerlInterpreter.h"
#include "heuristic/interpreters/MyPythonInterpreter.h"

ExternalPropagator::~ExternalPropagator()
{
    delete interpreter;
    delete fn;
    clearClausesToDelete();
}

ExternalPropagator::ExternalPropagator( 
    const string& filename,
    unsigned int interpr ) : Propagator()
{
    fn = new char[ filename.length() + 1 ];
    strcpy( fn, filename.c_str() );
    interpreter = NULL;
    
    if( interpr == PYTHON_INTERPRETER )
        interpreter = new MyPythonInterpreter( fn );
    else if( interpr == PERL_INTERPRETER )
        interpreter = new MyPerlInterpreter( fn );
    else
        ErrorMessage::errorGeneric( "Unsupported interpreter" );
    checkWellFormed();
    check_addedVarName = interpreter->checkMethod( method_plugins_addedVarName );
    check_onAtomElimination = interpreter->checkMethod( method_plugins_onAtomElimination );
    check_onLitAtLevelZero = interpreter->checkMethod( method_plugins_onLitAtLevelZero );    
    check_simplifyAtLevelZero = interpreter->checkMethod( method_plugins_simplifyAtLevelZero );
    check_onAnswerSet = interpreter->checkMethod( method_plugins_onAnswerSet );
}

bool
ExternalPropagator::isProgramIncoherent()
{
    if( interpreter->checkMethod( method_plugins_isProgramIncoherent ) )
        return interpreter->callIntMethod( method_plugins_isProgramIncoherent );
    return true;
}

bool
ExternalPropagator::onLiteralFalse(
    Solver& solver,
    Literal literal,
    int pos )
{
    vector< int > output;
    
    //True literals are stored in the trail
    if( solver.getCurrentDecisionLevel() > 0 )
    {
        interpreter->callListMethod( method_plugins_onLiteralTrue, literal.getOppositeLiteral().getId(), pos, output );    
        trail.push_back( literal.getOppositeLiteral() );
    }
    if( output.empty() || ( output.size() == 1 && output[ 0 ] == 0 ) )
        return true;
    
    Clause* reason = getReason( solver );
    //TODO: check for backjumping
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal lit = Literal::createLiteralFromInt( output[ i ] );
        //Literals inferred from the propagators are stored
        trail.push_back( lit );
        solver.assignLiteral( lit, reason );
    }
    clausesToDelete.push_back( reason );
    return true;
}

void
ExternalPropagator::reset(
    const Solver& solver )
{
    if( solver.getCurrentDecisionLevel() == 0 )
        clearClausesToDelete();
    vector< int > parameters;    
    while( !trail.empty() )
    {
        Literal lit = trail.back();
        if( !solver.isUndefined( lit ) )
            break;
        parameters.push_back( lit.getId() );
        trail.pop_back();
    }

    if( !parameters.empty() )
        interpreter->callVoidMethod( method_plugins_onLiteralsUndefined, parameters );
}

void
ExternalPropagator::attachWatches(
    Solver& solver )
{
    vector< int > output;
    interpreter->callListMethod( method_plugins_getLiterals, solver.numberOfVariables(), output );
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal lit = Literal::createLiteralFromInt( output[ i ] );
        solver.setFrozen( lit.getVariable() );
        solver.addPropagator( lit.getOppositeLiteral(), this, i );
    }
    output.clear();
    interpreter->callListMethod( method_plugins_getAtomsToFreeze, output );
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal lit = Literal::createLiteralFromInt( output[ i ] );
        solver.setFrozen( lit.getVariable() );
    }
}

void
ExternalPropagator::checkWellFormed()
{
    bool check = interpreter->checkMethod( method_plugins_onLiteralTrue ) &&
                 interpreter->checkMethod( method_plugins_onLiteralsUndefined ) &&
                 interpreter->checkMethod( method_plugins_getLiterals ) &&
                 interpreter->checkMethod( method_plugins_getReason ) &&
                 interpreter->checkMethod( method_plugins_getAtomsToFreeze );
    
    if( !check )
    {
        ErrorMessage::errorGeneric( "Methods " + 
                                    string( method_plugins_onLiteralTrue ) + ", " + 
                                    string( method_plugins_onLiteralsUndefined ) + ", " + 
                                    string( method_plugins_getLiterals ) + ", and " + 
                                    string( method_plugins_getReason ) + ", and " +
                                    string( method_plugins_getAtomsToFreeze ) +
                                    " are mandatory." );
    }
}

Clause*
ExternalPropagator::getReason(
    Solver& solver )
{
    vector< int > output;
    interpreter->callListMethod( method_plugins_getReason, output );
    Clause* clause = new Clause();
    clause->addLiteral( Literal::null );
    unsigned int max = 0;    
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal l = Literal::createLiteralFromInt( output[ i ] );
        clause->addLiteral( l );        
        unsigned int dl = solver.getDecisionLevel( l );
        if( dl > max )
            max = dl;
    }
    
    if( max < solver.getCurrentDecisionLevel() )
        solver.unroll( max );
    
    unsigned int countUndefined = 0;
    for( unsigned int i = 1; i < clause->size(); i++ )
        if( solver.isUndefined( clause->getAt( i ) ) )
            countUndefined++;
    
    if( countUndefined != 0 )
        ErrorMessage::errorGeneric( "Reason is not well-formed" );
    reset( solver );    
    return clause;
}

void
ExternalPropagator::clearClausesToDelete()
{
    for( unsigned int i = 0; i < clausesToDelete.size(); i++ )
        delete clausesToDelete[ i ];
    clausesToDelete.clear();
}

void
ExternalPropagator::addedVarName(
    Var var,
    const string& name )
{
    if( check_addedVarName )
        interpreter->callVoidMethod( method_plugins_addedVarName, var, name );
}

void
ExternalPropagator::onAtomElimination(
    Var var )
{
    if( check_onAtomElimination )
        interpreter->callVoidMethod( method_plugins_onAtomElimination, var );
}

void
ExternalPropagator::simplifyAtLevelZero(
    Solver& solver )
{
    if( check_simplifyAtLevelZero )
    {
        assert( solver.getCurrentDecisionLevel() == 0 );
        vector< int > output;
        interpreter->callListMethod( method_plugins_simplifyAtLevelZero, output );
        for( unsigned int i = 0; i < output.size(); i++ )
        {
            checkIdOfLiteral( solver, output[ i ] );
            if( !solver.addClause( Literal::createLiteralFromInt( output[ i ] ) ) )
                break;
        }
    }
}

void
ExternalPropagator::onLitAtLevelZero(
    Literal lit )
{
    if( check_onLitAtLevelZero )
        interpreter->callVoidMethod( method_plugins_onLitAtLevelZero, lit.getId() );
}

void ExternalPropagator::onAnswerSet(
    const Solver& solver )
{
    if( check_onAnswerSet )
    {
        vector< int > answerset;
        for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        {
            assert( !solver.isUndefined( i ) );
            if( solver.isTrue( i ) )
                answerset.push_back( i );
            else
                answerset.push_back( -i );
        }        
        interpreter->callVoidMethod( method_plugins_onAnswerSet, answerset );
    }
}

void ExternalPropagator::checkIdOfLiteral(
    const Solver& solver,
    int id ) const
{
    Var var = id > 0 ? id : -id;
    if( var == 0 || var > solver.numberOfVariables() )
        ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
}