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

#ifndef WASP_STATISTICS_H
#define WASP_STATISTICS_H

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solver;

#include "WaspConstants.h"
#include "WaspErrorMessage.h"
#include "WaspOptions.h"

#define statistics( solver, METHOD ) Statistics::inst( solver ).METHOD

class Statistics {    

    public:            
        inline Statistics() :
        parsingTime( 0 ), sateliteTime ( 0 ), solvingTime ( 0 ),
        separator( "\n---------------------------\n" ),
        numberOfAtoms( 0 ), numberOfConstraints( 0 ), numberOfDisjunctiveRules( 0 ),
        numberOfNormalRules( 0 ), numberOfChoiceRules( 0 ),
        numberOfCounts( 0 ), numberOfSums( 0 ), numberOfWeakConstraints( 0 ),
        clausesAfterSatelite( 0 ), binaryAfterSatelite( 0 ), ternaryAfterSatelite( 0 ),
        numberOfRestarts( 0 ), numberOfChoices( 0 ),
        numberOfLearnedClauses( 0 ), numberOfLearnedUnaryClauses( 0 ),
        numberOfLearnedBinaryClauses( 0 ), numberOfLearnedTernaryClauses( 0 ),
        sumOfSizeLearnedClauses( 0 ), minLearnedSize( MAXUNSIGNEDINT ), maxLearnedSize( 0 ),
        numberOfBinaryClauses( 0 ), numberOfTernaryClauses( 0 ), numberOfClauses( 0 ),          
        numberOfDeletion( 0 ), numberOfDeletionInvocation( 0 ),
        minDeletion( MAXUNSIGNEDINT ), maxDeletion( 0 ), shrink( 0 ),
        shrinkedClauses( 0 ), shrinkedLiterals( 0 ),
        max_literals( 0 ), tot_literals( 0 ), numberOfLearnedClausesFromPropagators( 0 ),
        numberOfLearnedUnaryClausesFromPropagators( 0 ),numberOfLearnedBinaryClausesFromPropagators( 0 ),
        numberOfLearnedTernaryClausesFromPropagators( 0 ), sumOfSizeLearnedClausesFromPropagators( 0 ),
        minLearnedSizeFromPropagators( MAXUNSIGNEDINT ), maxLearnedSizeFromPropagators( 0 ), removedComponents( 0 ),
        disabled( false ), generator( true ), numberOfUS( 0 ), maxSizeUS( 0 ), minSizeUS( MAXUNSIGNEDINT ), avgUS( 0 ),
        numberOfUCCandidates( 0 ), maxSizeUCCandidates( 0 ), minSizeUCCandidates( MAXUNSIGNEDINT ), avgUCCandidates( 0 ), numberOfAssums( 0 ), maxAssums( 0 ), 
        minAssums( MAXUNSIGNEDINT ), avgAssums( 0 ), numberOfCalling( 0 ), minTime( MAXUNSIGNEDINT ), 
        maxTime( 0 ), avgTime( 0 ), currentTime( 0 ), numberOfPartialChecks( 0 ), partialCheckWithUS( 0 ),
        clausesAfterSimplifications( 0 ), variablesAfterSimplifications( 0 ), numberOfVars( 0 ), trueAtLevelZero( 0 ),
        numberOfSolverCalls( 0 ), numberOfMultiAggregates( 0 ), sumOfBoundsMultiAggregates( 0 )
        {
        }

        inline ~Statistics() {}
        
        static void clean();        

        inline void onRestart() { numberOfRestarts++; }

