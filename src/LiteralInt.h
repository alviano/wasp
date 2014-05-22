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

#ifndef LITERALINT_H
#define LITERALINT_H

#include <cstdint>
#include <cassert>
#include <sstream>
#include <string>
using namespace std;

#include "util/Constants.h"
class Variable;

class Clause;
class Propagator;
class PostPropagator;
class Reason;
class Solver;

class Literal
{
    friend ostream &operator<<( ostream &, const Literal & );

    public:
        static const Literal null;
        static const Literal conflict;

        static inline Literal createFromAssignedVariable( Variable* );
        static inline Literal createOppositeFromAssignedVariable( Variable* );

        inline explicit Literal( Variable* variable = NULL, unsigned int sign = POSITIVE );
        
        inline Literal( const Literal& );
        inline ~Literal();

        inline int getId() const;

        inline bool isTrue() const;
        inline bool isFalse() const;
        inline bool isUndefined() const;

        inline bool setTrue();

        inline bool operator==( const Literal& ) const;
        inline bool operator!=( const Literal& ) const;

        inline void addWatchedClause( Clause* clause );
        inline void findAndEraseWatchedClause( Clause* clauses );

        inline void addClause( Clause* clause );
        inline void findAndEraseClause( Clause* clauses );   
        
        inline void addPropagator( Propagator* p, int position );
        inline void addPostPropagator( PostPropagator* p, int position );
        
        inline unsigned int getDecisionLevel() const;

        inline bool isImplicant( const Reason* reason ) const;
        
//        inline void onLearning( Learning* strategy );

        inline void onAging( unsigned int );

        inline unsigned int numberOfWatchedClauses() const;
        inline unsigned int numberOfOccurrences() const;
        
        inline Variable* getVariable();
        inline const Variable* getVariable() const;        
        
        /**
         * This function returns 0 if the literal is positive, 1 otherwise.
         */
        inline unsigned int getSign() const;
        
        inline Literal getOppositeLiteral();
        
        inline Clause* getOccurrence( unsigned int idx );
        
        inline bool isPositive() const;
        inline bool isNegative() const;
        
        inline void checkSubsumptionForClause( Solver& solver, Clause* clause );

        inline void addLiteralInShortClause( Literal );
        
    private:
        
        /**
         * This function returns 1 if the literal is positive, 0 otherwise.
         */
        inline unsigned int getOppositeSign() const;                
        
        uintptr_t signedVariable;
};

#endif