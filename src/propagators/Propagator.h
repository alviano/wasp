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

#ifndef WASP_PROPAGATOR_H
#define	WASP_PROPAGATOR_H

class Literal;
class Solver;
#include "../util/WaspConstants.h"

class PropagatorData
{
    public:
        inline PropagatorData() : position_( 0 ), isPositive_( false ), isId_( false ) {}
        
        inline PropagatorData( int pos )
        {
            if( pos > 0 ) { position_ = pos; isPositive_ = true; }
            else { position_ = -pos; isPositive_ = false; }
        }
        
        inline PropagatorData( int pos, bool isId )
        {
            if( pos > 0 ) { position_ = pos; isPositive_ = true; }
            else { position_ = -pos; isPositive_ = false; }
            isId_ = isId;
        }
        
        int position() const { return isPositive_ ? position_ : -position_; }        
        bool isId() const { return isId_; }
    
    private:
        unsigned int position_ : 30;
        unsigned int isPositive_ : 1;
        unsigned int isId_ : 1;
};

class Propagator
{
    public:
        inline Propagator() : inVectorOfUnroll( UINT_MAX ) {}
        virtual ~Propagator(){}

        virtual bool onLiteralFalse( Solver& solver, Literal literal, PropagatorData propData ) = 0;
        virtual void reset( const Solver& solver ) = 0;
        virtual void simplifyAtLevelZero( Solver& solver ) = 0;
        
        inline bool isInVectorOfUnroll( unsigned int dl ) const;
        inline void setInVectorOfUnroll( unsigned int dl );

    private:
        unsigned int inVectorOfUnroll;
};

bool
Propagator::isInVectorOfUnroll(
    unsigned int dl ) const
{
    return inVectorOfUnroll == dl;
}

void
Propagator::setInVectorOfUnroll(
    unsigned int dl )
{
    inVectorOfUnroll = dl;
}

#endif

