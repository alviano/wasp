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

#include "ConstantsManager.h"
#include "SharedFunctions.h"
#include <climits>
#include <iostream>

const unsigned halfMaxInt = UINT_MAX/2;

ConstantsManager::ConstantsManager() : constantsCounter(halfMaxInt) {

}

unsigned ConstantsManager::mapConstant(const std::string & key) {
    //TODO is it feasible?
    if(isUnsignedInteger(key)) {        
        return stoi(key);
    }
    if (!constantToIntMap.count(key)) {
        constantToIntMap[key] = constantsCounter;
        inverseMap.push_back(key);
        constantsCounter++;
        return constantsCounter - 1;
    }
    //TODO avoid duplicated access
    return constantToIntMap[key];
}

string ConstantsManager::unmapConstant(unsigned mapped) const{
    if(mapped>=halfMaxInt) {
        //std::cout<<mapped<<" unmapped to "<<inverseMap[mapped]<<"\n";        
        return inverseMap[mapped-halfMaxInt];
    }
    //std::cout<<mapped<<" unmapped to "<<mapped<<"\n";        
    return to_string(mapped);
}

