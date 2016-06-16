#ifndef RULE_H
#define RULE_H

#include <iostream>
#include "../stl/Vector.h"
#include "../Literal.h"
using namespace std;

class Rule
{
    friend ostream& operator<<( ostream& out, const Rule& rule );
    public:
        Vector< Literal > literals;
        bool handled;

        inline Rule() { handled = false; }
        inline Rule( unsigned head_ ) { handled = false; addHeadAtom( head_ ); }
        Rule( const Rule& init ) { handled = init.handled; literals.initFrom( init.literals ); }        

        inline bool isRemoved() const { return literals.empty(); }
        inline void remove() { literals.clearAndDelete(); }

        inline bool isFact() const { assert( !literals.empty() ); return literals.size() == 1 && literals.back().isPossiblySupportedHeadAtom(); }
        inline unsigned size() const { return literals.size(); }        

        inline void addHeadAtom( unsigned id ) { literals.push_back( Literal::newPossiblySupportedHeadAtom( id ) ); }
        inline void addNegativeLiteral( unsigned id ) { literals.push_back( Literal::newNegativeBodyLiteral( id ) ); }
        inline void addPositiveLiteral( unsigned id ) { literals.push_back( Literal::newUndefinedPositiveBodyLiteral( id ) ); }
        inline void addPositiveTrueLiteral( unsigned id ) { literals.push_back( Literal::newTruePositiveBodyLiteral( id ) ); }
        inline void addDoubleNegLiteral( unsigned id ) { literals.push_back( Literal::newDoubleNegatedBodyLiteral( id ) ); }        
};

#endif