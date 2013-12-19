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
            numberOfBinaryClauses( 0 ), numberOfTernaryClauses( 0 ),            
            numberOfDeletion( 0 ), numberOfDeletionInvokation( 0 ),
            minDeletion( MAXUNSIGNEDINT ), maxDeletion( 0 )
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
            }

            inline void afterPreprocessing( unsigned int vars, unsigned int clauses )
            {
                cerr << "Clauses after preprocessing    : " << clauses << endl;
                cerr << "Variables after preprocessing  : " << vars << endl;
                
                cerr << separator << endl;
            }

            inline void beforePreprocessing( unsigned int vars, unsigned int clauses )
            {
                cerr << "Clauses                        : " << clauses << endl;
                cerr << "   Binary                      : " << numberOfBinaryClauses << "(" << ( numberOfBinaryClauses / clauses ) * 100 << "%)" << endl;
                cerr << "   Ternary                     : " << numberOfTernaryClauses << "(" << ( numberOfTernaryClauses / clauses ) * 100 << "%)" << endl;
                cerr << "Variables                      : " << vars << endl;
                
                cerr << separator << endl;
            }

            inline void endSolving()
            {
                printStatistics();
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
            
            unsigned int numberOfDeletion;
            unsigned int numberOfDeletionInvokation;
            unsigned int minDeletion;
            unsigned int maxDeletion;
            
            void printStatistics()
            {
                cerr << separator << endl;
                cerr << "Learning" << endl << endl;
                cerr << "Number of Learned Clauses      : " << numberOfLearnedClauses << endl;
                if( numberOfLearnedClauses > 0 )
                {
                cerr << "   Unary                       : " << numberOfLearnedUnaryClauses << endl;
                cerr << "   Binary                      : " << numberOfLearnedBinaryClauses << endl;
                cerr << "   Ternary                     : " << numberOfLearnedTernaryClauses << endl;
                cerr << "   AVG Size                    : " << ( double ) ( sumOfSizeLearnedClauses / numberOfLearnedClauses ) << endl;
                cerr << "   Min Size                    : " << minLearnedSize << endl;
                cerr << "   Max Size                    : " << maxLearnedSize << endl;
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