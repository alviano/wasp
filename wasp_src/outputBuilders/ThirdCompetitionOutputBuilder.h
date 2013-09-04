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
 * File:   ThirdCompetitionOutputBuilder.h
 * Author: Carmine Dodaro
 *
 * Created on 04 September 2013, 12.47
 */

#ifndef THIRDCOMPETITIONOUTPUTBUILDER_H
#define THIRDCOMPETITIONOUTPUTBUILDER_H

#include "OutputBuilder.h"

class ThirdCompetitionOutputBuilder : public OutputBuilder
{
	public:
        inline ThirdCompetitionOutputBuilder();
        virtual void startModel();
        virtual void printLiteral( PositiveLiteral* );
        virtual void endModel();
        virtual void onProgramIncoherent();
};

ThirdCompetitionOutputBuilder::ThirdCompetitionOutputBuilder()
{
}

#endif /* THIRDCOMPETITIONOUTPUTBUILDER_H */
