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

#include "MyPythonInterpreter.h"
#include <cstring>
using namespace std;
#include "../util/WaspErrorMessage.h"

#ifdef ENABLE_PYTHON

MyPythonInterpreter::MyPythonInterpreter(
    char* filename,
    string scriptDirectory,
    bool callPyFinalize_ ) : Interpreter(), callPyFinalize( callPyFinalize_ )
{
    setenv( "PYTHONPATH", ".", 1 );
    Py_Initialize();
    PyObject* pName = 
    #ifdef PYTHON_THREE
        PyUnicode_FromString( filename );
    #else
        PyString_FromString( filename );
    #endif
    if( scriptDirectory != "" )
    {
        string s( "import sys; sys.path.append('" + scriptDirectory + "')" );
        PyRun_SimpleString( s.c_str() );
    }
    pModule = PyImport_Import( pName );
    Py_DECREF( pName );
    if( pModule == NULL )
    {
        if( PyErr_Occurred() )
            PyErr_Print();
        string message = "Module " + string( filename ) + " does not exist.\n";        
        WaspErrorMessage::errorGeneric( message );
    }
}

MyPythonInterpreter::~MyPythonInterpreter()
{
    Py_XDECREF( pModule );
    if( callPyFinalize )
        Py_Finalize();
}

void
MyPythonInterpreter::callListMethod(
    const string& method_name,
    const vector< int >& parameters,
    vector< int >& output )
{
    if( pModule == NULL )
        return;
    PyObject* pFunc = PyObject_GetAttrString( pModule, method_name.c_str() );
    if( pFunc && PyCallable_Check( pFunc ) )
    {
        PyObject* pArgs = PyTuple_New( parameters.size() );
        for( unsigned int i = 0; i < parameters.size(); i++ )
        {
            PyObject* pParam = Py_BuildValue( "i", parameters[ i ] );
            PyTuple_SetItem( pArgs, i, pParam );
        }
        PyObject* result = PyObject_CallObject( pFunc, pArgs );
        if( result )
        {
            bool isInt = 
            #ifdef PYTHON_THREE
                PyLong_Check( result );
            #else
                PyInt_Check( result );
            #endif
            if( PyList_Check( result ) )
            {
                int size = PyList_Size( result );
                for( int i = size - 1; i >= 0; i-- )
                {
                    PyObject* item = PyList_GetItem( result, i );                
                    bool isInt = 
                    #ifdef PYTHON_THREE
                        PyLong_Check( item );
                    #else
                        PyInt_Check( item );
                    #endif
                    if( isInt )
                    {
                        int value = 
                        #ifdef PYTHON_THREE
                            PyLong_AsLong( item );
                        #else
                            PyInt_AsLong( item );
                        #endif
                        output.push_back( value );
                    }
                }
                Py_XDECREF( result );
            }
            else if( isInt )
            {
                int value = 
                #ifdef PYTHON_THREE
                    PyLong_AsLong( result );
                #else
                    PyInt_AsLong( result );
                #endif
                output.push_back( value );
            }
            else if( PyErr_Occurred() )
                PyErr_Print();
        }
        else if( PyErr_Occurred() )
            PyErr_Print();
        Py_XDECREF( pArgs );
    }
    else
    {
        if( PyErr_Occurred() )
            PyErr_Print();
        printf( "Method %s not found\n", method_name.c_str() );
    }
    Py_XDECREF( pFunc );
}

