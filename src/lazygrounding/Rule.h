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

#ifndef RULE_H_ASPC
#define RULE_H_ASPC
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include "Atom.h"
#include "Literal.h"

enum RuleType {
    TYPE_1, TYPE_2, TYPE_3, TYPE_4
};

enum InequalitySign {
    GTE = 0, LTE, GT, LT, NE, UNASSIGNED
};

namespace aspc {

    class Rule {
    public:
        static unsigned rulesCounter;
        static string inequalityStrings[];
        Rule();
        Rule(const vector<aspc::Atom> & head, const vector<aspc::Literal> & body, const vector<tuple<string, InequalitySign, string> > & inequalities);
        Rule(const vector<aspc::Atom> & head, const vector<aspc::Literal> & body, const vector<tuple<string, InequalitySign, string> > & inequalities, bool reorderBody);
        virtual ~Rule();
        const vector<aspc::Atom> & getHead() const;
        const vector<aspc::Literal> & getBody() const;
        const vector<tuple<string, InequalitySign, string> > & getInequalities() const;
        vector<unsigned> getHeadToBodyVariableMap() const;
        RuleType getType() const;
        unsigned getRuleId() const;
        vector<map<unsigned, pair<unsigned, unsigned> > > getJoinIndicesWithJoinOrder(const vector<unsigned> & order) const;
        map<unsigned, pair<unsigned, unsigned> > getBodyToHeadVariablesMap() const;
        unsigned getBodySize() const;
        void print() const;
        bool containsNegation() const;
    private:
        vector<aspc::Atom> head;
        vector<aspc::Literal> body;
        int ruleId;
        vector<tuple<string, InequalitySign, string> > inequalities;
        //vector<tuple<string, AspProgramBuilder::InequalitySign, string> > inequalities;

    };
}

#endif

