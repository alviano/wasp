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
    check_onStartingSimplifications = interpreter->checkMethod( method_onStartingSimplifications );
    check_onRestart = interpreter->checkMethod( method_onRestart );
    check_onAnswerSet = interpreter->checkMethod( method_onAnswerSet );
    check_onStartingSolver = interpreter->checkMethod( method_onStartingSolver );
    check_onLitInImportantClause = interpreter->checkMethod( method_onLitInImportantClause );
    check_onVariableElimination = interpreter->checkMethod( method_onVariableElimination );
    check_onStartingParsing = interpreter->checkMethod( method_onStartingParsing );
    check_partialInterpretation = interpreter->checkMethod( method_partialInterpretation );
    check_onUnfoundedSet = interpreter->checkMethod( method_onUnfoundedSet );
    check_onLoopFormula = interpreter->checkMethod( method_onLoopFormula );
    check_initFallback = interpreter->checkMethod( method_initFallback );
    check_factorFallback = interpreter->checkMethod( method_factorFallback );
    check_signFallback = interpreter->checkMethod( method_signFallback );
    check_onNewClause = interpreter->checkMethod( method_onNewClause );
    check_onLitsTrue = interpreter->checkMethod( method_onLitsTrue );
    check_onVarUndefined = interpreter->checkMethod( method_onVarUndefined );

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
    vector< int > interpretation;
    if( check_partialInterpretation )
    {
        for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
        {
            if( solver.hasBeenEliminated( i ) || solver.isUndefined( i ) )
                continue;
            interpretation.push_back( solver.isTrue( i ) ? i : -i );                
        }
    }    
    interpreter->callListMethod( method_choiceVars, interpretation, result );
    sendTrueLiterals();
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
    resetInterpretationToSend();
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

void ExternalHeuristic::onStartingSimplifications()
{
    if( check_onStartingSimplifications )
        interpreter->callVoidMethod( method_onStartingSimplifications );
}

void ExternalHeuristic::onFinishedSimplifications()
{
    if( check_onFinishedSimplifications )
        interpreter->callVoidMethod( method_onFinishedSimplifications );
    if( minisatHeuristic )
    {
        minisatHeuristic->onFinishedSimplifications();        
        initFallback();
        factorFallback();
        signFallback();
    }
}

void ExternalHeuristic::initFallback()
{
    if( !check_initFallback )
        return;
    assert( minisatHeuristic );
    vector< int > output;
    interpreter->callListMethod( method_initFallback, output );
    if( output.size() % 2 != 0 )
        ErrorMessage::errorGeneric( error_initfallback );
    for( unsigned int i = 0; i < output.size() - 1; i = i + 2 )
    {
        int var = output[ i ];
        if( var <= 0 || ( unsigned int ) var > solver.numberOfVariables() )
            ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        int value = output[ i + 1 ];        
        minisatHeuristic->init( var, value );
    }
}

void ExternalHeuristic::factorFallback()
{
    if( !check_factorFallback )
        return;
    assert( minisatHeuristic );
    vector< int > output;
    interpreter->callListMethod( method_factorFallback, output );
    if( output.size() % 2 != 0 )
        ErrorMessage::errorGeneric( error_factorfallback );
    for( unsigned int i = 0; i < output.size() - 1; i = i + 2 )
    {
        int var = output[ i ];
        if( var <= 0 || ( unsigned int ) var > solver.numberOfVariables() )
            ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        int value = output[ i + 1 ];
        minisatHeuristic->setFactor( var, value );
    }
}

void ExternalHeuristic::signFallback()
{
    if( !check_signFallback )
        return;
    assert( minisatHeuristic );
    vector< int > output;
    interpreter->callListMethod( method_signFallback, output );
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        int lit = output[ i ];
        Var var = lit > 0 ? lit : -lit;
        if( var != 0 && var > solver.numberOfVariables() )
            ErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        minisatHeuristic->setSign( lit );
    }
}

void ExternalHeuristic::onNewClause( const Clause* clause )
{
    if( check_onNewClause )
    {
        vector< int > v;
        v.reserve( clause->size() );
        for( unsigned int i = 0; i < clause->size(); i++ )
            v.push_back( clause->getAt( i ).getId() );
        interpreter->callVoidMethod( method_onNewClause, v );
    }
}

