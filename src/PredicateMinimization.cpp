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

#include "PredicateMinimization.h"
#include "WaspFacade.h"
#include "outputBuilders/OutputBuilder.h"
#include "util/WaspErrorMessage.h"

#include <vector>
using namespace std;

bool startsWith(const string& orig, const string& str) {
    if(str.length() > orig.length()) return false;    
    for(unsigned int i = 0; i < str.size(); i++) if(orig[i] != str[i]) return false;    
    return true;
}

void PredicateMinimization::solve() {
    waspFacade.disableOutput();
    assert( wasp::Options::predMinimizationAlgorithm != NO_PREDMINIMIZATION );
    waspFacade.attachAnswerSetListener(this);    
    for(unsigned int j = 1; j <= waspFacade.numberOfVariables(); j++) {
        if(VariableNames::isHidden(j)) continue;
        const string& name = VariableNames::getName(j);
        for( unsigned int i = 0; i < wasp::Options::predicatesToMinimize.size(); i++ ) {
            if(startsWith(name, wasp::Options::predicatesToMinimize[i])) { waspFacade.freeze(j); candidates.push_back(j); originalCandidates.push_back(j); break; }
        }
    }
    
    if(candidates.empty()) {
        vector<Literal> assumptions;
        vector<Literal> conflict;
        waspFacade.solve(assumptions, conflict);
        goto end;        
    }    
    switch(wasp::Options::predMinimizationAlgorithm)
    {
        case PREDMIN_ENUMERATION:
            enumeration();
            break;
            
        case PREDMIN_GUESS_AND_CHECK:
            guessAndCheck();
            break;
        
        case PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE:
            guessAndCheckAndMinimize();
            break;
            
        case PREDMIN_GUESS_AND_CHECK_AND_SPLIT:
            guessAndCheckAndSplit();
            break;
            
        case PREDMIN_PREFERENCES:
            usePreferences();
            break;
            
        case PREDMIN_CORE:
        {
            unsigned int param = candidates.size();
            if(wasp::Options::minimizePredicateChunkPercentage != UINT_MAX)
                param = ( candidates.size() * wasp::Options::minimizePredicateChunkPercentage ) / 100;
            param = max(param, (unsigned int) 2);
            coreBased(param);
        }
            break;
            
        case NO_PREDMINIMIZATION:
        default:
            WaspErrorMessage::errorGeneric( "Invalid option for predicate minimization" );            
    }
    end:;
    waspFacade.enableOutput();
    if(enumeratedModels == 0)
        waspFacade.printIncoherence();
    else
        printTrueVars();
}

void PredicateMinimization::foundAnswerSet() {
    enumeratedModels++;
    vector<Var> trueVars_;
    for(unsigned int i = 0; i < originalCandidates.size(); i++ )
        if(waspFacade.isTrue(Literal(originalCandidates[i], POSITIVE)))
            trueVars_.push_back(originalCandidates[i]);        
    
    if(enumeratedModels == 1 || trueVars_.size() < trueVars.size()) trueVars.swap(trueVars_);    
}

void PredicateMinimization::enumeration() {
    wasp::Options::maxModels = UINT_MAX;
    waspFacade.solve();
}

void PredicateMinimization::guessAndCheck() {
    trace_msg(predmin, 1, "Algorithm guess and check");
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    while(true) {
        assumptions.clear();
        trace_msg(predmin, 2, "Computing new answer set");
        unsigned int res = waspFacade.solve(assumptions, conflict);
        if(res == INCOHERENT) break;
        trace_msg(predmin, 3, "Checking...");
        vector<Literal> lits;
        for(unsigned int i = 0; i < candidates.size(); i++) {
            Var v = candidates[i];
            if(waspFacade.isTrue(Literal(v, POSITIVE))) lits.push_back(Literal(v, NEGATIVE));
            else { lits.push_back(Literal(v, POSITIVE)); assumptions.push_back(Literal(v, NEGATIVE)); }
        }
        if( !waspFacade.addClause(lits) || waspFacade.solve(assumptions, conflict) == INCOHERENT) { trace_msg(predmin, 2, "...minimal: STOP"); break; }
        trace_msg(predmin, 2, "...not minimal!");        
    }
}

void PredicateMinimization::guessAndCheckAndMinimize() {
    trace_msg(predmin, 1, "Algorithm guess and check and minimize");
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    while(true) {
        trace_msg(predmin, 2, "Computing new answer set");
        unsigned int res = waspFacade.solve(assumptions, conflict);
        if(res == INCOHERENT) { trace_msg(predmin, 3, "...incoherent"); break; }
        trace_msg(predmin, 3, "...coherent");
        vector<Literal> lits;
        vector<Literal> unaryClauses;
        unsigned int j = 0;
        for(unsigned int i = 0; i < candidates.size(); i++) {
            Var v = candidates[j] = candidates[i];                
            if(waspFacade.isTrue(Literal(v, POSITIVE))) { lits.push_back(Literal(v, NEGATIVE)); j++; }
            else unaryClauses.push_back(Literal(v, NEGATIVE));            
        }
        candidates.resize(j);
        if(candidates.empty()) { trace_msg(predmin, 4, "no more candidates: stop!"); break; }
        
        for(unsigned int i = 0; i < unaryClauses.size(); i++) if(!waspFacade.addClause(unaryClauses[i])) { candidates.clear(); break; }
        if(!waspFacade.addClause(lits)) { candidates.clear(); break; }
    }
}

