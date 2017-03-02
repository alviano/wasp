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

#include "RQi.h"

RQi::RQi(unsigned height, unsigned predicateAriety): WQi(height, predicateAriety) {
    
}


RQi::~RQi() {
    for (unsigned k = 0; k <= 1; k++) {
        for (vector<unsigned>::const_iterator it = definedIdexes[k].begin(); it != definedIdexes[k].end(); it++) {
            delete nestedArray[k][*it];
        }
    }
}

