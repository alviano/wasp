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

#ifndef WQI_H
#define	WQI_H
#include <list>
#include <vector>
#include <string>
using namespace std;

class WQi {
public:
    enum MatchingType: int_fast8_t{INT, STRING, BOTH};
    WQi(unsigned height, unsigned predicateAriety);
    void insert(vector<unsigned> & value);
    virtual ~WQi();
    unsigned size() const;
    void next(vector<unsigned> & next) const;
    void pop(vector<unsigned> & tuple);
    bool contains(vector<unsigned> & tuple) const;
    void remove(vector<unsigned> & tuple);
    void printTree() const;
    void printTuples(const string & predicateName) const;
    unsigned getTupleCount() const;
protected:
    void printTuples(const string & prec, bool flag) const;
    vector<WQi*> nestedArray[2];
    vector<unsigned> definedIdexes[2];
    vector<bool> leafTruth[2];
    unsigned height;
    unsigned predicateAriety;

};

#endif	/* WQI_H */

