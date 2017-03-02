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
 * File:   AspCore2ProgramBuilder.cpp
 * Author: bernardo
 * 
 * Created on June 16, 2016, 2:53 PM
 */

#include "AspCore2ProgramBuilder.h"
#include <iostream>
#include <cstring>

AspCore2ProgramBuilder::AspCore2ProgramBuilder() : naf(false), inequalitySign(UNASSIGNED) {
}

void AspCore2ProgramBuilder::onAggregate(bool) {

}

void AspCore2ProgramBuilder::onAggregateElement() {

}

void AspCore2ProgramBuilder::onAggregateFunction(char*) {

}

void AspCore2ProgramBuilder::onAggregateGroundTerm(char*, bool) {

}

void AspCore2ProgramBuilder::onAggregateLowerGuard() {

}

void AspCore2ProgramBuilder::onAggregateNafLiteral() {

}

void AspCore2ProgramBuilder::onAggregateUnknownVariable() {

}

void AspCore2ProgramBuilder::onAggregateUpperGuard() {

}

void AspCore2ProgramBuilder::onAggregateVariableTerm(char*) {

}

void AspCore2ProgramBuilder::onArithmeticOperation(char) {

}

void AspCore2ProgramBuilder::onAtom(bool) {
}

void AspCore2ProgramBuilder::onBody() {

}

void AspCore2ProgramBuilder::onBodyLiteral() {
    if (inequalitySign != UNASSIGNED) {
        inequalities.push_back(tuple<string, InequalitySign, string>(buildingTerms[0], inequalitySign, buildingTerms[1]));
    } else {
        buildingBody.push_back(aspc::Literal(naf, aspc::Atom(predicateName, buildingTerms)));
        if (arietyMap.count(predicateName)) {
            //assert((*arietyMap)[predicateName] == buildingTerms->size());
        } else {
            arietyMap[predicateName] = buildingTerms.size();
        }
    }
    inequalitySign = UNASSIGNED;
    buildingTerms.clear();
    naf = false;
}

void AspCore2ProgramBuilder::onBuiltinAtom() {

}

void AspCore2ProgramBuilder::onChoiceAtom() {

}

void AspCore2ProgramBuilder::onChoiceElement() {

}

void AspCore2ProgramBuilder::onChoiceElementAtom() {

}

void AspCore2ProgramBuilder::onChoiceElementLiteral() {

}

void AspCore2ProgramBuilder::onChoiceLowerGuard() {

}

void AspCore2ProgramBuilder::onChoiceUpperGuard() {

}

void AspCore2ProgramBuilder::onConstraint() {
    program.addRule(aspc::Rule(buildingHead, buildingBody, inequalities, true));
    buildingBody.clear();
    buildingHead.clear();
    inequalities.clear();

}

void AspCore2ProgramBuilder::onDirective(char*, char*) {

}

void AspCore2ProgramBuilder::onEqualOperator() {

}

void AspCore2ProgramBuilder::onExistentialAtom() {

}

void AspCore2ProgramBuilder::onExistentialVariable(char*) {

}

void AspCore2ProgramBuilder::onFunction(char*, int) {

}

void AspCore2ProgramBuilder::onGreaterOperator() {
    inequalitySign = GT;
}

void AspCore2ProgramBuilder::onGreaterOrEqualOperator() {
    inequalitySign = GTE;
}

void AspCore2ProgramBuilder::onHead() {
}

void AspCore2ProgramBuilder::onHeadAtom() {
    buildingHead.push_back(aspc::Atom(predicateName, buildingTerms));
    if (arietyMap.count(predicateName)) {
        //assert((*arietyMap)[predicateName] == buildingTerms->size());
    } else {
        arietyMap[predicateName] = buildingTerms.size();
    }
    buildingTerms.clear();
}

void AspCore2ProgramBuilder::onLessOperator() {
    inequalitySign = LT;
}

void AspCore2ProgramBuilder::onLessOrEqualOperator() {
    inequalitySign = LTE;
}

void AspCore2ProgramBuilder::onNafLiteral(bool naf) {
    if (naf) {
        this->naf = true;
    }

}

void AspCore2ProgramBuilder::onPredicateName(char* predicateName) {
    //why the following does not work?
    //this->predicateName = predicateName;
    this->predicateName = predicateName;



}

void AspCore2ProgramBuilder::onQuery() {

}

void AspCore2ProgramBuilder::onRule() {
    if (buildingBody.empty()) {
        program.addFact(buildingHead.back());
    } else {
        //TODO check!
        //program->addFact(buildingHead->back());
        program.addRule(aspc::Rule(buildingHead, buildingBody, inequalities));
        buildingBody.clear();
        buildingHead.clear();
        inequalities.clear();
    }
}

void AspCore2ProgramBuilder::onTerm(int) {

}

void AspCore2ProgramBuilder::onTerm(char* value) {
    buildingTerms.push_back(value);
}

void AspCore2ProgramBuilder::onTermDash() {

}

void AspCore2ProgramBuilder::onTermParams() {

}

void AspCore2ProgramBuilder::onTermRange(char*, char*) {

}

void AspCore2ProgramBuilder::onUnequalOperator() {
    inequalitySign = NE;
}

void AspCore2ProgramBuilder::onUnknownVariable() {
    buildingTerms.push_back("_");
}

void AspCore2ProgramBuilder::onWeakConstraint() {

}

void AspCore2ProgramBuilder::onWeightAtLevels(int, int, int) {

}

const aspc::Program & AspCore2ProgramBuilder::getProgram() {
    return program;
}

const map<string, unsigned> & AspCore2ProgramBuilder::getArietyMap() {
    return arietyMap;
}