        inline void onChoice() {
            numberOfChoices++; 
            if( ( numberOfChoices % 5000 ) == 0 )
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

        inline void startShrinkingLearnedClause( unsigned int size ) { shrink = size; max_literals += size; }
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
            numberOfDeletionInvocation++;
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

        inline void onAddingClauseAfterSatelite( unsigned int size )
        {
            if( size == 2 )
                binaryAfterSatelite++;
            else if( size == 3 )
                ternaryAfterSatelite++;
            clausesAfterSatelite++;
        }
        
        inline void onAddingMultiAggregate( unsigned int nbOfBounds )
        {
            numberOfMultiAggregates++;
            sumOfBoundsMultiAggregates += nbOfBounds;        
        }

        inline void afterPreprocessing( unsigned int vars, unsigned int clauses )
        {
            clausesAfterSimplifications = clauses;
            variablesAfterSimplifications = vars;                
            if( disabled || !wasp::Options::stats )
                return;

            if(sateliteTime!=0) sateliteTime=time(0)-sateliteTime;
            if( wasp::Options::statsVerbosity >= 1 )
            {
            cerr << endl << "After satelite" << endl;
            cerr << "    Clauses                     : " << clausesAfterSatelite << endl;
            cerr << "      Binary                    : " << binaryAfterSatelite << " (" << ( ( double ) binaryAfterSatelite * 100 / ( double ) clausesAfterSatelite ) << "%)" << endl;
            cerr << "      Ternary                   : " << ternaryAfterSatelite << " (" << ( double )( ternaryAfterSatelite * 100 / ( double ) clausesAfterSatelite ) << "%)" << endl;
            cerr << "    Assigned vars               : " << vars << " (" << ( ( double ) vars * 100 / ( double ) numberOfVars ) << "%)" << endl;
            cerr << separator << endl;            
            }
            
            cerr << "Tight                           : " << ( ( cyclicHCComponents.size() + cyclicComponents.size() ) > removedComponents ? "no" : "yes" ) << endl;
            
            if( wasp::Options::statsVerbosity >= 2 )
            {
            if( cyclicComponents.size() > removedComponents )
            {
            cerr << "    Cyclic components           : " << cyclicComponents.size() - removedComponents << endl;
            for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
            {
                if( !cyclicComponents[ i ].second )
            cerr << "      Atoms in component " << ( i + 1 ) << "      : " << cyclicComponents[ i ].first << endl;                
            }
            }
            if( !cyclicHCComponents.empty() )
            {
            cerr << "    Cyclic HCComponents         : " << cyclicHCComponents.size() << endl;
            for( unsigned int i = 0; i < cyclicHCComponents.size(); i++ )
            {
                if( !cyclicHCComponents[ i ].second )
            cerr << "        Atoms in component " << ( i + 1 ) << "    : " << cyclicHCComponents[ i ].first << endl;                
            }
            }
            }
            cerr << separator << endl;                
        }

        inline void beforePreprocessing( unsigned int vars, unsigned int assignedVars, unsigned int clauses )
        {
            numberOfVars = vars;
            if( disabled || !wasp::Options::stats )
                return;
            
            cerr << "Parser" << endl << endl;
            cerr << "    Atoms                       : " << numberOfAtoms << endl;
            cerr << "    Constraints                 : " << numberOfConstraints << endl;
            cerr << "    Normal rules                : " << numberOfNormalRules << endl;
            cerr << "    Disjunctive rules           : " << numberOfDisjunctiveRules << endl;
            cerr << "    Choice rules                : " << numberOfChoiceRules << endl;
            cerr << "    Counts                      : " << numberOfCounts << endl;
            cerr << "    Sums                        : " << numberOfSums << endl;
            cerr << "    Weak constraints            : " << numberOfWeakConstraints << endl;
            cerr << separator << endl;

            if( wasp::Options::statsVerbosity >= 1 )
            {
            cerr << "Initial structures" << endl << endl;
            cerr << "    Clauses                     : " << numberOfClauses << endl;
            cerr << "      Binary                    : " << numberOfBinaryClauses << " (" << ( ( double ) numberOfBinaryClauses * 100 / ( double )numberOfClauses ) << "%)" << endl;
            cerr << "      Ternary                   : " << numberOfTernaryClauses << " (" << ( double )( numberOfTernaryClauses * 100 / ( double ) numberOfClauses ) << "%)" << endl;
            cerr << "      Left after propagation    : " << clauses << endl;
            cerr << "    MultiAggregates             : " << numberOfMultiAggregates << endl;
            cerr << "      Replaced aggregates       : " << numberOfMultiAggregates * sumOfBoundsMultiAggregates << endl;
            
            cerr << "    Variables                   : " << vars << endl;
            cerr << "      Assigned                  : " << assignedVars << " (" << ( ( double ) assignedVars * 100 / ( double ) numberOfVars ) << "%)" << endl;
            }
            sateliteTime = time( 0 );
        }

        inline void startSolving() { solvingTime=time(0); }
        inline void endSolving() { printStatistics(); }

        inline void addCyclicComponent( unsigned int numberOfAtoms ) { cyclicComponents.push_back( pair< unsigned int, bool >( numberOfAtoms, false ) ); }
        inline void removeComponent( unsigned int id ) { cyclicComponents[ id ].second = true; removedComponents++; }
        inline void addCyclicHCComponent( unsigned int numberOfAtoms ) { cyclicHCComponents.push_back( pair< unsigned int, bool >( numberOfAtoms, false ) ); }                        
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
                WaspErrorMessage::errorGeneric( "Error with statistics." );
            return *instances[ pos ];
        }

