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

#ifndef WASP_DEPENDENCYGRAPH_H
#define WASP_DEPENDENCYGRAPH_H

#include <vector>
#include <cassert>
#include "util/WaspConstants.h"
using namespace std;

class AdjacencyList;
class Solver;

class DependencyGraph 
{
    public:
        DependencyGraph( Solver& s );    
        ~DependencyGraph();    

        void addEdge( unsigned int v1, unsigned int v2 );
        void computeStrongConnectedComponents();        

        vector< Var >& getComponent( unsigned int pos ){ assert( pos < components_.size() ); return components_[ pos ]; }
        unsigned int numberComponents() const{ return components_.size(); }
        
        bool tight() const { return tight_; } //return numberOfCyclicComponents() == 0; }

    private:        
        DependencyGraph( const DependencyGraph& orig );

        vector< vector< Var > > components_;
        AdjacencyList& graph;
        Solver& solver;
        bool tight_;        
};

#endif
