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

#ifndef WASP_ATOMDATA_H
#define WASP_ATOMDATA_H

#include "Rule.h"
#include "WeightConstraint.h"
#include "AbstractRule.h"
#include "../stl/Vector.h"
class Clause;

class AtomData
{
    public:        
        unsigned supported : 1;
        unsigned numberOfHeadOccurrences : 31;
        Vector< AbstractRule > headOccurrences;
        Vector< AbstractRule > posOccurrences;
        Vector< AbstractRule > negOccurrences;
        Vector< AbstractRule > doubleNegOccurrences;
        Vector< WeightConstraint* > negWeightConstraintsOccurrences;
        Vector< unsigned int > positionsInNegWeightConstraints;
        Vector< WeightConstraint* > posWeightConstraintsOccurrences;
        Vector< unsigned int > positionsInPosWeightConstraints;   

        unsigned readNormalRule_headAtoms;
        unsigned readNormalRule_negativeLiterals;
        unsigned readNormalRule_positiveLiterals;

        WeightConstraint* weightConstraintRule;
        Clause* crule;

        inline AtomData( bool supported_ = false ) : 
        supported( supported_ ), numberOfHeadOccurrences( 0 ), readNormalRule_headAtoms( 0 ), 
        readNormalRule_negativeLiterals( 0 ), readNormalRule_positiveLiterals( 0 ), 
        weightConstraintRule( NULL ), crule( NULL ) {}

        inline bool isSupported() const { return supported; }
        inline void setSupported() { supported = true; }

        inline bool isWeightConstraint() const { return weightConstraintRule != NULL; }
        inline void setWeightConstraint( WeightConstraint* rule ) { assert( rule != NULL ); weightConstraintRule = rule; }

    inline void clear()
    {
        headOccurrences.clearAndDelete();
        posOccurrences.clearAndDelete();
        negOccurrences.clearAndDelete();
        doubleNegOccurrences.clearAndDelete();
        negWeightConstraintsOccurrences.clearAndDelete();
        positionsInNegWeightConstraints.clearAndDelete();
        posWeightConstraintsOccurrences.clearAndDelete();
        positionsInPosWeightConstraints.clearAndDelete();
    }

    AtomData( const AtomData& init ) : supported( init.supported ), 
    numberOfHeadOccurrences( init.numberOfHeadOccurrences ), 
    readNormalRule_headAtoms( init.readNormalRule_headAtoms ), 
    readNormalRule_negativeLiterals( init.readNormalRule_negativeLiterals ), 
    readNormalRule_positiveLiterals( init.readNormalRule_positiveLiterals ), 
    weightConstraintRule( init.weightConstraintRule ), crule( init.crule )
    {
        headOccurrences.initFrom( init.headOccurrences );
        posOccurrences.initFrom( init.posOccurrences );
        negOccurrences.initFrom( init.negOccurrences );
        doubleNegOccurrences.initFrom( init.doubleNegOccurrences );
        negWeightConstraintsOccurrences.initFrom( init.negWeightConstraintsOccurrences );
        positionsInNegWeightConstraints.initFrom( init.positionsInNegWeightConstraints );
        posWeightConstraintsOccurrences.initFrom( init.posWeightConstraintsOccurrences );
        positionsInPosWeightConstraints.initFrom( init.positionsInPosWeightConstraints );
    }

    AtomData& operator=( const AtomData& right ) 
    {
        supported = right.supported;
        numberOfHeadOccurrences = right.numberOfHeadOccurrences;
        readNormalRule_headAtoms = right.readNormalRule_headAtoms;
        readNormalRule_negativeLiterals = right.readNormalRule_negativeLiterals;
        readNormalRule_positiveLiterals = right.readNormalRule_positiveLiterals;
        weightConstraintRule = right.weightConstraintRule;
        crule = right.crule;

        headOccurrences.clearAndDelete();
        posOccurrences.clearAndDelete();
        negOccurrences.clearAndDelete();
        doubleNegOccurrences.clearAndDelete();
        negWeightConstraintsOccurrences.clearAndDelete();
        positionsInNegWeightConstraints.clearAndDelete();
        posWeightConstraintsOccurrences.clearAndDelete();
        positionsInPosWeightConstraints.clearAndDelete();

        headOccurrences.initFrom( right.headOccurrences );
        posOccurrences.initFrom( right.posOccurrences );
        negOccurrences.initFrom( right.negOccurrences );
        doubleNegOccurrences.initFrom( right.doubleNegOccurrences );
        negWeightConstraintsOccurrences.initFrom( right.negWeightConstraintsOccurrences );
        positionsInNegWeightConstraints.initFrom( right.positionsInNegWeightConstraints );
        posWeightConstraintsOccurrences.initFrom( right.posWeightConstraintsOccurrences );
        positionsInPosWeightConstraints.initFrom( right.positionsInPosWeightConstraints );

        return *this;
    }
};

#endif