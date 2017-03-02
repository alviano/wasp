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
 * File:   ExecutionManager.h
 * Author: bernardo
 *
 * Created on March 1, 2016, 2:43 PM
 */

#ifndef EXECUTIONMANAGER_H
#define	EXECUTIONMANAGER_H

#include "Program.h"
#include <map>
#include "WQi.h"
#include "RQi.h"
#include "AuxiliaryMap.h"
#include <iostream>
#include "Executor.h"

class ExecutionManager {
public:
    ExecutionManager();
    ~ExecutionManager();
    void compileDynamicLibrary();
    void parseFactsAndExecute(const char *filename);
    void launchExecutorOnFile(const char *filename);
    const std::vector<std::vector<aspc::Literal> > & getFailedConstraints();
    void executeProgramOnFacts(const std::vector<aspc::Atom> & problemIstance);
    const Executor & getExecutor();
private:
    std::vector<std::vector<aspc::Literal> > failedConstraints;
    Executor* executor;
    void (*destroy)(Executor*);
        
};

#endif	/* EXECUTIONMANAGER_H */

