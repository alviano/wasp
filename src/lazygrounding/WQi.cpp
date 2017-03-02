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

/* 
 * File:   WQi.cpp
 * Author: bernardo
 * 
 * Created on February 26, 2016, 11:17 AM
 */

#include <vector>

#include "WQi.h"
#include "ConstantsManager.h"
#include <iostream>
#include <algorithm>
#include <climits>

const unsigned halfMaxInt = UINT_MAX / 2;
//TODO test different initializations

WQi::WQi(unsigned height, unsigned predicateAriety) : height(height),
predicateAriety(predicateAriety) {

}

WQi::~WQi() {
}

void WQi::insert(vector<unsigned>& tuple) {
    //leaf array
    unsigned v = tuple[predicateAriety - height];
    int type = 0;
    if (v >= halfMaxInt) {
        v -= halfMaxInt;
        type = 1;
    }
    
    if (height == 1) {
        if (v >= leafTruth[type].size()) {
            leafTruth[type].resize(std::max(v + 1, (unsigned)(leafTruth[type].size()*2)), false);
        }
        if (!leafTruth[type][v]) {
            leafTruth[type][v] = true;
            definedIdexes[type].push_back(v);
        }
    }//nested array
    else {
        if (v >= nestedArray[type].size()) {
            nestedArray[type].resize(std::max(v + 1, (unsigned)(nestedArray[type].size()*2)), NULL);
        }
        if (!nestedArray[type][v]) {
            nestedArray[type][v] = new WQi(height - 1, predicateAriety);
            definedIdexes[type].push_back(v);
        }
        nestedArray[type][v]->insert(tuple);
    }

}

unsigned WQi::size() const {
    return definedIdexes[0].size() + definedIdexes[1].size();
}

void WQi::pop(vector<unsigned>& tuple) {
    next(tuple);
    remove(tuple);
    /*int value = definedIdexes.back();
    tuple.push_back(value);
    if (height != 1) {
        WQi* succ = nestedArray[value];
        succ -> pop(tuple);
        if (!succ->size()) {
            delete succ;
            nestedArray[value] = 0;
            definedIdexes.pop_back();
        }
    } else {
        leafTruth[value] = false;
        definedIdexes.pop_back();
    }*/


}

void WQi::next(vector<unsigned> & nextTuple) const {
    if (definedIdexes[0].size()) {
        nextTuple.push_back(definedIdexes[0].back());
        if (height > 1) {
            nestedArray[0][definedIdexes[0].back()]->next(nextTuple);
        }
    } else {
        nextTuple.push_back(definedIdexes[1].back() + halfMaxInt);
        if (height > 1) {
            nestedArray[1][definedIdexes[1].back()]->next(nextTuple);
        }
    }
}

//TODO do we always remove the first element according to the lists?

void WQi::remove(vector<unsigned>& tuple) {
    unsigned value = tuple[tuple.size() - height];
    int index = 0;
    if (value >= halfMaxInt) {
        value -= halfMaxInt;
        index = 1;
    }
    if (height > 1) {
        WQi* succ = nestedArray[index][value];
        succ -> remove(tuple);
        if (!succ->size()) {
            delete succ;
            nestedArray[index][value] = 0;
            definedIdexes[index].pop_back();
        }
    } else {
        leafTruth[index][value] = false;
        definedIdexes[index].pop_back();
    }
}

bool WQi::contains(vector<unsigned>& tuple) const {

    unsigned i = tuple.size() - height;
    unsigned value = tuple[i];
    int index = 0;
    if (value >= halfMaxInt) {
        value -= halfMaxInt;
        index = 1;
    }
    if (height == 1) {
        if (value >= leafTruth[index].size()) {
            return false;
        }
        return leafTruth[index][value];
    }
    if (value >= nestedArray[index].size() || !nestedArray[index][value]) {
        return false;
    }
    return nestedArray[index][value]->contains(tuple);
}

void WQi::printTree() const {
    string tab = "    ";
    for (unsigned k = 0; k <= 1; k++) {
        for (vector<unsigned>::const_iterator it = definedIdexes[k].begin(); it != definedIdexes[k].end(); it++) {
            for (unsigned i = 0; i < predicateAriety - height; i++) {
                cout << tab;
            }
            cout << *it;
            cout << "\n";
            if (height != 1) {
                nestedArray[k][*it]->printTree();
            }
        }
    }
}

void WQi::printTuples(const string & predicateName) const {
    printTuples(predicateName + "(", true);
}

void WQi::printTuples(const string & prec, bool flag) const {
    for (unsigned k = 0; k <= 1; k++) {
        for (vector<unsigned>::const_iterator it = definedIdexes[k].begin(); it != definedIdexes[k].end(); it++) {
            //string next = prec + to_string(*it);
            string next = prec + ConstantsManager::getInstance().unmapConstant(*it+k*halfMaxInt);
            if (height != 1) {
                next += ",";
                nestedArray[k][*it]->printTuples(next, flag);
            } else {
                next += ").\n";
                cout << next;
            }
        }
    }
}

unsigned WQi::getTupleCount() const {
    if (height == 1) {
        return definedIdexes[0].size() + definedIdexes[1].size();
    }
    unsigned cont = 0;
    for (unsigned k = 0; k <= 1; k++) {
        for (vector<unsigned>::const_iterator it = definedIdexes[k].begin(); it != definedIdexes[k].end(); it++) {
            cont += nestedArray[k][*it]->getTupleCount();
        }
    }
    return cont;

}

