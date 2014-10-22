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

class SignalException : public runtime_error
{
    public:
        SignalException( const string& _message ) : runtime_error( _message ) {}
};

class SignalHandler
{
    public:        
        SignalHandler()
        {            
        }
        
        void init()
        {
            signal( SIGTERM, SignalHandler::interrupted );
            signal( SIGINT, SignalHandler::interrupted );
            signal( SIGXCPU, SignalHandler::interrupted );
        }
        
        static void interrupted( int )
        {
            throw SignalException( "Killed. Bye!" );
        }            
};

int main( int argc, char** argv )
{
    wasp::Options::parse( argc, argv );    
    WaspFacade waspFacade;     
    wasp::Options::setOptions( waspFacade );        
    try
    {
        SignalHandler signalHandler;
        signalHandler.init();
        waspFacade.greetings();
        waspFacade.readInput();
        waspFacade.solve();
        waspFacade.onFinish();
    }
    catch ( SignalException& exceptions )
    {
        waspFacade.onKill();
        cerr << exceptions.what() << endl;
        EXIT_CODE = 11;
    }    
    
    return EXIT_CODE;
}

