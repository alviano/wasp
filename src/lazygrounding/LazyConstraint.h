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
 * File:   LazyConstraint.h
 * Author: bernardo
 *
 * Created on December 31, 2016, 3:59 PM
 */

#ifndef LAZYCONSTRAINT_H
#define LAZYCONSTRAINT_H
#include <string>
#include <vector>
class LazyConstraint {
public:   
    virtual ~LazyConstraint(){};
    virtual void setFilename(const std::string & filename)=0;
    virtual void onAtoms(const std::vector<std::string>& atoms, std::vector<unsigned> & watchedAtoms)=0;
    virtual bool onAnswerSet(const std::vector<unsigned int> & interpretation)=0;
    virtual void onCheckFail(std::vector<std::vector<int> >& constraints)=0;
    
};

#endif /* LAZYCONSTRAINT_H */

