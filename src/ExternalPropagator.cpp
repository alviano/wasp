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
    unsigned int interpr,
    Solver& solver,
    string scriptDirectory ) : Propagator()
{
    fn = new char[ filename.length() + 1 ];
    strcpy( fn, filename.c_str() );
    interpreter = NULL;
    
    if( interpr == PYTHON_INTERPRETER )
        interpreter = new MyPythonInterpreter( fn, scriptDirectory );
    else if( interpr == PERL_INTERPRETER )
        interpreter = new MyPerlInterpreter( fn, scriptDirectory );
    else
        ErrorMessage::errorGeneric( "Unsupported interpreter" );
    check_addedVarName = interpreter->checkMethod( method_plugins_addedVarName );
    check_onAtomElimination = interpreter->checkMethod( method_plugins_onVariableElimination );
    check_simplifyAtLevelZero = interpreter->checkMethod( method_plugins_simplifyAtLevelZero );
    check_onAnswerSet = interpreter->checkMethod( method_plugins_onAnswerSet );
    
    check_checkAnswerSet = interpreter->checkMethod( method_plugins_checkAnswerSet );
    check_checkPartialInterpretation = interpreter->checkMethod( method_plugins_checkPartialInterpretation );
    check_getReasonForCheckFailure = interpreter->checkMethod( method_plugins_getReasonsForCheckFailure );
    check_onNewLowerBound = interpreter->checkMethod( method_plugins_foundLB );
    check_onNewUpperBound = interpreter->checkMethod( method_plugins_foundUB );
    check_onLitsTrue = interpreter->checkMethod( method_plugins_onLitsTrue );
    check_onLitTrue = interpreter->checkMethod( method_plugins_onLitTrue );
    check_getReasonForLiteral = interpreter->checkMethod( method_plugins_getReasonForLiteral );
    check_endPropagation = interpreter->checkMethod( method_plugins_endPropagation );
    checkWellFormed();
    
    if( check_onLitsTrue )
        solver.addPropagatorAfterUnit( this );        
    
    if( check_checkAnswerSet )
        solver.addPropagatorAttachedToCheckAnswerSet( this );
    
    if( check_onNewLowerBound || check_onNewUpperBound )
        solver.addPropagatorAttachedToBounds( this );
    
    if( check_endPropagation )
        solver.addPropagatorAttachedToEndPropagation( this );
    
    if( check_checkPartialInterpretation )
        solver.addPropagatorAttachedToPartialChecks( this );
}

bool
ExternalPropagator::onLiteralFalse(
    Solver& solver,
    Literal literal,
    int )
{
    assert( solver.isFalse( literal ) );
    assert( !solver.conflictDetected() );
    vector< int > output;
    if( check_onLitsTrue )
    {
        trueLiterals.push_back( literal.getOppositeLiteral() );
        return true;
    }
    assert( check_onLitTrue );
    interpreter->callListMethod( method_plugins_onLitTrue, literal.getOppositeLiteral().getId(), solver.getCurrentDecisionLevel(), output );    
    //True literals are stored in the trail
    if( solver.getCurrentDecisionLevel() > 0 )
    {
        assert( !trail.existElement( literal.getOppositeLiteral() ) );
        trail.push_back( literal.getOppositeLiteral() );
    }
    
    computeReason( solver, output );
    return true;
}

void
ExternalPropagator::reset(
    const Solver& solver )
{
    if( solver.getCurrentDecisionLevel() == 0 )
        clearClausesToDelete();
    trueLiterals.clear();
    vector< int > parameters;
    parameters.push_back( solver.getCurrentDecisionLevel() );
    while( !trail.empty() )
    {
        Literal lit = trail.back();
        if( !solver.isUndefined( lit ) )
            break;
        parameters.push_back( lit.getId() );
        trail.pop_back();
    }
    
    if( parameters.size() > 1 )
       interpreter->callVoidMethod( method_plugins_onLiteralsUndefined, parameters );    
}

