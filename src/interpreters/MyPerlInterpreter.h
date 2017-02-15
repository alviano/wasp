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

#ifndef WASP_PERLINTERPRETER_H
#define WASP_PERLINTERPRETER_H

#include "Interpreter.h"
#include <string>
#include <vector>
using namespace std;

#ifdef ENABLE_PERL
#include <EXTERN.h>
#include <perl.h>

class MyPerlInterpreter : public Interpreter
{
    public:
        MyPerlInterpreter( char* filename, string );
        ~MyPerlInterpreter();

        void callListMethod( const string& method_name, const vector< int >& parameters, vector< int >& output );
        void callListMethod( const string& method_name, const vector< int >& parameters, vector< uint64_t >& output );
        void callVoidMethod( const string& method_name, int param1, const string& param2 );
        bool checkMethod( const string& method_name ) const;
        
    private:
        PerlInterpreter* my_perl;
        char** parameters;
};
#else

class MyPerlInterpreter : public Interpreter
{
    public:
        MyPerlInterpreter( char*, string ) {}
        ~MyPerlInterpreter() {}

        void callListMethod( const string&, const vector< int >&, vector< int >& ) {}
        void callListMethod( const string&, const vector< int >&, vector< uint64_t >& ) {}
        void callVoidMethod( const string&, int, const string& ) {}
        bool checkMethod( const string& ) const { return false; }        
};

#endif

#endif