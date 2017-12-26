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
    trace_msg(predmin, 1, "Starting solve");
    waspFacade.disableOutput();
    waspFacade.attachAnswerSetListener(this);
    for(unsigned int i = 1; i <= waspFacade.numberOfVariables(); i++) {
        if(VariableNames::isHidden(i)) continue;
        if(waspFacade.isFalse(i)) continue;
        //if(waspFacade.isTrue(i)) { assert( waspFacade.decisionLevel(i) == 0 ); addAnswer(i); continue; }
        candidates.push_back(i);
        waspFacade.freeze(i);
    }
    trace_msg(predmin, 2, "Computing first answer set...");
    if(!computeFirstModel()) { trace_msg(predmin, 3, "...incoherent: STOP"); waspFacade.enableOutput(); waspFacade.printIncoherence(); return; }
    trace_msg(predmin, 3, "...coherent");
    trace_action(predmin, 3, { trace_tag(cerr, predmin, 3); printVectorOfVars(candidates, "Candidates:"); });
    
    if(wasp::Options::queryAlgorithm==ITERATIVE_COHERENCE_TESTING_PREFERENCES)
        iterativeCoherenceTestingPreferences();
    else if(wasp::Options::queryAlgorithm==PRIME_IMPLICATE)
        primeImplicate();
    else
        minimalAlgorithm();
//    coreBased(2);
    
    for(unsigned int i = 0; i < candidates.size(); i++) addAnswer(candidates[i]);
    candidates.clear();
    waspFacade.enableOutput();
    printAnswers();
}

void CautiousReasoning::foundAnswerSet() {
    //Be careful: remove candidates only work with core-guided (maxsat) approaches
    unsigned int j=0;
    for(unsigned int i=0; i < candidates.size(); i++) {
        Var v = candidates[j] = candidates[i];
        assert(!waspFacade.isUndefined(v));
        if(!waspFacade.isTrue(v)) {
            if(coreUtil != NULL) removedCandidates.push_back(Literal(v, NEGATIVE));
            continue;
        }
        
        if(waspFacade.decisionLevel(v) == 0) addAnswer( v );
        else j++;
    }    
    candidates.resize(j);
    printCandidates();
}

void CautiousReasoning::addAnswer(Var v) {
    assert(find(answers.begin(), answers.end(), v) == answers.end());
    if(wasp::Options::queryVerbosity >= 2) cout << "Certain answer: " << Literal(v,POSITIVE) << endl;    
    answers.push_back(v);
    if(wasp::Options::queryVerbosity >= 1) cout << "["<< answers.size() << "," << candidates.size() << "]" << endl;
}

void CautiousReasoning::printCandidates() {
    if(wasp::Options::queryVerbosity >= 1) cout << "["<< answers.size() << "," << candidates.size() << "]" << endl;
    if(wasp::Options::queryVerbosity < 3) return;
    if(wasp::Options::queryVerbosity == 3) { cout << "Number of possible answers: " << candidates.size() << endl; return; }
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

void CautiousReasoning::iterativeCoherenceTestingRestricted(vector<Var>& myCandidates) {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    while(!myCandidates.empty()) {
        Var v = myCandidates.back();
        myCandidates.pop_back();
        assumptions.clear();
        assumptions.push_back(Literal(v, NEGATIVE));
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) { addAnswer(v); waspFacade.addClause(Literal(v, POSITIVE)); }
        else {
            unsigned int j = 0;
            for(unsigned int i = 0; i < myCandidates.size(); i++) {
                myCandidates[j] = myCandidates[i];
                if(waspFacade.isTrue(myCandidates[i])) j++;
            }
            myCandidates.resize(j);
        }
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

void CautiousReasoning::iterativeCoherenceTestingPreferences() {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    vector<Literal> preferences;
    
    while(!candidates.empty()) {
        Var v = candidates.back();
        candidates.pop_back();
        assumptions.clear();
        assumptions.push_back(Literal(v, NEGATIVE));
        preferences.clear();
        for(unsigned int i = 0; i < candidates.size(); i++) preferences.push_back(Literal(candidates[i], NEGATIVE));
        waspFacade.setPreferredChoices(preferences);
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) { addAnswer(v); waspFacade.addClause(Literal(v, POSITIVE)); }        
    }
}

void CautiousReasoning::primeImplicate() {
    trace_msg(predmin, 3, "Starting algorithm based on prime implicate");
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    while(!candidates.empty()) {
        assumptions.clear();
        trace_action(predmin, 4, { trace_tag(cerr, predmin, 5); printVectorOfVars(candidates, "Candidates:"); });
        for(unsigned int i = 0; i < candidates.size(); i++) assumptions.push_back(Literal(candidates[i], NEGATIVE));           
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == COHERENT) return;
        assert(result == INCOHERENT);        

        trace_action(predmin, 4, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(conflict, "Conflict:"); });        
        while(true) {
            assert(!conflict.empty());
            if(conflict.size() == 1) {            
                Literal lit = conflict[0];
                Var v = lit.getVariable();
                if(!VariableNames::isHidden(v) && find(answers.begin(), answers.end(), v) == answers.end()) addAnswer(v);

                waspFacade.addClause(lit.getOppositeLiteral());            
                for(unsigned int i = 0; i < candidates.size(); i++)
                    if(candidates[i] == v) {
                        if(coreUtil != NULL) removedCandidates.push_back(Literal(v, NEGATIVE));
                        candidates[i] = candidates.back();
                        candidates.pop_back();
                        break;
                    }
                break;
            }
            trace_msg(predmin, 5, "Minimizing conflict");
            assumptions = conflict;
            Literal toTest = assumptions.back();
            assumptions.pop_back();
            unsigned int result = waspFacade.solve(assumptions, conflict);
            if(result == COHERENT) { //all other candidates in the conflict are not cautiously true: test the latest one
                trace_msg(predmin, 7, "Coherent: testing " << toTest);
                assumptions.clear();
                assumptions.push_back(toTest);
                result = waspFacade.solve(assumptions, conflict);
                if(result == INCOHERENT) {
                    Var v = toTest.getVariable();
                    if(!VariableNames::isHidden(v) && find(answers.begin(), answers.end(), v) == answers.end()) addAnswer(v);
                }
                break;
            }
            else { trace_msg(predmin, 7, "Incoherent"); }
            trace_action(predmin, 4, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(conflict, "Conflict:"); });            
        }
    }
}

