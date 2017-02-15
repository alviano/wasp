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

#ifndef WASP_ISTREAM_H
#define WASP_ISTREAM_H

#include <iostream>

using namespace std;

class Istream
{
    public:
        explicit inline Istream( istream& in_ ) : in( in_ ), idx( 0 ) { buff[ 0 ] = '\0';  in.read( buff, 2047 ); buff[ in.gcount() ] = '\0'; }
        inline char next();
        inline bool read( unsigned int& value );
        inline bool read( uint64_t& value );
        inline bool read( int& value );
        inline bool read( string& value );
        inline void read( char& value );
        inline void getline( char* buff, unsigned size );
        inline bool readInfoDimacs( unsigned int& numberOfVariables, unsigned int& numberOfClauses, uint64_t& maxWeight );        
        inline bool eof(){ return buff[ 0 ] == EOF; }

    private:
        inline void skipBlanksAndComments();
        inline void skipBlanks();
        inline bool isCipher( char c ) const { return '0' <= c && c <= '9'; }
        inline bool isBlank( char c ) const;
        inline void resetBuffer();

        istream& in;
        char buff[ 2048 ];
        unsigned idx;
};

void
Istream::resetBuffer()
{
    idx = 0;
    if( in )
    {
        in.read( buff, 2047 );
        buff[ in.gcount() ] = '\0';
    }
    else
    {
        buff[ 0 ] = EOF;
    }
}

char
Istream::next()
{
    if( buff[ idx ] == '\0' )
        resetBuffer();
    return buff[ idx++ ];
}

bool
Istream::read(
    int& value )
{
    skipBlanksAndComments();

    int sign = 1;
    char c = next();    
    if( c == '-' )
    {
        sign = -1;        
        c = next();
    }

    if( !isCipher( c ) )
        return false;
    value = c - '0';    
    while( true )
    {
        c = next();    
        if( !isCipher( c ) )
        {
            value *= sign;
            return true;
        }
        value *= 10;
        value += c - '0';
    }
    
    return true;
}

bool
Istream::read(
    unsigned int& value )
{
    skipBlanksAndComments();

    char c = next();    

    if( !isCipher( c ) )
        return false;
    value = c - '0';
    while( true )
    {
        c = next();
        if( !isCipher( c ) )
        {
            return true;
        }
        value *= 10;
        value += c - '0';
    }
    
    return true;
}

bool
Istream::read(
    uint64_t& value )
{
    skipBlanksAndComments();

    char c = next();    

    if( !isCipher( c ) )
        return false;
    value = c - '0';
    while( true )
    {
        c = next();
        if( !isCipher( c ) )
        {
            return true;
        }
        value *= 10;
        value += c - '0';
    }
    
    return true;
}

bool
Istream::read(
    string& value )
{
    skipBlanks();
    value = "";
    char c = next();
    while( !isBlank( c ) )
    {
        value += c;
        c = next();
    }
    
    return true;
}

void
Istream::read(
    char& value )
{
    skipBlanks();
    value = next();
}

void
Istream::getline(
    char* buff,
    unsigned size )
{
    unsigned i = 0;
    buff[ i ] = next();
    while( buff[ i ] != '\n' && i < size )
    {
        buff[ ++i ] = next();
    }
    buff[ i ] = '\0';
}

void
Istream::skipBlanksAndComments()
{
    while( true )
    {
        if( buff[ idx ] == '\0' )
            resetBuffer();
        switch( buff[ idx ] )
        {
            case ' ':
            case '\t':
            case '\n':
                next();
                break;

            case 'c':
            {
                char tmp;
                while( ( tmp = next() ) != '\n' && tmp != '\0' );                
            }

            break;
            default:
                return;
        }
    }
}

void
Istream::skipBlanks()
{
    while( true )
    {
        switch( buff[ idx ] )
        {
            case ' ':
            case '\t':
            case '\n':
                next();
                break;

            default:
                return;
        }
    }
}

bool
Istream::isBlank(
    char c ) const
{
    switch( c )
    {
        case ' ':
        case '\t':
        case '\n':
            return true;

        default:
            return false;
    }
}

bool
Istream::readInfoDimacs(
    unsigned int& numberOfVariables,
    unsigned int& numberOfClauses,
    uint64_t& maxWeight )
{
    skipBlanksAndComments();
    if( next() != 'p' )
        return false;
    
    string type;
    read( type );
    
    if( type != "cnf" && type != "wcnf" )
        return false;

    read( numberOfVariables );
    read( numberOfClauses );
    
    if( type.at( 0 ) == 'w' )
        read( maxWeight );
    
    return true;
}

#endif