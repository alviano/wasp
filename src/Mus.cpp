#include "Mus.h"
#include "WaspFacade.h"
#include "QuickXPlain.h"
#include <unordered_map>
#include <string>
#include <cassert>
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

    WaspFacade checkFormula;
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
        if(result == INCOHERENT) return;

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
            if(!computeMUS(conflict))
                return;
            for(unsigned int i = 0; i < conflict.size(); i++)
                conflict[i] = Literal(idsmap[conflict[i].getVariable()], NEGATIVE);
            checkFormula.addClause(conflict);
        }
        else {
            checkFormula.addClause(clause);
        }
    }
}

bool MUS::computeMUS(vector<Literal>& conflict) {
    QuickXPlain quickXPlain(waspFacade);
    vector<Literal> mus;
    quickXPlain.minimizeUnsatCore(conflict, mus);
    conflict.clear();
    cout << "[MUS #" << numberOfMUSes+1 << "]:";
    for(vector<Literal>::iterator it = mus.begin(); it != mus.end(); ++it) {
        assert(!VariableNames::isHidden(it->getVariable()));
        conflict.push_back(Literal((*it).getVariable(), POSITIVE));
        cout << " " << Literal((*it).getVariable(), POSITIVE);
    }
    cout << endl;
    return(wasp::Options::maxModels == 0 || ++numberOfMUSes < wasp::Options::maxModels);   

    /*
    vector<Literal> assumptions;
    assumptions.swap(conflict);
    unordered_set<Var> mus;
    while(true) {
        Literal toTest = Literal::null;
        for(unsigned int i = 0; i < assumptions.size(); i++) {
            if(mus.find(assumptions[i].getVariable()) == mus.end()) {
                toTest = assumptions[i];
                assumptions[i] = assumptions.back();
                assumptions.pop_back();
                break;
            }
        }
        if(toTest == Literal::null) {
            conflict.clear();
            cout << "[MUS #" << numberOfMUSes+1 << "]:";
            for(unordered_set<Var>::iterator it = mus.begin(); it != mus.end(); ++it) {
                assert(!VariableNames::isHidden(*it));
                conflict.push_back(Literal(*it, POSITIVE));
                cout << " " << Literal(*it, POSITIVE);
            }
            cout << endl;
            return(wasp::Options::maxModels == 0 || ++numberOfMUSes < wasp::Options::maxModels);                
        }
        unsigned int result = waspFacade.solve(assumptions, conflict);        
        if(result == COHERENT) {
            assumptions.push_back(toTest);
            mus.insert(toTest.getVariable());
        }
    }*/
}