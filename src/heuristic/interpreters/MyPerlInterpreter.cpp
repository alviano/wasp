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

#include "MyPerlInterpreter.h"

#ifdef ENABLE_PERL

MyPerlInterpreter::MyPerlInterpreter( char* filename, string ) : Interpreter()
{
    parameters = new char*[ 2 ];
    parameters[ 0 ] = filename;
    parameters[ 1 ] = filename;
    int argc = 2;    
    PERL_SYS_INIT( &argc, &parameters );
    my_perl = perl_alloc();
    perl_construct( my_perl );
    perl_parse( my_perl, NULL, argc, parameters, NULL );
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
}

MyPerlInterpreter::~MyPerlInterpreter()
{
    perl_destruct( my_perl );
    perl_free( my_perl );
    PERL_SYS_TERM();
    delete [] parameters;
}

void MyPerlInterpreter::callListMethod( const string& method_name, const vector< int >& parameters, vector< int >& output )
{
    dSP;
    ENTER;
    SAVETMPS;
    PUSHMARK(SP);
    for( unsigned int i = 0; i < parameters.size(); i++ )
        XPUSHs( sv_2mortal( newSViv( parameters[ i ] ) ) );
    PUTBACK;
    int count = call_pv( method_name.c_str(), G_ARRAY );
    SPAGAIN;
    for( int i = 0; i < count; i++ )
    {
        int tmp = 0;
        tmp = POPi;        
        output.push_back( tmp );
    }
    PUTBACK;
    FREETMPS;
    LEAVE;
}

void MyPerlInterpreter::callListMethod( const string& method_name, const vector< int >& parameters, vector< uint64_t >& output )
{
    dSP;
    ENTER;
    SAVETMPS;
    PUSHMARK( SP );
    for( unsigned int i = 0; i < parameters.size(); i++ )
        XPUSHs( sv_2mortal( newSViv( parameters[ i ] ) ) );
    PUTBACK;
    int count = call_pv( method_name.c_str(), G_ARRAY );
    SPAGAIN;    
    for( int i = 0; i < count; i++ )
    {
        uint64_t tmp = 0;
        tmp = POPul;        
        output.push_back( tmp );
    }
    PUTBACK;
    FREETMPS;
    LEAVE;
}

void MyPerlInterpreter::callVoidMethod( const string& method_name, int param1, const string& param2 )
{
    dSP;
    ENTER;
    SAVETMPS;
    
    PUSHMARK( SP );
    XPUSHs( sv_2mortal( newSViv( param1 ) ) );
    XPUSHs( sv_2mortal( newSVpv( param2.c_str(), param2.length() ) ) );
    PUTBACK;
    call_pv( method_name.c_str(), G_DISCARD );
    FREETMPS;
    LEAVE;
}

bool MyPerlInterpreter::checkMethod( const string& method_name ) const
{
    STRLEN value;
    string check( "$value = exists &" + method_name + ";" );
    eval_pv( check.c_str(), TRUE );
    int result = SvIV( get_sv( "value", FALSE ) );    
    return result == 1;
}

#endif