void CautiousReasoning::minimalAlgorithm() {
    trace_msg(predmin, 3, "Starting algorithm based on minimality");
    if( wasp::Options::queryCoreCache &&
        ( wasp::Options::queryAlgorithm == ONE_QUERIES || wasp::Options::queryAlgorithm == KDYN_QUERIES || wasp::Options::queryAlgorithm == PMRES_QUERIES ) )
        coreUtil = new CoreBasedUtil();    
    
    unsigned int size = UINT_MAX;
    while(!candidates.empty() && size != candidates.size()) {        
        size = candidates.size();
        switch(wasp::Options::queryAlgorithm) {
            case PREFERENCE_QUERIES:
                findMinimalModelPreferences();
                break;
            case ONE_QUERIES:
                findMinimalModelOne();
                break;
            case KDYN_QUERIES:
                findMinimalModelK();
                break;
            case PMRES_QUERIES:                
                findMinimalModelPmres();
                break;
            default:
                assert(0);
        }
    }
}

void CautiousReasoning::findMinimalModelPreferences() {
    trace_msg(predmin, 4, "...computing minimal model using preferencess");
    vector<Literal> prefs;
    for(unsigned int i = 0; i < candidates.size(); i++)
        prefs.push_back(Literal(candidates[i], NEGATIVE));
    waspFacade.setPreferredChoices(prefs);

    vector<Literal> assumptions;
    vector<Literal> conflict;
    waspFacade.solve(assumptions, conflict);
}

void CautiousReasoning::findMinimalModelOne() {
    trace_msg(predmin, 4, "...computing minimal model using one");
    vector<Literal> assumptions;
    vector<Literal> conflict;  
    
    initAssumptions(assumptions);
    
    while(true) {
        unsigned int res = solveAndProcessCore(assumptions, conflict);
        if(res==CAUTIOUS_COHERENT_STOP) break;
        if(res==CAUTIOUS_SINGLE_CORE || res==CAUTIOUS_ALREDY_FOUND_CORE) continue;
        assert(res==CAUTIOUS_MULTI_CORE);
        assert(conflict.size() >= 2);
        
        vector<Literal> core;
        vector<Literal> auxAtoms;
        if(coreUtil != NULL) core = conflict;

        unsigned int n = conflict.size();
        unsigned int firstId = waspFacade.numberOfVariables()+1;
        for(unsigned int i=0; i < n-1; i++) {
            Var v = waspFacade.addVariable(true);
            conflict.push_back(Literal(v,POSITIVE));
            assumptions.push_back(Literal(v, NEGATIVE));
            if(coreUtil != NULL) auxAtoms.push_back(Literal(v, NEGATIVE));
        }

        for(unsigned int i=waspFacade.numberOfVariables(); i > firstId; i--) {
            vector<Literal> clause;
            clause.push_back(Literal(i,NEGATIVE));
            clause.push_back(Literal(i-1,POSITIVE));
            trace_action(predmin, 6, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(clause, "Adding clause:"); });
            waspFacade.addClause(clause);            
        }
        trace_action(predmin, 6, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(conflict, "Adding cardinality constraint (bound " + to_string(n-1) + "):"); });
        waspFacade.addCardinalityConstraint(conflict, n-1);
        if(coreUtil != NULL) coreUtil->addCore(core, auxAtoms);
    }
}

