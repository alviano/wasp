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
    else if(wasp::Options::predMinimizationAlgorithm==PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE)
        enumerationMinimize();
    else if(wasp::Options::predMinimizationAlgorithm==PREDMIN_GUESS_AND_CHECK_AND_SPLIT)
        enumerationSplit();
    else 
        enumerationUnsatCores();       
    
    if(numberOfModels == 0)
        waspFacade.printIncoherence();
}

void ReasoningPredicateMinimization::addCandidates(const vector<Var>& candidates_) {
    for( unsigned int i = 0; i < candidates_.size(); i++ ) {
        waspFacade.freeze(candidates_[i]);
        candidates.insert(candidates_[i]);
        originalCandidates.push_back(candidates_[i]);
    }       
}

void ReasoningPredicateMinimization::cautiousReasoning() {
    assert( wasp::Options::predMinimizationCautiousAlgorithm != NO_PREDMINIMIZATIONCAUTIOUS );
    for(unsigned int j = 1; j <= waspFacade.numberOfVariables(); j++) {     
        if(VariableNames::isHidden(j)) { continue; }
        const string& name = VariableNames::getName(j);
        bool added = false;
        for( unsigned int i = 0; i < wasp::Options::predicatesToMinimize.size(); i++ ) {
            if(Utils::startsWith(name, wasp::Options::predicatesToMinimize[i])) {
                waspFacade.freeze(j);
                candidates.insert(j);
                originalCandidates.push_back(j);                
                added = true;
                break;
            }
        }
        if(wasp::Options::predicatesToMinimizeCautious.empty()) {
            if(!added) {
                waspFacade.freeze(j);
                cautiousOriginalCandidates.push_back(j);
            }
        }
        else {
            for( unsigned int i = 0; i < wasp::Options::predicatesToMinimizeCautious.size(); i++ ) {
                if(Utils::startsWith(name, wasp::Options::predicatesToMinimizeCautious[i])) {
                    waspFacade.freeze(j);
                    candidates.insert(j);
                    cautiousOriginalCandidates.push_back(j);                
                    break;
                }
            }
        }
    }
        
    assert(wasp::Options::predMinimizationCautiousAlgorithm==PREDMINIMIZATIONCAUTIOUS_MINIMIZE);
    cautiousMinimize();
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
        if(numberOfModels >= wasp::Options::maxModels)
            break;
        waspFacade.addClause(clause);        
    }
}

//Implemented by Salvatore Fiorentino
void ReasoningPredicateMinimization::enumerationMinimize() {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    vector<Literal> constraint;

    for(unsigned int i = 0; i < originalCandidates.size(); i++)
        constraint.push_back(Literal(waspFacade.addVariable(true), NEGATIVE));    
    Var aux_plus = waspFacade.addVariable(true);
    constraint.push_back(Literal(aux_plus, NEGATIVE));
    waspFacade.addClause(constraint);

    for(unsigned int i = 0; i < originalCandidates.size(); i++)
        waspFacade.addClause(constraint[i].getOppositeLiteral() , Literal(originalCandidates[i], NEGATIVE)); // not aux_i -> not o    

    bool entered = false; 
    while(true)
    {
        for(unsigned int i = 0; i < originalCandidates.size(); i++)
            candidates.insert(originalCandidates[i]);
        assumptions.clear();
        unsigned result = waspFacade.solve(assumptions, conflict);
        assumptions.push_back(Literal(aux_plus, POSITIVE));
        entered = false ;
        while(result == COHERENT)
        {
            entered = true ;
            for(unsigned  i = 0 ; i < originalCandidates.size(); i++)
            {
                if(candidates.find(originalCandidates[i]) == candidates.end())
                    continue;
                if(waspFacade.isFalse(originalCandidates[i]))
                {
                    candidates.erase(originalCandidates[i]);
                    assumptions.push_back(Literal(originalCandidates[i], NEGATIVE));
                    assumptions.push_back(constraint[i].getOppositeLiteral());
                }
            }
            result = waspFacade.solve(assumptions, conflict);    
        }
        if(!entered)
            break;
        vector<Literal> clause;
        vector<Literal> assums;
        assums.push_back(Literal(aux_plus, NEGATIVE));
        for(unsigned i = 0; i <  originalCandidates.size(); i++)
        {
            if(candidates.find(originalCandidates[i]) != candidates.end())
            {
                assums.push_back(Literal(originalCandidates[i], POSITIVE));
                clause.push_back(Literal(originalCandidates[i], NEGATIVE));
            }
            else
                assums.push_back(Literal(originalCandidates[i], NEGATIVE));
            assums.push_back(constraint[i].getOppositeLiteral());
        }
        enumerationBacktracking(assums);
        if(numberOfModels >= wasp::Options::maxModels)
            break;
        if(!waspFacade.addClause(clause))
            break;
    }   
}

