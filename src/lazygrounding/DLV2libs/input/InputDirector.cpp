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

#include <iostream>
#include <string>
#include <cstring>
#include "InputDirector.h"
#include "../util/Assert.h"
#include "../util/ErrorMessage.h"
#include "aspcore2_lexer.hpp"
#include "aspcore2_parser.hpp"

using namespace std;
using namespace DLV2;

//////////////////////////////////////////////////////////////////////////////
// Some LEX/FLEX dependencies

extern "C" int yywrap() {
    return 1;
} // End-of-file handler for LEX
extern FILE* yyin; // Where LEX reads its input from

int
yyerror(
        InputDirector& director,
        const char* msg) {
    return director.onError(msg);
}

void
InputDirector::configureBuilder(
        InputBuilder* b) {
    assert_msg(b != NULL, "The input builder is null.");
    builder = b;
}

// Set a new ParserConstraint after deleting the current one. 
// The director acquires the ownership of the input ParserConstraint object, 
// and, as owner, it is deleting its ParserConstraint in the destructor.

void
InputDirector::setParserConstraint(
        ParserConstraint* c) {
    assert_msg(c != NULL, "The parser controller is null.");
    if (parserConstraint)
        delete parserConstraint;
    parserConstraint = c;
}

InputDirector::InputDirector() :
parserLine(0),
parserFile(NULL),
parserErrors(0),
parserStateInternal(false),
builder(NULL),
parserConstraint(new ParserConstraint()) {

}

InputDirector::~InputDirector() {
    if (parserConstraint)
        delete parserConstraint;
}

int
InputDirector::parse(
        vector<const char*> files) {
    if (files.size() == 0)
        ErrorMessage::errorGeneric("No input files.");
    else if (files.size() == 1 && !strcmp(files[0], "--"))
        return InputDirector::parse();

    for (unsigned i = 0; i < files.size(); i++) {
        FILE *file = fopen(files[i], "r");
        if (file) {
            int res = parse(files[i], file);
            fclose(file);
            if (res != 0)
                return res;
        } else {
            yyerror(*this, "Can't open input");
            return -1;
        }
    }
    return 0;
}

int
InputDirector::parse(
        int filesSize,
        const char **files) {
    for (int i = 1; i < filesSize; i++) {
        FILE *file = fopen(files[i], "r");
        if (file) {
            int res = parse(files[i], file);
            fclose(file);
            if (res != 0)
                return res;
        } else {
            yyerror(*this, "Can't open input");
            return -1;
        }
    }
    return 0;
}

int
InputDirector::parse(
        const char* fileName,
        FILE *file) {
    yyin = file;
    parserFile = fileName;
    parserLine = 1;
    yyparse(*this);

    if (parserErrors > 0) {
        cerr << "Aborting due to parser errors." << endl;
        return -1;
    }
    return 0;
}

int
InputDirector::parse() {
    yyin = stdin;
    parserFile = "STDIN";
    parserLine = 1;
    yyparse(*this);

    if (parserErrors > 0) {
        cerr << "Aborting due to parser errors." << endl;
        return -1;
    }
    return 0;
}

int
InputDirector::onError(
        const char* msg) // Error handler for YACC
{
    parserErrors++;

    if (!parserStateInternal) {
        if (parserFile && strlen(parserFile) > 0)
            cerr << parserFile << ": ";
        cerr << "line " << parserLine;

        cerr << ": ";
    } else
        cerr << "Internal parser invocation: ";

    cerr << msg << "." << endl;

    return 0;
}

InputBuilder* InputDirector::getBuilder() {
    return builder;
}

ParserConstraint* InputDirector::getParserConstraint() {
    return parserConstraint;
}

void InputDirector::onNewLine() {
    parserLine++;
}
