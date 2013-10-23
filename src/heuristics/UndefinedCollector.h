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

/* 
 * File:   UndefinedCollector.h
 * Author: Carmine Dodaro
 *
 * Created on 03 September 2013, 16.50
 */

#ifndef UNDEFINEDCOLLECTOR_H
#define	UNDEFINEDCOLLECTOR_H

class Clause;
class Variable;

class UndefinedCollector
{
    public:
        inline UndefinedCollector() {}
        virtual ~UndefinedCollector() {}
        virtual void collectUndefined( Variable* ) = 0;
};

#endif	/* UNDEFINEDCOLLECTOR_H */