void PredicateMinimization::guessAndCheckAndSplit() {
    trace_msg(predmin, 1, "Algorithm guess and check and split");
    vector<Literal> assumptions;
    vector<Literal> conflict;
        
    while(true) {
        trace_msg(predmin, 2, "Computing new answer set");
        unsigned int res = waspFacade.solve(assumptions, conflict);
        if(res == INCOHERENT) {
            if(assumptions.empty()) break;
            assert(assumptions.size() == 1);
            vector<Literal> unary;
            unary.push_back(assumptions.back().getOppositeLiteral());
            if(!waspFacade.addClause(unary)) break;
        }
        else
        {
            vector<Literal> unaryClauses;
            unsigned int j = 0;
            for(unsigned int i = 0; i < candidates.size(); i++) {
                Var v = candidates[j] = candidates[i];
                if(waspFacade.isTrue(Literal(v, POSITIVE))) j++;
                else unaryClauses.push_back(Literal(v, NEGATIVE));                
            }
            candidates.resize(j);
            for(unsigned int i = 0; i < unaryClauses.size(); i++) if(!waspFacade.addClause(unaryClauses[i])) break;
        }
        if(candidates.empty()) break;        
        assumptions.clear();
        assumptions.push_back(Literal(candidates.back(), NEGATIVE));
        candidates.pop_back();
    }
}

void PredicateMinimization::usePreferences() {
    vector<Literal> prefs;
    for(unsigned int i = 0; i < candidates.size(); i++)
        prefs.push_back(Literal(candidates[i], NEGATIVE));
    waspFacade.setPreferredChoices(prefs);
    
    vector<Literal> assumptions;
    vector<Literal> conflict;
    waspFacade.solve(assumptions, conflict);    
}

void PredicateMinimization::coreBased( unsigned int chunkSize ) {
    trace_msg(predmin, 1, "Algorithm core based. Chunk size: " << chunkSize);
    vector<Literal> assumptions;
    vector<Literal> conflict;

    trace_action(predmin, 3, { trace_tag(cerr, predmin, 3); printVectorOfVars(candidates, "Candidates:"); });
    while(!candidates.empty()) {
        assumptions.clear();
        for(unsigned int i = 0; i < chunkSize && i < candidates.size(); i++) assumptions.push_back(Literal(candidates[i], NEGATIVE));
        
        while(true) {
            unsigned int result = waspFacade.solve(assumptions, conflict);
            if(result == COHERENT) {
                trace_msg(predmin, 2, "Found answer set");
                vector<Literal> unaryClauses;
                unsigned int j = 0;
                for(unsigned int i = 0; i < candidates.size(); i++) {
                    Var v = candidates[j] = candidates[i];                
                    if(waspFacade.isTrue(Literal(v, POSITIVE))) j++;
                    else unaryClauses.push_back(Literal(v, NEGATIVE));            
                }
                candidates.resize(j);
                if(candidates.empty()) break;

                for(unsigned int i = 0; i < unaryClauses.size(); i++) if(!waspFacade.addClause(unaryClauses[i])) candidates.clear();
                break;
            }
            else {
                trace_msg(predmin, 2, "Incoherent");                
                trace_action(predmin, 3, { trace_tag(cerr, predmin, 3); printVectorOfLiterals(conflict, "Core:"); });
                
                if(conflict.size() == 0) { trace_msg(predmin, 3, "Found empty core!"); return; }
                if(conflict.size() == 1) {                    
                    if(!waspFacade.addClause(conflict[0].getOppositeLiteral())) return;
                    for(unsigned int i = 0; i < candidates.size(); i++) {
                        if(candidates[i] == conflict[0].getVariable()) { candidates[i] = candidates.back(); candidates.pop_back(); break; }
                    }
                }
                
                vector<Literal> assums;
                for(unsigned int i = 0; i < assumptions.size(); i++) {
                    bool found = false;
                    for(unsigned int j = 0; j < conflict.size(); j++)
                        if(assumptions[i] == conflict[j]) { found = true; break; }
                    if(!found) assums.push_back(assumptions[i]);
                }
                assumptions.swap(assums);
                trace_action(predmin, 3, { trace_tag(cerr, predmin, 3); printVectorOfLiterals(assums, "Assumptions:"); });
                if(assumptions.size() == 0) { trace_msg(predmin, 3, "Give up: switch to guess-check-minimize"); guessAndCheckAndMinimize(); break; }
            }
        }        
    }
}

void PredicateMinimization::printTrueVars()
{
    OutputBuilder* output = waspFacade.getOutputBuilder();
    assert(output != NULL);
    output->startModel();
    for( unsigned int i = 0; i < trueVars.size(); i++ ) output->printVariable(trueVars[ i ], true);
    output->endModel();
}