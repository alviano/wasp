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
#include "../Solver.h"
extern int EXIT_CODE;

void
MultiOutputBuilder::foundModelOptimization(
    Solver& solver,
    unsigned int cost,
    unsigned int numberOfLevels )
{
    cout << "u " << cost << endl;
    stringstream m;
    modelStream.swap( m );
    modelStream << COST;
    for( int i = numberOfLevels - 1; i >= 0; --i )
    {
        modelStream << " " << solver.getCostOfLevel( i, cost ) << WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR << ( i + 1 );
    }
    lastWeight = modelStream.str();
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
    stringstream m;
    modelStream.swap( m );
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
    {
        modelStream << VariableNames::getName( variable ) << ". ";
    }
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
    stringstream m;
    modelStream.swap( m );
}

void
MultiOutputBuilder::onFinish()
{    
    if( lastModel != "" )
    {
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
        cout << "v" << endl;
        cout << lastModel;
        cout << lastWeight << endl;
        if( optimum )
            cout << OPTIMUM_STRING << endl;
    }    
}

void
MultiOutputBuilder::foundLowerBound(
    unsigned int lb )
{
    cout << "l " << lb << endl;
}