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
 * File:   ProgramBuilder.h
 * Author: Carmine Dodaro
 *
 * Created on 23 July 2013, 13.39
 */

#ifndef PROGRAMBUILDER_H
#define	PROGRAMBUILDER_H

#include "AbstractBuilder.h"
class Solver;

class ProgramBuilder : public AbstractBuilder
{
    public:        
        ProgramBuilder( Solver* s );
        virtual void newVar();
        virtual void newVar( const string& name );
        
        virtual Clause* startClause();
        virtual Clause* startClause( unsigned int size );
        virtual void addLiteralInClause( int lit, Clause* clause );
        virtual void endClause( Clause* clause );
        
        virtual void startBuilding();
        virtual void endBuilding();
        
    private:
        Solver* solver;
};

#endif	/* PROGRAMBUILDER_H */

