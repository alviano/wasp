#include "Interpreter.h"
#include <climits>

void Interpreter::callVoidMethod( const string& method_name, const vector< int >& parameters )
{
    callIntMethod(method_name, parameters);
}

void Interpreter::callVoidMethod( const string& method_name )
{
    vector< int > params;
    return callVoidMethod(method_name, params);
}

void Interpreter::callVoidMethod( const string& method_name, int param1 )
{
    vector< int > params;
    params.push_back(param1);
    return callVoidMethod(method_name, params);
}

void Interpreter::callVoidMethod( const string& method_name, int param1, int param2 )
{
    vector< int > params;
    params.push_back(param1);
    params.push_back(param2);
    return callVoidMethod(method_name, params);
}

int Interpreter::callIntMethod( const string& method_name )
{
    vector< int > params;
    return callIntMethod(method_name, params);
}

int Interpreter::callIntMethod( const string& method_name, int param1 )
{
    vector< int > params;
    params.push_back(param1);
    return callIntMethod(method_name, params);
}

int Interpreter::callIntMethod( const string& method_name, int param1, int param2 )
{
    vector< int > params;
    params.push_back(param1);
    params.push_back(param2);
    return callIntMethod(method_name, params);
}

int Interpreter::callIntMethod( const string& method_name, const vector< int >& parameters )
{
    vector< int > output;
    callListMethod( method_name, parameters, output );
    if( output.size() == 1 )
        return output[ 0 ];
    else
        return INT_MAX;
}

void Interpreter::callListMethod( const string& method_name, vector< int >& output )
{
    vector< int > params;
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, vector< int >& output )
{
    vector< int > params;
    params.push_back( param1 );
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, int param2, vector< int >& output )
{
    vector< int > params;
    params.push_back( param1 );
    params.push_back( param2 );
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, int param2, int param3, vector< int >& output )
{
    vector< int > params;
    params.push_back( param1 );
    params.push_back( param2 );
    params.push_back( param3 );
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, vector< uint64_t >& output )
{
    vector< int > params;
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, vector< uint64_t >& output )
{
    vector< int > params;
    params.push_back( param1 );
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, int param2, vector< uint64_t >& output )
{
    vector< int > params;
    params.push_back( param1 );
    params.push_back( param2 );
    callListMethod( method_name, params, output );
}

void Interpreter::callListMethod( const string& method_name, int param1, int param2, int param3, vector< uint64_t >& output )
{
    vector< int > params;
    params.push_back( param1 );
    params.push_back( param2 );
    params.push_back( param3 );
    callListMethod( method_name, params, output );
}
