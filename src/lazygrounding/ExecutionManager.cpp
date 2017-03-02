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
 * File:   ExecutionManager.cpp
 * Author: bernardo
 * 
 * Created on March 1, 2016, 2:43 PM
 */

#include "ExecutionManager.h"
#include "DLV2libs/input/InputDirector.h"
#include "AspCore2InstanceBuilder.h"
#include "Executor.h"
#include <dlfcn.h>
#include <stdlib.h>
#include "Program.h"
#include <cassert>


std::string executablePath;

ExecutionManager::ExecutionManager() {
}

ExecutionManager::~ExecutionManager() {

    destroy(executor);
}

void ExecutionManager::launchExecutorOnFile(const char *filename) {
    executor->executeFromFile(filename);
    failedConstraints = executor->getFailedConstraints();
    //print failed constraints
    for (unsigned i = 0; i < failedConstraints.size(); i++) {
        for (unsigned j = 0; j < failedConstraints[i].size(); j++) {
            failedConstraints[i][j].print();
            cout<<" ";
        }
        cout << "\n";
    }
}

void ExecutionManager::parseFactsAndExecute(const char *filename) {
    DLV2::InputDirector director;
    AspCore2InstanceBuilder* builder = new AspCore2InstanceBuilder();
    director.configureBuilder(builder);
    vector<const char*> fileNames;
    fileNames.push_back(filename);
    director.parse(fileNames);
    executeProgramOnFacts(builder->getProblemInstance());
    delete builder;
}

void ExecutionManager::compileDynamicLibrary() {
    string command = "cd " + executablePath + " && make -f DynamicLibraryMake -s";
    system(command.c_str());
    string executorFile = executablePath + "/Executor.so";
    void* handle = dlopen(executorFile.c_str(), RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    Executor * (*create)();


    create = (Executor * (*)())dlsym(handle, "create_object");

    destroy = (void (*)(Executor*))dlsym(handle, "destroy_object");

    executor = (Executor*) create();
}

void ExecutionManager::executeProgramOnFacts(const std::vector<aspc::Atom> & program) {
    /*Executor* executor1 = new Executor();
    executor1->executeProgram(program);
    delete executor1;
    return;*/
    //system("g++ -std=c++11 -fPIC -shared Executor.cpp WQi.o RQi.o AuxiliaryMap.o Literal.o Program.o Atom.o -o Executor.so");

    //Executor* executor = new Executor();
    executor->executeProgramOnFacts(program);
    failedConstraints = executor->getFailedConstraints();

    

}

const std::vector<std::vector<aspc::Literal> > & ExecutionManager::getFailedConstraints() {
    return failedConstraints;
}

const Executor & ExecutionManager::getExecutor() {
    return *executor;
}