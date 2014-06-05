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

#ifndef _DEPENDENCYGRAPH_H
#define    _DEPENDENCYGRAPH_H

#include <unordered_set>
#include <vector>
#include "Component.h"

using namespace std;

class AdjacencyList;
class Solver;

class DependencyGraph 
{
    public:
        DependencyGraph( Solver& s );    
        ~DependencyGraph();    

        void addEdge( unsigned int v1, unsigned int v2 );
        void computeStrongConnectedComponents( vector< GUSData* >& gd );        

        Component* getCyclicComponent( unsigned int pos ){ assert( pos < cyclicComponents.size() ); return cyclicComponents[ pos ]; }
        unsigned int numberOfCyclicComponents() const{ return cyclicComponents.size(); }
        
        bool tight() const { return numberOfCyclicComponents() == 0; }

    private:        
        DependencyGraph( const DependencyGraph& orig );

        vector< Component* > cyclicComponents;
        AdjacencyList& graph;
        Solver& solver;
};

#endif
