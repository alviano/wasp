#include "Utils.h"
#include <cstdlib>
#include <climits>
#include <sstream>

using namespace std;
using namespace DLV2;

bool
Utils::parseLongInteger(
    const char* value,
    long int& result ) 
{
    char* err;
 
    int n=strtol(value, &err, 10);
    if ( *err )
        return false;
    
    result = n;
    return true;
}

bool
Utils::parseInteger(
    const char* value,
    int& result ) 
{
    char* err;
 
    int n=strtol(value, &err, 10);
    if ( *err || n < INT_MIN || n > INT_MAX )
        return false;
    
    result = n;
    return true;
}

bool 
Utils::isNumeric( 
    const char* pszInput, 
    int nNumberBase )
{
    istringstream iss( pszInput );
 
    if ( nNumberBase == 10 )
    {
        double dTestSink;
        iss >> dTestSink;
    }
    else if ( nNumberBase == 8 || nNumberBase == 16 )
    {
        int nTestSink;
        iss >> ( ( nNumberBase == 8 ) ? oct : hex ) >> nTestSink;
    }
    else
        return false;
 
    // Was any input successfully consumed/converted?
    if ( !iss )
        return false;
 
    // Was all the input successfully consumed/converted?
    return ( iss.rdbuf()->in_avail() == 0 );
}