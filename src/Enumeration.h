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

#ifndef ENUMERATION_H
#define	ENUMERATION_H


#include <vector>
using namespace std;

#include "Literal.h"
class Solver;

class Enumeration
{
    public:
        Enumeration(Solver& s) : solver(s), numberOfModels(0) {}
        virtual ~Enumeration() {}
        void solve();
        bool coherent() const { return numberOfModels > 0; }
        
    private:
        void enumerationBlockingClause();
        void enumerationBacktracking();
        void flipLatestChoice(vector<Literal>& choices, vector<bool>& checked);
        bool foundModel();
        Solver& solver;
        vector<Literal> assumptions;
        unsigned int numberOfModels;
};

#endif