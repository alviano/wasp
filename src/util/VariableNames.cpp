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

#include "VariableNames.h"
#include "WaspAssert.h"
#include <sstream>
using namespace std;
vector< string > VariableNames::variables;
bool VariableNames::toBePrinted = false;

bool
VariableNames::isHidden(
    Var v )
{
    //assert_msg( v < variables.size(), "Var " << v << " is out of index: " << variables.size() );
    if( v >= variables.size() )
        return true;
    return variables[ v ] == "";
}

const string&
VariableNames::getName(
    Var v )
{
    #ifdef TRACE_ON
    if( isHidden( v ) )
    {
        stringstream s;
        s << v;
        variables[ v ] = s.str();
    }
    #endif
    assert( !isHidden( v ) );
    return variables[ v ];
}

void
VariableNames::setName(
    Var v,
    string name )
{
    assert_msg( v < variables.size(), "Access element " << v << " in a vector of size " << variables.size() << "." );
    variables[ v ] = name;
}

void
VariableNames::addVariable()
{
    variables.push_back( "" );
}

void
VariableNames::setToBePrinted(
    Var v )
{
    variables[ v ] = "1";
}

bool
VariableNames::hasToBePrinted(
    Var v )
{
    return variables[ v ] == "1";
}

const vector< string >&
VariableNames::getVariables()
{
    return variables;
}