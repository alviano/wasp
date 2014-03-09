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

/**
 * The dependency graph.
 */
static boost::adjacency_list <> g;

#ifndef NDEBUG
static unsigned numOfInstances = 0;
#endif

DependencyGraph::DependencyGraph()
{
    // Check that DependencyGraph is instantiated just one time!
    // This for ensuring that variable g is used exactly by one instance.
    // If more than one instance is needed, this should be changed, maybe by 
    // introducing some vector of adjacency_list.
	// This check is performed only in debug mode.
    assert( numOfInstances++ == 0 );
}

DependencyGraph::~DependencyGraph()
{
    assert( numOfInstances-- == 1 );
    g.clear();
    assert( g.vertex_set().empty() );

    for( unsigned int i = 0; i < cyclicComponents.size(); i++ )
        delete cyclicComponents[ i ];
}

void
DependencyGraph::addEdge(
    unsigned int v1,
    unsigned int v2 )
{
    assert( v1 != v2 );
    boost::add_edge( v1, v2, g );
}

void
DependencyGraph::computeStrongConnectedComponents(
    vector< GUSData* >& gd )
{
    vector< unsigned int > strongConnectedComponents( boost::num_vertices( g ) ), discover_time( boost::num_vertices( g ) );
    vector< boost::default_color_type > color( boost::num_vertices( g ) );
    vector< boost::graph_traits< boost::adjacency_list< > >::vertex_descriptor > root( boost::num_vertices( g ) );
    unsigned int numberOfStrongConnectedComponents = boost::strong_components( g, &strongConnectedComponents[ 0 ] , boost::root_map( &root[ 0 ] ).color_map( &color[ 0 ] ).discover_time_map( &discover_time[ 0 ] ) );

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
