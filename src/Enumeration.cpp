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

#include "Enumeration.h"

#include "Solver.h"
#include "util/WaspOptions.h"
#include "util/WaspTrace.h"

#include <vector>
using namespace std;

void Enumeration::solve() {
    assert(wasp::Options::enumerationStrategy == ENUMERATION_BC || wasp::Options::enumerationStrategy == ENUMERATION_BT || wasp::Options::enumerationStrategy == ENUMERATION_BTO );
    if(wasp::Options::enumerationStrategy == ENUMERATION_BC)
        enumerationBlockingClause();
    else
        enumerationBacktracking();
    
    if(numberOfModels == 0) { trace_msg(enumeration, 1, "No model found."); solver.foundIncoherence(); return; }
}

void Enumeration::enumerationBlockingClause() {
    while(solver.solve() == COHERENT) {
        if(!foundModel()) break;
        if(!solver.addClauseFromModelAndRestart()) { trace_msg(enumeration, 1, "All models have been found."); break; }
    }
}

void Enumeration::enumerationBacktracking() {
    vector< bool > checked;
    //while( checked.size() <= solver.numberOfVariables() ) checked.push_back( false );
    unsigned int result = solver.solve();
    if(result == INCOHERENT) return;
        
    if(!foundModel()) return;
    solver.getChoicesWithoutAssumptions(assumptions);
    while(checked.size() < assumptions.size()) checked.push_back(false);
    
    flipLatestChoice(assumptions, checked);
    if(assumptions.empty()) return;
    solver.setComputeUnsatCores(true);
    begin:;
//    if(solver.getDecisionLevel(assumptions.back())-1 < solver.getCurrentDecisionLevel())
//        solver.unroll(solver.getDecisionLevel(assumptions.back())-1);
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

void Enumeration::flipLatestChoice(vector<Literal>& choices, vector<bool>& checked) {
    unsigned int size;
    while(true) {
        size = choices.size();
        if(size == 0) return;
        if(checked[size-1]) { choices.pop_back(); checked.pop_back(); }
        else break;
    }
    
    choices[size-1]=choices[size-1].getOppositeLiteral();    
    checked[size-1]=true;
    //for(unsigned int i=size+1; i < checked.size(); i++ ) checked[i]=false;
}

bool Enumeration::foundModel() {
    solver.printAnswerSet();
    trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
    if(++numberOfModels >= wasp::Options::maxModels) { trace_msg( enumeration, 1, "Enumerated " << wasp::Options::maxModels << "." ); return false; }    
    return true;
}
