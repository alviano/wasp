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

#include "CautiousReasoning.h"
#include "WaspFacade.h"

#include "outputBuilders/OutputBuilder.h"

#include <vector>
#include <iostream>
using namespace std;

void CautiousReasoning::solve() {
    waspFacade.disableOutput();
    waspFacade.attachAnswerSetListener(this);
    for(unsigned int i = 1; i <= waspFacade.numberOfVariables(); i++) {
        if(VariableNames::isHidden(i)) continue;
        if(waspFacade.isFalse(i)) continue;
        if(waspFacade.isTrue(i)) { assert( waspFacade.decisionLevel(i) == 0 ); answers.push_back(i); continue; }
        candidates.push_back(i);
        waspFacade.freeze(i);
    }   
    if(!computeFirstModel()) { waspFacade.enableOutput(); waspFacade.printIncoherence(); return; }
    
    minimalAlgorithm();
    
    for(unsigned int i = 0; i < candidates.size(); i++) addAnswer(candidates[i]);
    candidates.clear();
    waspFacade.enableOutput();
    printAnswers();
}

void CautiousReasoning::foundAnswerSet() {
    unsigned int j=0;
    for(unsigned int i=0; i < candidates.size(); i++) {
        Var v = candidates[j] = candidates[i];
        assert(!waspFacade.isUndefined(v));
        if(!waspFacade.isTrue(v)) continue;
        
        if(waspFacade.decisionLevel(v) == 0) addAnswer( v );
        else j++;
    }    
    candidates.resize(j);
    printCandidates();
}

void CautiousReasoning::addAnswer(Var v) {
    assert(find(answers.begin(), answers.end(), v) == answers.end());
    if(wasp::Options::queryVerbosity >= 1) cout << "Certain answer: " << Literal(v,POSITIVE) << endl;
    answers.push_back(v);
}

void CautiousReasoning::printCandidates() {
    if(wasp::Options::queryVerbosity < 2) return;
    if(wasp::Options::queryVerbosity == 2) { cout << "Number of possible answers: " << candidates.size() << endl; return; }
    cout << "Candidates:";
    for(unsigned int i = 0; i < candidates.size(); i++) cout << " " << Literal(candidates[i],POSITIVE);
    cout << endl;    
}

bool CautiousReasoning::computeFirstModel() {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    unsigned int result = waspFacade.solve(assumptions, conflict);
    if(result == INCOHERENT) return false;

    assert(result == COHERENT);    
    return true;
}

void CautiousReasoning::iterativeCoherenceTesting() {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    while(!candidates.empty()) {
        Var v = candidates.back();
        candidates.pop_back();
        assumptions.clear();
        assumptions.push_back(Literal(v, NEGATIVE));
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) { addAnswer(v); waspFacade.addClause(Literal(v, POSITIVE)); }
    }
}

void CautiousReasoning::overestimateReduction() {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    while(true) {
        assert(assumptions.empty() && conflict.empty());
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) return;
        vector<Literal> clause;
        for(unsigned int i = 0; i < candidates.size(); i++) clause.push_back(Literal(candidates[i], NEGATIVE));
        waspFacade.addClause(clause);
    }
}

void CautiousReasoning::minimalAlgorithm() {
    unsigned int size = UINT_MAX;    
    while(!candidates.empty() && size != candidates.size()) {        
        size = candidates.size();        
//        findMinimalModelPreferences();
        findMinimalModelOne();
    }
}

void CautiousReasoning::findMinimalModelPreferences() {
    vector<Literal> prefs;
    for(unsigned int i = 0; i < candidates.size(); i++)
        prefs.push_back(Literal(candidates[i], NEGATIVE));
    waspFacade.setPreferredChoices(prefs);

    vector<Literal> assumptions;
    vector<Literal> conflict;
    waspFacade.solve(assumptions, conflict);
}

void CautiousReasoning::findMinimalModelOne() {
    vector<Literal> assumptions;
    vector<Literal> conflict;        
    for(unsigned int i = 0; i < candidates.size(); i++) {
        assumptions.push_back(Literal(candidates[i], NEGATIVE));
        unsigned int res = waspFacade.solve(assumptions, conflict);
        if(res == COHERENT) return;
        
        if(conflict.size()==1) {
            Literal lit = conflict[0];
            Var v = lit.getVariable();
            if(!VariableNames::isHidden(v)) addAnswer(v);
            
            waspFacade.addClause(lit.getOppositeLiteral());
            for(unsigned int i = 0; i < assumptions.size(); i++)
                if(assumptions[i] == lit) { assumptions[i] = assumptions.back(); assumptions.pop_back(); break; }
            
            for(unsigned int i = 0; i < candidates.size(); i++)
                if(candidates[i] == v) { candidates[i] = candidates.back(); candidates.pop_back(); break; }
            continue;
        }
        assert(conflict.size() >= 2);

        vector<Literal> assums;
        for(unsigned int i = 0; i < assumptions.size(); i++) {
            bool found = false;
            for(unsigned int j = 0; j < conflict.size(); j++)
                if(assumptions[i] == conflict[j]) { found = true; break; }
            if(!found) assums.push_back(assumptions[i]);
        }
        assumptions.swap(assums);                
        
        unsigned int n = conflict.size();
        unsigned int firstId = waspFacade.numberOfVariables()+1;
        for(unsigned int i=0; i < n-1; i++) conflict.push_back(Literal(waspFacade.addVariable(true),POSITIVE));        

        for(unsigned int i = waspFacade.numberOfVariables(); i > firstId; i--) {
            vector<Literal> clause;
            clause.push_back(Literal(i,NEGATIVE));
            clause.push_back(Literal(i-1,POSITIVE));
            waspFacade.addClause(clause);
        }
        waspFacade.addCardinalityConstraint(conflict, n+1);
    }
}

void CautiousReasoning::printAnswers() {
    OutputBuilder* output = waspFacade.getOutputBuilder();
    assert(output != NULL);
    output->startModel();
    for(unsigned int i=0; i < answers.size(); i++) output->printVariable(answers[i], true);
    output->endModel();
}