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

#ifndef CAUTIOUSREASONING_H
#define	CAUTIOUSREASONING_H

#include "AnswerSetListener.h"
#include "util/WaspConstants.h"
#include "Literal.h"
#include "util/VariableNames.h"
#include <vector>
#include <iostream>
using namespace std;

#include "WaspFacade.h"

#define CAUTIOUS_MULTI_CORE 0
#define CAUTIOUS_COHERENT_STOP 1
#define CAUTIOUS_SINGLE_CORE 2

class CautiousReasoning : public AnswerSetListener
{
    public:
        CautiousReasoning(WaspFacade& waspFacade_) : waspFacade(waspFacade_) {}
        virtual ~CautiousReasoning() {}
        void foundAnswerSet();
        void solve();
        
    private:
        vector<Var> candidates;
        vector<Var> answers;
        WaspFacade& waspFacade;
        
        void addAnswer(Var v);
        void printCandidates();
        
        bool computeFirstModel();
        
        void iterativeCoherenceTesting();
        void iterativeCoherenceTestingRestricted(vector<Var>& myCandidates);
        void overestimateReduction();
        
        void minimalAlgorithm();
        void findMinimalModelPreferences();
        void findMinimalModelOne();
        void findMinimalModelK();
        void findMinimalModelPmres();
        void chunkDynamic(unsigned int chunkSize);
        void checkChunk(vector<Var>& myCandidates);
        void coreBased(unsigned int chunkSize);
        void checkCoreChunk(vector<Var>& myCandidates);
        void printAnswers();
        
        unsigned int solveAndProcessCore(vector<Literal>& assumptions, vector<Literal>& conflict);
        
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

