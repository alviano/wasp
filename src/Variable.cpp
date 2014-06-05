///*
// *
// *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
// *
// *  Licensed under the Apache License, Version 2.0 (the "License");
// *  you may not use this file except in compliance with the License.
// *  You may obtain a copy of the License at
// *
// *    http://www.apache.org/licenses/LICENSE-2.0
// *
// *  Unless required by applicable law or agreed to in writing, software
// *  distributed under the License is distributed on an "AS IS" BASIS,
// *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// *  See the License for the specific language governing permissions and
// *  limitations under the License.
// *
// */
//
//#include "Component.h"
//
//
//#include "Variable.h"
//#include "Clause.h"
//#include "Literal.h"
//#include "Learning.h"
//#include "Reason.h"
//#include "util/VariableNames.h"
//#include "Solver.h"
//
//ostream& operator<<( 
//    ostream& out,
//    const Variable& var )
//{
//    #ifndef NDEBUG    
//    if( VariableNames::isHidden( &var ) )
//        out << var.id;
//    else
//    #endif
//    out << VariableNames::getName( &var );
//    return out;
//}
//
//ostream&
//Variable::print(
//    ostream& out ) const
//{
//    out << *this;
//    return out;
//}
//
//Variable::~Variable()
//{
////    if( heuristicCounters[ 0 ] )
////        delete heuristicCounters[ 0 ];
////    
////    if( heuristicCounters[ 1 ] )
////        delete heuristicCounters[ 1 ];
//    if( definition )
//        delete definition;
//    
//    if( signOfEliminatedVariable == ELIMINATED_BY_DISTRIBUTION )
//    {
//        for( unsigned int i = 0; i < numberOfOccurrences( POSITIVE ); i++ )
//            delete allOccurrences[ POSITIVE ][ i ];
//        
//        for( unsigned int i = 0; i < numberOfOccurrences( NEGATIVE ); i++ )
//            delete allOccurrences[ NEGATIVE ][ i ];
//    }
//}
//
////void
////Variable::setHeuristicCounterForLiterals( 
////    HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral )
////{
////    heuristicCounters[ 0 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
////    heuristicCounters[ 1 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
////}
//
////void
////Variable::onLearning( 
////    Learning* strategy )
////{    
////    //The implicant can be NULL if the literal is a choice.
////    if( implicant != NULL )
////    {
////        implicant->onLearning( strategy );
////    }
////}
//
//void
//Variable::onLearningForUnfounded(
//    Learning& learning )
//{
//    assert( component != NULL );
//    component->onLearningForUnfounded( id, learning );
//}
//
//void
//Variable::unitPropagation(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
//
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif
//    
//    WatchedList< Clause* >& wl = watchedLists[ ( getTruthValue() >> 1 ) ];
//
//    Literal complement = Literal::createOppositeFromAssignedVariable( this );
//
//    unsigned j = 0;
//    for( unsigned i = 0; i < wl.size(); ++i )
//    {
//        Clause* clause = wl[ j ] = wl[ i ];
//        assert( "Next clause to propagate is null." && clause != NULL );
//        trace( solving, 5, "Considering clause %s.\n", toString( *clause ).c_str() );
//        if( clause->onLiteralFalse( complement ) )
//        {
//            solver.assignLiteral( clause );
//            if( solver.conflictDetected() )
//            {
//                while( i < wl.size() )
//                    wl[ j++ ] = wl[ i++ ];
//                break;
//            }
//            ++j;
//        }
//        else if( clause->getAt( 1 ) == complement )
//        {
//            assert( !solver.conflictDetected() );
//            ++j;
//        }
//        else
//            assert( !solver.conflictDetected() );
//    }
//    wl.shrink( j );
//}
//
//void
//Variable::shortPropagation(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
//
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif
//        
//    Vector< Literal >& binary = binaryClauses[ ( getTruthValue() >> 1 ) ];
////    Literal complement = Literal::createOppositeFromAssignedVariable( this );    
//    
//    trace_msg( solving, 2, "Propagation of binary clauses for literal " << Literal::createFromAssignedVariable( this ) );
//    for( unsigned i = 0; i < binary.size(); ++i )
//    {
//        if( solver.conflictDetected() )
//            break;
//        
//        Literal lit = binary[ i ];
//        if( !lit.isTrue() )
//        {
//            trace_msg( solving, 5, "Inferring " << lit << " as true" );        
//            solver.assignLiteral( lit, this );
//        }
//    }
//}
//
//void
//Variable::propagation(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
//
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif
//    
//    Vector< pair< Propagator*, int > >& wl = propagators[ ( getTruthValue() >> 1 ) ];
//
//    Literal complement = Literal::createOppositeFromAssignedVariable( this );    
//    
//    for( unsigned i = 0; i < wl.size(); ++i )
//    {
//        if( solver.conflictDetected() )
//            break;
//        Propagator* propagator = wl[ i ].first;
//        assert( "Post propagator is null." && propagator != NULL );
//        propagator->onLiteralFalse( solver, complement, wl[ i ].second );        
//    }
//}
//
//void
//Variable::postPropagation(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
//
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif
//    
//    Vector< pair< PostPropagator*, int > >& wl = postPropagators[ ( getTruthValue() >> 1 ) ];
//
//    Literal complement = Literal::createOppositeFromAssignedVariable( this );    
//    
//    for( unsigned i = 0; i < wl.size(); ++i )
//    {
//        PostPropagator* postPropagator = wl[ i ].first;
//        assert( "Post propagator is null." && postPropagator != NULL );
//        bool res = postPropagator->onLiteralFalse( complement, wl[ i ].second );
//        
//        if( res )
//            solver.addPostPropagator( postPropagator );
//    }
//}
//
//void
//Variable::propagateAtLevelZero(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
//
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif
//    
//    {
//        trace_msg( solving, 2, "Propagating " << Literal::createFromAssignedVariable( this ) << " as true at level 0" );
//        Vector< Clause* >& wl = allOccurrences[ 1 - ( getTruthValue() >> 1 ) ];
//        Literal literal = Literal::createFromAssignedVariable( this );
//        
//        for( unsigned i = 0; i < wl.size(); ++i )
//        {
//            Clause* clause = wl[ i ];
//            trace_msg( solving, 5, "Considering clause " << *clause );
//            clause->detachClauseFromAllLiterals( literal );
//            solver.deleteClause( clause );
//        }
//        wl.clearAndDelete();
//    }
//    
//    {
//        assert( !solver.conflictDetected() );
//        trace_msg( solving, 2, "Propagating " << Literal::createOppositeFromAssignedVariable( this ) << " as false at level 0" );
//        Vector< Clause* >& wl = allOccurrences[ ( getTruthValue() >> 1 ) ];
//        Literal complement = Literal::createOppositeFromAssignedVariable( this );
//        
//        for( unsigned i = 0; i < wl.size(); ++i )
//        {
//            Clause* clause = wl[ i ];
//            assert( "Next clause to propagate is null." && clause != NULL );
//            trace( solving, 5, "Considering clause %s.\n", toString( *clause ).c_str() );
//            clause->removeLiteral( complement );
//            if( clause->size() == 1 )
//            {
//                if( !clause->getAt( 0 ).isTrue() )
//                {
//                    trace_msg( solving, 5, "Assigning literal " << clause->getAt( 0 ) << " as true" );
//                    solver.assignLiteral( clause->getAt( 0 ) );
//                    if( solver.conflictDetected() )
//                        break;
//                }
//                clause->detachClauseFromAllLiterals();
//                solver.deleteClause( clause );
//                
//            }
//            else
//                assert( !solver.conflictDetected() );
//        }
//        wl.clearAndDelete();
//    }
//
//    assert( allOccurrences[ POSITIVE ].size() == 0 );
//    assert( allOccurrences[ NEGATIVE ].size() == 0 );
//}
//
//void
//Variable::propagateAtLevelZeroSatelite(
//    Solver& solver )
//{
//    assert( !solver.conflictDetected() );
//    assert( FALSE == 1 && TRUE == 2 );
//
//    #ifndef NDEBUG
//    unsigned int sign = ( getTruthValue() >> 1 );
//    assert( sign <= 1 );
//    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
//    #endif    
//    
//    {
//        Vector< Clause* >& wl = allOccurrences[ 1 - ( getTruthValue() >> 1 ) ];
//        Literal literal = Literal::createFromAssignedVariable( this );
//        
//        for( unsigned i = 0; i < wl.size(); ++i )
//        {
//            Clause* clause = wl[ i ];
//            assert( !clause->hasBeenDeleted() );
//            trace_msg( solving, 5, "Considering clause " << *clause );
//            clause->detachClauseFromAllLiterals( literal );
//            solver.markClauseForDeletion( clause );
//        }
//        wl.clearAndDelete();
//    }
//    
//    {
//        assert( !solver.conflictDetected() );
//        trace_msg( solving, 2, "Propagating " << Literal::createOppositeFromAssignedVariable( this ) << " as false at level 0" );
//        Vector< Clause* >& wl = allOccurrences[ ( getTruthValue() >> 1 ) ];
//        Literal complement = Literal::createOppositeFromAssignedVariable( this );
//        
//        for( unsigned i = 0; i < wl.size(); ++i )
//        {
//            Clause* clause = wl[ i ];
//            assert( "Next clause to propagate is null." && clause != NULL );
//            trace( solving, 5, "Considering clause %s.\n", toString( *clause ).c_str() );
//            clause->removeLiteral( complement );
//            if( clause->size() == 1 )
//            {
//                if( !clause->getAt( 0 ).isTrue() )
//                {
//                    trace_msg( solving, 5, "Assigning literal " << clause->getAt( 0 ) << " as true" );
//                    solver.assignLiteral( clause->getAt( 0 ) );
//                    if( solver.conflictDetected() )
//                        break;
//                }
//                clause->detachClauseFromAllLiterals();
//                solver.markClauseForDeletion( clause );
//            }
//            else
//            {
//                solver.onStrengtheningClause( clause );
//                assert( !solver.conflictDetected() );
//            }
//        }
//        wl.clearAndDelete();
//    }
//}
//
//void 
//Variable::removeAllClauses(
//    Solver& solver )
//{
//    for( unsigned i = 0; i < allOccurrences[ POSITIVE ].size(); ++i )
//    {
//        Clause* clause = allOccurrences[ POSITIVE ][ i ];
//        solver.removeClauseNoDeletion( clause );
//        clause->onRemovingNoDelete( Literal( this, POSITIVE ) );
//    }
//    allOccurrences[ POSITIVE ].clearAndDelete();
//
//    for( unsigned i = 0; i < allOccurrences[ NEGATIVE ].size(); ++i )
//    {
//        Clause* clause = allOccurrences[ NEGATIVE ][ i ];
//        solver.removeClauseNoDeletion( clause );
//        clause->onRemovingNoDelete( Literal( this, NEGATIVE ) );
//    }
//    allOccurrences[ NEGATIVE ].clearAndDelete();
//}
//
//void 
//Variable::markAllClauses(
//    Solver& solver )
//{
//    for( unsigned i = 0; i < allOccurrences[ POSITIVE ].size(); ++i )
//    {
//        Clause* clause = allOccurrences[ POSITIVE ][ i ];
//        clause->detachClauseFromAllLiterals( Literal( this, POSITIVE ) );
//        solver.markClauseForDeletion( clause );
//    }
//    allOccurrences[ POSITIVE ].clearAndDelete();
//
//    for( unsigned i = 0; i < allOccurrences[ NEGATIVE ].size(); ++i )
//    {
//        Clause* clause = allOccurrences[ NEGATIVE ][ i ];
//        clause->detachClauseFromAllLiterals( Literal( this, NEGATIVE ) );
//        solver.markClauseForDeletion( clause );
//    }
//    allOccurrences[ NEGATIVE ].clearAndDelete();
//}
//
//void
//Variable::checkSubsumptionForClause(
//    Solver& solver,
//    Clause* clause,
//    unsigned sign )
//{
//    Vector< Clause* >& wl = allOccurrences[ sign ];
//    unsigned j = 0;
//    for( unsigned i = 0; i < wl.size(); ++i )
//    {
//        Clause* current = wl[ j ] = wl[ i ];
//        assert( !current->hasBeenDeleted() );
//        trace_msg( satelite, 2, "Considering clause " << *current );
//        if( clause != current && current->size() < solver.getSatelite()->getSubsumptionLimit() )
//        {
//            SubsumptionData data = clause->subsumes( *current );
//            if( data == SUBSUMPTION )
//            {
//                trace_msg( satelite, 1, "Clause " << *clause << " subsumes clause " << *current );
//                current->detachClauseFromAllLiterals( Literal( this, sign ) );
//                solver.markClauseForDeletion( current );
//            }
//            else if( data == SELFSUBSUMPTION )
//            {
//                if( current->getAt( current->size() - 1 ) != Literal( this, sign ) )
//                {
//                    if( current->size() > 2 )
//                        ++j;
//                    else
//                        assert( current->getAt( 0 ) == Literal( this, sign ) );
//                    current->getAt( current->size() -  1 ).findAndEraseClause( current );
//                }
//                current->removeLastLiteralNoWatches();
//                if( current->size() == 1 )
//                {
//                    solver.getSatelite()->addTrueLiteral( current->getAt( 0 ) );
//                    if( current->getAt( 0 ) != Literal( this, sign ) )
//                        current->detachClauseFromAllLiterals();
//                    solver.markClauseForDeletion( current );
//                }
//                else
//                {
//                    current->recomputeSignature();
//                    solver.onStrengtheningClause( current );
//                    trace_msg( satelite, 2, "Clause after removing literal is: " << *current );
//                }
//            }
//            else
//            {
//                ++j;
//            }
//        }
//        else
//        {
//            ++j;
//        }
//    }    
//    wl.shrink( j );
//}
//
//void
//Variable::onLearning(
//    Learning* strategy,
//    Literal )
//{
//    Literal lit = Literal::createFromAssignedVariable( this );
//    strategy->onNavigatingLiteral( lit );
//}
//
//bool
//Variable::onNavigatingLiteralForAllMarked(
//    Learning* strategy,
//    Literal )
//{
//    Literal lit = Literal::createFromAssignedVariable( this );
//    return strategy->onNavigatingLiteralForAllMarked( lit );    
//}
