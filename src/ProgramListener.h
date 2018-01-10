/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
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

#ifndef PROGRAMLISTENER_H
#define	PROGRAMLISTENER_H

#include <vector>
using namespace std;

class ProgramListener
{
    public:
        virtual void addedClause(const vector<int>& literals) = 0;
        virtual void addedAggregate(unsigned int id, const vector<int>& literals, const vector<uint64_t>& weights, uint64_t bound) = 0;        
        virtual ~ProgramListener() {}
};

#endif
