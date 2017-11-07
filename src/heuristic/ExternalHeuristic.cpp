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
#include "../util/WaspConstants.h"
#include "../util/WaspErrorMessage.h"
#include "../util/WaspOptions.h"
#include "../Solver.h"
#include "../interpreters/MyPerlInterpreter.h"
#include "../interpreters/MyPythonInterpreter.h"

ExternalHeuristic::ExternalHeuristic( Solver& s, char* filename, unsigned int interpr, string scriptDirectory ) : MinisatHeuristic( s )
{
    if( filename == NULL )
        WaspErrorMessage::errorGeneric( "Script file is not specified." );

    if( interpr == PYTHON_INTERPRETER )
        interpreter = new MyPythonInterpreter( filename, scriptDirectory, wasp::Options::callPyFinalize );
    else if( interpr == PERL_INTERPRETER )
        interpreter = new MyPerlInterpreter( filename, scriptDirectory );
    else
        WaspErrorMessage::errorGeneric( "Unsupported interpreter" );    
    bool check_choiceVars = interpreter->checkMethod( method_selectLiteral );
    
    if( !check_choiceVars )
        WaspErrorMessage::errorGeneric( "Method " + string( method_selectLiteral ) + " is mandatory." );
    
    check_onConflict = interpreter->checkMethod( method_onConflict );
    check_onDeletion = interpreter->checkMethod( method_onDeletion );
    check_onLearningClause = interpreter->checkMethod( method_onLearningClause );
    check_onLitInImportantClause = interpreter->checkMethod( method_onLitInImportantClause );
    check_onLitInvolvedInConflict = interpreter->checkMethod( method_onLitInvolvedInConflict );
    check_onLoopFormula = interpreter->checkMethod( method_onLoopFormula );
    check_onNewClause = interpreter->checkMethod( method_onNewClause );
    check_onRestart = interpreter->checkMethod( method_onRestart );
    check_onUnfoundedSet = interpreter->checkMethod( method_onUnfoundedSet );
    
    check_initFallback = interpreter->checkMethod( method_initFallback );
    check_factorFallback = interpreter->checkMethod( method_factorFallback );
    check_signFallback = interpreter->checkMethod( method_signFallback );
    
    status = CHOICE;    
    value = 0;
}

ExternalHeuristic::~ExternalHeuristic() { delete interpreter; }

void ExternalHeuristic::selectLiteral()
{
    vector< int > result;
    interpreter->callListMethod( method_selectLiteral, result );    
    unsigned int size = result.size();
    if( size == 0 )
        WaspErrorMessage::errorGeneric( error_selectLiteral );
    
    assert( size > 0 );
    if( result[ 0 ] == 0 )
    {
        if( size == 1 )
            WaspErrorMessage::errorGeneric( error_selectLiteral );
        
        status = result[ 1 ];
        switch( status )
        {
            case DO_RESTART:                
                if( size != 2 ) WaspErrorMessage::errorGeneric( error_selectLiteral );
                break;
            case FALLBACK_HEURISTIC:
            case UNROLL:                
            case CHOICE:
                if( size != 3 ) WaspErrorMessage::errorGeneric( error_selectLiteral );
                value = result[ 2 ];
                break;
            default:
                WaspErrorMessage::errorGeneric( error_selectLiteral );
        }
    }
}

void ExternalHeuristic::onConflictProtected()
{
    if( check_onConflict ) interpreter->callVoidMethod( method_onConflict );
}

void ExternalHeuristic::onDeletion()
{
    if( check_onDeletion ) interpreter->callVoidMethod( method_onDeletion );
}

void ExternalHeuristic::initFallback()
{
    if( !check_initFallback )
        return;
    vector< int > output;
    interpreter->callListMethod( method_initFallback, output );
    if( output.size() % 2 != 0 )
        WaspErrorMessage::errorGeneric( error_initfallback );
    if( output.empty() )
        return;    
    for( unsigned int i = 0; i < output.size() - 1; i = i + 2 )
    {
        int var = output[ i ];
        if( var <= 0 || ( unsigned int ) var > solver.numberOfVariables() )
            WaspErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        int value = output[ i + 1 ];        
        init( var, value );
    }
}

