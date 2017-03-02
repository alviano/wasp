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
 * File:   Constants.h
 * Author: cesco
 *
 * Created on 4 aprile 2014, 16.29
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <set>

namespace DLV2
{
// Input builder constants
#define BUILDER_MOCK_OBJECTS 0
#define BUILDER_DLV_DB 1
#define BUILDER_IN_MEMORY 2
#define BUILDER_SELECTOR 3
#define BUILDER_DEPGRAPH 4
#define BUILDER_EMPTY 5
    
// Output builder constants
#define OUTPUT_ASPCORE2 0
#define OUTPUT_SILENT 1
    
// Dependency graph constants
#define POSITIVE_EDGE 0
#define NEGATIVE_EDGE 1
    
// DLV2 constants
#define DLV2_STRING "DLV 2.0\n"
    
// Error messages
#define ERROR_PARSING "\nError during parsing"
#define ERROR_GENERIC "\nGeneric error"
#define ERROR_DB "\nDatabase error"
#define ERROR_PARSING_CODE 100
#define ERROR_GENERIC_CODE 110
#define ERROR_DB_CODE 120
    
// New types
typedef unsigned int INPUT_BUILDER_POLICY;
typedef unsigned int OUTPUT_POLICY;

namespace DB{
    
    // Database constants
    #define TABLE_NAMES "table_names"
    #define EXECUTION_TIMESTAMP "execution_timestamp"
    #define PREDICATE_TIMESTAMP "predicate_timestamp"
    
    // New types
    typedef size_t index_t;
    typedef std::set< unsigned > DBRuleSet;
    typedef int64_t Timestamp;
};

};

#endif	/* CONSTANTS_H */

