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

#include <map>
#include <list>
#include <set>
#include <iostream>
#include "Program.h"

aspc::Program::Program() {

}

aspc::Program::~Program() {

}

void aspc::Program::addRule(const Rule & r) {
    rules.push_back(r);
    for (const Literal & literal : r.getBody()) {
        rules_by_type[r.getType()][literal.getPredicateName()].insert(rules.size()-1);
    }

}

void aspc::Program::addFact(const aspc::Atom & r) {
    facts.push_back(r);
}

const aspc::Rule & aspc::Program::getRule(unsigned i) const {
    return rules[i];
}

unsigned aspc::Program::getRulesSize() const {
    return rules.size();
}

const aspc::Atom & aspc::Program::getFact(unsigned i) const {
    return facts[i];
}

const vector<aspc::Atom>& aspc::Program::getFacts() const {
    return facts;
}

unsigned aspc::Program::getFactsSize() const {
    return facts.size();
}

const map<RuleType, map<string, set<unsigned> > >& aspc::Program::getRulesByType() const {
    return rules_by_type;
}


const set<unsigned> & aspc::Program::getRulesByTypeAndPredicateName(const string & predicateName, RuleType type) const {
    return rules_by_type.at(type).at(predicateName);
}

void aspc::Program::clearFacts() {
    facts.clear();
}

void aspc::Program::print() const {
    for (const Rule & rule : rules) {
        rule.print();
    }
}
