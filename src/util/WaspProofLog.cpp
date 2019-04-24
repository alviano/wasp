#include "WaspProofLog.h"

#include <cassert>

namespace proof {

#ifndef PROOF_LOG

bool setLogFile( const char* ) {}
void bodyDefinition( int ) {}
void addition( const std::vector<int>& ) {}
void completionRuleAddition() {}
void completionSupportAddition() {}
void extension( int, const std::vector<int>& ) {}
void deletion( const std::vector<int>& ) {}
void loop() {}
void unfoundedSetAddition() {}

#else

#define log( msg, ... ) proof_log( msg, ##__VA_ARGS__ )

static FILE* log_file = stderr;

bool setLogFile( const char* filename ) {
    return true;
}

void bodyDefinition( int id ) {
    log( "b %d", id );
    log( " 0\n" );
}

void addition( const std::vector<int>& lits ) {
    log( "a" );
    for(auto lit : lits) log( " %d", -lit );
    log( " 0\n" );
}

void completionRuleAddition() {

}

void completionSupportAddition() {

}

void extension( int var, const std::vector<int>& lits ) {
    assert( var > 0 );
    log( "e %d", var );
    for(auto lit : lits) log( " %d", -lit );
    log( " 0\n" );
}

void deletion( const std::vector<int>& lits ) {
    log( "d" );
    for(auto lit : lits) log( " %d", -lit );
    log( "0\n" );
}

void loop( const std::vector<unsigned>& unfounded ) {
    for(unsigned int i = 0; i < unfounded.size(); i++) {
        log("l %d", unfounded[i]);
        for(unsigned int j = 0; j < unfounded.size(); j++) {
            if(i == j) continue;
            log(" %d", unfounded[j]);
        }
        log(" 0\n");
    }    
}

void unfoundedSetAddition() {

}

#endif

}

#ifdef PROOF_LOG

FILE*
getLogFile()
{
    return proof::log_file;
}

#endif
