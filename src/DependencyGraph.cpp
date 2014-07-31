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

class AdjacencyList: public boost::adjacency_list<>
{
};

DependencyGraph::DependencyGraph( Solver& s )
: graph( *new AdjacencyList() ), solver( s ), tight_( true )
{
}

DependencyGraph::~DependencyGraph()
{
    graph.clear();
    assert( graph.vertex_set().empty() );
    delete &graph;

//    for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
//        delete cyclicComponents[ i ];
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
DependencyGraph::computeStrongConnectedComponents()
{
    vector< unsigned int > strongConnectedComponents( boost::num_vertices( graph ) ), discover_time( boost::num_vertices( graph ) );
    vector< boost::default_color_type > color( boost::num_vertices( graph ) );
    vector< boost::graph_traits< boost::adjacency_list< > >::vertex_descriptor > root( boost::num_vertices( graph ) );
    unsigned int numberOfStrongConnectedComponents = boost::strong_components( graph, &strongConnectedComponents[ 0 ] , boost::root_map( &root[ 0 ] ).color_map( &color[ 0 ] ).discover_time_map( &discover_time[ 0 ] ) );

    assert( numberOfStrongConnectedComponents > 0 );
    vector< vector< Var > > components( numberOfStrongConnectedComponents );

    for( vector< int >::size_type i = 0; i != strongConnectedComponents.size(); ++i )
    {
        unsigned int currentComponentId = strongConnectedComponents[ i ];
        assert( currentComponentId < components.size() );
    
        components[ currentComponentId ].push_back( i );
        tight_ = ( components[ currentComponentId ].size() > 1 ) ? false : tight_;        
    }    
    
    components_.swap( components );
}
