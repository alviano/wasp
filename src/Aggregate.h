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

#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <cassert>
#include <iostream>
#include <list>

#include "PostPropagator.h"
#include "Learning.h"
#include "Literal.h"
#include "stl/Vector.h"
#include "util/Options.h"
#include "util/Trace.h"

class Clause;
using namespace std;

#define POS 1
#define NEG -1

class Aggregate : public PostPropagator
{
    friend ostream& operator<<( ostream& out, const Aggregate& aggregate );
    public:
        inline Aggregate();
        inline ~Aggregate() {}
        
        virtual bool onLiteralFalse( Literal lit, int pos );
        
        inline unsigned int size() const { return literals.size() - 1; }

        inline void addLiteral( Literal lit, unsigned int weight ) { literals.push_back( lit ); weights.push_back( weight ); watched.push_back( true ); }
        
        virtual Clause* getClauseToPropagate( Learning& learning );
        
        inline Literal getLiteral( unsigned int i ) const { assert( i < literals.size() ); return literals[ i ]; }
        inline unsigned int getWeight( unsigned int i ) const { assert( i < weights.size() ); return weights[ i ]; }
        
        unsigned int getLevelOfBackjump( unsigned int bound );
        bool updateBound( unsigned int bound );        

//        inline void setCounterW1( unsigned int value ){ counterW1 = value; }
//        inline void setCounterW2( unsigned int value ){ counterW2 = value; }
        virtual void reset();
        
        void attachAggregate();

    private:
        inline Aggregate( const Aggregate& orig );
        
        list< Clause* > clausesToPropagate;
        vector< Literal > literals;
        vector< unsigned int > weights;
        vector< bool > watched;
        
        int active;
        
        unsigned int counterW1;
        unsigned int counterW2;
        
        unsigned int umax;
        Vector< int > trail;
        
        void createClauseFromTrail( Literal lit );
        #ifndef NDEBUG
        bool checkDecisionLevelsOrder( Clause* ) const;
        bool checkLiteralHasBeenInferred( Literal lit ) const;
        #endif
};

Aggregate::Aggregate() : PostPropagator(), active( 0 ), counterW1( 0 ), counterW2( 0 ), umax( 1 )
{
    literals.push_back( Literal::null );
    weights.push_back( 0 );    
    watched.push_back( false );
}

#endif