        inline void disable() { disabled = true; }
        inline void enable() { disabled = false; }  

        inline void startParsing() { 
            if( wasp::Options::stats ) {
                parsingTime = time( 0 ); 
                if( wasp::Options::statsVerbosity >= 1 )
                    cerr << "start parsing..." << endl;
            } 
        }
        inline void endParsing() { 
            if( wasp::Options::stats ) { 
                if(parsingTime!=0) parsingTime = time( 0 ) - parsingTime;
                if( wasp::Options::statsVerbosity >= 1 )
                    cerr << "...end parsing" << endl;
            } 
        }
        inline void startSCCs() {}
        inline void endSCCs() {}
        inline void startCompletion() {}
        inline void endCompletion() {}

        inline void setChecker() { generator = false; }                        

        inline void startCheckerInvocation( bool isPartial, time_t curr )
        {
            if( isPartial )
                numberOfPartialChecks++;

            numberOfCalling++;
            currentTime = curr;
        }

        inline void endCheckerInvocation( time_t curr )
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
            numberOfUCCandidates++;
            avgUCCandidates += size;
            if( size < minSizeUCCandidates )
                minSizeUCCandidates = size;

            if( size > maxSizeUCCandidates )
                maxSizeUCCandidates = size;
        }

        inline void assumptions( unsigned int size )
        {
            numberOfAssums++;
            avgAssums += size;
            if( size < minAssums )
                minAssums = size;

            if( size > maxAssums )
                maxAssums = size;
        }

        inline void onDeletingChecker( unsigned int i ) { printCheckerStats( i ); } 

        inline void setTrueAtLevelZero( unsigned int nb ) { trueAtLevelZero = nb; }      

        inline void addedAtom() { numberOfAtoms++; }
        inline void readConstraint() { numberOfConstraints++; }
        inline void readNormalRule() { numberOfNormalRules++; }
        inline void readDisjunctiveRule() { numberOfDisjunctiveRules++; }
        inline void readChoiceRule() { numberOfChoiceRules++; }
        inline void readCount() { numberOfCounts++; }
        inline void readSum() { numberOfSums++; }
        inline void readWeakConstraints( unsigned int numb ) { numberOfWeakConstraints += numb; }
        
        inline void calledSolver() { numberOfSolverCalls++; }
        
        inline void printTime()
        {
            if( disabled || !wasp::Options::stats )
                return;
            
            if(solvingTime!=0)
                solvingTime = time( 0 ) - solvingTime;
            cerr << "    Parsing time                : " << parsingTime << " s" << endl;
            cerr << "    Satelite time               : " << sateliteTime << " s" << endl;
            cerr << "    Solving time                : " << solvingTime << " s" << endl;
        }

    private:
        static vector< Statistics* > instances;
        static unordered_map< Solver*, unsigned int > solverToStats;
        inline Statistics( const Statistics& ){ assert( 0 ); }

        time_t parsingTime;
        time_t sateliteTime;
        time_t solvingTime;        
        
        string separator;

        unsigned int numberOfAtoms;
        unsigned int numberOfConstraints;
        unsigned int numberOfDisjunctiveRules;
        unsigned int numberOfNormalRules;
        unsigned int numberOfChoiceRules;
        unsigned int numberOfCounts;
        unsigned int numberOfSums;
        unsigned int numberOfWeakConstraints;

        unsigned int clausesAfterSatelite;
        unsigned int binaryAfterSatelite;
        unsigned int ternaryAfterSatelite;

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
        unsigned int numberOfDeletionInvocation;
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

        unsigned int numberOfUCCandidates;
        unsigned int maxSizeUCCandidates;
        unsigned int minSizeUCCandidates;
        double avgUCCandidates;

        unsigned int numberOfAssums;
        unsigned int maxAssums;
        unsigned int minAssums;
        double avgAssums;

        unsigned int numberOfCalling;
        time_t minTime;
        time_t maxTime;
        time_t avgTime;
        time_t currentTime;       
        
        unsigned int numberOfPartialChecks;
        unsigned int partialCheckWithUS;
        unsigned int clausesAfterSimplifications;
        unsigned int variablesAfterSimplifications;
        unsigned int numberOfVars;

        unsigned int trueAtLevelZero;
        
        unsigned int numberOfSolverCalls;
        
        unsigned int numberOfMultiAggregates;
        unsigned int sumOfBoundsMultiAggregates;

        void printStatistics()
        {
            if( disabled || !wasp::Options::stats )
                return;
            cerr << separator << endl;
            cerr << "Learning" << endl << endl;
            cerr << "    Learned clauses             : " << numberOfLearnedClauses << endl;
            if( numberOfLearnedClauses > 0 && wasp::Options::statsVerbosity >= 1 )
            {
            cerr << "      Unary                     : " << numberOfLearnedUnaryClauses << endl;
            cerr << "      Binary                    : " << numberOfLearnedBinaryClauses << endl;
            cerr << "      Ternary                   : " << numberOfLearnedTernaryClauses << endl;
            cerr << "      AVG Size                  : " << ( ( double ) sumOfSizeLearnedClauses / ( double ) numberOfLearnedClauses ) << endl;
            cerr << "      Min Size                  : " << minLearnedSize << endl;
            cerr << "      Max Size                  : " << maxLearnedSize << endl;                
            cerr << endl;
            }
            
            cerr << "    Learned post propagators    : " << numberOfLearnedClausesFromPropagators << endl;
            if( numberOfLearnedClausesFromPropagators > 0 && wasp::Options::statsVerbosity >= 1 )
            {
            cerr << "      Unary                     : " << numberOfLearnedUnaryClausesFromPropagators << endl;
            cerr << "      Binary                    : " << numberOfLearnedBinaryClausesFromPropagators << endl;
            cerr << "      Ternary                   : " << numberOfLearnedTernaryClausesFromPropagators << endl;
            cerr << "      AVG Size                  : " << ( ( double ) sumOfSizeLearnedClausesFromPropagators / ( double ) numberOfLearnedClausesFromPropagators ) << endl;
            cerr << "      Min Size                  : " << minLearnedSizeFromPropagators << endl;
            cerr << "      Max Size                  : " << maxLearnedSizeFromPropagators << endl;
            cerr << endl;
            }
            
            cerr << "    Deletion calls              : " << numberOfDeletionInvocation << endl;
            if( numberOfDeletionInvocation > 0 && wasp::Options::statsVerbosity >= 1 )
            {
            cerr << "      Deletion                  : " << numberOfDeletion << endl;
            cerr << "      Min deletion              : " << minDeletion << endl;
            cerr << "      Max deletion              : " << maxDeletion << endl;            
            }

            cerr << separator << endl;
            cerr << "Solver" << endl << endl;
            cerr << "    Calls                       : " << numberOfSolverCalls << endl;
            cerr << "    Choices                     : " << numberOfChoices << endl;
            cerr << "    Restarts                    : " << numberOfRestarts << endl;
            cerr << "    Shrinked clauses            : " << shrinkedClauses << " (" << ( ( double ) shrinkedClauses * 100 / ( double ) numberOfLearnedClauses ) << "%)" << endl;
            cerr << "    Shrinked literals           : " << shrinkedLiterals << " (" << ( ( double ) shrinkedLiterals * 100 / ( double ) sumOfSizeLearnedClauses ) << "%)" << endl;
            cerr << "    Conflict literals           : " << tot_literals <<  " (deleted " << ( ( max_literals - tot_literals ) * 100 / ( double ) max_literals ) << "%)" << endl; 
            cerr << endl;                                        
        }                

        void printPartialStatistics()
        {
            if( disabled || !wasp::Options::stats  || wasp::Options::statsVerbosity < 2 )
                return;
            
            cerr << "    Choices: " << numberOfChoices << " - Learned from unit and propagators: " << numberOfLearnedClauses << " - Learned from post propagators: " << numberOfLearnedClausesFromPropagators << " - Deleted: " << numberOfDeletion << " - Restarts: " << numberOfRestarts << endl;
        }

        void printCheckerStats( unsigned int i )
        {
            if( generator || disabled || !wasp::Options::stats || wasp::Options::statsVerbosity < 2 )
                return;            
            cerr << endl << endl;
            cerr << separator << endl;
            cerr << "Checker [ " << i << " ]" << endl;
            cerr << "    Checker clauses             : " << numberOfClauses << endl;
            cerr << "      Binary                    : " << numberOfBinaryClauses << " (" << ( ( double ) numberOfBinaryClauses * 100 / ( double )numberOfClauses ) << "%)" << endl;
            cerr << "      Ternary                   : " << numberOfTernaryClauses << " (" << ( double )( numberOfTernaryClauses * 100 / ( double ) numberOfClauses ) << "%)" << endl;                                
            cerr << "    Clauses after satelite      : " << clausesAfterSimplifications << " (original number was: " << ( numberOfClauses - numberOfBinaryClauses ) << ")" << endl;                
            cerr << "    Checker variables           : " << numberOfVars << endl;
            cerr << "      Assigned after satelite   : " << variablesAfterSimplifications << endl;
            cerr << "    HC vars always true         : " << trueAtLevelZero << endl;

            printStatistics();                
            
            cerr << "    Number of calling           : " << numberOfCalling << endl;
            cerr << "      Sum Time                  : " << avgTime << " s" << endl;
            cerr << "      Min Time                  : " << minTime << " s" << endl;
            cerr << "      Max Time                  : " << maxTime << " s" << endl;
            cerr << "      Avg Time                  : " << ( numberOfCalling == 0 ? -1 : avgTime / numberOfCalling ) << " s" << endl;                
            cerr << "      Partial checks            : " << numberOfPartialChecks << " (" << ( numberOfCalling == 0 ? -1 : ( ( double ) numberOfPartialChecks / ( double ) numberOfCalling ) * 100 ) << "% of total)" << endl;
            cerr << "        Found USs               : " << partialCheckWithUS << " (" << ( numberOfPartialChecks == 0 ? -1 : ( ( double ) partialCheckWithUS / ( double ) numberOfPartialChecks ) * 100 ) << "% of total)" << endl;
            cerr << endl;
            cerr << "    Unfounded sets                " << endl;
            cerr << "      Min Size                  : " << minSizeUS << endl;
            cerr << "      Max Size                  : " << maxSizeUS << endl;
            cerr << "      Avg Size                  : " << ( numberOfUS == 0 ? -1 : avgUS / numberOfUS ) << endl;
            cerr << endl;
            cerr << "    Unfounded set candidates    " << endl;
            cerr << "      Min Number                : " << minSizeUCCandidates << endl;
            cerr << "      Max Number                : " << maxSizeUCCandidates << endl;
            cerr << "      Avg Number                : " << ( numberOfUCCandidates == 0 ? -1 : avgUCCandidates / numberOfUCCandidates ) << endl;
            cerr << endl;
            cerr << "    Assumptions                   " << endl;
            cerr << "      Min Number                : " << minAssums << endl;
            cerr << "      Max Number                : " << maxAssums << endl;
            cerr << "      Avg Number                : " << ( numberOfAssums == 0 ? -1 : avgAssums / numberOfAssums ) << endl;
        }
};

#endif
