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

#include "DependencyGraph.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

#include "util/Statistics.h"

class AdjacencyList: public boost::adjacency_list<>
{
};

DependencyGraph::DependencyGraph()
: graph( *new AdjacencyList() )
{
}

DependencyGraph::~DependencyGraph()
{
    graph.clear();
    assert( graph.vertex_set().empty() );
    delete &graph;

    for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
        delete cyclicComponents[ i ];
}

void
DependencyGraph::addEdge(
    unsigned int v1,
    unsigned int v2 )
{
    assert( v1 != v2 );
    boost::add_edge( v1, v2, graph );
}

void
DependencyGraph::computeStrongConnectedComponents(
    vector< GUSData* >& gd )
{
    vector< unsigned int > strongConnectedComponents( boost::num_vertices( graph ) ), discover_time( boost::num_vertices( graph ) );
    vector< boost::default_color_type > color( boost::num_vertices( graph ) );
    vector< boost::graph_traits< boost::adjacency_list< > >::vertex_descriptor > root( boost::num_vertices( graph ) );
    unsigned int numberOfStrongConnectedComponents = boost::strong_components( graph, &strongConnectedComponents[ 0 ] , boost::root_map( &root[ 0 ] ).color_map( &color[ 0 ] ).discover_time_map( &discover_time[ 0 ] ) );

    assert( numberOfStrongConnectedComponents > 0 );
    vector< Component* > components( numberOfStrongConnectedComponents, NULL );

    for( vector< int >::size_type i = 0; i != strongConnectedComponents.size(); ++i )
    {
        unsigned int currentComponentId = strongConnectedComponents[ i ];
        assert( currentComponentId < components.size() );
        if( components[ currentComponentId ] == NULL )
            components[ currentComponentId ] = new Component( gd );
    
        components[ currentComponentId ]->addVariable( i );
    }

    unsigned int id = 0;
    for( unsigned int i = 0; i < components.size(); i++ )
    {
        Component* currentComponent = components[ i ];
        assert( currentComponent != NULL );
        if( currentComponent->isCyclic() )
        {
            statistics( addCyclicComponent( currentComponent->size() ) );
            cyclicComponents.push_back( currentComponent );
            currentComponent->setId( id++ );
        }
        else
            delete currentComponent;
    }
}
