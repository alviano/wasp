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

#include "ExternalHeuristic.h"
#include "../util/Constants.h"
#include "../util/ErrorMessage.h"
#include "../Solver.h"
#include "interpreters/MyPerlInterpreter.h"
#include "interpreters/MyPythonInterpreter.h"

ExternalHeuristic::ExternalHeuristic( Solver& s, char* filename, unsigned int interpr ) : HeuristicStrategy( s )
{
    if( filename == NULL )
        ErrorMessage::errorGeneric( "Please specify the script file" );

    if( interpr == PYTHON_INTERPRETER )
        interpreter = new MyPythonInterpreter( filename );
    else if( interpr == PERL_INTERPRETER )
        interpreter = new MyPerlInterpreter( filename );
    else
        ErrorMessage::errorGeneric( "Unsupported interpreter" );    
    bool check_choiceVars = interpreter->checkMethod( method_choiceVars );
    bool check_onLastChoiceContradictory = interpreter->checkMethod( method_onChoiceContradictory );
    ignorePolarity = interpreter->checkMethod( method_ignorePolarity );

    if( !check_choiceVars )
        ErrorMessage::errorGeneric( "Method " + string( method_choiceVars ) + " is mandatory." );
    
    if( !check_onLastChoiceContradictory && !ignorePolarity )
        ErrorMessage::errorGeneric( "Please select exactly one method between " + string( method_onChoiceContradictory ) + " and " + string( method_ignorePolarity ) + "." );
    
    if( check_onLastChoiceContradictory && ignorePolarity )
        ErrorMessage::errorGeneric( "Please select exactly one method between " + string( method_onChoiceContradictory ) + " and " + string( method_ignorePolarity ) + "." );
    
    minisatHeuristic = NULL;
    bool check_fallback = interpreter->checkMethod( method_fallback );
    if( check_fallback )
        minisatHeuristic = new MinisatHeuristic( s );
    
    check_addedVarName = interpreter->checkMethod( method_addedVarName );
    check_onConflict = interpreter->checkMethod( method_onConflict );
    check_onDeletion = interpreter->checkMethod( method_onDeletion );
    check_onFinishedParsing = interpreter->checkMethod( method_onFinishedParsing );
    check_onFinishedSimplifications = interpreter->checkMethod( method_onFinishedSimplifications );
    check_onLearningClause = interpreter->checkMethod( method_onLearningClause );
    check_onLitAtLevelZero = interpreter->checkMethod( method_onLitAtLevelZero );
    check_onLitInvolvedInConflict = interpreter->checkMethod( method_onLitInvolvedInConflict );
    check_onLitInLearntClause = interpreter->checkMethod( method_onLitInLearntClause );
    check_onRestart = interpreter->checkMethod( method_onRestart );
    check_onAnswerSet = interpreter->checkMethod( method_onAnswerSet );
    check_onStartingSolver = interpreter->checkMethod( method_onStartingSolver );
    check_onLitInImportantClause = interpreter->checkMethod( method_onLitInImportantClause );
    check_onVariableElimination = interpreter->checkMethod( method_onVariableElimination );
    check_onUnrollingVariable = interpreter->checkMethod( method_onUnrollingVariable );
    check_onStartingParsing = interpreter->checkMethod( method_onStartingParsing );
    status = CHOICE;    
    numberOfFallbackSteps = 0;
    unrollVariable = 0;    
}

ExternalHeuristic::~ExternalHeuristic()
{
    delete interpreter;
}

