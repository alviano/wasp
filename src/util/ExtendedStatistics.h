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

#ifndef WASP_EXTENDED_STATISTICS_H
#define WASP_EXTENDED_STATISTICS_H

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Solver;

#include "WaspConstants.h"
#include "WaspErrorMessage.h"
#include "../Literal.h"

#ifdef ESTATS_ON
    #define estatistics( solver, METHOD ) ExtendedStatistics* estats = ExtendedStatistics::inst( solver ); if( estats != NULL ) estats->METHOD
    
    class VarExtendedStats {
        public:
            VarExtendedStats() : conflictLiteral( 0 ), usedInConflictResolution( 0 ), inLearnedClause( 0 ), usedAsChoice( 0 ) {}
            
            unsigned int conflictLiteral;
            unsigned int usedInConflictResolution;
            unsigned int inLearnedClause;
            unsigned int usedAsChoice;
    };

    class ExtendedStatistics {    
        
        public:
            inline ~ExtendedStatistics()
            {
                while( !varStats.empty() )
                {
                    delete varStats.back();
                    varStats.pop_back();
                }
            }
            
            inline static ExtendedStatistics* inst( Solver* s )
            {
                if( stats != NULL )
                    return ( s == &( stats->solver ) ) ? stats : NULL;
                stats = new ExtendedStatistics( *s );                    
                return stats;
            }
            
            inline void onNewVar( Var id )
            {
                while( varStats.size() <= id )
                    varStats.push_back( new VarExtendedStats() );
            }
            
            inline void onNewConflictLiteral( Literal lit )
            {
                varStats[ lit.getVariable() ]->conflictLiteral++;
            }
            
            inline void onLiteralUsedInConflictResolution( Literal lit )
            {
                varStats[ lit.getVariable() ]->usedInConflictResolution++;
            }
            
            inline void onLiteralAddInLearnedClause( Literal lit )
            {
                varStats[ lit.getVariable() ]->inLearnedClause++;
            }
            
            inline void onLiteralUsedAsChoice( Literal lit )
            {
                varStats[ lit.getVariable() ]->usedAsChoice++;
            }
            
            inline void endSolving()
            {
                printExtendedStatistics();
            }
            
            inline void checkSolver( Solver* s )
            {
                if( s != &( stats->solver ) )
                    WaspErrorMessage::errorGeneric( "Error with statistics." );
            }
            
        private:
            inline ExtendedStatistics( Solver& s ) :
                solver( s ),
                separator( "\n---------------------------\n" )
            {
            }
            inline ExtendedStatistics( const ExtendedStatistics& );
            
            Solver& solver;
            static ExtendedStatistics* stats;            
            string separator;            
            vector< VarExtendedStats* > varStats;
            void printExtendedStatistics();
    };

#else
    #define estatistics( solver, METHOD )
#endif

#endif
