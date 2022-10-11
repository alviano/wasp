#include "Mus.h"
#include "WaspFacade.h"
#include "QuickXPlain.h"
#include "ReasoningPredicateMinimization.h"
#include <unordered_map>
#include <string>
#include <cassert>
#include <iostream>
using namespace std;

void MUS::enumeration() {
    assert(wasp::Options::predicatesMUS.size() > 0);
    for(unsigned int j = 1; j <= waspFacade.numberOfVariables(); j++) {     
        if(VariableNames::isHidden(j)) { continue; }
        const string& name = VariableNames::getName(j);
        for( unsigned int i = 0; i < wasp::Options::predicatesMUS.size(); i++ ) {
            if(Utils::startsWith(name, wasp::Options::predicatesMUS[i])) { waspFacade.freeze(j); candidates.push_back(j); break; }
        }
    }

    if(candidates.size() == 0) {        
        vector<Literal> assumptions;
        vector<Literal> conflict;
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT)
            printMUS(assumptions, conflict);        
    }
    else {
        if(wasp::Options::musAlgorithm == MUS_EMAX) {
            enumerationEmax();            
        }
        else
            enumerationCaMUS();
    }
    printAnswers();
}

void MUS::enumerationEmax() {
    WaspFacade checkFormula;
    Solver& s = const_cast<Solver&>(checkFormula.getSolver());
    s.initFrom(waspFacade.getSolver());
    unordered_map<Var,Var> idsmap;
    vector<Literal> preferredChoices;

    for(unsigned int i = 0; i < candidates.size(); i++) {
        Var newId = checkFormula.addVariable(true);
        idsmap[candidates[i]] = newId;
        preferredChoices.push_back(Literal(newId, POSITIVE));
    }
    checkFormula.setPreferredChoices(preferredChoices);        

    while(true) {
        vector<Literal> assumptions;
        vector<Literal> conflict;
        unsigned int result = checkFormula.solve(assumptions, conflict);
        if(result == INCOHERENT) break;

        assumptions.clear();
        conflict.clear();
        vector<Literal> clause;
        for(unsigned int i = 0; i < candidates.size(); i++) {
            if(checkFormula.isTrue(idsmap[candidates[i]]))
                assumptions.push_back(Literal(candidates[i], POSITIVE));
            else
                clause.push_back(Literal(idsmap[candidates[i]]));
        }
        result = waspFacade.solve(assumptions, conflict);        
        if(result == INCOHERENT) {            
            if(!computeAndPrintMUS(conflict))
                break;
            for(unsigned int i = 0; i < conflict.size(); i++)
                conflict[i] = Literal(idsmap[conflict[i].getVariable()], NEGATIVE);
            checkFormula.addClause(conflict);
        }
        else {
            checkFormula.addClause(clause);
        }
    }
}

