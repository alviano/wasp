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

#include "WaspOutputBuilder.h"

#include "../util/VariableNames.h"
#include <iostream>
using namespace std;

void
WaspOutputBuilder::startModel()
{
    first = true;
    cout << "{";
}

void
WaspOutputBuilder::printVariable( 
    Var variable,
    bool isTrue )
{
    if( VariableNames::isHidden( variable ) )
        return;

    if( isTrue )
    {
        if( !first )
            cout << ", ";
        else
            first = false;

        cout << VariableNames::getName( variable );
    }
}

void
WaspOutputBuilder::endModel()
{
    cout << "}" << endl;
}

void
WaspOutputBuilder::onProgramIncoherent()
{
     cout << NOMODEL << endl;
}

void
WaspOutputBuilder::greetings()
{
    cout << WASP_STRING << endl;
}