//Implemented by Salvatore Fiorentino
void ReasoningPredicateMinimization::enumerationSplit(){
    vector<Literal> assumptions;
    vector<Literal> conflict;
    vector<Literal> constraint;

    while(true)
    {
        assumptions.clear();
        constraint.clear();
        for(unsigned int i = 0; i < originalCandidates.size(); i++)
            candidates.insert(originalCandidates[i]);        

        unsigned int res = waspFacade.solve(assumptions, conflict);
        if(res == INCOHERENT)
            break;
            
        for(unsigned int i = 0; i < originalCandidates.size(); i++)
        {
            if(candidates.find(originalCandidates[i]) == candidates.end())
                continue;

            Var a = originalCandidates[i];           
            assumptions.push_back(Literal(a, NEGATIVE));
            unsigned int res = waspFacade.solve(assumptions, conflict);
            assumptions.pop_back();
            if(res == INCOHERENT)
            {
                assumptions.push_back(Literal(a, POSITIVE)); 
                constraint.push_back(Literal(originalCandidates[i], NEGATIVE));
            }
            else
            {
                for(unsigned int i = 0 ; i < originalCandidates.size(); ++i)
                {
                    if(candidates.find(originalCandidates[i]) != candidates.end() && waspFacade.isFalse(originalCandidates[i]))
                    {
                        assumptions.push_back(Literal(originalCandidates[i], NEGATIVE));
                        candidates.erase(originalCandidates[i]);        
                    }
                }   
            }
        }
        
        enumerationBacktracking(assumptions);
        if(numberOfModels >= wasp::Options::maxModels)
            break;
        if(!waspFacade.addClause(constraint))
            break;
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
            if(numberOfModels >= wasp::Options::maxModels)
                break;
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
    numberOfModelsInEnumeration = 0;
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
    if(++numberOfModelsInEnumeration >= maxModelsInEnumeration) return;
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
        if(++numberOfModelsInEnumeration >= maxModelsInEnumeration) return;        
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
    if(printCandidatesOnly) {
        cout << "[" << prefix << " #" << (numberOfModels+1) << "]: ";
        for(unsigned int i = 0; i < originalCandidates.size(); i++)
            if(waspFacade.isTrue(originalCandidates[i]))
                cout << " " << Literal(originalCandidates[i]);
        cout << endl;
    }
    else
        waspFacade.printAnswerSet();
    trace_msg( enumeration, 1, "Model number: " << numberOfModels + 1 );
    if(++numberOfModels >= wasp::Options::maxModels) { trace_msg( enumeration, 1, "Enumerated " << wasp::Options::maxModels << "." ); return false; }    
    return true;
}

void ReasoningPredicateMinimization::cautiousMinimize() {
    vector<Var> ofs;
    vector<Var> qfs;
    vector<Var> qts;
    
    vector<Literal> clause;
    for(unsigned int i = 0; i < originalCandidates.size(); i++) {
        ofs.push_back(waspFacade.addVariable(true));
        //VariableNames::setName(ofs.back(), "of(" + VariableNames::getName(originalCandidates[i]) + ")");
        clause.push_back(Literal(ofs.back(), POSITIVE));
        waspFacade.addClause(Literal(ofs.back(), NEGATIVE), Literal(originalCandidates[i], NEGATIVE));
    }
    ofs.push_back(waspFacade.addVariable(true));
    //VariableNames::setName(ofs.back(), "of");
    clause.push_back(Literal(ofs.back(), NEGATIVE));  
    waspFacade.addClause(clause);
    clause.clear();
    vector<Literal> clause2;
    for(unsigned int i = 0; i < cautiousOriginalCandidates.size(); i++) {
        qfs.push_back(waspFacade.addVariable(true));
        //VariableNames::setName(qfs.back(), "qf(" + VariableNames::getName(cautiousOriginalCandidates[i]) + ")");
        clause.push_back(Literal(qfs.back(), POSITIVE));
        waspFacade.addClause(Literal(qfs.back(), NEGATIVE), Literal(cautiousOriginalCandidates[i], NEGATIVE));    

        qts.push_back(waspFacade.addVariable(true));
        //VariableNames::setName(qts.back(), "qt(" + VariableNames::getName(cautiousOriginalCandidates[i]) + ")");
        clause2.push_back(Literal(qts.back(), POSITIVE));
        waspFacade.addClause(Literal(qts.back(), NEGATIVE), Literal(cautiousOriginalCandidates[i], POSITIVE));    
    }
    qfs.push_back(waspFacade.addVariable(true));
    //VariableNames::setName(qfs.back(), "qf");
    clause.push_back(Literal(qfs.back(), NEGATIVE));  
    waspFacade.addClause(clause);    
    qts.push_back(waspFacade.addVariable(true));
    //VariableNames::setName(qts.back(), "qt");
    clause2.push_back(Literal(qts.back(), NEGATIVE));
    waspFacade.addClause(clause2);
    
    vector<Literal> assumptions;
    vector<Literal> conflict;
    bool flag = true;
    unsigned int result;
    vector<bool> interpretation;
    interpretation.reserve(waspFacade.numberOfVariables());

    unsigned int originalNumberOfCandidates = cautiousOriginalCandidates.size();
    unsigned int removedCandidates = 0;

    while(true) {
        if(flag) {
            assumptions.clear();        
            assumptions.push_back(Literal(ofs.back(), NEGATIVE));
            assumptions.push_back(Literal(qfs.back(), POSITIVE));
            assumptions.push_back(Literal(qts.back(), NEGATIVE));           
            result = waspFacade.solve(assumptions, conflict);
            if(result == INCOHERENT) break;                    
            for(unsigned int i = 0; i <= waspFacade.numberOfVariables(); i++)
                interpretation[i] = waspFacade.isTrue(i);        
        }
        assumptions.clear();
        assumptions.push_back(Literal(ofs.back(), POSITIVE));
        assumptions.push_back(Literal(qfs.back(), NEGATIVE));
        assumptions.push_back(Literal(qts.back(), POSITIVE));        
        for(unsigned int i = 0; i < cautiousOriginalCandidates.size(); i++)
            if(waspFacade.isTrue(cautiousOriginalCandidates[i]))
                assumptions.push_back(Literal(qts[i], NEGATIVE));                    

        for(unsigned int i = 0; i < originalCandidates.size(); i++)
            if(waspFacade.isFalse(originalCandidates[i])) {
                assumptions.push_back(Literal(originalCandidates[i], NEGATIVE));
                assumptions.push_back(Literal(ofs[i], NEGATIVE));
            }

        result = waspFacade.solve(assumptions, conflict);        
        if(result == INCOHERENT) {
            unsigned int j = 0;
            for(unsigned int i = 0; i < cautiousOriginalCandidates.size(); i++) {
                cautiousOriginalCandidates[j] = cautiousOriginalCandidates[i];
                qfs[j] = qfs[i];
                qts[j] = qts[i];                
                if(interpretation[cautiousOriginalCandidates[j]]) j++;
                else {
                    waspFacade.addClause(Literal(qts[i], NEGATIVE));
                    waspFacade.addClause(Literal(qfs[i], NEGATIVE));
                    removedCandidates++;
                }
            }
            if(wasp::Options::silent < 1)
                cout << "[PROGRESS] Considering " << ((float)(originalNumberOfCandidates-removedCandidates)*100/originalNumberOfCandidates) << "% of candidates (" << (originalNumberOfCandidates-removedCandidates) << " out of " << originalNumberOfCandidates << ")" << endl;
            qfs[j] = qfs.back();
            qts[j] = qts.back();
            cautiousOriginalCandidates.resize(j);
            qts.resize(j+1);
            qfs.resize(j+1);
            flag = true;
        }
        else {
            bool allTrues = true;
            for(unsigned int i = 0; i < cautiousOriginalCandidates.size(); i++)
                if(waspFacade.isFalse(cautiousOriginalCandidates[i])) {
                    allTrues = false;
                    break;
                }
            if(allTrues) {
                clause.clear();
                for(unsigned int i = 0; i < originalCandidates.size(); i++)
                    if(waspFacade.isTrue(originalCandidates[i]))
                        clause.push_back(Literal(originalCandidates[i], NEGATIVE));
                waspFacade.addClause(clause);
                flag = true;
            }
            else {
                flag = false;
                for(unsigned int i = 0; i <= waspFacade.numberOfVariables(); i++)
                    interpretation[i] = waspFacade.isTrue(i);
            }
        }        
    }
   
    if(wasp::Options::silent < 2) {
        cout << "Answers:";
        for(unsigned int i = 0; i < cautiousOriginalCandidates.size(); i++)
            cout << " " << Literal(cautiousOriginalCandidates[i]);
        cout << endl;
    }
    else {
        cout << "Number of printed answers: " << cautiousOriginalCandidates.size() << endl;
    }
}

void ReasoningPredicateMinimization::onKill() {
    if(wasp::Options::silent == 2) {
        cout << "Number of printed answers: " << numberOfModels << endl;
    }
}