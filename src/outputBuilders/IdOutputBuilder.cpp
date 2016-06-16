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

#include "IdOutputBuilder.h"
#include "DimacsOutputBuilder.h"

void
IdOutputBuilder::startModel()
{
    cout << "% Solution number: " << ++numberOfModels << endl;   
    cout << "s";
}

void 
IdOutputBuilder::printVariable(
    Var v,
    bool isTrue )
{
    cout << ( isTrue ? " " : " -" ) << v;
}

void
IdOutputBuilder::endModel()
{
    cout << endl;
}

void
IdOutputBuilder::onProgramIncoherent()
{
    cout << INCOHERENT << endl;
}

void
IdOutputBuilder::greetings()
{
    cout << "% " << WASP_STRING;
}

void
IdOutputBuilder::optimumFound()
{
    OutputBuilder::optimumFound();
    numberOfModels = 0;
}