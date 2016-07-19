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
#ifndef PREDICATEMINIMIZATION_H
#define	PREDICATEMINIMIZATION_H

#include "util/VariableNames.h"
#include "Literal.h"
#include <vector>
using namespace std;
class Clause;
class Solver;

class PredicateMinimization
{
    public:
        inline PredicateMinimization( Solver& s ) : solver( s ) {}
        virtual ~PredicateMinimization() {}
        unsigned int minimize();
                
    private:
        inline PredicateMinimization( const PredicateMinimization& orig );
        
        unsigned int enumerationBC();
        unsigned int guessAndCheck();
        unsigned int guessAndCheckAndMinimize();
        unsigned int guessAndCheckAndSplit();
        void minimizeAnswerSet();
        void minimizeAnswerSetSplit();
        bool computeFirstModel();        
        bool checkAnswerSet();
        
        unsigned int countTrue();
        
        void computeTrueVars();
        void printTrueVars();
        
        vector< Var > atomsToMinimize;
        vector< Var > trueVars;
        Solver& solver;
        
        #ifdef TRACE_ON
        inline void printVectorOfVars( const vector< Var >& v, const string& description )
        {
            cout << description;
            for( unsigned int i = 0; i < v.size(); i++ )
                if( !VariableNames::isHidden( v[ i ] ) )
                    cout << " " << VariableNames::getName( v[ i ] );
            cout << endl;
        }
        
        inline void printVectorOfLiterals( const vector< Literal >& v, const string& description )
        {
            cout << description;
            for( unsigned int i = 0; i < v.size(); i++ )
                cout << " " << v[ i ];
            cout << endl;
        }
        #endif
};

#endif

