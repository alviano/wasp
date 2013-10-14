/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   WaspFacade.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 17.00
 */

#ifndef WASPFACADE_H
#define	WASPFACADE_H

#include "Constants.h"
#include "Solver.h"
#include "input/Dimacs.h"
#include "inputBuilders/SATFormulaBuilder.h"

class WaspFacade
{
    public:
        inline WaspFacade();
        inline ~WaspFacade();
        inline void readInput();
        inline void solve();
        
    private:
        Solver solver;
        unsigned int numberOfModels;
        unsigned int maxModels;
};

WaspFacade::WaspFacade() : numberOfModels( 0 ), maxModels( UINT_MAX )
{
}

WaspFacade::~WaspFacade()
{

}

void
WaspFacade::readInput()
{
    SATFormulaBuilder satFormulaBuilder( &solver );    
    Dimacs dimacs( &satFormulaBuilder );
    dimacs.parse();
    maxModels = 1;
}

void
WaspFacade::solve()
{
    solver.init();
    if( !solver.preprocessing() )
    {
        solver.foundIncoherence();
        return;
    }
    
    while( numberOfModels < maxModels && solver.solve() )
    {
        solver.printAnswerSet();
        numberOfModels++;
        if( !solver.addClauseFromModelAndRestart() )
            break;
    }
    
    if( numberOfModels == 0 )
    {
        solver.foundIncoherence();
    }
}

#endif	/* WASPFACADE_H */
