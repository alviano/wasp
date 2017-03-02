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
 * File:   AuxiliaryMap.cpp
 * Author: bernardo
 * 
 * Created on March 7, 2016, 2:17 PM
 */

#include <vector>

#include "AuxiliaryMap.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <climits>

const unsigned halfMaxInt = UINT_MAX / 2;

AuxiliaryMap::AuxiliaryMap(unsigned height, vector<unsigned> * keyIndices) : height(height),
keySize(keyIndices->size()), keyIndices(keyIndices) {
    if(keySize == 0) {        
        leafTuples[0].resize(1);
    }
}


AuxiliaryMap::AuxiliaryMap() {
}


AuxiliaryMap::~AuxiliaryMap() {
    
    for(AuxiliaryMap* auxMap: allocatedMaps) {
        delete auxMap;
    }
}

void AuxiliaryMap::insert2(const vector<unsigned>& value) {
    //TODO  can we do better?
    if(!keySize) {
        leafTuples[0][0].push_back(value);
        return;
    }
    unsigned v = value[(*keyIndices)[keySize - height]];
    unsigned type = 0;
    if (v >= halfMaxInt) {
        v -= halfMaxInt;
        type = 1;
    }
    //leaf array
    if (height==1) {
        if(v>=leafTuples[type].size()) {
            leafTuples[type].resize(max(v+1,(unsigned)(leafTuples[type].size()*2)));
        }
        leafTuples[type][v].push_back(value);
    }//nested array
    else {
        if(v>=nestedArray[type].size()) {
            nestedArray[type].resize(max(v+1,(unsigned)(nestedArray[type].size()*2)), NULL);
        }
        if (nestedArray[type][v] == NULL) {
            nestedArray[type][v] = new AuxiliaryMap(height - 1, keyIndices);
            allocatedMaps.push_back(nestedArray[type][v]);
        }
        nestedArray[type][v]->insert2(value);
    }
}

vector<vector<unsigned> >* AuxiliaryMap::getValues(vector<unsigned>& tuple) {
    //TODO  can we do better?
    if(!keySize) {
        return &(leafTuples[0][0]);
    }
    unsigned index = tuple.size() - height;
    unsigned v = tuple[index];
    unsigned type = 0;
    if (v >= halfMaxInt) {
        v -= halfMaxInt;
        type = 1;
    }
    if (height == 1) {
        if(v>=leafTuples[type].size() || leafTuples[type][v].empty()) {
            return NULL;
        }
        return &(leafTuples[type][v]);
    }
    if(v>=nestedArray[type].size() || !nestedArray[type][v]) {
        return NULL;
    }
    return nestedArray[type][v]->getValues(tuple);
}