void ExternalHeuristic::onNewBinaryClause( Literal lit1, Literal lit2 )
{
    if( check_onNewClause )
    {
        vector< int > v;
        v.push_back( lit1.getId() );
        v.push_back( lit2.getId() );
        interpreter->callVoidMethod( method_onNewClause, v );        
    }
}


void ExternalHeuristic::onLearningClause( unsigned int lbd, const Clause* clause )
{
    if( check_onLearningClause )
    {
        unsigned int size = clause->size();
        vector< int > v;
        v.reserve( size + 2 );
        v.push_back( lbd );
        v.push_back( size );
        for( unsigned int i = 0; i < clause->size(); i++ )
            v.push_back( clause->getAt( i ).getId() );
        interpreter->callVoidMethod( method_onLearningClause, v );
    }
}

void ExternalHeuristic::onLitTrue( Literal lit )
{
    if( check_onLitsTrue )
        interpretationToSend.push_back( lit.getId() );        
}

void ExternalHeuristic::sendTrueLiterals()
{
    if( check_onLitsTrue && !interpretationToSend.empty() )
    {        
        interpreter->callVoidMethod( method_onLitsTrue, interpretationToSend );
        interpretationToSend.clear();
    }   
}

void ExternalHeuristic::onLoopFormula( const Clause* clause )
{
    if( check_onLoopFormula )
    {
        unsigned int size = clause->size();
        vector< int > v;
        v.reserve( size + 2 );
        v.push_back( clause->lbd() );
        v.push_back( size );
        for( unsigned int i = 0; i < clause->size(); i++ )
            v.push_back( clause->getAt( i ).getId() );
        interpreter->callVoidMethod( method_onLearningClause, v );
    }
}

void ExternalHeuristic::onUnfoundedSet( const Vector< Var >& unfoundedSet )
{
    if( check_onUnfoundedSet )
    {
        vector< int > v;
        v.reserve( unfoundedSet.size() );
        for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
            v.push_back( unfoundedSet[ i ] );
        interpreter->callVoidMethod( method_onUnfoundedSet, v );
    }
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

void ExternalHeuristic::onRestart()
{
    if( check_onRestart )
        interpreter->callVoidMethod( method_onRestart );
    clearStatus();
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

void ExternalHeuristic::onVarUndefined( Var var )
{
    if( check_onVarUndefined )
        interpreter->callVoidMethod( method_onVarUndefined, var );
    if( minisatHeuristic )
        minisatHeuristic->onVarUndefined( var );
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
               clearStatus();
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
            clearStatus();
            goto begin;
        }
            
        if( status == TRIGGER_INCOHERENCE )
        {
            status = CHOICE;
            clearStatus();
            return Literal::null;
        }
        
        if( status == FALLBACK_HEURISTIC )
        {
            if( minisatHeuristic == NULL )
                ErrorMessage::errorGeneric( "Fallback heuristic has not been initialized. Please, add the fallback method in your script." );
            status = CHOICE;
            clearStatus();
            goto init;
        }
        
        if( status == UNROLL )
        {
            if( unrollVariable <= 0 || unrollVariable > solver.numberOfVariables() )
                ErrorMessage::errorGeneric( "Cannot unroll variable " + to_string( unrollVariable ) + ", it does not exist." );
            
            solver.unrollVariable( unrollVariable );
            unrollVariable = 0;
            resetPreviousChoices();
            resetInterpretationToSend();
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

void ExternalHeuristic::clearStatus()
{
    choices.clear();
    previousChoices.clear();
    interpretationToSend.clear();
}

void ExternalHeuristic::resetPreviousChoices()
{
    if( check_onConflict )
    {
        while( !previousChoices.empty() )
        {
            if( solver.isUndefined( previousChoices.back() ) )
                previousChoices.pop_back();
            else
                break;
        }        
    }
}

void ExternalHeuristic::resetInterpretationToSend()
{
    if( check_onLitsTrue )
    {
        while( !interpretationToSend.empty() )
        {
            Var v = interpretationToSend.back() > 0 ? interpretationToSend.back() : -interpretationToSend.back();
            unsigned int sign = interpretationToSend.back() > 0 ? POSITIVE : NEGATIVE;
            Literal lit( v, sign );
            if( solver.isUndefined( lit ) )
                interpretationToSend.pop_back();
            else
                break;
        }        
    }
}