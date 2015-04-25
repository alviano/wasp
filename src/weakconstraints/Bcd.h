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

#ifndef BCD_H
#define BCD_H

#include "WeakInterface.h"

class BcdData
{    
    public:
        friend ostream& operator<<( ostream& o, const BcdData& bcd )
        {
            if( bcd.numberOfLiterals() == 0 )
                return o << "[]";
            o << "[" << bcd.getLiteral( 0 ) << "=" << bcd.getWeight( 0 );
            for( unsigned int i = 1; i < bcd.numberOfLiterals(); i++ )
                o << ", " << bcd.getLiteral( i ) << "=" << bcd.getWeight( i );
            o << "]";
            return o;
        }
        
        inline BcdData() : lowerBound_( 0 ), middle_( 0 ), upperBound_( 1 ), removed_( 0 ), aggregateAux( UINT_MAX ) {}
        
        inline void copyFrom( BcdData* core )
        {
            assert( core != NULL );            
            BcdData& tmpCore = *core;
            this->lowerBound_ += tmpCore.lb();
            this->upperBound_ += tmpCore.ub();
            for( unsigned int j = 0; j < tmpCore.numberOfLiterals(); j++ )
            {
                literals_.push_back( tmpCore.getLiteral( j ) );
                weights_.push_back( tmpCore.getWeight( j ) );
            }
        }
        
        inline void addElement( Literal lit, uint64_t weight )
        {
            literals_.push_back( lit );
            weights_.push_back( weight );            
            upperBound_ += weight;            
        }
        
        inline void computeMiddle()
        {
            middle_ = ( lowerBound_ + 1 == upperBound_ ) ? upperBound_ : ( upperBound_ + lowerBound_ ) / 2;            
        }

        inline void restore() { removed_ = false; }
        inline void remove() { removed_ = true; }
        inline bool isRemoved() { return removed_; }        
        inline uint64_t lb() const { return lowerBound_; }
        inline uint64_t ub() const { return upperBound_; }
        inline uint64_t middle() const { return middle_; }

        inline void setLb( uint64_t lb ) { lowerBound_ = lb; }
        inline void setUb( uint64_t ub ) { upperBound_ = ub; }
        
        inline unsigned int numberOfLiterals() const { assert( literals_.size() == weights_.size() ); return literals_.size(); }
        inline Literal getLiteral( unsigned int pos ) const { assert( pos < literals_.size() ); return literals_[ pos ]; }
        inline uint64_t getWeight( unsigned int pos ) const { assert( pos < weights_.size() ); return weights_[ pos ]; }
        
        inline vector< Literal >& getLiterals() { return literals_; }
        inline vector< uint64_t >& getWeights() { return weights_; }
        
        inline Var getAggregateAux() const { return aggregateAux; }
        inline void setAggregateAux( Var v ) { aggregateAux = v; }
        
        inline void sort() { mergesort( 0, literals_.size() - 1 ); }        
    private:
        uint64_t lowerBound_;
        uint64_t middle_;
        uint64_t upperBound_;
        bool removed_;
        Var aggregateAux;        

        vector< Literal > literals_;
        vector< uint64_t > weights_;
        
        void mergesort( int left, int right )
        {
            if( left < right )
            {
                int center = ( left + right ) / 2;
                mergesort( left, center );
                mergesort( center + 1, right );
                merge( left, center, right );
            }
        }

        void merge( int left, int center, int right )
        {
            Literal* auxLiterals = new Literal[ right + 1 ];
            uint64_t* auxWeights = new uint64_t[ right + 1 ];

            int i, j;
            for( i = center + 1; i > left; i-- )
            {                    
                auxLiterals[ i - 1 ] = literals_[ i - 1 ];
                auxWeights[ i - 1 ] = weights_[ i - 1 ];
            }
            for( j = center; j < right; j++ )
            {
                auxLiterals[ right + center - j ] = literals_[ j + 1 ];
                auxWeights[ right + center - j ] = weights_[ j + 1 ];
            }
            for( int k = left; k <= right; k++ )
            {
                if( auxWeights[ j ] > auxWeights[ i ] )
                {
                    literals_[ k ] = auxLiterals[ j ];
                    weights_[ k ] = auxWeights[ j-- ];
                }
                else
                {
                    literals_[ k ] = auxLiterals[ i ];
                    weights_[ k ] = auxWeights[ i++ ];
                }
            }

            delete [] auxWeights;
            delete [] auxLiterals;
        }
};

class Bcd : public WeakInterface
{
    public:
        inline Bcd( Solver& s ) : WeakInterface( s ), numberOfModels( 0 ) {}
        inline ~Bcd();        
        unsigned int run();

    private:
        inline void computeIntersectingCores( vector< BcdData* >& intersectingCores );
        inline bool hasToFinishBcd() const;
        void createBcdData( vector< BcdData* >& intersectingCores );
//        bool createFalseAggregate( BcdData* bcd );
        inline uint64_t computeCostOfBcdData( const BcdData* bcdData ) const;        
        bool addAggregateBcd( BcdData* bcdData );
        inline Var addBinaryClauseForAggregateBcd( Var aggrId );

