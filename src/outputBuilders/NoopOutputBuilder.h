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

#ifndef WASP_NOOPOUTPUTBUILDER_H
#define WASP_NOOPOUTPUTBUILDER_H

#include "../util/WaspConstants.h"
#include "../stl/Vector.h"
#include "OutputBuilder.h"

#include <iostream>
using namespace std;

class NoopOutputBuilder : public OutputBuilder
{
    public:
        NoopOutputBuilder() : OutputBuilder() {}
        virtual ~NoopOutputBuilder() {}
        virtual void startModel() {}
        virtual void printVariable( Var , bool ) {}
        virtual void endModel() {}
        virtual void onProgramIncoherent() {}
        virtual void greetings(){}
        virtual void foundLowerBound( uint64_t ) {}
        virtual void foundUpperBound( uint64_t ) {}
        void foundModelOptimization( const Vector< uint64_t >& costs )
        {
            modelStream.str( std::string() );
            modelStream << COST;
            for( int i = costs.size() - 1; i >= 0; --i )
                modelStream << " " << costs[ i ] << WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR << ( i + 1 );
            lastWeight = modelStream.str();
        }
        virtual void onFinish() {}
        virtual void onKill() {}
        virtual void foundModel(){}
        virtual void optimumFound()
        {
            if( wasp::Options::silent == 0 )
                OutputBuilder::optimumFound();
        }

        void print()
        {
            if( wasp::Options::silent == 0 )
                cout << lastWeight << endl;
        }
        
    private:
        string lastModel;
        stringstream modelStream;        
        string lastWeight;
};

#endif
