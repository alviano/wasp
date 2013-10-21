/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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
 * File:   NoRestartsStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 7 August 2013, 9.33
 */

#ifndef NORESTARTSSTRATEGY_H
#define	NORESTARTSSTRATEGY_H

#include "RestartsStrategy.h"

class NoRestartsStrategy : public RestartsStrategy
{
    public:
        inline NoRestartsStrategy() {}
        virtual bool onLearningClause() { return false; }
        virtual void onLearningUnaryClause() {}
};

#endif	/* NORESTARTSSTRATEGY_H */

