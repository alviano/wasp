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

#ifndef WASP_LITERAL_H
#define WASP_LITERAL_H

#include <cstdint>
#include <cassert>
#include <sstream>
#include <string>
using namespace std;

#include "util/WaspConstants.h"
class Literal
{
    friend ostream &operator<<( ostream &, const Literal & );

    public:
        static const Literal null;
        static const Literal conflict;

        inline explicit Literal( Var v = 0, unsigned int sign = POSITIVE );
        
        static Literal createLiteralFromInt( int lit )
        {
            assert( lit != 0 );
            if( lit > 0 ) { Literal l( lit, POSITIVE ); return l; }
            else { Literal l( -lit, NEGATIVE ); return l; } 
        }
        
        inline ~Literal() {}

        inline int getId() const { return isPositive() ? getVariable() : -getVariable(); }
        inline unsigned int getIndex() const { return ( variable << 1 ) | sign; }

        inline bool operator==( const Literal& literal ) const { return variable == literal.variable && sign == literal.sign; }
        inline bool operator!=( const Literal& literal ) const { return !( *this == literal ); }

        inline Var getVariable() const { return variable; }
        inline void setVariable( Var v ) { variable = v; }
                
        /**
         * This function returns 0 if the literal is positive, 1 otherwise.
         */
        inline unsigned int getSign() const { assert( "Variable has not been set." && variable != 0 ); return sign; }
        
        inline Literal getOppositeLiteral() const { return Literal( getVariable(), getOppositeSign() ); }

        inline bool isPositive() const { return sign == POSITIVE; }
        inline bool isNegative() const { return !isPositive(); }
        
        inline void setPositive() { sign = POSITIVE; }
        inline void setNegative() { sign = NEGATIVE; }
        
        static Literal newUndefinedPositiveBodyLiteral( Var v ) { return Literal( v, NEGATIVE, true, false ); }
        static Literal newTruePositiveBodyLiteral( Var v ) { return Literal( v, NEGATIVE, true, true ); }
        static Literal newDoubleNegatedBodyLiteral( Var v ) { return Literal( v, NEGATIVE, false, false ); }
        static Literal newNegativeBodyLiteral( Var v ) { return Literal( v, POSITIVE, false, false ); }
        static Literal newPossiblySupportedHeadAtom( Var v ) { return Literal( v, POSITIVE, true, false ); }
        static Literal newUnsupportedHeadAtom( Var v ) { return Literal( v, POSITIVE, true, true ); }
        
        inline bool isUndefinedPositiveBodyLiteral() const { return sign & ~toBeRemoved & relevantForDependencyGraph; }
        inline bool isTruePositiveBodyLiteral() const { return sign & toBeRemoved; }
        inline bool isDoubleNegatedBodyLiteral() const { return sign & ~relevantForDependencyGraph; }
        inline bool isNegativeBodyLiteral() const { return ( ~sign & ~relevantForDependencyGraph ) & 1; }
        inline bool isHeadAtom() const { return ~sign & relevantForDependencyGraph; }
        inline bool isPossiblySupportedHeadAtom() const { return ~sign & relevantForDependencyGraph & ~toBeRemoved; }
        inline bool isUnsupportedHeadAtom() const { return ~sign & relevantForDependencyGraph & toBeRemoved; }
        
        inline bool isToBeRemoved() const { return toBeRemoved; }
        inline bool isPositiveBodyLiteral() const { return sign & ( relevantForDependencyGraph | toBeRemoved ); }
        inline void setUnsupportedHeadAtom() { assert( isPossiblySupportedHeadAtom() ); toBeRemoved = 1; assert( isUnsupportedHeadAtom() ); }
        
    private:
        inline Literal( Var v, bool s, bool d, bool r ) : variable( v ), sign( s ), relevantForDependencyGraph( d ), toBeRemoved( r ) {}
        
        /**
         * This function returns 1 if the literal is positive, 0 otherwise.
         */
        inline unsigned int getOppositeSign() const { assert( "Variable has not been set." && variable != 0 ); return ( ~sign ) & 1; }
        
        unsigned int variable : 29;
        unsigned int sign : 1;
        unsigned int relevantForDependencyGraph : 1;
        unsigned int toBeRemoved : 1;
};

Literal::Literal(
    Var v,
    unsigned int sign_ ) : variable( v ), sign( sign_ )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );
    assert( sign == 0 || sign == 1 );
    assert( ( sign == 0 && isPositive() ) || ( sign == 1 && !isPositive() ) );
    assert( getVariable() == v );
}

#endif