void
ExternalPropagator::attachWatches(
    Solver& solver )
{
    if( interpreter->checkMethod( method_plugins_getLiterals ) )
    {
        vector< int > input;
        assert( solver.getCurrentDecisionLevel() == 0 );
        input.push_back( solver.numberOfVariables() );
        for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        {
            if( solver.isUndefined( i ) )
                continue;
            input.push_back( solver.isTrue( i ) ? i : -i );            
        }

        vector< int > output;
        interpreter->callListMethod( method_plugins_getLiterals, input, output );
        for( unsigned int i = 0; i < output.size(); i++ )
        {
            checkIdOfLiteral( solver, output[ i ] );
            Literal lit = Literal::createLiteralFromInt( output[ i ] );
            solver.setFrozen( lit.getVariable() );
            solver.addPropagator( lit.getOppositeLiteral(), this, i );
        }
    }
    if( interpreter->checkMethod( method_plugins_getVariablesToFreeze ) )
    {
        vector< int > output;
        interpreter->callListMethod( method_plugins_getVariablesToFreeze, output );
        for( unsigned int i = 0; i < output.size(); i++ )
        {
            checkIdOfLiteral( solver, output[ i ] );
            Literal lit = Literal::createLiteralFromInt( output[ i ] );
            solver.setFrozen( lit.getVariable() );
        }
    }
}
void
ExternalPropagator::checkWellFormed(
    const string& method )
{
   if( !interpreter->checkMethod( method ) )
        ErrorMessage::errorGeneric( "Method " + method + " is required." ); 
}

void
ExternalPropagator::checkWellFormed()
{
//    checkWellFormed( method_plugins_getLiterals );
//    checkWellFormed( method_plugins_getReason );
//    checkWellFormed( method_plugins_getVariablesToFreeze );
//    checkWellFormed( method_plugins_onLiteralsUndefined );
    if( !check_checkAnswerSet && ( ( check_onLitTrue && check_onLitsTrue ) || ( !check_onLitTrue && !check_onLitsTrue ) ) )
        ErrorMessage::errorGeneric( "If " + string( method_plugins_checkAnswerSet ) + " is not used then exactly one method between " + string( method_plugins_onLitTrue ) + " and " +  string( method_plugins_onLitsTrue ) + " is required." );    
    
    if( check_endPropagation && !check_onLitTrue )
        ErrorMessage::errorGeneric( "If " + string( method_plugins_endPropagation ) + " is used then method " + string( method_plugins_onLitTrue ) + " is required." );    
    
    if( check_endPropagation && check_onLitsTrue )
        ErrorMessage::errorGeneric( "If " + string( method_plugins_endPropagation ) + " is used then method " + string( method_plugins_onLitsTrue ) + " cannot be used." );    
    
    if( ( check_onLitTrue || check_onLitsTrue ) && 
        ( ( !interpreter->checkMethod( method_plugins_getReason ) && !interpreter->checkMethod( method_plugins_getReasonForLiteral ) ) || !interpreter->checkMethod( method_plugins_getLiterals ) || !interpreter->checkMethod( method_plugins_onLiteralsUndefined ) ) )
        ErrorMessage::errorGeneric( "If " + string( method_plugins_onLitTrue ) + " or " + string( method_plugins_onLitsTrue ) + " are used then " + 
                                    string( method_plugins_getReason ) + " (or, alternatively, " + string( method_plugins_getReasonForLiteral ) + ") , " + 
                                    string( method_plugins_getLiterals ) + ", and " + 
                                    string( method_plugins_onLiteralsUndefined ) + " are required." );
    if( interpreter->checkMethod( method_plugins_getReason ) && interpreter->checkMethod( method_plugins_getReasonForLiteral ) )
        ErrorMessage::errorGeneric( "Exactly one method between " + string( method_plugins_getReason ) + " and " + string( method_plugins_getReasonForLiteral ) + " can be used." );
        
    if( ( check_checkAnswerSet && !check_getReasonForCheckFailure ) || ( !check_checkAnswerSet && check_getReasonForCheckFailure ) )
        if( !check_checkPartialInterpretation )
            ErrorMessage::errorGeneric( "Method " + string( method_plugins_getReasonsForCheckFailure ) + " is required when " +  string( method_plugins_checkAnswerSet ) + " is used." );

    if( ( check_checkPartialInterpretation && !check_getReasonForCheckFailure ) || ( !check_checkPartialInterpretation && check_getReasonForCheckFailure ) )
        if( !check_checkAnswerSet )
            ErrorMessage::errorGeneric( "Method " + string( method_plugins_getReasonsForCheckFailure ) + " is required when " +  string( method_plugins_checkPartialInterpretation ) + " is used." );        
}

