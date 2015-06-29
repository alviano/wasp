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

#include <cstdlib>
#include <csignal>

#include "WaspFacade.h"
#include "util/Options.h"
using namespace std;

int EXIT_CODE = 0;

WaspFacade* waspFacadePointer = NULL;

void my_handler( int )
{
    cerr << "Killed: Bye!" << endl;
    EXIT_CODE = 11;
    waspFacadePointer->onKill();
    delete waspFacadePointer;
    exit( EXIT_CODE );
}

int main( int argc, char** argv )
{
    wasp::Options::parse( argc, argv );
    waspFacadePointer = new WaspFacade();
    WaspFacade& waspFacade = *waspFacadePointer;
    wasp::Options::setOptions( waspFacade );        
    
    signal( SIGINT, my_handler );
    signal( SIGTERM, my_handler );
    signal( SIGXCPU, my_handler );
    
    waspFacade.readInput();
    waspFacade.solve();
    waspFacade.onFinish();
    delete waspFacadePointer; 
    return EXIT_CODE;
}

