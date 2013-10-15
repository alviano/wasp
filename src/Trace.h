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
 * File:   Trace.h
 * Author: Mario Alviano
 *
 * Created on 15 October 2013, 08.35
 */

#ifndef TRACE_H
#define TRACE_H
#include <cstdio>
/*
 * The tracing macros are expanded only if TRACE_OFF is not defined.
 * The tracing structures are defined only if TRACE_OFF is not defined.
 */
#ifndef TRACE_ON
    #   define trace( type, level, msg, ... )
    #   define traceIf( type, level, condition, msg, ... )
    #   define setTraceLevel( type, level )
#else
    #   define trace( type, level, msg, ... ) \
        if( options.traceLevels.type >= level ) \
        { \
            for( unsigned __indent_Level__ = 0; __indent_Level__ < level; ++__indent_Level__ ) \
                fprintf( stderr, " " ); \
            fprintf( stderr, msg, ##__VA_ARGS__ ); \
        }
    #   define traceIf( type, level, condition, msg, ... ) \
        if( condition ) \
            trace( type, level, msg, ##__VA_ARGS__ )
    #   define setTraceLevel( type, level ) \
            options.traceLevels.type = level

namespace wasp
{
    /**
    * This class contains an unsigned integer for each kind of trace present in
    * wasp.
    */
    class TraceLevels
    {
        friend class Options;

        public:
            unsigned std;
            unsigned parser;
            unsigned mg;
            unsigned mc;
            unsigned us;

        private:
            TraceLevels() : std( 0 ), parser( 0 ){}
    };
};
#endif

#endif
