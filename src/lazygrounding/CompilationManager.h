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

#ifndef COMPILATIONMANAGER_H
#define	COMPILATIONMANAGER_H

#include "Program.h"
#include "AspCore2ProgramBuilder.h"
#include "Indentation.h"
#include <string>
#include <set>

class CompilationManager {
public:
    CompilationManager();
    void compileFile(const std::string & filename);
    void generateCompilableProgram(const aspc::Program & program, AspCore2ProgramBuilder* builder);
    void setOutStream(std::ostream* outputTarget);
    const std::set<std::string> & getBodyPredicates() {
        return bodyPredicates;
    }
    
private:
    bool matchConstants(const aspc::Rule & rule, const aspc::Atom & atom, Indentation & ind);
    void generateHeadTupleAndMatchConstants(const aspc::Rule & rule, Indentation & ind, const set<string> & bodyPredicates);
    void setHeadVariables(Indentation & ind, const aspc::Rule & rule);
    void checkInequalities(const aspc::Rule & rule, Indentation & ind);
    
    std::ostream* out;
    std::set<std::string> bodyPredicates;

};

#endif	/* COMPILATIONMANAGER_H */

