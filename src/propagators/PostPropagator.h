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

#ifndef WASP_POSTPROPAGATOR_H
#define WASP_POSTPROPAGATOR_H

class Clause;
class Learning;
class Literal;

class PostPropagator
{
    public:
        inline PostPropagator() : inserted( false ) {}
        virtual ~PostPropagator() {}
        
        virtual bool onLiteralFalse( Literal literal ) = 0;
        virtual Clause* getClauseToPropagate( Learning& learning ) = 0;
        
        inline void onAdding();
        inline void onRemoving();
        
        bool hasBeenAdded() const { return inserted; }
        virtual bool hasToAddClause() const { return true; }
    
        virtual void reset() = 0;
        
    private:
        bool inserted;

};

void
PostPropagator::onAdding()
{
    inserted = true;
}

void
PostPropagator::onRemoving()
{
    inserted = false;    
}

#endif