Clause*
ExternalPropagator::getReason(
    Solver& solver,
    Literal currentLiteral )
{
    vector< int > output;
    if( currentLiteral == Literal::null )
    {
        assert( !check_getReasonForLiteral );
        interpreter->callListMethod( method_plugins_getReason, output );
    }
    else
    {
        assert( check_getReasonForLiteral );
        interpreter->callListMethod( method_plugins_getReasonForLiteral, currentLiteral.getId(), output );
    }
   
    if( output.empty() )
    {
        if( solver.getCurrentDecisionLevel() != 0 )
            ErrorMessage::errorGeneric( "Reason is not well-formed: At least one of the literals in the reason must be inferred at the current decision level." );
        else
            return NULL;
    }
    
    Clause* clause = new Clause();
    clause->addLiteral( currentLiteral );
    unsigned int max = 0;    
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal l = Literal::createLiteralFromInt( output[ i ] );
        if( solver.isUndefined( l ) )
            ErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is undefined." );
        if( solver.isTrue( l ) )
            ErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is true." );
        if( solver.getDecisionLevel( l ) == 0 )
            continue;
        clause->addLiteral( l );
        unsigned int dl = solver.getDecisionLevel( l );
        if( dl > max )
            max = dl;
    }
    
    if( max < solver.getCurrentDecisionLevel() )
        ErrorMessage::errorGeneric( "Reason is not well-formed: At least one of the literals in the reason must be inferred at the current decision level." );    
    return clause;
}