void MUS::enumerationCaMUS() {    
    vector<Literal> assumptions;
    vector<Literal> conflict;
    vector<Literal> cardinalityConstraint;
    vector<Literal> preferredChoices;
    WaspFacade allMCS;

    unsigned int maxId = 0;
    for(unsigned int i = 0; i < candidates.size(); i++) {
        cardinalityConstraint.push_back(Literal(candidates[i], POSITIVE));
        Var auxId = waspFacade.addVariable(true);
        VariableNames::setName(auxId, "aux" + to_string(i+1));
        cardinalityConstraint.push_back(Literal(auxId, POSITIVE));
        assumptions.push_back(Literal(auxId, NEGATIVE));
        preferredChoices.push_back(Literal(candidates[i], NEGATIVE));        
        if(candidates[i] > maxId) maxId = candidates[i];
    }    
    
    bool res_ = waspFacade.addCardinalityConstraint(cardinalityConstraint, candidates.size());
    if(!res_) {
        printMUS(vector<Literal>(), conflict);
        return;
    }
    while(allMCS.numberOfVariables() < maxId) allMCS.addVariable(true);
    
    unsigned int k = 0;
    unsigned int result;
    unsigned int numberOfMCSes = 0;    
    while(k <= candidates.size()) {
        result = waspFacade.solve(assumptions, conflict);        
        if(result == INCOHERENT) {
            if(k == candidates.size()) break;
            assumptions[k] = assumptions[k].getOppositeLiteral();
            k++;
        }
        else {
            vector<Literal> mcs;
            for(unsigned int i = 0; i < candidates.size(); i++)
                if(waspFacade.isFalse(candidates[i])) {                    
                    mcs.push_back(Literal(candidates[i]));                    
                }
            waspFacade.addClause(mcs);
            allMCS.addClause(mcs);
            if(++numberOfMCSes >= wasp::Options::mcsThreshold) {
                numberOfMCSes = 0;
                allMCS.setPreferredChoices(preferredChoices);
                unsigned int currentNumberOfMUSes = 0;
                while(numberOfMUSes < wasp::Options::musNumberOfMuses) {
                    vector<Literal> tmpAssumptions;                
                    unsigned int result = allMCS.solve(tmpAssumptions, conflict);
                    if(result == COHERENT) {
                        vector<Literal> toTest;
                        unsigned int nbCandidates = 0;
                        for(unsigned int i = 0; i < candidates.size(); i++)
                            if(allMCS.isTrue(candidates[i]))
                                toTest.push_back(Literal(candidates[i], POSITIVE));
                        nbCandidates = toTest.size();
                        for(unsigned int i = 0; i < assumptions.size(); i++)
                            toTest.push_back(Literal(assumptions[i].getVariable(), POSITIVE));
                        result = waspFacade.solve(toTest, conflict);
                        if(result == COHERENT) break;
                        toTest.resize(nbCandidates);                        
                        printMUS(toTest, conflict);
                        currentNumberOfMUSes++;
                        numberOfMUSes++;
                        if(wasp::Options::maxModels != 0 && numberOfMUSes >= wasp::Options::maxModels)
                            return;
                        for(unsigned int i = 0; i < toTest.size(); i++)
                            toTest[i] = toTest[i].getOppositeLiteral();
                        //waspFacade.addClause(toTest);
                        allMCS.addClause(toTest);                        
                    }
                    else {
                        break;
                    }
                }
                vector<Literal> reset;
                allMCS.setPreferredChoices(reset);
            }
        }
    }
    reasoning = new ReasoningPredicateMinimization(allMCS, numberOfMUSes, true);    
    reasoning->setPrefix("MUS");
    reasoning->addCandidates(candidates);
    reasoning->setMaxNumberOfModelsInEnumeration(1);
    reasoning->enumerationPreferences();
    reasoning->printTotalNumberOfModels();
}

