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

#include "CompilationManager.h"
#include "Indentation.h"
#include "SharedFunctions.h"
#include <ostream>
#include <fstream>
#include <cassert>
#include <map>
#include <set>
#include <list>
#include "DLV2libs/input/InputDirector.h"
#include "AspCore2ProgramBuilder.h"
using namespace std;


const string tab = "    ";

CompilationManager::CompilationManager() : out(&std::cout) {
}

void CompilationManager::setOutStream(std::ostream* outputTarget) {
    this->out = outputTarget;
}

void CompilationManager::compileFile(const string &filename) {
    DLV2::InputDirector director;
    AspCore2ProgramBuilder* builder = new AspCore2ProgramBuilder();
    director.configureBuilder(builder);
    vector<const char*> fileNames;
    fileNames.push_back(filename.c_str());
    director.parse(fileNames);
    generateCompilableProgram(builder->getProgram(), builder);
    delete builder;

}

void CompilationManager::checkInequalities(const aspc::Rule & rule, Indentation & ind) {
    const vector<tuple<string, InequalitySign, string> > & inequalities = rule.getInequalities();
    for (unsigned i = 0; i < inequalities.size(); i++) {
        const string & a = get<0>(inequalities[i]);
        const string & b = get<2>(inequalities[i]);
        bool found = false;
        for (unsigned j = 0; j < rule.getBody().size() && !found; j++) {
            if (!rule.getBody()[j].isNegated()) {
                for (unsigned k = 0; k < rule.getBody()[i].getAriety(); k++) {
                    if (a == rule.getBody()[j].getTermAt(k)) {
                        if (i != 0) {
                            *out << " && ";
                        } else {
                            *out << ind << "if(";
                        }
                        *out << "(*tupleStack[" << j << "])[" << k << "]" << aspc::Rule::inequalityStrings[get<1>(inequalities[i])];
                        found = true;
                        break;
                    }
                }
            }
        }

        found = false;
        for (unsigned j = 0; j < rule.getBody().size() && !found; j++) {
            if (!rule.getBody()[j].isNegated()) {
                for (unsigned k = 0; k < rule.getBody()[i].getAriety(); k++) {
                    if (b == rule.getBody()[j].getTermAt(k)) {
                        *out << "(*tupleStack[" << j << "])[" << k << "]";
                        found = true;
                        break;
                    }
                }
            }
        }
    }
}

bool CompilationManager::matchConstants(const aspc::Rule & rule, const aspc::Atom & atom, Indentation & ind) {
    bool thingsToCheck = false;
    for (unsigned i = 0; i < atom.getTermsSize(); i++) {
        if (!atom.isVariableTermAt(i)) {
            if (thingsToCheck) {
                *out << " && ";
            } else {
                *out << ind << "if(";
            }
            if (isUnsignedInteger(atom.getTermAt(i))) {
                *out << "tuple[" << i << "]==" << atom.getTermAt(i);
            } else {
                //TODO use more efficientVersion of mapConstant?
                *out << "tuple[" << i << "]==ConstantsManager::getInstance().mapConstant(\"" << escapeDoubleQuotes(atom.getTermAt(i)) << "\")";
            }
            thingsToCheck = true;
        } else if (atom.getTermAt(i) != "_") {
            //same variable appear twice in a literal
            for (unsigned j = i + 1; j < atom.getTermsSize(); j++) {
                if (atom.getTermAt(i) == atom.getTermAt(j)) {
                    if (thingsToCheck) {
                        *out << " && ";
                    } else {
                        *out << ind << "if(";
                    }
                    *out << "tuple[" << i << "]==tuple[" << j << "]";
                    thingsToCheck = true;
                }
            }
        }

    }
    const vector<tuple<string, InequalitySign, string> > & inequalities = rule.getInequalities();
    for (unsigned i = 0; i < inequalities.size(); i++) {
        const string & a = get<0>(inequalities[i]);
        const string & b = get<2>(inequalities[i]);
        if (b[0] <= 'A' || b[0] >= 'Z') { //b is a constant
            for (unsigned j = 0; j < atom.getTermsSize(); j++) {
                if (a == atom.getTermAt(j)) {
                    if (thingsToCheck) {
                        *out << " && ";
                    } else {
                        *out << ind << "if(";
                    }
                    *out << "tuple[" << j << "]" << aspc::Rule::inequalityStrings[get<1>(inequalities[i])] << b;
                    thingsToCheck = true;
                }
            }
        } else {
            //a and b are both variables
            for (unsigned j = 0; j < atom.getTermsSize(); j++) {
                if (a == atom.getTermAt(j)) {
                    for (unsigned k = 0; k < atom.getTermsSize(); k++) {
                        if (b == atom.getTermAt(k)) {
                            if (thingsToCheck) {
                                *out << " && ";
                            } else {
                                *out << ind << "if(";
                            }
                            *out << "tuple[" << j << "]" << aspc::Rule::inequalityStrings[get<1>(inequalities[i])] << "tuple[" << k << "]";
                            thingsToCheck = true;
                        }
                    }
                }
            }
        }
    }
    return thingsToCheck;
}

