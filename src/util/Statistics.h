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

#ifndef STATISTICS_H
#define STATISTICS_H

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solver;

#include "Constants.h"
#include "ErrorMessage.h"

#ifdef STATS_ON
    #define statistics( solver, METHOD ) Statistics::inst( solver ).METHOD
    
    class Statistics {    
        
        public:
            inline Statistics() :
            separator( "\n---------------------------\n" ),
            numberOfRestarts( 0 ), numberOfChoices( 0 ),
            numberOfLearnedClauses( 0 ), numberOfLearnedUnaryClauses( 0 ),
            numberOfLearnedBinaryClauses( 0 ), numberOfLearnedTernaryClauses( 0 ),
            sumOfSizeLearnedClauses( 0 ), minLearnedSize( MAXUNSIGNEDINT ), maxLearnedSize( 0 ),
            numberOfBinaryClauses( 0 ), numberOfTernaryClauses( 0 ), numberOfClauses( 0 ),          
            numberOfDeletion( 0 ), numberOfDeletionInvokation( 0 ),
            minDeletion( MAXUNSIGNEDINT ), maxDeletion( 0 ), shrink( 0 ),
            shrinkedClauses( 0 ), shrinkedLiterals( 0 ),
            max_literals( 0 ), tot_literals( 0 ), numberOfLearnedClausesFromPropagators( 0 ),
            numberOfLearnedUnaryClausesFromPropagators( 0 ),numberOfLearnedBinaryClausesFromPropagators( 0 ),
            numberOfLearnedTernaryClausesFromPropagators( 0 ), sumOfSizeLearnedClausesFromPropagators( 0 ),
            minLearnedSizeFromPropagators( MAXUNSIGNEDINT ), maxLearnedSizeFromPropagators( 0 ), removedComponents( 0 ),
            disabled( false ), generator( true ), numberOfUS( 0 ), maxSizeUS( 0 ), minSizeUS( 0 ), avgUS( 0 ),
            numberOfOR( 0 ), maxSizeOR( 0 ), minSizeOR( MAXUNSIGNEDINT ), avgOR( 0 ), numberOfAND( 0 ), maxSizeAND( 0 ), 
            minSizeAND( MAXUNSIGNEDINT ), avgAND( 0 ), numberOfCalling( 0 ), minTime( MAXUNSIGNEDINT ), 
            maxTime( 0 ), avgTime( 0 ), currentTime( 0 ), numberOfPartialChecks( 0 ), partialCheckWithUS( 0 )
            {
            }

            inline ~Statistics()
            {
                while( !instances.empty() )
                {
                    delete instances.back();
                    instances.pop_back();
                }
            }

            inline void onRestart()
            {
                numberOfRestarts++;
            }

            inline void onChoice()
            {
                numberOfChoices++;
                
                if( ( numberOfChoices % 10000 ) == 0 )
                    printPartialStatistics();
            }

            inline void onLearning( unsigned int size )
            {
                if( size < minLearnedSize )
                    minLearnedSize = size;

                if( size > maxLearnedSize )
                    maxLearnedSize = size;

                sumOfSizeLearnedClauses += size;
                numberOfLearnedClauses++;
                if( size == 1 )
                    numberOfLearnedUnaryClauses++;
                if( size == 2 )
                    numberOfLearnedBinaryClauses++;
                else if( size == 3 )
                    numberOfLearnedTernaryClauses++;
            }
            
            inline void onLearningFromPropagators( unsigned int sizeFromPropagators )
            {
                if( sizeFromPropagators < minLearnedSizeFromPropagators )
                    minLearnedSizeFromPropagators = sizeFromPropagators;

                if( sizeFromPropagators > maxLearnedSizeFromPropagators )
                    maxLearnedSizeFromPropagators = sizeFromPropagators;

                sumOfSizeLearnedClausesFromPropagators += sizeFromPropagators;
                numberOfLearnedClausesFromPropagators++;
                if( sizeFromPropagators == 1 )
                    numberOfLearnedUnaryClausesFromPropagators++;
                if( sizeFromPropagators == 2 )
                    numberOfLearnedBinaryClausesFromPropagators++;
                else if( sizeFromPropagators == 3 )
                    numberOfLearnedTernaryClausesFromPropagators++;
            }            
            
            inline void startShrinkingLearnedClause( unsigned int size )
            {
                shrink = size;
                max_literals += size;
            }
            
            inline void endShrinkingLearnedClause( unsigned int size )
            {
                tot_literals += size;
                shrink -= size;
                if( shrink > 0 )
                {
                    shrinkedClauses++;
                    shrinkedLiterals += shrink;
                }
            }

            inline void onDeletion( unsigned int /*numberOfLearnedClauses*/, unsigned int del )
            {
                numberOfDeletionInvokation++;
                numberOfDeletion += del;
                
                if( del < minDeletion )
                    minDeletion = del;
                            
                if( del > maxDeletion )
                    maxDeletion = del;
            }            

            inline void onAddingClause( unsigned int size )
            {
                if( size == 2 )                
                    numberOfBinaryClauses++;
                else if( size == 3 )
                    numberOfTernaryClauses++;
                
                numberOfClauses++;
            }

            inline void afterPreprocessing( unsigned int vars, unsigned int clauses )
            {
                if( disabled )
                    return;
                cerr << "Clauses after satelite         : " << clauses << endl;
                cerr << "Variables after satelite       : " << vars << endl;
                
                cerr << separator << endl;
                cerr << "Tight                          : " << ( ( cyclicHCComponents.size() + cyclicComponents.size() ) > removedComponents ? "no" : "yes" ) << endl;
                if( cyclicComponents.size() > removedComponents )
                {
                cerr << "Cyclic components              : " << cyclicComponents.size() - removedComponents << endl;
                for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
                {
                    if( !cyclicComponents[ i ].second )
                cerr << "   Atoms in component " << ( i + 1 ) << "        : " << cyclicComponents[ i ].first << endl;                
                }
                }
                if( !cyclicHCComponents.empty() )
                {
                cerr << "Cyclic HCComponents            : " << cyclicHCComponents.size() << endl;
                for( unsigned int i = 0; i < cyclicHCComponents.size(); i++ )
                {
                    if( !cyclicHCComponents[ i ].second )
                cerr << "   Atoms in component " << ( i + 1 ) << "        : " << cyclicHCComponents[ i ].first << endl;                
                }
                }
                
                cerr << separator << endl;                
            }

            inline void beforePreprocessing( unsigned int vars, unsigned int clauses )
            {
                if( disabled )
                    return;
                cerr << "Clauses after first propagation: " << clauses << endl;
                cerr << "Original Number of Clauses     : " << numberOfClauses << endl;
                cerr << "   Binary                      : " << numberOfBinaryClauses << " (" << ( ( double ) numberOfBinaryClauses * 100 / ( double )numberOfClauses ) << "%)" << endl;
                cerr << "   Ternary                     : " << numberOfTernaryClauses << " (" << ( double )( numberOfTernaryClauses * 100 / ( double ) numberOfClauses ) << "%)" << endl;
                cerr << "Variables                      : " << vars << endl;
                
                cerr << separator << endl;
            }

            inline void endSolving()
            {
                printStatistics();
            }
            
            inline void addCyclicComponent( unsigned int numberOfAtoms )
            {
                cyclicComponents.push_back( pair< unsigned int, bool >( numberOfAtoms, false ) );
            }
            
            inline void removeComponent( unsigned int id )
            {
                cyclicComponents[ id ].second = true;
                removedComponents++;
            }
            
            inline void addCyclicHCComponent( unsigned int numberOfAtoms )
            {
                cyclicHCComponents.push_back( pair< unsigned int, bool >( numberOfAtoms, false ) );
            }                        
            
            inline static Statistics& inst( Solver* s )
            {
                unsigned int pos;
                if( solverToStats.find( s ) == solverToStats.end() )
                {
                    Statistics* stats = new Statistics();
                    solverToStats[ s ] = instances.size();
                    instances.push_back( stats );
                }

                pos = solverToStats[ s ];
                if( pos >= instances.size() )
                    ErrorMessage::errorGeneric( "Error with statistics." );
                return *instances[ pos ];
            }
            
            inline void disable() { disabled = true; }
            inline void enable() { disabled = false; }            
            
            inline void startParsing() { cerr << "Start parsing" << endl; }      
            inline void endParsing() { cerr << "End parsing" << endl; }            
            inline void startSCCs() { cerr << "Start SCCs" << endl; }
            inline void endSCCs() { cerr << "End SCCs" << endl; }
            inline void startCompletion() { cerr << "Start completion" << endl; }
            inline void endCompletion() { cerr << "End completion" << endl; }
            
            inline void setGenerator( bool gen ) { generator = gen; }                        

            inline void startCheckerInvokation( bool isPartial, time_t curr )
            {
                if( isPartial )
                    numberOfPartialChecks++;
                
                numberOfCalling++;
                currentTime = curr;
            }
            
            inline void endCheckerInvokation( time_t curr )
            {
                time_t c = curr - currentTime;
                avgTime += c;
                if( c < minTime )
                    minTime = c;
                
                if( c > maxTime )
                    maxTime = c;
            }
            
            inline void foundUS( bool isPartial, unsigned int size )
            {
                if( isPartial )
                    partialCheckWithUS++;
                numberOfUS++;
                avgUS += size;
                
                if( size < minSizeUS )
                    minSizeUS = size;
                
                if( size > maxSizeUS )
                    maxSizeUS = size;
            }
            
            inline void assumptionsOR( unsigned int size )
            {
                numberOfOR++;
                avgOR += size;
                if( size < minSizeOR )
                    minSizeOR = size;
                
                if( size > maxSizeOR )
                    maxSizeOR = size;
            }
            
            inline void assumptionsAND( unsigned int size )
            {
                numberOfAND++;
                avgAND += size;
                if( size < minSizeAND )
                    minSizeAND = size;
                
                if( size > maxSizeAND )
                    maxSizeAND = size;
            }
            
            inline void onDeletingChecker( unsigned int i )
            {
                printCheckerStats( i );
            }            
            
        private:
            static vector< Statistics* > instances;
            static unordered_map< Solver*, unsigned int > solverToStats;
            inline Statistics( const Statistics& ){ assert( 0 ); }
            
            string separator;

            unsigned int numberOfRestarts;
            unsigned int numberOfChoices;
            
            unsigned int numberOfLearnedClauses;
            unsigned int numberOfLearnedUnaryClauses;
            unsigned int numberOfLearnedBinaryClauses;
            unsigned int numberOfLearnedTernaryClauses;
            unsigned int sumOfSizeLearnedClauses;
            unsigned int minLearnedSize;
            unsigned int maxLearnedSize;
            
            unsigned int numberOfBinaryClauses;
            unsigned int numberOfTernaryClauses;
            unsigned int numberOfClauses;
            
            unsigned int numberOfDeletion;
            unsigned int numberOfDeletionInvokation;
            unsigned int minDeletion;
            unsigned int maxDeletion;
            
            uint64_t shrink;
            uint64_t shrinkedClauses;
            uint64_t shrinkedLiterals;
            
            uint64_t max_literals; 
            uint64_t tot_literals;
            
            unsigned int numberOfLearnedClausesFromPropagators;
            unsigned int numberOfLearnedUnaryClausesFromPropagators;
            unsigned int numberOfLearnedBinaryClausesFromPropagators;
            unsigned int numberOfLearnedTernaryClausesFromPropagators;
            unsigned int sumOfSizeLearnedClausesFromPropagators;
            unsigned int minLearnedSizeFromPropagators;
            unsigned int maxLearnedSizeFromPropagators;            
            
            vector< pair< unsigned int, bool > > cyclicComponents;
            vector< pair< unsigned int, bool > > cyclicHCComponents;
            unsigned int removedComponents;
            
            bool disabled;
            bool generator;
            
            unsigned int numberOfUS;
            unsigned int maxSizeUS;
            unsigned int minSizeUS;
            double avgUS;
            
            unsigned int numberOfOR;
            unsigned int maxSizeOR;
            unsigned int minSizeOR;
            double avgOR;
            
            unsigned int numberOfAND;
            unsigned int maxSizeAND;
            unsigned int minSizeAND;
            double avgAND;
            
            unsigned int numberOfCalling;
            time_t minTime;
            time_t maxTime;
            time_t avgTime;
            time_t currentTime;
            
            unsigned int numberOfPartialChecks;
            unsigned int partialCheckWithUS;
            
            void printStatistics()
            {
                if( disabled )
                    return;
                cerr << separator << endl;
                cerr << "Learning" << endl << endl;
                cerr << "Learned clauses                : " << numberOfLearnedClauses << endl;
                if( numberOfLearnedClauses > 0 )
                {
                cerr << "   Unary                       : " << numberOfLearnedUnaryClauses << endl;
                cerr << "   Binary                      : " << numberOfLearnedBinaryClauses << endl;
                cerr << "   Ternary                     : " << numberOfLearnedTernaryClauses << endl;
                cerr << "   AVG Size                    : " << ( ( double ) sumOfSizeLearnedClauses / ( double ) numberOfLearnedClauses ) << endl;
                cerr << "   Min Size                    : " << minLearnedSize << endl;
                cerr << "   Max Size                    : " << maxLearnedSize << endl;                
                }
                
                cerr << "Learned from propagators       : " << numberOfLearnedClausesFromPropagators << endl;
                if( numberOfLearnedClausesFromPropagators > 0 )
                {
                cerr << "   Unary                       : " << numberOfLearnedUnaryClausesFromPropagators << endl;
                cerr << "   Binary                      : " << numberOfLearnedBinaryClausesFromPropagators << endl;
                cerr << "   Ternary                     : " << numberOfLearnedTernaryClausesFromPropagators << endl;
                cerr << "   AVG Size                    : " << ( ( double ) sumOfSizeLearnedClausesFromPropagators / ( double ) numberOfLearnedClausesFromPropagators ) << endl;
                cerr << "   Min Size                    : " << minLearnedSizeFromPropagators << endl;
                cerr << "   Max Size                    : " << maxLearnedSizeFromPropagators << endl;
                }
                                
                cerr << separator << endl;
                cerr << "Deletion" << endl << endl;
                cerr << "Deletion Invokation            : " << numberOfDeletionInvokation << endl;
                if( numberOfDeletionInvokation > 0 )
                {
                cerr << "Number of Deletion             : " << numberOfDeletion << endl;
                cerr << "Min Number of Deletion         : " << minDeletion << endl;
                cerr << "Max Number of Deletion         : " << maxDeletion << endl;
                }
                
                cerr << separator << endl;
                cerr << "Solver" << endl << endl;
                cerr << "Number of choices              : " << numberOfChoices << endl;
                cerr << "Number of restarts             : " << numberOfRestarts << endl;
                cerr << "Shrinked clauses               : " << shrinkedClauses << " (" << ( ( double ) shrinkedClauses * 100 / ( double ) numberOfLearnedClauses ) << "%)" << endl;
                cerr << "Shrinked literals              : " << shrinkedLiterals << " (" << ( ( double ) shrinkedLiterals * 100 / ( double ) sumOfSizeLearnedClauses ) << "%)" << endl;
                cerr << "Conflict literals              : " << tot_literals <<  " (deleted " << ( ( max_literals - tot_literals ) * 100 / ( double ) max_literals ) << "%)" << endl; 
            }

            void printPartialStatistics()
            {
                if( disabled )
                    return;
                cerr << "Choices " << numberOfChoices << " - Learned " << numberOfLearnedClauses + numberOfLearnedClausesFromPropagators - numberOfDeletion << " - Restarts " << numberOfRestarts << endl;
            }
            
            void printCheckerStats( unsigned int i )
            {
                if( generator || disabled )
                    return;
                cerr << endl << endl;
                cerr << separator << endl;
                cerr << "Checker [ " << i << " ]" << endl;
                cerr << separator << endl;
                
                cerr << "Checker Number of Clauses      : " << numberOfClauses << endl;
                cerr << "   Binary                      : " << numberOfBinaryClauses << " (" << ( ( double ) numberOfBinaryClauses * 100 / ( double )numberOfClauses ) << "%)" << endl;
                cerr << "   Ternary                     : " << numberOfTernaryClauses << " (" << ( double )( numberOfTernaryClauses * 100 / ( double ) numberOfClauses ) << "%)" << endl;
                
                printStatistics();                
                cerr << separator << endl;

                cerr << "Number of calling              : " << numberOfCalling << endl;
                cerr << "       Sum Time(s)             : " << avgTime << endl;
                cerr << "       Min Time(s)             : " << minTime << endl;
                cerr << "       Max Time(s)             : " << maxTime << endl;
                cerr << "       Avg Time(s)             : " << ( numberOfCalling == 0 ? -1 : avgTime / numberOfCalling ) << endl;                
                cerr << "       Partial checks          : " << numberOfPartialChecks << "(" << ( numberOfCalling == 0 ? -1 : ( ( double ) numberOfPartialChecks / ( double ) numberOfCalling ) * 100 ) << "% of total)" << endl;
                cerr << "           Found USs           : " << partialCheckWithUS << "(" << ( numberOfPartialChecks == 0 ? -1 : ( ( double ) partialCheckWithUS / ( double ) numberOfPartialChecks ) * 100 ) << "% of total)" << endl;
                cerr << "UnfoundedSets                  : " << endl;
                cerr << "       Min Size                : " << minSizeUS << endl;
                cerr << "       Max Size                : " << maxSizeUS << endl;
                cerr << "       Avg Size                : " << ( numberOfUS == 0 ? -1 : avgUS / numberOfUS ) << endl;
                cerr << "AssumptionsOR                    " << endl;
                cerr << "       Min Number              : " << minSizeOR << endl;
                cerr << "       Max Number              : " << maxSizeOR << endl;
                cerr << "       Avg Number              : " << ( numberOfOR == 0 ? -1 : avgOR / numberOfOR ) << endl;
                cerr << "AssumptionsAND                    " << endl;
                cerr << "       Min Number              : " << minSizeAND << endl;
                cerr << "       Max Number              : " << maxSizeAND << endl;
                cerr << "       Avg Number              : " << ( numberOfAND == 0 ? -1 : avgAND / numberOfAND ) << endl;
            }
    };

#else
    #define statistics( solver, METHOD )
#endif

#endif