void ExternalHeuristic::choiceVars( vector< int >& result, int& status )
{
    interpreter->callListMethod( method_choiceVars, result );
    unsigned int size = result.size();
    if( size == 0 )
        ErrorMessage::errorGeneric( error_choicevars );
    
    assert( size > 0 );
    if( result[ 0 ] == 0 )
    {
        if( size == 1 )
            ErrorMessage::errorGeneric( error_choicevars );
        
        status = result[ 1 ];
        switch( status )
        {
            case DO_RESTART:                
            case TRIGGER_INCOHERENCE:
                if( size != 2 )
                    ErrorMessage::errorGeneric( error_choicevars );
                break;
            case FALLBACK_HEURISTIC:
            case UNROLL:
                if( size != 3 )
                    ErrorMessage::errorGeneric( error_choicevars );
                break;
            default:
                ErrorMessage::errorGeneric( error_choicevars );
        }
        if( status == FALLBACK_HEURISTIC )
            numberOfFallbackSteps = result[ 2 ] <= 0 ? INT_MAX : result[ 2 ];
        else if ( status == UNROLL )
            unrollVariable = result[ 2 ];
    }
}

void ExternalHeuristic::addedVarName( Var var, const string& name )
{
    if( check_addedVarName )
        interpreter->callVoidMethod( method_addedVarName, var, name );
}

void ExternalHeuristic::onConflict()
{
    if( minisatHeuristic )
        minisatHeuristic->onConflict();
    choices.clear();
    if( check_onConflict )
    {
        Literal previous = Literal::null;
        for( unsigned int i = 0; i < previousChoices.size(); i++ )
        {
            Literal lit = previousChoices[ i ];
            if( solver.isUndefined( lit ) )
            {
                interpreter->callVoidMethod( method_onConflict, previous.getId() );
                previousChoices.resize( i );
                return;
            }
            else if( !ignorePolarity && solver.isFalse( lit ) )
            {
                onChoiceContradictory( lit.getId() );
                previousChoices.resize( i );
                return;
            }
            previous = lit;
        }
        interpreter->callVoidMethod( method_onConflict, previous.getId() );
    }    
}

void ExternalHeuristic::onDeletion()
{
    if( check_onDeletion )
        interpreter->callVoidMethod( method_onDeletion );
}

void ExternalHeuristic::onFinishedParsing()
{
    if( check_onFinishedParsing )
    {
        vector< int > output;        
        interpreter->callListMethod( method_onFinishedParsing, output );
        
        for( unsigned int i = 0; i < output.size(); i++ )
        {
            int lit = output[ i ];
            Var v = lit > 0 ? lit : -lit;
            if( v == 0 || v > solver.numberOfVariables() )
                ErrorMessage::errorGeneric( "Variable " + to_string( v ) + " does not exist." );
            else
                solver.setFrozen( v );
        }
    }
}

void ExternalHeuristic::onStartingParsing()
{
    if( check_onStartingParsing )
        interpreter->callVoidMethod( method_onStartingParsing );
}

void ExternalHeuristic::onFinishedSimplifications()
{
    if( check_onFinishedSimplifications )
        interpreter->callVoidMethod( method_onFinishedSimplifications );
    if( minisatHeuristic )
        minisatHeuristic->onFinishedSimplifications();
}

void ExternalHeuristic::onLearningClause( unsigned int lbd, unsigned int size )
{
    if( check_onLearningClause )
        interpreter->callVoidMethod( method_onLearningClause, lbd, size );
}

void ExternalHeuristic::onLitAtLevelZero( Literal lit )
{
    if( check_onLitAtLevelZero )
        interpreter->callVoidMethod( method_onLitAtLevelZero, lit.isPositive() ? lit.getVariable() : -lit.getVariable() );
}

void ExternalHeuristic::onLitInvolvedInConflict( Literal lit )
{
    if( check_onLitInvolvedInConflict )
        interpreter->callVoidMethod( method_onLitInvolvedInConflict, lit.isPositive() ? lit.getVariable() : -lit.getVariable() );
    if( minisatHeuristic )
        minisatHeuristic->onLitInvolvedInConflict( lit );
}

void ExternalHeuristic::onLitInLearntClause( Literal lit )
{
    if( check_onLitInLearntClause )
        interpreter->callVoidMethod( method_onLitInLearntClause, lit.isPositive() ? lit.getVariable() : -lit.getVariable() );
}

void ExternalHeuristic::onRestart()
{
    if( check_onRestart )
        interpreter->callVoidMethod( method_onRestart );
    choices.clear();
    previousChoices.clear();
}