void CompilationManager::generateHeadTupleAndMatchConstants(const aspc::Rule & rule, Indentation & ind, const set<string> & bodyPredicates) {
    //TODO handle disjunction and constraints
    const aspc::Atom & headAtom = rule.getHead().front();
    unsigned ruleId = rule.getRuleId();
    *out << ind << "vector<unsigned> generatingTuple" << ruleId << "(" << headAtom.getAriety() << ");\n";
    for (unsigned i = 0; i < headAtom.getTerms().size(); i++) {
        if (!headAtom.isVariableTermAt(i)) {
            *out << ind << "generatingTuple" << ruleId << "[" << i << "] = ConstantsManager::getInstance().mapConstant(\"" << escapeDoubleQuotes(headAtom.getTermAt(i)) << "\");\n";
        } else {
            *out << ind << "generatingTuple" << ruleId << "[" << i << "] = " << "tuple[" << rule.getHeadToBodyVariableMap()[i] << "];\n";
        }
    }

    *out << ind++ << "if(!r" << headAtom.getPredicateName() << ".contains(generatingTuple" << ruleId << ")) {\n";
    if (bodyPredicates.count(headAtom.getPredicateName())) {
        *out << ind << "w" << headAtom.getPredicateName() << ".insert(generatingTuple" << ruleId << ");\n";
    } else {
        *out << ind << "r" << headAtom.getPredicateName() << ".insert(generatingTuple" << ruleId << ");\n";
    }
    //*out << ind << "w" << headAtom->getPredicateName() << ".insert(generatingTuple" << ruleId << ");\n";
    *out << --ind << "}\n";
}

void CompilationManager::setHeadVariables(Indentation & ind, const aspc::Rule & rule) {
    map<unsigned, pair<unsigned, unsigned> > bodyToHeadVariablesMap = rule.getBodyToHeadVariablesMap();
    for (map<unsigned, pair<unsigned, unsigned> >::iterator it = bodyToHeadVariablesMap.begin(); it != bodyToHeadVariablesMap.end(); it++) {
        *out << ind << "head[" << it->first << "] = (*tupleStack[" << it->second.first << "])[" << it->second.second << "];\n";
    }
}

