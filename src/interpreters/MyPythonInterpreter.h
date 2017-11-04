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

#ifndef WASP_PYTHONINTERPRETER_H
#define WASP_PYTHONINTERPRETER_H

#include "Interpreter.h"
#include <string>
#include <vector>
using namespace std;

#ifdef ENABLE_PYTHON

#include <Python.h>

class MyPythonInterpreter : public Interpreter
{
    public:
        MyPythonInterpreter( char* filename, string, bool );
        ~MyPythonInterpreter();
        
        void callListMethod( const string& method_name, const vector< int >& parameters, vector< int >& output );
        void callListMethod( const string& method_name, const vector< int >& parameters, vector< uint64_t >& output );
        void callVoidMethod( const string& method_name, int param1, const string& param2 );
        bool checkMethod( const string& method_name ) const;
        void addElementInMap( const string& map_name, const string& key, unsigned int value ); 
        bool checkAttribute( const string& map_name ) const;

    private:        
        PyObject* pModule;
        bool callPyFinalize;
};

#else

class MyPythonInterpreter : public Interpreter
{
    public:
        MyPythonInterpreter( char*, string, bool ){}
        ~MyPythonInterpreter() {}
        
        void callListMethod( const string&, const vector< int >&, vector< int >& ) {}
        void callListMethod( const string&, const vector< int >&, vector< uint64_t >& ) {}
        void callVoidMethod( const string&, int, const string& ) {}
        bool checkMethod( const string& ) const { return false; }
        void addElementInMap( const string&, const string&, unsigned int ) {}
        bool checkAttribute( const string& ) const { return false; }
};

#endif

#endif
