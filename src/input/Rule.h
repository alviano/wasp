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

#ifndef WASP_RULE_H
#define WASP_RULE_H

#include <iostream>
#include "../stl/Vector.h"
#include "../Literal.h"
using namespace std;

class Rule
{
    friend ostream& operator<<( ostream& out, const Rule& rule );
    public:
        Vector< Literal > literals;        

        inline Rule() { initData(); }
        inline Rule( unsigned head_ ) { initData(); addHeadAtom( head_ ); }
        Rule( const Rule& init ) { 
            handled = init.handled;
            createdBody = init.createdBody;
            handledForModelChecker = init.handledForModelChecker;
            body = init.body;
            literals.initFrom( init.literals );
        }        

        inline bool isRemoved() const { return literals.empty(); }
        inline void remove() { literals.clearAndDelete(); }

        inline bool isFact() const { assert( !literals.empty() ); return literals.size() == 1 && literals.back().isPossiblySupportedHeadAtom(); }
        inline unsigned size() const { return literals.size(); }        

        inline void addHeadAtom( unsigned id ) { literals.push_back( Literal::newPossiblySupportedHeadAtom( id ) ); }
        inline void addNegativeLiteral( unsigned id ) { literals.push_back( Literal::newNegativeBodyLiteral( id ) ); }
        inline void addPositiveLiteral( unsigned id ) { literals.push_back( Literal::newUndefinedPositiveBodyLiteral( id ) ); }
        inline void addPositiveTrueLiteral( unsigned id ) { literals.push_back( Literal::newTruePositiveBodyLiteral( id ) ); }
        inline void addDoubleNegLiteral( unsigned id ) { literals.push_back( Literal::newDoubleNegatedBodyLiteral( id ) ); }  
        
        inline bool isHandled() const { return handled; }
        inline void setHandled() { handled = 1; }
        
        inline bool isBodyCreated() const { return createdBody; }
        inline void setBodyCreated() { createdBody = 1; }
        
        inline bool isHandledForModelChecker( unsigned int nb ) const { return handledForModelChecker == nb; }
        inline void setHandledForModelChecker( unsigned int nb ) { handledForModelChecker = nb; }
        
        inline void setBodyAux( int b ) { body = b; }
        inline int getBodyAux() const { return body; }
                
    private:
        unsigned int handled : 1;
        unsigned int createdBody : 1;
        unsigned int handledForModelChecker : 30;
        int body;
        
        inline void initData() { handled = 0; createdBody = 0; handledForModelChecker = 0; body = 0; }
};

#endif