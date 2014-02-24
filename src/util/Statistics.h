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
#define	STATISTICS_H

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "Constants.h"

#ifdef STATS_ON
    #define statistics( METHOD ) Statistics::inst().METHOD
    
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
            minLearnedSizeFromPropagators( MAXUNSIGNEDINT ), maxLearnedSizeFromPropagators( 0 )
            {
            }

            inline ~Statistics(){}

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

            inline void onDeletion( unsigned int numberOfLearnedClauses, unsigned int del )
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
                cerr << "Clauses after satelite         : " << clauses << endl;
                cerr << "Variables after satelite       : " << vars << endl;
                
                cerr << separator << endl;
                cerr << "Tight                          : " << ( cyclicComponents.empty() ? "yes" : "no" ) << endl;
                if( !cyclicComponents.empty() )
                {
                cerr << "Cyclic components              : " << cyclicComponents.size() << endl;
                for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
                {
                cerr << "   Atoms in component " << ( i + 1 ) << "        : " << cyclicComponents[ i ] << endl;
                }
                }
                
                cerr << separator << endl;                
            }

            inline void beforePreprocessing( unsigned int vars, unsigned int clauses )
            {
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
                cyclicComponents.push_back( numberOfAtoms );
            }
            
            inline static Statistics& inst(){ return statistics; }

        private:
            static Statistics statistics;
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
            
            vector< unsigned int > cyclicComponents;             
            
            void printStatistics()
            {
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
                cerr << "Choices " << numberOfChoices << " - Learned " << numberOfLearnedClauses - numberOfDeletion << " - Restarts " << numberOfRestarts << endl;
            }
    };

#else
    #define statistics( METHOD )
#endif

#endif	/* _STATISTICS_H */