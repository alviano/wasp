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
 * File:   AuxiliaryMap.h
 * Author: bernardo
 *
 * Created on March 7, 2016, 2:17 PM
 */

#ifndef AUXILIARY_MAP_H
#define	AUXILIARY_MAP_H
#include <list>
#include <vector>

using namespace std;

class AuxiliaryMap {
public:
    AuxiliaryMap();
    AuxiliaryMap(unsigned height);
    AuxiliaryMap(unsigned height, vector<unsigned> * keyIndices);
    void insert2(const vector<unsigned> & value);
    virtual ~AuxiliaryMap();
    vector<vector<unsigned> >* getValues(vector<unsigned>& tuple) ;
protected:
    vector<AuxiliaryMap*> nestedArray[2];
    vector<vector<vector<unsigned> > > leafTuples[2];
    int height;
    int keySize;
    vector<unsigned>* keyIndices;    
    vector<AuxiliaryMap*> allocatedMaps;

};

#endif	/* AUXILIARYMAP_H */

