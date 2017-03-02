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

#ifndef PROGRAM_H_ASPC
#define PROGRAM_H_ASPC
#include <vector>
#include <map>
#include <list>
#include <set>
#include "Rule.h"


namespace aspc {

    class Program {
    public:
        Program();
        virtual ~Program();
        void addRule(const aspc::Rule & r);
        void addFact(const aspc::Atom & f);
        unsigned getRulesSize() const;
        unsigned getFactsSize() const;
        const aspc::Rule & getRule(unsigned i) const;
        const aspc::Atom & getFact(unsigned i) const;
        const vector<aspc::Atom> & getFacts() const;
        const map<RuleType, map<string, set<unsigned> > > & getRulesByType() const;
        const set<unsigned> & getRulesByTypeAndPredicateName(const string & predicateName, RuleType type) const;
        void clearFacts();
        void print() const;

    private:
        vector<aspc::Rule> rules; //only rules are compiled
        vector<aspc::Atom> facts; //not compiled


        map<RuleType, map<string, set<unsigned> > > rules_by_type;

    };
}

#endif

