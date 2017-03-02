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
 * File:   LazyConstraintImpl.cpp
 * Author: bernardo
 * 
 * Created on January 9, 2017, 11:05 AM
 */

#include "LazyConstraintImpl.h"
#include <fstream>
#include "CompilationManager.h"
#include "ExecutionManager.h"
#include "Literal.h"

void LazyConstraintImpl::setFilename(const std::string & filename) {

    std::ofstream outfile("Executor.cpp");
    //CompilationManager manager(&std::cout);
    compilationManager.setOutStream(&outfile);
    compilationManager.compileFile(filename);
    outfile.close();
    executionManager.compileDynamicLibrary();
    

}

aspc::Atom parseAtom(const std::string & atomString) {
    string predicateName;
    unsigned i = 0;
    for (i = 0; i < atomString.size(); i++) {
        if (atomString[i] == '(') {
            predicateName = atomString.substr(0, i);
            break;
        }
        if (i == atomString.size() - 1) {
            predicateName = atomString.substr(0);
        }
    }
    vector<string> terms;
    for (; i < atomString.size(); i++) {
        char c = atomString[i];
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            int start = i;
            int openBrackets = 0;
            while ((c != ' ' && c != ',' && c != ')') || openBrackets>0) {
                if(c=='(') {
                    openBrackets++;
                }else if(c==')') {
                    openBrackets--;
                }
                i++;
                c = atomString[i];
            }
            terms.push_back(atomString.substr(start, i - start));
        }
    }
    return aspc::Atom(predicateName, terms);

}

void LazyConstraintImpl::onAtoms(const std::vector<std::string>& atoms, std::vector<unsigned> & watchedAtoms) {
    int i = 0;
    for (const std::string & atomString : atoms) {
        //cout<<atomString<<endl;
        const aspc::Atom & atom = parseAtom(atomString);
        //atom.print();
        //cout<<endl;
        this->atoms.push_back(atom);
        atomsMap[atomString] = i;
        if(compilationManager.getBodyPredicates().count(atom.getPredicateName())) {
            interestingAtoms.push_back(i);
        }
        i++;
    }
    watchedAtoms = interestingAtoms;

}

bool LazyConstraintImpl::onAnswerSet(const std::vector<unsigned int> & interpretation) {

    //add only needed atoms
    std::vector<aspc::Atom> facts;
    for(unsigned atomId: interestingAtoms) {
        if(interpretation[atomId])
            facts.push_back(atoms[atomId]);
    }
    executionManager.executeProgramOnFacts(facts);
    return executionManager.getFailedConstraints().empty();

}

void LazyConstraintImpl::onCheckFail(std::vector<std::vector<int> >& constraints) {
    //For every failed constraint, the list of atoms that failed the constraint
    constraints.resize(executionManager.getFailedConstraints().size());
    for (unsigned i = 0; i < executionManager.getFailedConstraints().size(); i++) {
        //cout << "reasons: \n";
        for (const aspc::Literal & literal : executionManager.getFailedConstraints()[i]) {
            //literal.print();
            //cout << " ";
            if(!atomsMap.count(literal.getAtom().toString())) {
                continue;
            }
            if (literal.isNegated()) {
                constraints[i].push_back(-atomsMap[literal.getAtom().toString()]);
            } else {
                constraints[i].push_back(atomsMap[literal.getAtom().toString()]);
            }
        }
        //cout << "\n";
    }
}