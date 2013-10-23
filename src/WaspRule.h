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

/* 
 * File:   WaspRule.h
 * Author: Carmine Dodaro
 *
 * Created on 09 September 2013, 18.02
 */

#ifndef WASPRULE_H
#define	WASPRULE_H

#include <cassert>

#include "Clause.h"
using namespace std;

class AuxLiteral;

/**
 *  This class represents a WaspRule.
 * 
 */
class WaspRule : public Clause
{
    public:

        /**
         * Constructor of WaspRule which takes in input the number of literals in the WaspRule.
         * Note that the watches are random generated.
         * 
         * @param size the numbers of literals.
         */
        inline WaspRule( unsigned int size );
//        void setAuxLiteral( AuxLiteral* auxLiteral );

//        void attachWaspRule();
//        void onLiteralTrue( Literal* literal, Solver& solver );
//        void onAuxLiteralTrue( Solver& solver );
//        virtual void onLearning( LearningStrategy* );
//        void onLearningForFalseAuxLiteral( LearningStrategy* );
//        void onLearningForTrueAuxLiteral( LearningStrategy* );
    
    protected:
        virtual ostream& print( ostream& out ) const;
        
    private:

        WaspRule( const WaspRule& )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }

        AuxLiteral* auxLiteral;
        Literal* firstTrueLiteral;
};

WaspRule::WaspRule(
    unsigned int size ) :
        Clause( size ),
        auxLiteral( NULL ),
        firstTrueLiteral( NULL )
{
}

#endif	/* WASPRULE_H */

