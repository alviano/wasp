#include "Mus.h"
#include "WaspFacade.h"
#include "QuickXPlain.h"
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
        enumerationEmax();        
    }
    if(wasp::Options::silent == 2) {
        cout << "Number of printed answers: " << numberOfMUSes << endl;
    }
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

bool MUS::computeAndPrintMUS(vector<Literal>& conflict) {
    vector<Literal> mus;
    minimize(mus, conflict);    
    printMUS(mus, conflict);
    return(wasp::Options::maxModels == 0 || ++numberOfMUSes < wasp::Options::maxModels);   
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
    if(wasp::Options::silent == 2) {
        cout << "Number of printed answers: " << numberOfMUSes << endl;
    }
}