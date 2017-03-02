/*
 *
 *  Copyright 2016 Bernardo Cuteri, Francesco Ricca.
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

#include "Atom.h"
#include "ConstantsManager.h"
#include <string>
#include <iostream>

aspc::Atom::Atom() {
}

aspc::Atom::Atom(const std::string & predicateName, const std::vector<std::string> & terms): predicateName(predicateName), terms(terms) {
    
}

const std::string & aspc::Atom::getTermAt(unsigned i) const {
    return terms[i];
}

unsigned aspc::Atom::getAriety() const {
    return terms.size();
}

bool aspc::Atom::isVariableTermAt(unsigned i) const {
    return (terms[i][0]>='A' && terms[i][0]<='Z') || terms[i]=="_";
    
}

const string & aspc::Atom::getPredicateName() const {
    return predicateName;
}

vector<unsigned> aspc::Atom::getIntTuple() const {
    vector<unsigned> tuple(terms.size());
    for(unsigned i=0;i<terms.size();i++) {
        tuple[i] = ConstantsManager::getInstance().mapConstant(terms[i]);
        //tuple[i] = stoi((*terms)[i]);
    }
    return tuple;
}

const std::vector<std::string> & aspc::Atom::getTerms() const {
    return terms;
}

unsigned aspc::Atom::getTermsSize() const {
    return terms.size();
}

void aspc::Atom::print() const {
    std::cout<<predicateName<<"(";
    for(unsigned i=0;i<terms.size();i++) {
        cout<<terms[i];
        if(i!=terms.size()-1) {
            cout<<",";
        }
    }
    cout<<")";
}

string aspc::Atom::toString() const {
    string result;
    result+=(predicateName+"(");
    for(unsigned i=0;i<terms.size();i++) {
        result+=terms[i];
        if(i!=terms.size()-1) {
            result+=",";
        }
    }
    result+=")";
    return result;
}
