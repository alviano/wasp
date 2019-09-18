#include "StreamReasoning.h"

#include "WaspFacade.h"

void StreamReasoning::solve() {
    //this is just a test
    vector<Literal> assumptions;
    vector<Literal> conflict;
    
    for(unsigned int i = 1; i <= waspFacade.numberOfVariables(); i++) {
        if(VariableNames::isHidden(i))
            continue;
        const string& name = VariableNames::getName(i);        
        if(name == "a") {
            assumptions.push_back(Literal::createLiteralFromInt(i));
        } else if(name == "b") {
            assumptions.push_back(Literal::createLiteralFromInt(-i));
        }
    }
    
    unsigned int result = waspFacade.solve(assumptions, conflict);
    if(result == COHERENT) {
        cout << "Coherent under assumptions" << endl;
    }
    else {
        assert(result == INCOHERENT);
        cout << "Incoherent under assumptions" << endl;
    }

    cout << "Learned clauses: " << clauses.size() << endl;
    for(auto it = clauses.begin(); it != clauses.end(); ++it) {
        Clause* clausePointer = *it;
        Clause& clause = *clausePointer;
        cout << "Processing clause:" << endl;
        for(unsigned int i = 0; i < clause.size(); i++) {
            cout << "\t Lit in position " << i << " - id: " << clause[i].getId();
            if(!VariableNames::isHidden(clause[i].getVariable()))
                cout << " - name: " << VariableNames::getName(clause[i].getVariable()) << endl;
        }
        waspFacade.freeze(clausePointer); //Solver is restarted from level 0. After this it is not possible to retrieve the answer set (you have to call solve again)
    }
    
    assumptions.clear();
    result = waspFacade.solve(assumptions, conflict);
    if(result == COHERENT) {
        cout << "Coherent without assumptions" << endl;
        for(unsigned int i = 1; i <= waspFacade.numberOfVariables(); i++) {
            cout << "Var " << i << " is true? ";
            if(waspFacade.isTrue(i))
                cout << "yes" << endl;
            else
                cout << "no" << endl;
        }
        waspFacade.printAnswerSet();
    }
    else {
        assert(result == INCOHERENT);
        cout << "Incoherent without assumptions" << endl;
    }            
}