/*void MUS::enumerationCaMUS() {    
    vector<Literal> assumptions;
    vector<Literal> conflict;
    vector<Literal> cardinalityConstraint;
    vector<Literal> preferredChoices;

    for(unsigned int i = 0; i < candidates.size(); i++) {
        cardinalityConstraint.push_back(Literal(candidates[i], POSITIVE));
        Var auxId = waspFacade.addVariable(true);
        VariableNames::setName(auxId, "aux" + to_string(i+1));
        cardinalityConstraint.push_back(Literal(auxId, NEGATIVE));
        assumptions.push_back(Literal(auxId, NEGATIVE));
        preferredChoices.push_back(Literal(candidates[i], NEGATIVE));
    }

    cout << "CC:";
    for(unsigned int i = 0; i < cardinalityConstraint.size(); i++)
        cout << " " << cardinalityConstraint[i];
    cout << " >= " << candidates.size() << endl;
    printStructureOfLiterals(assumptions, "assumptions", "");

    bool res_ = waspFacade.addCardinalityConstraint(cardinalityConstraint, candidates.size());
    if(!res_) {
        printMUS(vector<Literal>(), conflict);
        return;
    }
    
    unsigned int k = 0;
    unsigned int result;
    unsigned int numberOfMCSes = 0;
    WaspFacade allMCS;
    while(k < candidates.size()) {
        result = waspFacade.solve(assumptions, conflict);
        if(result == INCOHERENT) {
            assumptions[k] = assumptions[k].getOppositeLiteral();
            k++;
        }
        else {
            vector<Literal> mcs;
            for(unsigned int i = 0; i < candidates.size(); i++)
                if(waspFacade.isFalse(candidates[i])) {                    
                    mcs.push_back(Literal(candidates[i]));                    
                }
            printStructureOfLiterals(mcs, "mcs", "");
            waspFacade.addClause(mcs);
            allMCS.addClause(mcs);
            if(++numberOfMCSes >= wasp::Options::mcsThreshold) {
                numberOfMCSes = 0;
                allMCS.setPreferredChoices(preferredChoices);
                unsigned int currentNumberOfMUSes = 0;
                while(numberOfMUSes < wasp::Options::musNumberOfMuses) {
                    vector<Literal> tmpAssumptions;                
                    unsigned int result = allMCS.solve(tmpAssumptions, conflict);
                    if(result == COHERENT) {
                        vector<Literal> toTest;
                        for(unsigned int i = 0; i < candidates.size(); i++)
                            if(allMCS.isTrue(candidates[i]))
                                toTest.push_back(Literal(candidates[i], POSITIVE));
                        result = waspFacade.solve(toTest, conflict);
                        if(result == COHERENT) break;
                        printMUS(toTest, conflict);
                        currentNumberOfMUSes++;
                        numberOfMUSes++;
                        if(wasp::Options::maxModels != 0 && numberOfMUSes >= wasp::Options::maxModels)
                            return;
                        for(unsigned int i = 0; i < toTest.size(); i++)
                            toTest[i] = toTest[i].getOppositeLiteral();
                        waspFacade.addClause(toTest);
                        allMCS.addClause(toTest);                        
                    }
                    else {
                        break;
                    }
                }
                vector<Literal> reset;
                allMCS.setPreferredChoices(reset);
            }
        }
    }
        
    reasoning = new ReasoningPredicateMinimization(allMCS, numberOfMUSes, true);    
    reasoning->setPrefix("MUS");
    reasoning->addCandidates(candidates);
    reasoning->setMaxNumberOfModelsInEnumeration(1);
    reasoning->enumerationPreferences();
    reasoning->printTotalNumberOfModels();
}*/

bool MUS::computeAndPrintMUS(vector<Literal>& conflict) {
    vector<Literal> mus;
    minimize(mus, conflict);    
    printMUS(mus, conflict);
    ++numberOfMUSes;
    return(wasp::Options::maxModels == 0 || numberOfMUSes < wasp::Options::maxModels);   
}

void MUS::minimize(vector<Literal>& mus, const vector<Literal>& conflict) const {
    if(!conflict.empty()) {
        QuickXPlain quickXPlain(waspFacade);
        quickXPlain.minimizeUnsatCore(conflict, mus);
    }
}

void MUS::printMUS(const vector<Literal>& mus, vector<Literal>& conflict) const {
    conflict.clear();
    if(wasp::Options::silent < 2)
        cout << "[MUS #" << numberOfMUSes+1 << "]:";
    for(auto& element : mus) {
        assert(!VariableNames::isHidden(element.getVariable()));
        conflict.push_back(Literal(element.getVariable(), POSITIVE));
        if(wasp::Options::silent < 1)
            cout << " " << Literal(element.getVariable(), POSITIVE);
    }
    if(wasp::Options::silent < 2)
        cout << endl;    
}

void MUS::onKill() {
    if(reasoning != nullptr) {
        reasoning->printTotalNumberOfModels();
    }
    else {
        if(wasp::Options::silent == 2) {
            cout << "Number of printed answers: " << numberOfMUSes << endl;
        }
    }
}

void MUS::printAnswers() {
    if(reasoning == nullptr && wasp::Options::silent == 2) {    
        cout << "Number of printed answers: " << numberOfMUSes << endl;
    }
}