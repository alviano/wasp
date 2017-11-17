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

#include "AnswerSetListener.h"
#include <vector>
#include <iostream>
#include "util/WaspConstants.h"
#include "util/VariableNames.h"
#include "Literal.h"
using namespace std;

class WaspFacade;

class PredicateMinimization : public AnswerSetListener
{
    public:
        PredicateMinimization(  WaspFacade& waspFacade_ ) : waspFacade( waspFacade_ ), enumeratedModels( 0 ) {}
        virtual ~PredicateMinimization() {}
        void solve();        

    private:
        WaspFacade& waspFacade;
        
        void enumeration();
        void guessAndCheck();
        void guessAndCheckAndMinimize();
        void guessAndCheckAndSplit();
        void usePreferences();
        void coreBased( unsigned int chunkSize );
        
        virtual void foundAnswerSet();
        void printTrueVars();
        
        vector<Var> originalCandidates;
        vector<Var> candidates;
        vector<Var> trueVars;
        unsigned int enumeratedModels;
        
        #ifdef TRACE_ON
        inline void printVectorOfVars(const vector<Var>& v, const string& description) {
            cerr << description;
            for(unsigned int i = 0; i < v.size(); i++) if(!VariableNames::isHidden(v[i])) cerr << " " << VariableNames::getName(v[i]);
            cerr << endl;
        }
        
        inline void printVectorOfLiterals(const vector<Literal>& v, const string& description) {
            cerr << description;
            for(unsigned int i = 0; i < v.size(); i++) cerr << " " << v[ i ];
            cerr << endl;
        }
        #endif
};

#endif
