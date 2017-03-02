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

#ifndef ATOM_H_ASPC
#define ATOM_H_ASPC
#include <string>
#include <vector>
using namespace std;

namespace aspc {

    class Atom {
    public:
        Atom();
        Atom(const string & predicateName, const vector<string> & terms);
        const string & getPredicateName() const;
        const string & getTermAt(unsigned) const;
        unsigned getTermsSize() const;
        const vector<string> & getTerms() const;
        unsigned getAriety() const;
        bool isVariableTermAt(unsigned) const;
        vector<unsigned> getIntTuple() const;
        void print() const;
        string toString() const;
    private:
        string predicateName;
        vector<string> terms;
    };

}
#endif

