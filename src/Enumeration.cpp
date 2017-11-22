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
    assert(wasp::Options::enumerationStrategy == ENUMERATION_BC || wasp::Options::enumerationStrategy == ENUMERATION_BT);
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
    while( checked.size() <= solver.numberOfVariables() ) checked.push_back( false );
    unsigned int result = solver.solve();
    if(result == INCOHERENT) return;
        
    if(!foundModel()) return;
    solver.getChoicesWithoutAssumptions(assumptions);
    
    flipLatestChoice(assumptions, checked);
    if(assumptions.empty()) return;
    begin:;
    if(solver.getDecisionLevel(assumptions.back())-1 < solver.getCurrentDecisionLevel())
        solver.unroll(solver.getDecisionLevel(assumptions.back())-1);
    solver.clearConflictStatus();
    result = solver.solve(assumptions);
    if(result == INCOHERENT) {
        unsigned int bl=solver.getCurrentDecisionLevel();
        if(bl == 0) {
            unsigned int k=0;
            for(unsigned int i=0; i < assumptions.size(); i++) {
                assumptions[k] = assumptions[i];
                if(solver.getDecisionLevel(assumptions[i]) != 0) k++;
            }
            assumptions.resize(k);
        }
        else {
//            assert( !assums.empty() );
//            assert_msg( solver.getDecisionLevel( assums.back() ) <= bl, solver.getDecisionLevel( assums.back() ) << " != " << bl );
            //TO CHECK: probably this is useless code.
            while(!assumptions.empty() && solver.getDecisionLevel( assumptions.back() ) > bl) assumptions.pop_back();
        }
    }
    else {
        if(!foundModel()) return;
        solver.getChoicesWithoutAssumptions(assumptions);
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
        if(checked[size]) choices.pop_back();
        else break;
    }
    
    choices[size-1]=choices[size-1].getOppositeLiteral();    
    checked[size]=true;
    for(unsigned int i=size+1; i < checked.size(); i++ ) checked[i]=false;
}

bool Enumeration::foundModel() {
    solver.printAnswerSet();
    trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
    if(++numberOfModels >= wasp::Options::maxModels) { trace_msg( enumeration, 1, "Enumerated " << wasp::Options::maxModels << "." ); return false; }    
    return true;
}
