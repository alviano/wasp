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

#ifndef REASONINGPREDICATEMINIMIZATION_H
#define	REASONINGPREDICATEMINIMIZATION_H

#include "AnswerSetListener.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include "util/WaspConstants.h"
#include "util/VariableNames.h"
#include "Literal.h"
using namespace std;

class WaspFacade;

class ReasoningPredicateMinimization
{
    public:
        ReasoningPredicateMinimization(  WaspFacade& waspFacade_, unsigned int numberOfModels_ = 0, bool printCandidatesOnly_ = false ) : waspFacade( waspFacade_ ), numberOfModels( numberOfModels_ ), printCandidatesOnly(printCandidatesOnly_) {}
        virtual ~ReasoningPredicateMinimization() {}
        void enumeration();
        void cautiousReasoning();
        void addCandidates(const vector<Var>& candidates_);
        void enumerationPreferences();     
        void setMaxNumberOfModelsInEnumeration(unsigned int maxNumber) { maxModelsInEnumeration = maxNumber; }
        void setPrefix(string prefix_) { prefix = prefix_; }
        void printTotalNumberOfModels();

    private:
        WaspFacade& waspFacade;

        void enumerationBacktracking(const vector<Literal>& assums);
        void flipLatestChoice(vector<Literal>& choices, vector<bool>& checked);
        bool foundModel();        
        
        vector<Var> originalCandidates;
        unordered_set<Var> candidates;        
        unsigned int numberOfModels;
        unsigned int lastOriginalVar = 0;
        unsigned int numberOfModelsInEnumeration = 0;
        unsigned int maxModelsInEnumeration = UINT_MAX;
        bool printCandidatesOnly;
        string prefix;
        
        void enumerationUnsatCores();
        void enumerationMinimize();  
        void enumerationSplit();


        vector<Var> cautiousOriginalCandidates;
        void cautiousMinimize();
        void cautiousBase();
        unsigned int knownAnswers = 0;
        unsigned int leftCandidates = 0;
        unsigned int originalNumberOfCandidatesCautious = 0;
};

#endif
