/*
 *
 *  Copyright 2014 Mario Alviano, Carmine Dodaro, Francesco Ricca and
 *                 Pierfrancesco Veltri.
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
 * File:   InputDirector.h
 * Author: cesco
 *
 * Created on 17 marzo 2014, 13.10
 */

#ifndef INPUTDIRECTOR_H
#define INPUTDIRECTOR_H

#include "InputBuilder.h"
#include "ParserConstraintAspCore2Strict.h"
#include <vector>
#include <cstdio>

namespace DLV2
{
    
    class InputDirector {
    public:

        InputDirector();
        ~InputDirector();

        void configureBuilder( InputBuilder* );
        InputBuilder* getBuilder();
        void setParserConstraint( ParserConstraint* );
        ParserConstraint* getParserConstraint();

        //////////////////////////////////////////////////////////////////////////////
        // Parsing functions
        int parse( std::vector<const char*> files);
        int parse( int filesSize, const char **files );
        int parse( const char* filename, FILE *file );
        int parse( );

        int onError(const char* msg);
        void onNewLine();


    private:

        int parserLine;
        const char* parserFile;
        unsigned parserErrors;
        bool parserStateInternal;
        InputBuilder* builder;
        ParserConstraint* parserConstraint;
    };
    
};

//////////////////////////////////////////////////////////////////////////////
// Bug Fixes for Various Environments

// BUG-FIX: bison[1.24] fails to prototype its interfaces yylex() and yyerror().
extern "C" int yylex(DLV2::InputDirector& director);
extern "C" int yyerror(DLV2::InputDirector&, const char*);

#endif	/* INPUTDIRECTOR_H */