        void foundModel();
        void foundCore( const Clause& unsatCore );
        
        void updateLowerBound( BcdData* core );
        inline void deleteOldCores();
        bool processModifiedCores();
        inline void computeAssumptions();
        inline Var addAuxVariableBcd()
        {            
            Var v = addAuxVariable();
            assert( toKeepInAssumptions.size() == auxVarsToBcdData.size() );
            while( auxVarsToBcdData.size() <= v )
            {
                toKeepInAssumptions.push_back( true );
                auxVarsToBcdData.push_back( NULL );
            }
            return v;
        }
        
        vector< BcdData* > disjCores;
        Vector< BcdData* > modifiedCores;
        Vector< Literal > assumptionsToAdd;
        vector< BcdData* > auxVarsToBcdData;
        vector< bool > toKeepInAssumptions;
        unsigned int numberOfModels;
        unsigned int originalNumberOfVariables;
};

Bcd::~Bcd()
{
    for( unsigned int i = 0; i < disjCores.size(); i++ )
        delete disjCores[ i ];
}

Var
Bcd::addBinaryClauseForAggregateBcd(
    Var aggrId )
{    
    Literal lit( addAuxVariableBcd(), POSITIVE );
    solver.addClause( lit, Literal( aggrId, NEGATIVE ) );
    
    assert( !solver.isFalse( aggrId ) );
    if( solver.isTrue( aggrId ) )
    {
        #ifndef NDEBUG
        bool res =
        #endif
        solver.addClauseRuntime( lit );
        assert( res );
    }
     
    return lit.getVariable();
}

void
Bcd::computeIntersectingCores(
    vector< BcdData* >& intersectingCores )
{
    trace_msg( weakconstraints, 2, "Computing intersection between cores" );
    for( unsigned int i = 0; i < disjCores.size(); i++ )
    {
        BcdData& core = *( disjCores[ i ] );
        for( unsigned int j = 0; j < core.numberOfLiterals(); j++ )
        {
            Var v = core.getLiteral( j ).getVariable();            
            if( visited( v ) )
            {
                trace_msg( weakconstraints, 3, "Removing Bcd data " << core );
                intersectingCores.push_back( disjCores[ i ] );
                core.remove();
                bool res = solver.addClauseRuntime( Literal( core.getAggregateAux(), POSITIVE ) );
                assumptionsToAdd.findAndRemove( Literal( core.getAggregateAux(), NEGATIVE ) );                
                if( !res )                    
                    cout << "probably return false" << endl;
                break;
            }
        }
    }    
}

bool
Bcd::hasToFinishBcd() const
{
    trace_msg( weakconstraints, 3, "Checking termination of the algorithm" );    
    for( unsigned int i = 0; i < disjCores.size(); i++ )
    {
        if( disjCores[ i ]->lb() + 1 < disjCores[ i ]->ub() )
        {
            trace_msg( weakconstraints, 4, "Continue for " << *disjCores[ i ] << ". (Lower,Upper) = (" << disjCores[ i ]->lb() << "," << disjCores[ i ]->ub() << ")"  ); 
            return false;
        }
    }
    
    return true;
}

uint64_t
Bcd::computeCostOfBcdData(
    const BcdData* bcdData ) const
{
    uint64_t upper = 0;
    const BcdData& bcd = *bcdData;
    for( unsigned int j = 0; j < bcd.numberOfLiterals(); j++ )
        if( solver.isTrue( bcd.getLiteral( j ) ) )
            upper += bcd.getWeight( j );    
    return upper;
}

void
Bcd::deleteOldCores()
{
    unsigned int j = 0;
    for( unsigned int i = 0; i < disjCores.size(); i++ )
    {
        disjCores[ j ] = disjCores[ i ];
        if( !disjCores[ i ]->isRemoved() )
            j++;
        else
        {
            if( disjCores[ i ]->getAggregateAux() != UINT_MAX )
            {
                assert_msg( disjCores[ i ]->getAggregateAux() < auxVarsToBcdData.size(), disjCores[ i ]->getAggregateAux() << " >= " << auxVarsToBcdData.size() );
                auxVarsToBcdData[ disjCores[ i ]->getAggregateAux() ] = NULL;
            }
            delete disjCores[ i ];
        }
    }
    disjCores.resize( j );
}

void
Bcd::computeAssumptions()
{
    assumptions.clear();
    computeAssumptionsAND();
    unsigned int k = 0;
    for( unsigned int i = 0; i < assumptionsToAdd.size(); i++ )
    {
        assumptionsToAdd[ k ] = assumptionsToAdd[ i ];
        Var v = assumptionsToAdd[ i ].getVariable();
        assert( v < toKeepInAssumptions.size() );
        if( toKeepInAssumptions[ v ] )
            assumptions.push_back( assumptionsToAdd[ k++ ] );
    }
    assumptionsToAdd.shrink( k );
    
    trace_action( weakconstraints, 2, 
    {
        trace_tag( cerr, weakconstraints, 2 );
        cerr << "AssumptionsAND: [";
        for( unsigned int i = 0; i < assumptions.size(); i++ )
            cerr << " " << assumptions[ i ];
        cerr << " ]" << endl;
    });
}

#endif