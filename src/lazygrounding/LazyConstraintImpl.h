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
 * File:   LazyConstraintImpl.h
 * Author: bernardo
 *
 * Created on January 9, 2017, 11:05 AM
 */

#ifndef LAZYCONSTRAINTIMPL_H
#define LAZYCONSTRAINTIMPL_H

#include "LazyConstraint.h"
#include <vector>
#include "Atom.h"
#include "ExecutionManager.h"
#include "CompilationManager.h"
#include <unordered_map>


class LazyConstraintImpl: public LazyConstraint {
public: 
    virtual void setFilename(const std::string & filename) override;
    virtual void onAtoms(const std::vector<std::string>& atoms, std::vector<unsigned> & watchedAtoms) override;
    virtual bool onAnswerSet(const std::vector<unsigned int> & interpretation) override;
    virtual void onCheckFail(std::vector<std::vector<int> >& constraints) override;

private:
    std::vector<aspc::Atom> atoms;
    ExecutionManager executionManager;
    CompilationManager compilationManager;
    std::unordered_map<string, int> atomsMap;
    std::vector<unsigned> interestingAtoms;
};

#endif /* LAZYCONSTRAINTIMPL_H */

