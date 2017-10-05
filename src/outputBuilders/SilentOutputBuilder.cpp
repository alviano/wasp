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

#include "SilentOutputBuilder.h"
#include "../util/WaspOptions.h"
//void
//SilentOutputBuilder::onProgramIncoherent()
//{
//}

void
SilentOutputBuilder::greetings()
{    
}

void
SilentOutputBuilder::startModel()
{
    assert( wasp::Options::silent == 0 );
    WaspOutputBuilder::startModel();    
}

void
SilentOutputBuilder::printVariable( Var v, bool isTrue )
{
    assert( wasp::Options::silent == 0 );
    WaspOutputBuilder::printVariable( v, isTrue );
}

void
SilentOutputBuilder::endModel()
{
    assert( wasp::Options::silent == 0 );
    WaspOutputBuilder::endModel();
}

void
SilentOutputBuilder::onProgramIncoherent()
{
    if( wasp::Options::silent == 0 )        
        WaspOutputBuilder::onProgramIncoherent();
}

void
SilentOutputBuilder::onFinish()
{
    if( wasp::Options::silent > 1 )
        cout << "Number of printed answers: " << nbOfModels << endl;
}

void
SilentOutputBuilder::foundModel()
{
    nbOfModels++;
    if( wasp::Options::silent == 1 )
        cout << "Model: " << nbOfModels << endl;
}

void
SilentOutputBuilder::onKill()
{
    onFinish();
}