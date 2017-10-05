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

#include "MultiOutputBuilder.h"
#include "../util/VariableNames.h"
#include "../util/WaspOptions.h"
extern int EXIT_CODE;

void
MultiOutputBuilder::foundModelOptimization(
    const Vector< uint64_t >& costs )
{
    modelStream.str( std::string() );
    modelStream << COST;
    for( int i = costs.size() - 1; i >= 0; --i )
        modelStream << " " << costs[ i ] << WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR << ( i + 1 );
    lastWeight = modelStream.str();
    EXIT_CODE = 10;
}

void
MultiOutputBuilder::optimumFound()
{
    optimum = true;    
    EXIT_CODE = 30;
}

void
MultiOutputBuilder::startModel()
{
    EXIT_CODE = 10;
    modelStream.str( std::string() );
    modelStream << ANSWER << endl;
}

void
MultiOutputBuilder::printVariable(
    Var variable,
    bool isTrue )
{
    if( VariableNames::isHidden( variable ) )
        return;
    if( isTrue )
        modelStream << VariableNames::getName( variable ) << ". ";    
}

void
MultiOutputBuilder::endModel()
{
    modelStream << endl;
    lastModel = modelStream.str();    
}

void
MultiOutputBuilder::onProgramIncoherent()
{
    EXIT_CODE = 20;
    modelStream << NOMODEL_COMPETITION_OUTPUT << endl;
    lastModel = modelStream.str();
    modelStream.str(std::string());
}

void
MultiOutputBuilder::onFinish()
{    
    if( lastModel != "" )
    {
        if( !wasp::Options::printLastModelOnly )
            cout << "v" << endl;
        cout << lastModel;
        cout << lastWeight << endl;
        if( optimum )
            cout << OPTIMUM_STRING << endl;
    }
}

void
MultiOutputBuilder::onKill()
{
    if( lastModel != "" )
    {
        if( !wasp::Options::printLastModelOnly )
            cout << "v" << endl;
        cout << lastModel;
        cout << lastWeight << endl;
        if( optimum )
            cout << OPTIMUM_STRING << endl;
    }    
}

void
MultiOutputBuilder::foundLowerBound(
    uint64_t lb )
{
    if( !wasp::Options::printLastModelOnly )        
        cout << "l " << lb << endl;
    else
        OutputBuilder::foundLowerBound( lb );
}