Clause*
ExternalPropagator::getReasonForCheckFailure(
    Solver& solver )
{
    assert( check_getReasonForCheckFailure );
    vector< int > output;
    interpreter->callListMethod( method_plugins_getReasonsForCheckFailure, output );
    if( output.size() <= 2 || output[ 0 ] != 0 || output.back() != 0 )
        ErrorMessage::errorGeneric( "Reasons must start and terminate with 0." );
    
    vector< Clause* > clauses;
    Clause* clause = new Clause();
    for( unsigned int i = 1; i < output.size(); i++ )
    {
        if( output[ i ] == 0 )
        {
            clause->setLearned();
            if( solver.glucoseHeuristic() && clause->size() > 2 )
                clause->setLbd( solver.computeLBD( *clause ) );
            clauses.push_back( clause );
            clause = new Clause();
            continue;
        }
        checkIdOfLiteral( solver, output[ i ] );
        Literal l = Literal::createLiteralFromInt( output[ i ] );
        if( solver.isUndefined( l ) )
            ErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is undefined." );
        if( solver.isTrue( l ) )
            ErrorMessage::errorGeneric( "Reason is not well-formed: Literal with id " + to_string( l.getId() ) + " is true." );
        if( solver.getDecisionLevel( l ) > 0 )
            clause->addLiteral( l );
    }
    assert( clause->size() == 0 );
    delete clause;
    
    if( clauses.size() == 1 )
    {
        if( clauses[ 0 ]->size() >= 2 )
            Learning::sortClause( clauses[ 0 ], solver );
        return clauses[ 0 ];
    }

    solver.unrollToZero();
    reset( solver );
    for( unsigned int i = 0; i < clauses.size(); i++ )
        solver.addLearnedClause( clauses[ i ], true );
    return NULL;
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
        interpreter->callVoidMethod( method_plugins_onVariableElimination, var );
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
ExternalPropagator::onAnswerSet(
    const Solver& solver )
{
    if( check_onAnswerSet )
    {
        vector< int > answerset;
        answerset.push_back( 0 );
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

void
ExternalPropagator::checkIdOfLiteral(
    const Solver& solver,
    int id ) const
{
    Var var = id > 0 ? id : -id;
    if( var == 0 || var > solver.numberOfVariables() )
        ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
    if( solver.hasBeenEliminated( var ) )
        ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " has been removed from initial simplifications. Use " + method_plugins_getVariablesToFreeze + " to prevent the elimination of the variables." );
}

void
ExternalPropagator::foundLowerBound(
    uint64_t lb )
{
    if( check_onNewLowerBound )
        interpreter->callVoidMethod( method_plugins_foundLB, lb );    
}

void
ExternalPropagator::foundUpperBound(
    uint64_t ub )
{
    if( check_onNewUpperBound )
        interpreter->callVoidMethod( method_plugins_foundUB, ub );
}

bool
ExternalPropagator::checkAnswerSet(
    Solver& solver )
{
    bool retValue = true;
    if( check_checkAnswerSet )
    {
        vector< int > answerset;
        answerset.push_back( 0 );
        for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        {
            assert( !solver.isUndefined( i ) );
            if( solver.isTrue( i ) )
                answerset.push_back( i );
            else
                answerset.push_back( -i );
        }
        retValue = interpreter->callIntMethod( method_plugins_checkAnswerSet, answerset ) != 0;
    }
    return retValue;
}

bool
ExternalPropagator::checkPartialInterpretation(
    Solver& solver )
{
    bool retValue = true;
    if( check_checkPartialInterpretation )
    {
        vector< int > interpretation;
        interpretation.push_back( 0 );
        for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        {
            if( solver.isTrue( i ) )
                interpretation.push_back( i );
            else if( solver.isFalse( i ) )
                interpretation.push_back( -i );
        }
        retValue = interpreter->callIntMethod( method_plugins_checkPartialInterpretation, interpretation ) != 0;
    }
    return retValue;
}

void
ExternalPropagator::endUnitPropagation(
    Solver& solver )
{
    assert( check_onLitsTrue );
    assert( !check_onLitTrue );
    vector< int > input;
    vector< int > output;
    if( trueLiterals.empty() )
        return;
    input.push_back( solver.getCurrentDecisionLevel() );
    for( unsigned int i = 0; i < trueLiterals.size(); i++ )
    {
        assert( solver.isTrue( trueLiterals[ i ] ) );
        //True literals are stored in the trail
        if( solver.getCurrentDecisionLevel() > 0 )
            trail.push_back( trueLiterals[ i ] );
        input.push_back( trueLiterals[ i ].getId() );
    }    
    interpreter->callListMethod( method_plugins_onLitsTrue, input, output );
    trueLiterals.clear();
    computeReason( solver, output );    
}

void
ExternalPropagator::endPropagation(
    Solver& solver )
{
    assert( check_endPropagation && check_onLitTrue && !check_onLitsTrue );
    vector< int > output;
    interpreter->callListMethod( method_plugins_endPropagation, solver.getCurrentDecisionLevel(), output );
    computeReason( solver, output );
}

void
ExternalPropagator::handleConflict(
    Solver& solver,
    Literal conflictLiteral )
{
    assert( solver.isFalse( conflictLiteral ) );
    Clause* clause = getReason( solver, check_getReasonForLiteral ? conflictLiteral : Literal::null );
    if( clause == NULL )
    {
        clause = new Clause();
        clause->addLiteral( Literal::null );
    }
    
    clause->setLearned();
    clause->setAt( 0, conflictLiteral );
    unsigned int size = clause->size();
    if( size > 1 )
    {
        Learning::sortClause( clause, solver );
        if( solver.glucoseHeuristic() )
            clause->setLbd( solver.computeLBD( *clause ) );
        assert( !solver.isUndefined( clause->getAt( 1 ) ) );
        if( solver.getDecisionLevel( clause->getAt( 1 ) ) < solver.getCurrentDecisionLevel() )
        {
            solver.unroll( solver.getDecisionLevel( clause->getAt( 1 ) ) );
            reset( solver );            
        }
        assert( !solver.isUndefined( clause->getAt( 1 ) ) );
    }
    else
    {
        assert( solver.getCurrentDecisionLevel() == 0 );
        delete clause;
        solver.assignLiteral( Literal::createLiteralFromInt( 1 ) );
        return;
    }
    solver.addLearnedClause( clause, false );
    solver.assignLiteral( clause );
}

void
ExternalPropagator::computeReason(
    Solver& solver,
    const vector< int >& output )
{
    if( output.empty() || ( output.size() == 1 && output[ 0 ] == 0 ) )
        return;
    
    bool conflictDetected = false;
    Literal conflictLiteral = Literal::null;
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        conflictLiteral = Literal::createLiteralFromInt( output[ i ] );
        if( solver.isFalse( conflictLiteral ) )
        {
            conflictDetected = true;
            break;
        }        
    }
    
    if( conflictDetected )
    {
        handleConflict( solver, conflictLiteral );
        return;
    }
    
    Clause* reason = NULL;
    if( !check_getReasonForLiteral )
    {
        reason = getReason( solver, Literal::null );
        clausesToDelete.push_back( reason );
    }
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        checkIdOfLiteral( solver, output[ i ] );
        Literal lit = Literal::createLiteralFromInt( output[ i ] );
        if( solver.isTrue( lit ) )
            continue;
        
        if( check_getReasonForLiteral )
        {
            Clause* r = getReason( solver, lit );
            solver.assignLiteral( r );
            clausesToDelete.push_back( r );
        }
        else
            solver.assignLiteral( lit, reason );
    }
}

void
ExternalPropagator::onStartingSolver()
{
    if( interpreter->checkMethod( method_plugins_onStartingSolver ) )
        interpreter->callVoidMethod( method_plugins_onStartingSolver );
}