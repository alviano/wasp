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
#include <vector>
using namespace std;

class WaspFacade;

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
        void overestimateReduction();
        
        void minimalAlgorithm();
        void findMinimalModelPreferences();
        void findMinimalModelOne();
        void printAnswers();
        

};

#endif

