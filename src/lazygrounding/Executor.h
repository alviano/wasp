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

#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <map>
#include "WQi.h"
#include "RQi.h"
#include "AuxiliaryMap.h"
#include "Literal.h"
#include <iostream>
class Executor
{
public:
  Executor();
  /* use virtual otherwise linker will try to perform static linkage */
  virtual ~Executor();
  virtual void executeProgramOnFacts(const vector<aspc::Atom> & p);
  virtual void executeFromFile(const char* factsFile);
  virtual const std::vector<std::vector<aspc::Literal> > & getFailedConstraints() {
      return failedConstraints;
  }
  virtual const std::vector<std::string> getBodyLiterals() {
      return bodyLiterals;
  }
private:
  std::vector<std::vector<aspc::Literal> > failedConstraints;
  std::vector<std::string> bodyLiterals;

};

#endif
