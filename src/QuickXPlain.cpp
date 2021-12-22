/*
 *
 *  Copyright 2015 Mario Alviano, Carmine Dodaro, Francesco Ricca, and Philip
 *  Gasteiger.
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

#include "QuickXPlain.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

#include "WaspFacade.h"

void QuickXPlain::minimizeUnsatCore(const vector< Literal >& unsatCore, vector<Literal>& conflict, unsigned int level) {
    conflict.clear();
    if(!unsatCore.empty()) minimizeUnsatCore( level, vector< Literal >(), vector< Literal >(), unsatCore, conflict);    
}

void QuickXPlain::minimizeUnsatCore(unsigned int level, const vector< Literal >& toCheck, const vector< Literal >& addedToCheck, const vector< Literal >& toSplit, vector<Literal>& result) {    
    result.clear();
    if(!addedToCheck.empty() && waspFacade.solve(toCheck, conflict_) == INCOHERENT) {}
    else if(toSplit.size() == 1) { result.push_back(toSplit[0]); }
	else {
	    vector<Literal> firstHalf;
	    vector<Literal> secondHalf;
		partitionVector(toSplit, firstHalf, secondHalf);

        vector<Literal> secondResult;
        vector<Literal> toCheck1;
        appendToVector(toCheck, firstHalf, toCheck1);
    	minimizeUnsatCore(level + 1, toCheck1, firstHalf, secondHalf, secondResult);
		vector<Literal> firstResult;
        vector<Literal> toCheck2;
        appendToVector(toCheck, secondResult, toCheck2);
        minimizeUnsatCore(level + 1, toCheck2, secondResult, firstHalf, firstResult);
    	appendToVector(secondResult, firstResult, result);  	
	}
}

void QuickXPlain::partitionVector(const vector< Literal >& toSplit, vector< Literal >& v1, vector< Literal >& v2) {
    unsigned int splitAt = (toSplit.size()+1)/2;
    for(unsigned int i = 0; i < splitAt; i++) v1.push_back(toSplit[i]);
    for(unsigned int i = splitAt; i < toSplit.size(); i++) v2.push_back( toSplit[i] );
}

void QuickXPlain::appendToVector(const vector< Literal >& v1, const vector< Literal >& v2, vector<Literal>& added) {
    unordered_set<int> addedSet;
    added.clear();
    for(unsigned int i = 0; i < v1.size(); i++) {
        added.push_back(v1[i]);
        addedSet.insert(v1[i].getId());
    }

    for(unsigned int i = 0; i < v2.size(); i++)
        if(addedSet.find(v2[i].getId()) == addedSet.end())
            added.push_back(v2[i]);    
}