void MyPythonInterpreter::callListMethod(
    const string& method_name,
    const vector< int >& parameters,
    vector< uint64_t >& output )
{
    if( pModule == NULL )
        return;
    PyObject* pFunc = PyObject_GetAttrString( pModule,method_name.c_str() );
    if( pFunc && PyCallable_Check( pFunc ) )
    {
        PyObject* pArgs = PyTuple_New( parameters.size() );
        for( unsigned int i = 0; i < parameters.size(); i++ )
        {
            PyObject* pParam = Py_BuildValue( "i", parameters[ i ] );
            PyTuple_SetItem( pArgs, i, pParam );
        }
        PyObject* result = PyObject_CallObject( pFunc, pArgs );
        if( result )
        {
            bool isInt = PyLong_Check( result );
            if( PyList_Check( result ) )
            {
                int size = PyList_Size( result );
                for( int i = size - 1; i >= 0; i-- )
                {
                    PyObject* item = PyList_GetItem( result, i );
                    if( PyLong_Check( item ) )
                    {
                        uint64_t value = PyLong_AsUnsignedLongLongMask( item );
                        output.push_back( value );
                    }
                }
                Py_XDECREF( result );
            }
            else if( isInt )
            {
                uint64_t value =
                #ifdef PYTHON_THREE 
                    PyLong_AsUnsignedLongLongMask( result );
                #else
                    PyInt_AsUnsignedLongLongMask( result );
                #endif
                output.push_back( value );
            }
            else if( PyErr_Occurred() )
                PyErr_Print();
        }
        else if( PyErr_Occurred() )
            PyErr_Print();        
        Py_XDECREF( pArgs );
    }
    else
    {
        if( PyErr_Occurred() )
            PyErr_Print();
        printf("Method %s not found\n", method_name.c_str());
    }
    Py_XDECREF( pFunc );    
}

void MyPythonInterpreter::callVoidMethod(
    const string& method_name,
    int param1,
    const string& param2 )
{
    if( pModule == NULL )
        return;
    PyObject* pFunc = PyObject_GetAttrString( pModule, method_name.c_str() );
    if( pFunc && PyCallable_Check( pFunc ) )
    {
        PyObject* pArgs = PyTuple_New( 2 );
        PyObject* param1_python = 
        #ifdef PYTHON_THREE
            PyLong_FromSize_t( param1 );
        #else
            PyInt_FromSize_t( param1 );
        #endif

        PyObject* param2_python =
        #ifdef PYTHON_THREE
            PyUnicode_FromString( param2.c_str() );
        #else
            PyString_FromString( param2.c_str() );
        #endif

        PyTuple_SetItem( pArgs, 0, param1_python );
        PyTuple_SetItem( pArgs, 1, param2_python );        
        PyObject* result = PyObject_CallObject( pFunc, pArgs );
        if( result )
            Py_XDECREF( result );
        else if( PyErr_Occurred() )
            PyErr_Print();
        Py_XDECREF( pArgs );
    }
    else
    {
        if( PyErr_Occurred() )
            PyErr_Print();
        printf( "Method %s not found\n", method_name.c_str() );
    }
    Py_XDECREF( pFunc );
}

bool MyPythonInterpreter::checkMethod(
    const string& method_name ) const
{
    bool res = false;
    if( pModule == NULL )
        return res;
    PyObject* pFunc = PyObject_GetAttrString( pModule, method_name.c_str() );
    if( pFunc && PyCallable_Check( pFunc ) )
       res = true;
    else if( PyErr_Occurred() )
        PyErr_Clear();
        
    Py_XDECREF( pFunc );
    return res;
}

void MyPythonInterpreter::addElementInMap(
    const string& map_name,
    const string& key,
    unsigned int value )
{
    PyObject* pFunc = PyObject_GetAttrString( pModule, map_name.c_str() );
    if( pFunc )
    {
        PyObject* value_python = 
        #ifdef PYTHON_THREE
            PyLong_FromSize_t( value );
        #else
            PyInt_FromSize_t( value );
        #endif

        char *cstr = new char[key.length() + 1];
        strcpy(cstr, key.c_str());        
        int res = PyMapping_SetItemString(pFunc, cstr, value_python);
        if( res == -1 || PyErr_Occurred() )
            PyErr_Print();
        delete [] cstr;
    }
    else
    {
        if( PyErr_Occurred() )
            PyErr_Print();
        printf( "Element %s not found\n", map_name.c_str() );
    }
    Py_XDECREF( pFunc );
}

bool MyPythonInterpreter::checkAttribute(
    const string& map_name ) const
{
    bool res = false;
    if( pModule == NULL )
        return res;
    PyObject* pFunc = PyObject_GetAttrString( pModule, map_name.c_str() );
    if( pFunc )
       res = true;
    else if( PyErr_Occurred() )
        PyErr_Clear();
        
    Py_XDECREF( pFunc );
    return res;
}

#endif