void ExternalHeuristic::onAnswerSet()
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
        interpreter->callVoidMethod( method_onAnswerSet, answerset );
    }
}

void ExternalHeuristic::onStartingSolver( unsigned int nVars, unsigned int nClauses )
{
    if( check_onStartingSolver )
        interpreter->callVoidMethod( method_onStartingSolver, nVars, nClauses );
}

void ExternalHeuristic::onLitInImportantClause( Literal lit )
{
    if( check_onLitInImportantClause )
        interpreter->callVoidMethod( method_onLitInImportantClause, lit.isPositive() ? lit.getVariable() : -lit.getVariable() );
    if( minisatHeuristic )
        minisatHeuristic->onLitInImportantClause( lit );
}

void ExternalHeuristic::onVariableElimination( Var var )
{
    if( check_onVariableElimination )
        interpreter->callVoidMethod( method_onVariableElimination, var );
}

void ExternalHeuristic::onUnrollingVariable( Var var )
{
    if( check_onUnrollingVariable )
        interpreter->callVoidMethod( method_onUnrollingVariable, var );
    if( minisatHeuristic )
        minisatHeuristic->onUnrollingVariable( var );
}

void ExternalHeuristic::onChoiceContradictory( int choice )
{    
    interpreter->callVoidMethod( method_onChoiceContradictory, choice );
}

Literal ExternalHeuristic::makeAChoiceProtected()
{  
    init:;
    if( minisatHeuristic && numberOfFallbackSteps > 0 )
    {
        numberOfFallbackSteps--;
        return minisatHeuristic->makeAChoice();
    }

    begin:;
    if( choices.empty() )
        choiceVars( choices, status );

    if( status == CHOICE )
    {
        while( !choices.empty() )
        {
            int choice = choices.back();
            choices.pop_back();
            Var varChoice = choice > 0 ? choice : -choice;
            if( varChoice == 0 || varChoice > solver.numberOfVariables() )
                ErrorMessage::errorGeneric( "Variable " + to_string( varChoice ) + " does not exist." );
            
            if( solver.hasBeenEliminated( varChoice ) )
                continue;

            Literal lit( varChoice, choice > 0 ? POSITIVE : NEGATIVE );
            previousChoices.push_back( lit );
            
            if( solver.isUndefined( lit ) )
                return lit;
            
            if( ignorePolarity )
                continue;
            if( solver.isFalse( lit ) )
            {
               onChoiceContradictory( choice );
               choices.clear();
               previousChoices.pop_back();
               goto begin;
            }
        }
        //All choices given are already true or the variable has been eliminated;
        goto begin;
    }
    else
    {
        if( status == DO_RESTART )
        {
            solver.doRestart();
            status = CHOICE;
            choices.clear();
            previousChoices.clear();
            goto begin;
        }
            
        if( status == TRIGGER_INCOHERENCE )
        {
            status = CHOICE;
            choices.clear();
            previousChoices.clear();
            return Literal::null;
        }
        
        if( status == FALLBACK_HEURISTIC )
        {
            if( minisatHeuristic == NULL )
                ErrorMessage::errorGeneric( "Fallback heuristic has not been initialized. Please, add the fallback method in your script." );
            status = CHOICE;
            choices.clear();
            previousChoices.clear();
            numberOfFallbackSteps = INT_MAX;
            goto init;
        }
        
        if( status == UNROLL )
        {
            if( unrollVariable <= 0 || unrollVariable > solver.numberOfVariables() )
                ErrorMessage::errorGeneric( "Cannot unroll variable " + to_string( unrollVariable ) + ", it does not exist." );
            
            solver.unrollVariable( unrollVariable );
            unrollVariable = 0;
        }
    }
    
    status = CHOICE;
    if( minisatHeuristic )
    {
        fprintf( stderr, "Warning: using default heuristic.\n" );    
        return minisatHeuristic->makeAChoice();
    }
    
    ErrorMessage::errorGeneric( "There are no other choices, maybe you want to add a fallback heuristic." );
    //Useless, just to remove the warning.
    return Literal::null;
}