/*
 * Adapted from aspino: https://github.com/alviano/aspino/
 */
void CautiousReasoning::findMinimalModelK() {
    trace_msg(predmin, 4, "...computing minimal model using k");
    vector<Literal> assumptions;
    vector<Literal> conflict;  
    
    initAssumptions(assumptions);
    
    while(true) {
        unsigned int res = solveAndProcessCore(assumptions, conflict);        
        if(res==CAUTIOUS_COHERENT_STOP) break;
        if(res==CAUTIOUS_SINGLE_CORE || res==CAUTIOUS_ALREDY_FOUND_CORE) continue;
        assert(res==CAUTIOUS_MULTI_CORE);
        assert(conflict.size() >= 2);
        
        vector<Literal> core;
        vector<Literal> auxAtoms;
        if(coreUtil != NULL) core = conflict;

        //TO UNIFORM WITH ASPINO
        for(unsigned int i = 0; i < conflict.size(); i++) conflict[i] = conflict[i].getOppositeLiteral();
        
        const int b = conflict.size() <= 2 ? 8 : ceil(log10(conflict.size()) * 16);
        const int m = ceil(2.0 * conflict.size() / (b-2.0));
        const int N = ceil(
                (
                    conflict.size()         // literals in the core
                    + conflict.size() - 1   // new soft literals
                    + 2 * (m-1)             // new connectors
                ) / (m * 2.0)
            );
        // ceil((conflict.size() + m) / static_cast<double>(m));
        trace_msg(predmin, 5, "At most " << N*2 << " elements in " << m << " new constraints");

        Literal prec = Literal::null;
        for(;;) {
            assert(conflict.size() > 0);

            vector<Literal> cc;

            int i = N;
            if(prec != Literal::null) { cc.push_back(prec); i--; }
            for(; i > 0; i--) {
                if(conflict.size() == 0) break;
                cc.push_back(conflict.back().getOppositeLiteral());
                conflict.pop_back();
            }
            assert(cc.size() > 0);
            unsigned int bound = cc.size()-1;

            if(conflict.size() > 0) bound++;

            for(unsigned int i = 0; i < bound; i++) {
                Var v = waspFacade.addVariable(true);                
                cc.push_back(Literal(v, NEGATIVE));
                if(i != 0) waspFacade.addClause(Literal(v-1,NEGATIVE), Literal(v,POSITIVE)); // symmetry breaker
                if(i == 0 && conflict.size() > 0) prec = Literal(v, POSITIVE);                
                else { assumptions.push_back(Literal(v,POSITIVE)); if(coreUtil != NULL) auxAtoms.push_back(Literal(v, POSITIVE)); }
            }

            trace_msg(predmin, 6, "Add cardinality constraint of size " << cc.size());
            waspFacade.addCardinalityConstraint(cc, bound);

            if(conflict.size() == 0) break;
        }

        assert(conflict.size() == 0);
        if(coreUtil != NULL) coreUtil->addCore(core, auxAtoms);
    }
}

/*
 * Adapted from aspino: https://github.com/alviano/aspino/
 */
