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
 * File:   Indentation.cpp
 * Author: bernardo
 * 
 * Created on March 10, 2016, 5:22 PM
 */

#include "Indentation.h"

Indentation::Indentation(int depth) :depth(depth){
}

Indentation::Indentation(const Indentation & ind) :depth(ind.depth){
}

ostream& operator<<(ostream& o, const Indentation& ind) {
    for(int i=0;i<ind.depth;i++) {
        o<<"    ";
    }
    return o;
}



Indentation& Indentation::operator++() {
    depth++;
    return *this;
}

Indentation& Indentation::operator--() {
    depth--;
    return *this;
}

Indentation Indentation::operator++(int) {
    Indentation ind(*this);
    depth++;
    return ind;
}

Indentation Indentation::operator--(int) {
    Indentation ind(*this);
    depth--;
    return ind;
}




