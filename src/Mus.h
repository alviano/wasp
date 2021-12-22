#ifndef MUS_H
#define MUS_H

#include "Literal.h"

class WaspFacade;

#include <vector>
#include <iostream>
#include "util/VariableNames.h"
using namespace std;

class MUS {

    public:
        MUS(  WaspFacade& waspFacade_ ) : waspFacade( waspFacade_ ), numberOfMUSes(0) {}
        virtual ~MUS() {}

        void enumeration();

    private:
        bool computeMUS(vector<Literal>& conflict);
        template <class T>
        static void printStructureOfLiterals(T elem, const string& value, const string& postfix) {
            cout << value;
            for(auto it = elem.begin(); it != elem.end(); ++it) {
                if(VariableNames::isHidden((*it).getVariable()))
                    cout << " " << (*it).getId();
                else
                    cout << " " << *it;
            }
            cout << " " << postfix << endl;
        }

        WaspFacade& waspFacade;
        vector<Var> candidates;
        unsigned int numberOfMUSes;
};

#endif