void CautiousReasoning::findMinimalModelPmres() {
    trace_msg(predmin, 4, "...computing minimal model using pmres");
    vector<Literal> assumptions;
    vector<Literal> conflict;  
    
    initAssumptions(assumptions);
    
    while(true) {
        unsigned int res = solveAndProcessCore(assumptions, conflict);
        if(res==CAUTIOUS_COHERENT_STOP) break;
        if(res==CAUTIOUS_SINGLE_CORE || res==CAUTIOUS_ALREDY_FOUND_CORE) continue;
        assert(res==CAUTIOUS_MULTI_CORE);
        assert(conflict.size() >= 2);
    
        Literal prec = Literal::null;
        vector<Literal> lits;
        
        vector<Literal> core;
        vector<Literal> auxAtoms;
        if(coreUtil != NULL) core = conflict;
        
        //TO UNIFORM WITH ASPINO
        for(unsigned int i = 0; i < conflict.size(); i++) conflict[i] = conflict[i].getOppositeLiteral();
        
        while(conflict.size() > 0) {
            if(prec == Literal::null) prec = conflict.back().getOppositeLiteral();
            else {
                // disjunction
                Var v = waspFacade.addVariable(true);
                if(coreUtil != NULL) auxAtoms.push_back(Literal(v, POSITIVE));
                assumptions.push_back(Literal(v, POSITIVE));
                lits.push_back(prec);
                lits.push_back(conflict.back().getOppositeLiteral());
                lits.push_back(Literal(v, NEGATIVE));
                waspFacade.addClause(lits);
                lits.clear();
                waspFacade.addClause(prec.getOppositeLiteral(), Literal(v, POSITIVE));
                waspFacade.addClause(conflict.back(), Literal(v, POSITIVE));

                if(conflict.size() > 1) {
                    // conjunction
                    v = waspFacade.addVariable(true);
                    waspFacade.addClause(prec, Literal(v, NEGATIVE));
                    waspFacade.addClause(conflict.back().getOppositeLiteral(), Literal(v, NEGATIVE));
                    lits.push_back(prec.getOppositeLiteral());
                    lits.push_back(conflict.back());
                    lits.push_back(Literal(v, POSITIVE));
                    waspFacade.addClause(lits);
                    lits.clear();

                    prec = Literal(v, POSITIVE);
                }
            }

            conflict.pop_back();
        }
        assert(conflict.size() == 0);
        if(coreUtil != NULL) coreUtil->addCore(core, auxAtoms);
    }
}

unsigned int CautiousReasoning::solveAndProcessCore(vector<Literal>& assumptions, vector<Literal>& conflict) {
    trace_action(predmin, 5, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(assumptions, "Assumptions:"); });
    bool exists = false;
    vector<Literal> auxAtoms;
    
    if(coreUtil != NULL) {
        coreUtil->newComputation();
        for(unsigned int i = 0; i < assumptions.size(); i++) coreUtil->setAssumption(assumptions[i]);
        exists = coreUtil->checkExistenceOfCore(conflict, auxAtoms);
    }
    
    if(!exists) {
        unsigned int res = waspFacade.solve(assumptions, conflict);
        trace_action(predmin, 5, { trace_tag(cerr, predmin, 5); printVectorOfLiterals(conflict, "Conflict:"); });
        if(res == COHERENT) { trace_action(predmin, 6, { trace_tag(cerr, predmin, 6); waspFacade.enableOutput(); waspFacade.printAnswerSet(); waspFacade.disableOutput(); }); return CAUTIOUS_COHERENT_STOP; }

        if(conflict.size() == 1) {
            Literal lit = conflict[0];
            Var v = lit.getVariable();
            if(!VariableNames::isHidden(v)) addAnswer(v);

            waspFacade.addClause(lit.getOppositeLiteral());
            for(unsigned int i = 0; i < assumptions.size(); i++)
                if(assumptions[i] == lit) { assumptions[i] = assumptions.back(); assumptions.pop_back(); break; }

            for(unsigned int i = 0; i < candidates.size(); i++)
                if(candidates[i] == v) {
                    if(coreUtil != NULL) removedCandidates.push_back(Literal(v, NEGATIVE));
                    candidates[i] = candidates.back();
                    candidates.pop_back();
                    break;
                }
            return CAUTIOUS_SINGLE_CORE;
        }        
    }
    assert_msg(conflict.size() >= 2, "Conflict size " << conflict.size());
    
    vector<Literal> assums;    
    for(unsigned int i=0; i < assumptions.size(); i++) {
        bool found = false;
        for(unsigned int j=0; j < conflict.size(); j++)
            if(assumptions[i] == conflict[j]) { found=true; break; }
        if(!found) assums.push_back(assumptions[i]);
    }
    if(exists) {
        assert(coreUtil != NULL);
        for(unsigned int i = 0; i < auxAtoms.size(); i++) assums.push_back(auxAtoms[i]);
    }
    assumptions.swap(assums);
    
    return exists ? CAUTIOUS_ALREDY_FOUND_CORE : CAUTIOUS_MULTI_CORE;
}

void CautiousReasoning::chunkDynamic(unsigned int chunkSize) {
    vector<Var> myCandidates;    
    while(!candidates.empty()) {
        myCandidates.clear();
        for(unsigned int i = 0; i < candidates.size() && i < chunkSize; i++) myCandidates.push_back(candidates[i]);        
        checkChunk(myCandidates);
        unsigned int j = 0;
        for(unsigned int i = 0; i < candidates.size(); i++) {
            Var v = candidates[j] = candidates[i];
            if(waspFacade.isFalse(v)) continue;
            
            if(waspFacade.isTrue(v) && waspFacade.decisionLevel(v) == 0) { addAnswer(v); continue; }            
            j++;
        }
        candidates.resize(j);        
    }
}