void ExternalHeuristic::factorFallback()
{
    if( !check_factorFallback )
        return;
    vector< int > output;    
    interpreter->callListMethod( method_factorFallback, output );
    if( output.size() % 2 != 0 )
        WaspErrorMessage::errorGeneric( error_factorfallback );
    if( output.empty() )
        return;
    for( unsigned int i = 0; i < output.size() - 1; i = i + 2 )
    {
        int var = output[ i ];
        if( var <= 0 || ( unsigned int ) var > solver.numberOfVariables() )
            WaspErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        int value = output[ i + 1 ];
        setFactor( var, value );
    }
}

void ExternalHeuristic::signFallback()
{
    if( !check_signFallback )
        return;
    vector< int > output;
    interpreter->callListMethod( method_signFallback, output );
    for( unsigned int i = 0; i < output.size(); i++ )
    {
        int lit = output[ i ];
        Var var = lit > 0 ? lit : -lit;
        if( var != 0 && var > solver.numberOfVariables() )
            WaspErrorMessage::errorGeneric( "Variable " + to_string( var ) + " does not exist." );
        
        setSign( lit );
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

void ExternalHeuristic::onNewBinaryClauseProtected( Literal lit1, Literal lit2 )
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

void ExternalHeuristic::onLitInvolvedInConflictProtected( Literal lit )
{
    if( check_onLitInvolvedInConflict ) interpreter->callVoidMethod( method_onLitInvolvedInConflict, lit.getId() );    
}

void ExternalHeuristic::onRestart()
{
    if( check_onRestart ) interpreter->callVoidMethod( method_onRestart );
}

void ExternalHeuristic::onLitInImportantClauseProtected( Literal lit )
{
    if( check_onLitInImportantClause ) interpreter->callVoidMethod( method_onLitInImportantClause, lit.getId() );    
}

Literal ExternalHeuristic::makeAChoiceProtected()
{  
    if( status == FALLBACK_HEURISTIC && value-- >= 0 )
        return MinisatHeuristic::makeAChoiceProtected();
    
    while( true )
    {
        selectLiteral();        
        switch( status )
        {
            case CHOICE:
            {
                Var varChoice = value > 0 ? value : -value;
                if( varChoice == 0 || varChoice > solver.numberOfVariables() )
                    WaspErrorMessage::errorGeneric( "Variable " + to_string( varChoice ) + " does not exist." );

                if( !solver.hasBeenEliminated( varChoice ) )
                {
                    Literal lit = Literal::createLiteralFromInt( value );
                    if( solver.isUndefined( lit ) )
                        return lit;
                    else if( solver.isFalse( lit ) )
                        WaspErrorMessage::errorGeneric( "Contradiction in the choice" );
                }
                //All choices given are already true or the variable has been eliminated;
                break;
            }

            case DO_RESTART:
                solver.doRestart();
                return Literal::null;

            case FALLBACK_HEURISTIC:
                if( value <= 0 ) value = INT_MAX;
                value--;
                return MinisatHeuristic::makeAChoiceProtected();                

            case UNROLL:
                if( value <= 0 || (unsigned int) value > solver.numberOfVariables() )
                WaspErrorMessage::errorGeneric( "Cannot unroll variable " + to_string( value ) + ", it does not exist." );            
                if( !solver.hasBeenEliminated( value ) )
                    WaspErrorMessage::errorGeneric( "Cannot unroll deleted variable " + to_string( value ) + "." );
                solver.unrollVariable( value );
                return Literal::null;
                
            default:
                fprintf( stderr, "Warning: using default heuristic.\n" );    
                return MinisatHeuristic::makeAChoiceProtected();
        }        
    }    
}