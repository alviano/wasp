#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
using namespace std;

class Interpreter
{
    public:
        Interpreter(){}
        virtual ~Interpreter(){}
        virtual void callVoidMethod( const string& method_name );
        virtual void callVoidMethod( const string& method_name, const vector< int >& parameters );
        virtual void callVoidMethod( const string& method_name, int param1 );
        virtual void callVoidMethod( const string& method_name, int param1, int param2 );
        virtual void callVoidMethod( const string& method_name, int param1, const string& param2 ) = 0;
        virtual int callIntMethod( const string& method_name );
        virtual int callIntMethod( const string& method_name, const vector< int >& parameters );
        virtual int callIntMethod( const string& method_name, int param1 );
        virtual int callIntMethod( const string& method_name, int param1, int param2 );
        virtual void callListMethod( const string& method_name, vector< int >& output );
        virtual void callListMethod( const string& method_name, int param1, vector< int >& output );
        virtual void callListMethod( const string& method_name, int param1, int param2, vector< int >& output );
        virtual void callListMethod( const string& method_name, int param1, int param2, int param3, vector< int >& output );
        virtual void callListMethod( const string& method_name, const vector< int >& parameters, vector< int >& output ) = 0;
        virtual bool checkMethod( const string& method_name ) const = 0;
};

#endif
