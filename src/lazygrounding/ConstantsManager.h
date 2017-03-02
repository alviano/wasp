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

#ifndef CONSTANTSMANAGER_H
#define	CONSTANTSMANAGER_H

#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

class ConstantsManager {
public:
    static ConstantsManager& getInstance() {
        static ConstantsManager instance;
        return instance;
    }
    ConstantsManager();
    unsigned mapConstant(const std::string & constant);
    string unmapConstant(unsigned mapped) const;
private:
    unordered_map<string, int> constantToIntMap;
    vector<string> inverseMap;
    int constantsCounter;
};

#endif	/* CONSTANTSMANAGER_H */

