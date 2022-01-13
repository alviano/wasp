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

#include "ReasoningPredicateMinimization.h"
#include "WaspFacade.h"
#include "outputBuilders/OutputBuilder.h"
#include "util/WaspErrorMessage.h"

#include <vector>
#include <iostream>
using namespace std;

void ReasoningPredicateMinimization::enumeration() {
    assert( wasp::Options::predMinimizationAlgorithm != NO_PREDMINIMIZATION );
    for(unsigned int j = 1; j <= waspFacade.numberOfVariables(); j++) {     
        if(VariableNames::isHidden(j)) { continue; }
        const string& name = VariableNames::getName(j);
        for( unsigned int i = 0; i < wasp::Options::predicatesToMinimize.size(); i++ ) {
            if(Utils::startsWith(name, wasp::Options::predicatesToMinimize[i])) {
                waspFacade.freeze(j);
                candidates.insert(j);
                originalCandidates.push_back(j);                
                break;
            }
        }
    }    
        
    if(wasp::Options::predMinimizationAlgorithm==PREDMIN_PREFERENCES)
        enumerationPreferences();
    else
        enumerationUnsatCores();
    
    if(numberOfModels == 0)
        waspFacade.printIncoherence();
}

void ReasoningPredicateMinimization::enumerationPreferences() {
    vector<Literal> preferences;
    for(unordered_set<Var>::iterator it = candidates.begin(); it != candidates.end(); ++it)
        preferences.push_back(Literal(*it, NEGATIVE));
    waspFacade.setPreferredChoices(preferences);
    while(true) {        
        vector<Literal> assumptions;
        vector<Literal> conflict;
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT)
            break;
        vector<Literal> all;        
        vector<Literal> clause;  
        for(unsigned int i = 0; i < originalCandidates.size(); i++)
            if(waspFacade.isTrue(originalCandidates[i])) {
                all.push_back(Literal(originalCandidates[i], POSITIVE));
                clause.push_back(Literal(originalCandidates[i], NEGATIVE));
            }
            else {
                all.push_back(Literal(originalCandidates[i], NEGATIVE));                        
            }            
        
        enumerationBacktracking(all);
        waspFacade.addClause(clause);        
    }
}

void ReasoningPredicateMinimization::enumerationUnsatCores() {
    lastOriginalVar = waspFacade.numberOfVariables();
    while(true) {        
        vector<Literal> assumptions;
        vector<Literal> conflict;

        for(unordered_set<Var>::iterator it = candidates.begin(); it != candidates.end(); ++it)
            assumptions.push_back(Literal(*it, NEGATIVE));
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == COHERENT) {
            vector<Literal> all;
            vector<Literal> clause;            
            for(unsigned int i = 0; i < originalCandidates.size(); i++)
                if(waspFacade.isTrue(originalCandidates[i])) {
                    all.push_back(Literal(originalCandidates[i], POSITIVE));
                    clause.push_back(Literal(originalCandidates[i], NEGATIVE));
                }
                else {
                    all.push_back(Literal(originalCandidates[i], NEGATIVE));                        
                }                
            for(unsigned int var = lastOriginalVar+1; var <= waspFacade.numberOfVariables(); var++) {
                if(waspFacade.isTrue(var))
                    all.push_back(Literal(var, POSITIVE));
                else
                    all.push_back(Literal(var, NEGATIVE));
            } 
            enumerationBacktracking(all);
            waspFacade.addClause(clause);            
        }
        else {
            if(conflict.empty()) break;
            vector<Literal> constraint;
            Var previousVar = 0;         
            for(unsigned int i = 0; i < conflict.size(); i++) {
                constraint.push_back(conflict[i]);
                candidates.erase(conflict[i].getVariable());
                if(i != 0) {
                    Var id = waspFacade.addVariable();
                    constraint.push_back(Literal(id, POSITIVE));
                    candidates.insert(id);
                    if(previousVar != 0)
                        waspFacade.addClause(Literal(previousVar,NEGATIVE), Literal(id, POSITIVE));
                    previousVar = id;
                }
            }            
            waspFacade.addCardinalityConstraint(constraint, conflict.size()-1);              
        }        
    }    
}

void ReasoningPredicateMinimization::enumerationBacktracking(const vector<Literal>& assums) {
    vector< bool > checked;        
    vector<Literal> assumptions;
    for(unsigned int i = 0; i < assums.size(); i++) {
        if(waspFacade.isTrue(assums[i]) && waspFacade.decisionLevel(assums[i]) == 0) continue;        
        assumptions.push_back(assums[i]);
        checked.push_back(true);
    }        

    vector<Literal> conflict;
    unsigned int result = waspFacade.solve(assumptions, conflict);
    assert(result != INCOHERENT);    
    
    if(!foundModel()) return;
    Solver& solver = const_cast<Solver&>(waspFacade.getSolver());
    solver.getChoicesWithoutAssumptions(assumptions);
    while(checked.size() < assumptions.size()) checked.push_back(false);

    flipLatestChoice(assumptions, checked);
    if(assumptions.empty()) return;    
    solver.setComputeUnsatCores(true);
    begin:;
    solver.unrollToZero();
    solver.clearConflictStatus();
    result = solver.solve(assumptions);
    if(result == INCOHERENT) {
        const Clause* c = solver.getUnsatCore();
        if(c->size()==0) return;
        assert(!assumptions.empty());
        assert(!checked.empty());
        if(solver.getCurrentDecisionLevel()==0 || c->size()==1) {
            unsigned int k=0;
            assert(assumptions.size() == checked.size());
            for(unsigned int i=0; i < assumptions.size(); i++) {
                assumptions[k] = assumptions[i];
                checked[k] = checked[i];
                if(solver.getDecisionLevel(assumptions[i]) != 0) k++;
            }
            assert(assumptions.size() == checked.size());
            assumptions.resize(k);
            checked.resize(k);
        }
        else {
            if(wasp::Options::enumerationStrategy == ENUMERATION_BTO) {
                unordered_set<int> core;
                assert(c != NULL);            
                for(unsigned int i = 0; i < c->size(); i++) core.insert(c->getAt(i).getId());
                unsigned int i = assumptions.size()-1;
                assert(core.find(assumptions[i].getId()) != core.end());
                while(i>0) {
                    if(core.find(assumptions[i-1].getId()) != core.end()) {
                        assumptions[i] = assumptions[i].getOppositeLiteral();
                        checked[i] = true;
                        break;
                    }
                    swap(assumptions[i], assumptions[i-1]);
                    swap(checked[i], checked[i-1]);
                    i--;
                }    
            }
        }
    }
    else {
        if(!foundModel()) return;
        solver.getChoicesWithoutAssumptions(assumptions);
        while(checked.size() < assumptions.size()) checked.push_back(false);
    }
    
    flipLatestChoice(assumptions, checked);
    if(assumptions.empty()) return;
    goto begin;
}

void ReasoningPredicateMinimization::flipLatestChoice(vector<Literal>& choices, vector<bool>& checked) {
    unsigned int size;
    while(true) {
        size = choices.size();
        if(size == 0) return;
        if(checked[size-1]) { choices.pop_back(); checked.pop_back(); }
        else break;
    }
    
    choices[size-1]=choices[size-1].getOppositeLiteral();    
    checked[size-1]=true;
}

bool ReasoningPredicateMinimization::foundModel() {
    waspFacade.printAnswerSet();
    trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
    if(++numberOfModels >= wasp::Options::maxModels) { trace_msg( enumeration, 1, "Enumerated " << wasp::Options::maxModels << "." ); return false; }    
    return true;
}
