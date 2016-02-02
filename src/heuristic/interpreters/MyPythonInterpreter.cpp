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
#include "../../util/ErrorMessage.h"

#ifdef ENABLE_PYTHON

MyPythonInterpreter::MyPythonInterpreter( char* filename ) : Interpreter()
{
    setenv( "PYTHONPATH", ".", 1 );
    Py_Initialize();
    PyObject* pName = PyString_FromString( filename );
    pModule = PyImport_Import( pName );
    Py_DECREF( pName );
    if( pModule == NULL )
    {
        if (PyErr_Occurred())
            PyErr_Print();
        string message = "Module " + string( filename ) + " does not exist.\n";        
        ErrorMessage::errorGeneric( message );
    }
}

MyPythonInterpreter::~MyPythonInterpreter()
{
    Py_XDECREF(pModule);
    Py_Finalize();
}

void MyPythonInterpreter::callListMethod( const string& method_name, const vector< int >& parameters, vector< int >& output )
{
    if(pModule == NULL)
        return;
    PyObject* pFunc = PyObject_GetAttrString(pModule,method_name.c_str());
    if(pFunc && PyCallable_Check(pFunc))
    {
        PyObject* pArgs = PyTuple_New(parameters.size());
        for(unsigned int i = 0; i < parameters.size(); i++)
        {
            PyObject* pParam = Py_BuildValue("i",parameters[i]);
            PyTuple_SetItem(pArgs, i, pParam);            
        }
        PyObject* result = PyObject_CallObject(pFunc, pArgs);
        if(result && PyList_Check(result))
        {
            int size = PyList_Size(result);
            for(int i = size - 1; i >= 0; i--)
            {
                PyObject* item = PyList_GetItem(result, i);                
                if (PyInt_Check(item))
                {
                    int value = PyInt_AsLong(item);                    
                    output.push_back(value);
                }
            }
            Py_XDECREF(result);            
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();        
        }
        Py_XDECREF(pArgs);
    }
    else
    {
        if (PyErr_Occurred())
            PyErr_Print();
        printf("Method %s not found\n", method_name.c_str());
    }
    Py_XDECREF(pFunc);    
}

void MyPythonInterpreter::callVoidMethod( const string& method_name, int param1, const string& param2 )
{
    if(pModule == NULL)
        return;
    PyObject* pFunc = PyObject_GetAttrString(pModule,method_name.c_str());
    if(pFunc && PyCallable_Check(pFunc))
    {
        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyInt_FromSize_t(param1));
        PyTuple_SetItem(pArgs, 1, PyString_FromString(param2.c_str()));        
        PyObject* result = PyObject_CallObject(pFunc, pArgs);
        if(result)
            Py_XDECREF(result);
        Py_XDECREF(pArgs);
    }
    else
    {
        if (PyErr_Occurred())
            PyErr_Print();
        printf("Method %s not found\n", method_name.c_str());
    }
    Py_XDECREF(pFunc);    
}

bool MyPythonInterpreter::checkMethod( const string& method_name ) const
{
    bool res = false;
    if(pModule == NULL)
        return res;
    PyObject* pFunc = PyObject_GetAttrString(pModule,method_name.c_str());
    if(pFunc && PyCallable_Check(pFunc))
       res = true;
    else
    {
        if (PyErr_Occurred())
            PyErr_Clear();
    }
        
    Py_XDECREF(pFunc);
    return res;
}

#endif