void CompilationManager::generateCompilableProgram(const aspc::Program & program, AspCore2ProgramBuilder* builder) {

    //program.print();
    //cout << endl;
    //*out << "COMPILING PROGRAM\n";

    Indentation ind(0);

    *out << ind << "#include \"Executor.h\"\n\n";
    *out << ind << "#include \"ConstantsManager.h\"\n\n";
    *out << ind << "#include \"DLV2libs/input/InputDirector.h\"\n\n";
    *out << ind << "#include \"AspCore2InstanceBuilder.h\"\n\n";
    *out << ind++ << "extern \"C\" Executor* create_object() {\n";

    *out << ind << "return new Executor;\n";
    *out << --ind << "}\n";

    *out << ind++ << "extern \"C\" void destroy_object( Executor* object ) {\n";
    *out << ind << "delete object;\n";
    *out << --ind << "}\n";

    *out << ind << "Executor::~Executor() {}\n\n";

    *out << ind << "Executor::Executor() {}\n\n";
    //perform join functions

    for (unsigned r = 0; r < program.getRulesSize(); r++) {
        const aspc::Rule & rule = program.getRule(r);
        if (rule.getType() == TYPE_4) {
            unsigned headSize = rule.getHead().size();
            unsigned bodySize = rule.getBodySize();
            //constants match is done beforehand otherwise we have to push rules info into execution code
            //TODO remove last parameter for rules that are not constraints
            string constrainStructures = "";
            if (rule.getHead().empty()) {
                constrainStructures = ", string ruleBodyPredicates[], std::vector<std::vector<aspc::Literal> > & failedConstraints, const vector<unsigned> & bodyArieties, const vector<map<unsigned, string> > & bodyConstantsOfNegLit";

            }
            *out << ind++ << "void performJoin_" << r << "(unsigned index, WQi* wqi, RQi* rqi, const vector<unsigned> & joinOrder, const vector<unsigned> & tuple0, AuxiliaryMap** auxiliaryMaps, vector<map<unsigned, pair<unsigned, unsigned> > > & keys_indices, vector<AuxiliaryMap* > & atomMaps, const vector<bool> & negativeLiterals" + constrainStructures + ", WQi* wQiNeg[]) {\n";
            //TODO handle negative literals! change the head derivation to avoid negated literals
            //TODO current assumption is that negated literals are iterated before positive literals (rules are of the type head:- negBody, posBody))
            //many changes required in the Rule class
            /*
             *out << ind << "cout<<\"Attempting join with \"<<endl;\n";
             *out << ind++ << "for(unsigned v:tuple0) {\n";
             *out << ind << "cout<<ConstantsManager::getInstance().unmapConstant(v)<<\" \";\n";
             *out << --ind << "}\n";
             *out << ind << "cout<<endl;\n";
             */

            *out << ind++ << "for(AuxiliaryMap* p: atomMaps) {\n";
            *out << ind << "p->insert2(tuple0);\n";
            *out << --ind << "}\n";
            //*out << ind << "cout<<\"tuple added to its auxiliary maps\"<<endl;\n";
            *out << ind++ << "if(negativeLiterals[index]) {\n";
            *out << ind << "return;\n";
            *out << --ind << "}\n";
            *out << ind << "const vector<unsigned>* tupleStack[" << bodySize << "];\n";
            *out << ind << "tupleStack[index]=&tuple" << 0 << ";\n";
            for (unsigned i = 0; i < bodySize - 1; i++) {
                *out << ind << "AuxiliaryMap* auxMap = auxiliaryMaps[" << i << "];\n";

                //create key
                *out << ind << "vector<unsigned> key_" << i << ";\n";

                *out << ind++ << "for(map<unsigned, pair<unsigned, unsigned> >::iterator it = keys_indices[" << i << "].begin();it != keys_indices[" << i << "].end();it++) {\n";
                *out << ind << "key_" << i << ".push_back((*tupleStack[joinOrder[it->second.first]])[it->second.second]);\n";
                *out << --ind << "}\n";

                /*
                 *out << ind << "cout<<\"attempting join in rule with key\"<<endl;\n";
                 *out << ind++ << "for(unsigned v:key_"<<i<<") {\n";
                 *out << ind << "cout<<ConstantsManager::getInstance().unmapConstant(v)<<\" \";\n";
                 *out << --ind << "}\n";
                 *out << ind << "cout<<endl;\n"; 
                 */

                *out << ind++ << "if(!negativeLiterals[joinOrder[" << i + 1 << "]] || !wQiNeg[joinOrder[" << i + 1 << "]]->contains(key_" << i << ")){\n";
                *out << ind << "vector<vector<unsigned> >* matchingTuples_" << i << "=auxMap->getValues(key_" << i << ");\n";

                *out << ind++ << "if((matchingTuples_" << i << "!=NULL && !negativeLiterals[joinOrder[" << i + 1 << "]]) || (matchingTuples_" << i << "==NULL && negativeLiterals[joinOrder[" << i + 1 << "]])) {\n";

                *out << ind << "bool negative" << i << " = (matchingTuples_" << i << " == NULL);\n";
                //*out << ind << "cout<<\"join found\"<<endl;\n";
                *out << ind++ << "if(negative" << i << ") {\n";
                //*out << ind << "cout<<\"join with negative literal\"<<endl;\n";
                *out << ind << "matchingTuples_" << i << " = new vector<vector<unsigned> >(1);\n";
                *out << --ind << "}\n";
                *out << ind++ << "for(const vector<unsigned> & tuple" << i + 1 << ": *matchingTuples_" << i << ") {\n";

                /*
                 *out << ind << "cout<<\"joining with \"<<endl;\n";
                 *out << ind++ << "for(unsigned v:tuple"<<i+1<<") {\n";
                 *out << ind << "cout<<ConstantsManager::getInstance().unmapConstant(v)<<\" \";\n";
                 *out << --ind << "}\n";
                 *out << ind << "cout<<endl;\n";
                 */

                *out << ind << "tupleStack[joinOrder[" << i + 1 << "]]=&tuple" << i + 1 << ";\n";

                if (i == bodySize - 2) {
                    //CHECK INEQUALITIES
                    checkInequalities(rule, ind);
                    if (rule.getInequalities().size()) {
                        ind++;
                        *out << ") {\n";
                    }
                    if (headSize == 0) {
                        *out << ind << "vector<aspc::Literal>failedConstraint;\n";
                        *out << ind++ << "for(unsigned i=0;i<" << bodySize << ";i++) {\n";
                        *out << ind << "const vector<unsigned>* tuple=tupleStack[joinOrder[i]];\n";
                        //*out << ind++ << "if(i==0) {\n";
                        //*out << ind << "cout<<ruleBodyPredicates[" << l << "]<<\" \";\n";
                        //*out << --ind << "}\n";
                        //*out << ind++ << "else {\n";
                        //*out << ind << "cout<<ruleBodyPredicates[joinOrder[i-1]]<<\" \";\n";
                        //*out << --ind << "}\n";
                        *out << ind << "vector<string> terms;\n";

                        *out << ind++ << "if(!tuple->empty()) {\n";
                        *out << ind++ << "for(unsigned v:*tuple) {\n";
                        *out << ind << "terms.push_back(ConstantsManager::getInstance().unmapConstant(v));\n";
                        *out << --ind << "}\n";
                        *out << --ind << "}\n";
                        *out << ind++ << "else {\n";
                        *out << ind << "terms.resize(bodyArieties[joinOrder[i]]);\n";
                        *out << ind++ << "for(map<unsigned, string>::const_iterator it=bodyConstantsOfNegLit[joinOrder[i]].begin();it!=bodyConstantsOfNegLit[joinOrder[i]].end();it++) {\n";
                        *out << ind << "terms[it->first]=it->second;\n";
                        *out << --ind << "}\n";
                        //TODO assumptions rules are of the type head:-negBody, posBody
                        //*out << ind++ << "for (int k = 0; k < keys_indices.size(); k++) {\n";
                        //*out << ind++ << "if (negativeLiterals[joinOrder[k]] == true) {\n";
                        *out << ind++ << "for (std::map<unsigned, pair<unsigned, unsigned> >::iterator it = keys_indices[i-1].begin(); it != keys_indices[i-1].end(); it++) {\n";
                        *out << ind << "terms[it->first]=ConstantsManager::getInstance().unmapConstant((*tupleStack[joinOrder[it->second.first]])[it->second.second]);\n";
                        *out << --ind << "}\n";
                        //* out << --ind << "}\n";
                        //*out << --ind << "}\n";
                        *out << --ind << "}\n";
                        //*out << ind << "cout<<\"\\n\";\n";

                        *out << ind << "aspc::Atom toAddAtom(ruleBodyPredicates[joinOrder[i]], terms);\n";
                        *out << ind << "aspc::Literal toAddLiteral(negativeLiterals[joinOrder[i]], toAddAtom);\n";
                        *out << ind << "failedConstraint.push_back(toAddLiteral);\n";

                        //*out << ind++ << "for(int i=0;i<terms->size();i++) {\n";
                        //*out << ind << "cout<<(*terms)[i]<<\" \";\n";
                        //*out << --ind << "}\n";
                        //*out << ind << "cout<<\"\\n\";\n";
                        *out << --ind << "}\n";

                        //*out << ind << "cout<<\"Join succeded, constraint violated\\n\";\n";
                        *out << ind << "failedConstraints.push_back(failedConstraint);\n";
                    } else {
                        *out << ind << "vector<unsigned> head(" << rule.getHead().front().getAriety() << ");\n";
                        setHeadVariables(ind, rule);
                        //save head constants
                        const aspc::Atom & headAtom = rule.getHead().front();
                        for (unsigned i = 0; i < headAtom.getTerms().size(); i++) {
                            if (!headAtom.isVariableTermAt(i)) {
                                *out << ind << "head[" << i << "] = " << headAtom.getTermAt(i) << ";\n";
                            }
                        }
                        //*out << ind << "cout<<\"bottom\\n\";\n";
                        *out << ind++ << "if(wqi!=NULL && !rqi->contains(head)) {\n";

                        //*out << ind++ << "for(int i=0;i<head.size();i++){\n";
                        //*out << ind << "cout<<head[i]<<\"\\n\";\n";
                        //*out << --ind << "}\n";
                        *out << ind << "wqi->insert(head);\n";
                        *out << --ind << "}\n";
                        *out << ind++ << "else if(wqi==NULL) {\n";
                        *out << ind << "rqi->insert(head);\n";
                        *out << --ind << "}\n";
                    }
                    if (rule.getInequalities().size()) {
                        *out << --ind << "}\n";
                    }
                }


            }
            for (unsigned i = 0; i < rule.getBodySize() - 1; i++) {
                *out << --ind << "}\n";

                *out << ind++ << "if(negative" << rule.getBodySize() - 2 - i << ") {\n";
                *out << ind << "delete matchingTuples_" << rule.getBodySize() - 2 - i << ";\n";
                *out << --ind << "}\n";

                *out << --ind << "}\n";

                *out << --ind << "}\n";
            }
            //*out << ind << "cout<<endl;\n";
            *out << --ind << "}\n\n";
        }
    }


    //handle arieties
    *out << ind++ << "void Executor::executeFromFile(const char* filename) {\n";
    *out << ind << "DLV2::InputDirector director;\n";
    *out << ind << "AspCore2InstanceBuilder* builder = new AspCore2InstanceBuilder();\n";
    *out << ind << "director.configureBuilder(builder);\n";
    *out << ind << "vector<const char*> fileNames;\n";
    *out << ind << "fileNames.push_back(filename);\n";
    *out << ind << "director.parse(fileNames);\n";
    *out << ind << "executeProgramOnFacts(builder->getProblemInstance());\n";
    *out << ind << "delete builder;\n";
    *out << --ind << "}\n";

    *out << ind++ << "void Executor::executeProgramOnFacts(const vector<aspc::Atom> & program) {\n";
    /*
     *out << ind << "cout<<\"Program facts size inside dynamic library\"<<program.size()<<endl;\n";
     *out << ind << "cout<<\"Printing facts inside dynamic library\"<<endl;\n";
     *out << ind++ << "for(const aspc::Atom & atom:program) {\n";

     *out << ind << "atom.print();\n";
     *out << ind << "cout<<endl;\n";
     *out << --ind << "}\n";
     */


    //*out << ind << "cout<<\"program facts size inside dynamic library\"<<program.getFactsSize()<<endl;\n";
    *out << ind << "failedConstraints.erase(failedConstraints.begin(), failedConstraints.end());\n";
    //handle join orders, it is done within execution so they could be changed
    *out << ind << "vector<vector<vector<unsigned> > > joinOrders(" << program.getRulesSize() << ");\n";
    vector<vector<vector<unsigned> > > joinOrders(program.getRulesSize());
    for (unsigned r = 0; r < program.getRulesSize(); r++) {
        unsigned bodySize = program.getRule(r).getBodySize();
        joinOrders[r].resize(bodySize);
        *out << ind << "joinOrders[" << r << "].resize(" << bodySize << ");\n";
        for (unsigned i = 0; i < bodySize; i++) {
            joinOrders[r][i].push_back(i);
            *out << ind << "joinOrders[" << r << "][" << i << "].push_back(" << i << ");\n";
            for (int j = bodySize - 1; j >= 0; j--) {
                //for (int j = 0; j < bodySize; j++) {
                if (i != (unsigned) j) {
                    joinOrders[r][i].push_back(j);
                    *out << ind << "joinOrders[" << r << "][" << i << "].push_back(" << j << ");\n";
                }
            }
        }
    }
    set<string> headPredicates;
    list<string> bodyPredicatesList;
    set<string> allPredicates;

    for (unsigned i = 0; i < program.getRulesSize(); i++) {
        const vector<aspc::Atom> & head = program.getRule(i).getHead();
        for (const aspc::Atom & atom : head) {
            headPredicates.insert(atom.getPredicateName());
            allPredicates.insert(atom.getPredicateName());
        }
        const vector<aspc::Literal> & body = program.getRule(i).getBody();
        for (const aspc::Literal & literal : body) {
            if (!bodyPredicates.count(literal.getPredicateName())) {
                bodyPredicatesList.push_back(literal.getPredicateName());
            }
            bodyPredicates.insert(literal.getPredicateName());
            allPredicates.insert(literal.getPredicateName());
        }
    }


    const map<string, unsigned> & arietyMap = builder->getArietyMap();
    //TODO use predicates enumeration instead of names

    //data structure init
    *out << ind << "map<string, WQi*> wQiMap;\n";
    *out << ind << "map<string, RQi*> rQiMap;\n";

    for (string predicate : bodyPredicates) {
        unsigned ariety = arietyMap.at(predicate);
        *out << ind << "WQi " << "w" << predicate << "(" << ariety << ", " << ariety << ")" << ";\n";
        *out << ind << "wQiMap[\"" << predicate << "\"] = &w" << predicate << ";\n";
    }

    //TODO Executor not able to see WQi.cpp
    for (string predicate : allPredicates) {
        unsigned ariety = arietyMap.at(predicate);
        *out << ind << "RQi " << "r" << predicate << "(" << ariety << ", " << ariety << ")" << ";\n";
        *out << ind << "rQiMap[\"" << predicate << "\"] = &r" << predicate << ";\n";
    }


    //map<pair<string, vector<int> >, tuple<int, int, int> > usedMaps;
    for (unsigned i = 0; i < program.getRulesSize(); i++) {
        unsigned ruleId = program.getRule(i).getRuleId();
        const vector<aspc::Literal> & body = program.getRule(i).getBody();

        if (program.getRule(i).getType() == TYPE_4) {

            *out << ind << "string ruleBodyPredicates_" << ruleId << "[] = {";
            for (unsigned j = 0; j < body.size(); j++) {
                *out << "\"" << body[j].getPredicateName() << "\"";
                if (j < program.getRule(i).getBody().size() - 1) {
                    *out << ", ";
                }
            }
            *out << "};\n";

            *out << ind << "vector<bool> negativeLiterals_" << ruleId << ";\n";
            *out << ind << "vector<unsigned> bodyArieties_" << ruleId << ";\n";
            *out << ind << "vector<map<unsigned,string> > bodyConstantsOfNegLit_" << ruleId << "(" << body.size() << ");\n";

            for (unsigned j = 0; j < program.getRule(i).getBodySize(); j++) {
                if (body[j].isNegated()) {
                    for (unsigned k = 0; k < body[j].getAriety(); k++) {
                        if (!body[j].isVariableTermAt(k)) {
                            *out << ind << "bodyConstantsOfNegLit_" << ruleId << "[" << j << "][" << k << "]=\"" << body[j].getTermAt(k) << "\";\n";
                        }
                    }
                    *out << ind << "negativeLiterals_" << ruleId << ".push_back(true);\n";
                } else {
                    *out << ind << "negativeLiterals_" << ruleId << ".push_back(false);\n";
                }
                *out << ind << "bodyArieties_" << ruleId << ".push_back(" << program.getRule(i).getBody()[j].getAriety() << ");\n";

            }
            const vector<aspc::Literal> & body = program.getRule(i).getBody();
            for (unsigned c = 0; c < body.size(); c++) {
                //TODO Adjust domainArieties size or introduce keyIndices in aux maps
                //TODO don't forget to test on reachability
                *out << ind << "AuxiliaryMap* auxMaps_" << i << "_" << c << "[" << body.size() - 1 << "];\n";
            }

            //*out << ind << "AuxiliaryMap* auxMaps_" << program->getRule(i)->getRuleId() << "[" << body->size() << "][" << body->size() - 1 << "];\n";

            *out << ind << "vector<vector<AuxiliaryMap*> > auxMapsByAtom_" << ruleId << "(" << body.size() << ");\n";
            for (unsigned j = 0; j < body.size(); j++) {
                string ruleAndAtomSuffix = "_" + to_string(i) + "_" + to_string(j);
                //joinIndices_ruleId_atom1 -> vector<map<int, pair<int, in> > >: 
                // vector -> joining atom2 according to joinOrder
                // map -> key = term in atom2
                // map -> value -> pair_int1 = index of atom 3, pair_int2 = term of atom 3 joining term in atom2
                vector<map<unsigned, pair<unsigned, unsigned> > > joinIndices = program.getRule(i).getJoinIndicesWithJoinOrder(joinOrders[i][j]);
                *out << ind << "vector<map<unsigned, pair<unsigned,unsigned> > > joinIndices_" << ruleId << "_" << j << "(" << body.size() - 1 << ");\n";
                for (unsigned k = 0; k < body.size() - 1; k++) {
                    //aux maps are stored according to join ordering
                    unsigned joinIndex = joinOrders[ruleId][j][k + 1];
                    //int joinKeyLength = program->getRule(i)->getBody()->at(joinIndex)->getAriety();
                    unsigned joinKeyLength = joinIndices[k].size();
                    //get all joining variables indices
                    for (map<unsigned, pair<unsigned, unsigned> >::iterator it = joinIndices[k].begin(); it != joinIndices[k].end(); it++) {
                        *out << ind << "pair<unsigned, unsigned> value_" << ruleId << "_" << j << "_" << k << "_" << it->first << "(" << it->second.first << ", " << it->second.second << ");\n";
                        *out << ind << "joinIndices_" << ruleId << "_" << j << "[" << k << "][" << it->first << "]=value_" << ruleId << "_" << j << "_" << k << "_" << it->first << ";\n";
                    }
                    //TODO substitute predicateAriety with effective key size
                    *out << ind << "vector<unsigned> keyIndices" << ruleAndAtomSuffix << "_" << k << ";\n";

                    //vector<int> usedAuxMapKeyVector;
                    for (map<unsigned, pair<unsigned, unsigned> >::iterator it = joinIndices[k].begin(); it != joinIndices[k].end(); it++) {
                        //*out << ind++ << "for(int i=0; i<" << joinKeyLength << "; i++){\n";
                        *out << ind << "keyIndices" << ruleAndAtomSuffix << "_" << k << ".push_back(" << it->first << ");\n";
                        //*out << --ind << "}\n";
                        //usedAuxMapKeyVector.push_back(it->first);
                    }

                    //pair<string, vector<int> > usedAuxMapKey(body[k].getPredicateName(), usedAuxMapKeyVector);
                    //if (usedMaps.count(usedAuxMapKey)) {
                    //  const tuple<int, int, int> & value = usedMaps[usedAuxMapKey];
                    //*out << ind << "AuxiliaryMap* toAddMap" << ruleAndAtomSuffix << "_" << k << " = auxMaps[" << get<0>(value)<<"]["  << get<1>(value)<<"]["  << get<2>(value)<<"];\n";
                    //} else {
                    *out << ind << "AuxiliaryMap toAddMap" << ruleAndAtomSuffix << "_" << k << "(" << joinKeyLength << ", &keyIndices" << ruleAndAtomSuffix << "_" << k << ");\n";
                    *out << ind << "auxMapsByAtom_" << ruleId << "[" << joinIndex << "].push_back(&toAddMap" << ruleAndAtomSuffix << "_" << k << ");\n";
                    //  tuple<int, int, int> value(ruleId, j, k);
                    // usedMaps[usedAuxMapKey] = value;
                    //}
                    *out << ind << "auxMaps_" << ruleId << "_" << j << "[" << k << "]= &toAddMap" << ruleAndAtomSuffix << "_" << k << ";\n";
                    //joining atom at position joinOrders[k] with all atoms preceding it

                }
            }
        }
    }

    //feed facts
    //*out << ind << "cout<<\"facts\\n\";\n";
    *out << ind++ << "for(unsigned i=0;i<program.size();i++) {\n";
    //*out << ind << "cout<<i<<\"\\n\";\n";
    *out << ind++ << "if(!wQiMap.count(program[i].getPredicateName())) {\n";
    if (!headPredicates.empty()) {
        *out << ind << "program[i].print();\n";
        *out << ind << "cout<<\".\\n\";\n";
    }
    *out << --ind << "}\n";

    /**out << ind++ << "if(!wQiMap.count(program->getFact(i)->getPredicateName())) {\n";
     *out << ind++ << "if(!rQiMap.count(program->getFact(i)->getPredicateName())) {\n";
     *out << ind << "rQiMap[program->getFact(i)->getPredicateName()] = new RQi(program->getFact(i)->getAriety(),program->getFact(i)->getAriety());\n";
     *out << --ind << "}\n";
     *out << ind << "rQiMap[program->getFact(i)->getPredicateName()]->insert(tuple);\n";
     *out << --ind << "}\n";
     */
    *out << ind++ << "else {\n";
    *out << ind << "vector<unsigned> tuple = program[i].getIntTuple();\n";
    *out << ind << "wQiMap[program[i].getPredicateName()]->insert(tuple);\n";

    /*
     *out << ind << "cout<<\"Inserting tuple in working set of \"<<program[i].getPredicateName()<<endl;\n";
     *out << ind++ << "for(unsigned v:tuple) {\n";
     *out << ind << "cout<<ConstantsManager::getInstance().unmapConstant(v)<<\" \";\n";
     *out << --ind << "}\n";
     *out << ind << "cout<<endl<<endl;\n";
     */

    //*out << ind++ << "for(int j=0;j<tuple.size();j++) {\n";
    //*out << ind << "cout<<tuple[j]<<endl;\n";
    //*out << --ind << "}\n";
    //*out << ind << "cout<<wQiMap[program->getFact(i)->getPredicateName()]->getTupleCount()<<endl;\n";
    //*out << ind << "cout<<\"done\"<<endl;\n";


    *out << --ind << "}\n";
    *out << --ind << "}\n";
    //*out << ind << "cout<<\"facts done\"<<endl;\n";

    for (unsigned i = 0; i < program.getRulesSize(); i++) {
        //if(program.getRule(i).containsNegation()) {
        *out << ind << "WQi* negWS_" << i << "[" << program.getRule(i).getBodySize() << "];\n";
        for (unsigned j = 0; j < program.getRule(i).getBodySize(); j++) {
            if (program.getRule(i).getBody()[j].isNegated()) {

                *out << ind << "negWS_" << i << "[" << j << "]=wQiMap[\"" << program.getRule(i).getBody()[j].getPredicateName() << "\"];\n";
            }
        }
        //}
    }
    //*out << ind << "cout<<\"done\\n\";\n";


    //main loop
    const map<RuleType, map<string, set<unsigned> > >& rulesByType = program.getRulesByType();
    if (bodyPredicatesList.size()) {
        *out << ind << "while(";
        list<string>::iterator it = bodyPredicatesList.begin();
        *out << "w" << *it << ".size()";
        it++;
        for (; it != bodyPredicatesList.end(); it++) {
            *out << " || w" << *it << ".size()";
        }
        *out << ") {\n";
        ind++;

        for (it = bodyPredicatesList.begin(); it != bodyPredicatesList.end(); it++) {
            *out << ind++ << "while(w" << *it << ".size()) {\n";
            //*out << ind<<"cout<<w"<<*it<<".size()<<\"\\n\";\n";

            //while we have more tuples in WQi
            //*out << tab << tab << "vector<int> tuple(" << (*arietyMap)[*it] << ");\n";
            *out << ind << "vector<unsigned> tuple;\n";

            *out << ind << "tuple.reserve(" << arietyMap.at(*it) << ");\n";

            //*out << ind << "w" << *it << ".pop(tuple);\n";
            *out << ind << "w" << *it << ".next(tuple);\n";
            /*
             *out << ind << "cout<<\"Extracting tuple from working set of \"<<\"" << *it << "\"<<endl;\n";
             *out << ind++ << "for(unsigned v:tuple) {\n";
             *out << ind << "cout<<ConstantsManager::getInstance().unmapConstant(v)<<\" \";\n";
             *out << --ind << "}\n";
             *out << ind << "cout<<endl<<endl;\n";
             */
            //remove tuple from WQi
            *out << ind << "w" << *it << ".remove(tuple);\n";

            //add it to RQi
            //TODO fix it.
            if (!headPredicates.empty()) {
                *out << ind << "r" << *it << ".insert(tuple);\n";
            }


            //match rules?
            if (rulesByType.count(TYPE_1) && rulesByType.at(TYPE_1).count(*it)) {
                const set<unsigned> & matchingRules1 = program.getRulesByTypeAndPredicateName(*it, TYPE_1);
                //TODO wildcards
                if (!matchingRules1.empty()) {
                    for (set<unsigned>::const_iterator it = matchingRules1.begin(); it != matchingRules1.end(); it++) {
                        const aspc::Rule & rule = program.getRule(*it);
                        //match constants
                        const aspc::Atom & atom = rule.getBody().front().getAtom();
                        bool constantMatchRequired = matchConstants(rule, atom, ind);


                        //if we have to match something close the if 
                        if (constantMatchRequired) {
                            ind++;
                            *out << ") {\n";
                        }

                        //now that constants are matched.. create the head tuple

                        generateHeadTupleAndMatchConstants(rule, ind, bodyPredicates);

                        if (constantMatchRequired) {
                            *out << --ind << "}\n";
                        }
                    }
                }
            }
            if (rulesByType.count(TYPE_4) && rulesByType.at(TYPE_4).count(*it)) {
                const set<unsigned> & matchingRules4 = program.getRulesByTypeAndPredicateName(*it, TYPE_4);

                if (!matchingRules4.empty()) {
                    for (set<unsigned>::const_iterator it2 = matchingRules4.begin(); it2 != matchingRules4.end(); it2++) {
                        //match constants
                        const aspc::Rule & rule = program.getRule(*it2);
                        unsigned ruleId = rule.getRuleId();
                        //TODO disjunction
                        string headPredicate = "";
                        if (rule.getHead().size())
                            headPredicate = rule.getHead().front().getPredicateName();

                        string rQi = "NULL";
                        string wQi = "NULL";
                        if (bodyPredicates.count(headPredicate)) {
                            wQi = "&w" + headPredicate;
                        }
                        if (rule.getHead().size()) {
                            rQi = "&r" + headPredicate;

                        }

                        string ruleBodyPredicates = "NULL";
                        string constraintStructures = "";
                        if (rule.getHead().empty()) {
                            ruleBodyPredicates = "ruleBodyPredicates_" + std::to_string(ruleId);
                            constraintStructures = ", " + ruleBodyPredicates + ", failedConstraints, bodyArieties_" + std::to_string(ruleId) + ", bodyConstantsOfNegLit_" + std::to_string(ruleId);

                        }
                        for (unsigned l = 0; l < rule.getBody().size(); l++) {

                            if (rule.getBody()[l].getPredicateName() == *it) {
                                bool constantMatchRequired = matchConstants(rule, rule.getBody()[l].getAtom(), ind);
                                //if we have to match something close the if 
                                if (constantMatchRequired) {
                                    ind++;
                                    *out << ") {\n";
                                }
                                *out << ind << "performJoin_" << ruleId << "(" << l << ", " << wQi << ", " << rQi << ", joinOrders[" << ruleId << "][" << l << "], tuple, auxMaps_" << ruleId << "_" << l << ", joinIndices_" << ruleId << "_" << l << ", auxMapsByAtom_" << ruleId << "[" << l << "], negativeLiterals_" << ruleId << constraintStructures << ", negWS_" << ruleId << ");\n";
                                if (constantMatchRequired) {
                                    *out << --ind << "}\n";
                                }
                            }
                        }
                    }
                }
            }
            *out << --ind << "}\n";
        }

        *out << --ind << "}\n";
    }
    //DECOMMENT TO PRINT THE MODEL

    //don't print the model if the invocation is on constraints
    if (!headPredicates.empty()) {
        *out << ind++ << "for (map<string, RQi*>::iterator it = rQiMap.begin(); it != rQiMap.end(); it++) {\n";
        *out << ind << "it->second->printTuples(it->first);\n";
        *out << --ind << "}\n";

    }
    *out << --ind << "}\n";



}