void CautiousReasoning::checkChunk(vector<Var>& myCandidates) {
    vector<Literal> assumptions;
    Var v = waspFacade.addVariable(true);
    assumptions.push_back(Literal(v,NEGATIVE));
    vector<Literal> conflict;    
    while(!myCandidates.empty()) {
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) break;
        assert(result == COHERENT);
        
        vector<Literal> clause;
        unsigned int j = 0;
        for(unsigned int i = 0; i < myCandidates.size(); i++) {
            myCandidates[j] = myCandidates[i];
            if(!waspFacade.isTrue(myCandidates[i])) continue;

            clause.push_back(Literal(myCandidates[i],NEGATIVE));
            j++;
        }
        myCandidates.resize(j);
        clause.push_back(Literal(v,POSITIVE));
        waspFacade.addClause(clause);
    }
    assert(waspFacade.isTrue(Literal(v,POSITIVE)));
    for(unsigned int i = 0; i < myCandidates.size(); i++)
        waspFacade.addClause(Literal(myCandidates[i],POSITIVE));
}

void CautiousReasoning::coreBased(unsigned int chunkSize) {
    vector<Var> myCandidates;
    while(!candidates.empty()) {
        unsigned int i = 0;
        vector<Var> tmp;
        for(; i < chunkSize && i < candidates.size(); i++) myCandidates.push_back(candidates[i]);
        for(unsigned int j = i; j < candidates.size(); j++) tmp.push_back(candidates[j]);
        candidates.swap(tmp);
        checkCoreChunk(myCandidates);
    }
}

void CautiousReasoning::checkCoreChunk(vector<Var>& myCandidates) {
    vector<Literal> assumptions;
    vector<Literal> conflict;
    while(!myCandidates.empty()) {
        assumptions.clear();
        for(unsigned int i=0; i < myCandidates.size(); i++) assumptions.push_back(Literal(myCandidates[i],NEGATIVE));

        while(true) {
            unsigned int result = waspFacade.solve(assumptions,conflict);
            if(result == COHERENT) {
                unsigned int j = 0;
                for(unsigned int i = 0; i < myCandidates.size(); i++) {
                    myCandidates[j] = myCandidates[i];
                    if(waspFacade.isFalse(myCandidates[i])) continue;
                    assert(waspFacade.isTrue(myCandidates[i]));
                    if(waspFacade.decisionLevel(myCandidates[i]) == 0) { addAnswer(myCandidates[i]); continue; }
                    j++;
                }
                myCandidates.resize(j);
                break;
            }
            assert(result == INCOHERENT);
            
            vector<Literal> assums;
            for(unsigned int i = 0; i < assumptions.size(); i++) {
                bool found = false;
                for(unsigned int j = 0; j < conflict.size(); j++)
                    if(assumptions[i] == conflict[j]) { found = true; break; }
                if(!found) assums.push_back(assumptions[i]);
            }
            assumptions.swap(assums);
            
            assert(!conflict.empty());
            if(conflict.size() == 1) {
                Var v = conflict[0].getVariable();
                addAnswer(v);
                for(unsigned int i = 0; i < myCandidates.size(); i++)
                    if(myCandidates[i] == v) { myCandidates[i] = myCandidates.back(); myCandidates.pop_back(); }
                waspFacade.addClause(Literal(v,POSITIVE));
            }
            
            if(assumptions.empty()) { iterativeCoherenceTestingRestricted(myCandidates); break; }
        }
    }
}

void CautiousReasoning::printAnswers() {
    if(wasp::Options::silent==1) cout << "Number of cautious consequences: " << answers.size() << endl;
    if(wasp::Options::silent>=1) return;
    OutputBuilder* output = waspFacade.getOutputBuilder();
    assert(output != NULL);
    output->startModel();
    for(unsigned int i=0; i < answers.size(); i++) output->printVariable(answers[i], true);
    output->endModel();
}

void CautiousReasoning::initAssumptions(vector<Literal>& assumptions) {
    for(unsigned int i = 0; i < candidates.size(); i++) assumptions.push_back(Literal(candidates[i], NEGATIVE));    
    if(coreUtil != NULL) { coreUtil->resetComputation(removedCandidates); removedCandidates.clear(); }
}