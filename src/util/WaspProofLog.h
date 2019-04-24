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

#ifndef WASP_PROOF_LOG_H
#define WASP_PROOF_LOG_H

#include <cstdio>
#include <vector>

#ifndef PROOF_LOG
    #define proof_log( msg, ... )
    #define proof_block( blck )
#else
    FILE* getLogFile();
    #define proof_log( msg, ... ) \
        fprintf( getLogFile(), msg, ##__VA_ARGS__ );
    #define proof_block( block ) \
        { block }
#endif


namespace proof {

bool setLogFile( const char* filename );
void bodyDefinition( int id );
void addition( const std::vector<int>& lits );
void completionRuleAddition();
void completionSupportAddition();
void extension( int var, const std::vector<int>& lits );
void deletion( const std::vector<int>& lits );
void loop( const std::vector<unsigned>& unfounded );
void unfoundedSetAddition();

}

#endif
