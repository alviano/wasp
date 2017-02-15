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

#include "Component.h"
#include "../Literal.h"
#include "../util/WaspOptions.h"
#include "../Learning.h"
#include "../Clause.h"
#include "../Solver.h"
#include "../outputBuilders/CompetitionOutputBuilder.h"

Component::~Component()
{
    for( unsigned int i = 0; i < clausesToDelete.size(); i++ )
        delete clausesToDelete[ i ];    
}

ostream& operator<<( ostream& o, const Component& c )
{
    o << "[";
    for( unsigned int i = 0; i < c.size(); i++ )
        o << " " << VariableNames::getName( c.getVariable( i ) );
    o << " ]";
    return o;
}

Clause*
Component::getClauseToPropagate(
    Learning& learning )
{
    begin:;
    if( unfoundedSet.empty() )
    {
        assert( !done );
        clauseToPropagate = NULL;
        
        assert( !variablesWithoutSourcePointer.empty() || checkSourcePointersStatus() );
        assert( !variablesWithoutSourcePointer.empty() || checkFoundedStatus() );
        if( variablesWithoutSourcePointer.empty() )
            return NULL;
             
        if( first )
            computeGUSFirst();
        else
            computeGUS();
        
        assert( !unfoundedSet.empty() || variablesWithoutSourcePointer.empty() );
        assert( !unfoundedSet.empty() || checkSourcePointersStatus() );
        assert( !unfoundedSet.empty() || checkFoundedStatus() );
        if( unfoundedSet.empty() )
            return NULL;
        assert( clauseToPropagate == NULL );
        clauseToPropagate = learning.learnClausesFromUnfoundedSet( unfoundedSet );
        solver.onUnfoundedSet( unfoundedSet );
        solver.onLoopFormula( clauseToPropagate );
        clausesToDelete.push_back( clauseToPropagate );
        trace_msg( unfoundedset, 2, "Reasons of unfounded sets: " << *clauseToPropagate );
        goto begin;
    }
    else
    {
        assert( clauseToPropagate != NULL );
        
        if( done )
        {
            Var variable = updateClauseToPropagate();
            if( variable == 0 )
            {
                assert( unfoundedSet.empty() );
                done = 0;
                clauseToPropagate = NULL;
                goto begin;
            }
            
            Clause* c = clauseToPropagate;
            if( solver.isTrue( variable ) )
            {
                assert( c->getAt( 0 ) == Literal( variable, FALSE ) );
                assert( solver.getDecisionLevel( variable ) == solver.getCurrentDecisionLevel() );

                c = new Clause( clauseToPropagate->size() );
                c->copyLiterals( *clauseToPropagate );
                if( solver.glucoseHeuristic() )
                    c->setLbd( clauseToPropagate->lbd() );
                c->setLearned();
                reset();
                assert( !done );
                assert( clauseToPropagate == NULL );
            }
            return c;              
        }
        
        if( conflict != 0 )
            return handleConflict();

        //Keep small clauses
        if( clauseToPropagate->size() <= 3 || unfoundedSet.size() == 1 )
        {
            Clause* c = inferFalsityOfUnfoundedAtoms();
            if( c != NULL )
                return c;

            goto begin;
        }
                        
        assert( unfoundedSet.size() >= 2 );        
        assert( clauseToPropagate->size() > 3 );
        
        assert( !done );
        clauseToPropagate->addLiteralInLearnedClause( Literal::null );
        clauseToPropagate->swapLiterals( 0, 1 );
        clauseToPropagate->swapLiterals( 0, clauseToPropagate->size() - 1 );

//        assert( unfoundedSet.empty() );
//        clauseToPropagate = NULL;
        done = 1;
        goto begin;
//        return NULL;
    }

    goto begin;
}
void
Component::computeGUSFirst()
{
    assert( first );
    first = false;
    while( added.size() <= gusData.size() )
        added.push_back( numberOfCalls );

    trace_msg( unfoundedset, 1, "Starting the computation of unfounded sets for component " << id << " " << *this );    
    removeFalseAtomsAndPropagateUnfoundedness();

    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ i ];
        if( getGUSData( variable ).isFounded() )
            continue;

        trace_msg( unfoundedset, 1, "Looking for a new source pointer for the variable " << Literal( variable, POSITIVE ) );
        lookForANewSourcePointer( variable );
    }

    assert( unfoundedSet.empty() );
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ i ];
        getGUSData( variable ).setOutQueue();

        if( getGUSData( variable ).isFounded() )
            continue;

        trace_msg( unfoundedset, 1, "The variable " << Literal( variable, POSITIVE ) << " is in the unfounded set" );
        assert( !solver.isFalse( variable ) );
        getGUSData( variable ).setInUnfoundedSet();
        unfoundedSet.push_back( variable );
    }
    variablesWithoutSourcePointer.clear();
}

void
Component::computeGUS()
{
    assert( !first );
    trace_msg( unfoundedset, 1, "Starting the computation of unfounded sets for component " << id << " " << *this );

    removeFalseAtomsAndPropagateUnfoundedness();

    unsigned int i = 0;
    for( ; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ i ];

        //The variable will be removed from the vector after the computation of unfounded set.
        getGUSData( variable ).setOutQueue();
        assert( !getGUSData( variable ).isInQueue() );

        if( getGUSData( variable ).isFounded() )
            continue;

        trace_msg( unfoundedset, 1, "Considering " << Literal( variable, POSITIVE ) << " which has no source pointer" );
        if( computeUnfoundedSet( variable ) )
            break;
        
        assert( getGUSData( variable ).isFounded() );
    }

    assert( ( i == variablesWithoutSourcePointer.size() && unfoundedSet.empty() ) || ( i != variablesWithoutSourcePointer.size() && !unfoundedSet.empty() ) );
    //if i == variablesWithoutSourcePointer.size() this "for" performs no iterations thus the vector will be then clear.
    unsigned int j = 0;
    for( unsigned int k = i + 1; k < variablesWithoutSourcePointer.size(); k++ )
    {
        Var variable = variablesWithoutSourcePointer[ k ];
        assert( j < variablesWithoutSourcePointer.size() );
        assert( !solver.isFalse( variable ) );
        
        if( !getGUSData( variable ).isFounded() )
            variablesWithoutSourcePointer[ j++ ] = variable;
        else
            getGUSData( variable ).setOutQueue();        
    }
    variablesWithoutSourcePointer.shrink( j );
}

void
Component::reset()
{
    trace_msg( unfoundedset, 1, "Calling reset for component " << id << " - " << *this );
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        getGUSData( variablesWithoutSourcePointer[ i ] ).setOutQueue();
        setFounded( variablesWithoutSourcePointer[ i ] );
    }
    variablesWithoutSourcePointer.clear();

    for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
    {
        getGUSData( unfoundedSet[ i ] ).removeFromUnfoundedSet();
        setFounded( unfoundedSet[ i ] );
    }
    unfoundedSet.clear();
    conflict = 0;
    clauseToPropagate = NULL;
    done = 0;
}

bool
Component::onLiteralFalse(
    Literal lit )
{
    trace_msg( unfoundedset, 1, "Literal " << lit << " is false " << *this );
    return propagateFalseForGUS( lit );
}

void
Component::removeFalseAtomsAndPropagateUnfoundedness()
{
    trace_msg( unfoundedset, 1, "Removing false atoms from variables without source pointers" );
    unsigned int k = 0;
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
    {
        Var variable = variablesWithoutSourcePointer[ k ] = variablesWithoutSourcePointer[ i ];
        
        if( solver.isFalse( variable ) )
            setFounded( variable );
        
        if( getGUSData( variable ).isFounded() )
        {
            trace_msg( unfoundedset, 1, "Variable " << Literal( variable, POSITIVE ) << " is founded or false" );            
            getGUSData( variable ).setOutQueue();
            continue;
        }
        
        assert( !solver.isFalse( variable ) );

        k++;
        if( getGUSData( variable ).isPropagated() )
            continue;

        trace_msg( unfoundedset, 1, "Variable " << Literal( variable, POSITIVE ) << " lost the source pointer" );
        getGUSData( variable ).setPropagated();
        propagateLiteralLostSourcePointer( Literal( variable ) );
    }
    variablesWithoutSourcePointer.shrink( k );
}

bool
Component::iterationOnSupportedByThisExternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variables supported by literal " << lit << " externally" );    
    Vector< Var >& wl = getGUSData( lit.getVariable() ).supportedByThisExternalRule[ lit.getSign() ];

    unsigned i = 0;
    unsigned j = 0;
    for( ; i < wl.size(); ++i )
    {
        Var variable = wl[ j ] = wl[ i ];
        assert( solver.getComponent( variable ) != NULL );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true" ) << " and " << ( ( getGUSData( variable ).isInQueue() ) ? "in queue" : "not in queue" ) );        
        assert( !getGUSData( variable ).isInQueue() || variablesWithoutSourcePointer.existElement( variable ) );
        if( solver.getComponent( variable )->getId() != id || solver.isFalse( variable ) )
        {            
            ++j;
            continue;
        }

        if( !getGUSData( variable ).isInQueue() )
            variableHasNoSourcePointer( variable );
        else if( getGUSData( variable ).isFounded() )
            setUnfounded( variable );
        else            
            ++j;
        assert( !getGUSData( variable ).isFounded() );
    }
    wl.shrink( j );
    return i != j;
}
bool
Component::iterationOnSupportedByThisInternal(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on variable supported by literal " << lit << " internally" );
    Vector< Var >& wl = getGUSData( lit.getVariable() ).supportedByThisInternalRule[ lit.getSign() ];

    unsigned i = 0;
    unsigned j = 0;
    for( ; i < wl.size(); ++i )
    {
        Var variable = wl[ j ] = wl[ i ];
        assert( solver.getComponent( variable ) != NULL && solver.getComponent( variable )->getId() == id );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true" ) << " and " << ( ( getGUSData( variable ).isInQueue() ) ? "in queue" : "not in queue" ) );
        assert( !getGUSData( variable ).isInQueue() || variablesWithoutSourcePointer.existElement( variable ) );
        if( solver.isFalse( variable ) )
        {
            ++j;
            continue;
        }

        if( !getGUSData( variable ).isInQueue() )
            variableHasNoSourcePointer( variable );
        else if( getGUSData( variable ).isFounded() )
            setUnfounded( variable );
        else
            ++j;        
        assert( !getGUSData( variable ).isFounded() );
    }
    wl.shrink( j );
    return i != j;
}

void
Component::iterationOnAuxSupportedByThis(
    Literal lit )
{
    trace_msg( unfoundedset, 2, "Iterating on aux variable supported by literal " << lit );
    vector< Var >& vec = getGUSData( lit.getVariable() ).auxVariablesSupportedByThis[ lit.getSign() ];
    
    for( unsigned int i = 0; i < vec.size(); i++ )
    {
        Var variable = vec[ i ];
        assert( getGUSData( variable ).isAux() );
        trace_msg( unfoundedset, 3, "Considering variable " << Literal( variable, POSITIVE ) << " which is " << ( solver.isFalse( variable ) ? "false" : "true/undefined" ) << " and " << ( ( getGUSData( variable ).isInQueue() ) ? "in queue" : "not in queue" ) );
        if( solver.isFalse( variable ) )
            continue;

        assert( solver.getComponent( variable ) == this );
        assert( !getGUSData( variable ).isInQueue() || variablesWithoutSourcePointer.existElement( variable ) );
        if( !getGUSData( variable ).isInQueue() )
            variableHasNoSourcePointer( variable );
        else if( getGUSData( variable ).isFounded() )
            setUnfounded( variable );

        assert( !getGUSData( variable ).isFounded() );
        getGUSData( variable ).numberOfSupporting++;
    }
}

void
Component::foundSourcePointer(
    Var variableWithSourcePointer,
    vector< Var >& vars,
    vector< Literal >& lits )
{
    assert( getGUSData( variableWithSourcePointer ).isFounded() );
    for( unsigned int i = 0; i < getGUSData( variableWithSourcePointer ).possiblySupportedByThis[ POSITIVE ].size(); i++ )
    {
        Var var = getGUSData( variableWithSourcePointer ).possiblySupportedByThis[ POSITIVE ][ i ];
        assert( solver.getComponent( var ) == this );
        if( !solver.isFalse( var ) && !getGUSData( var ).isFounded() )
        {
            trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << Literal( var, POSITIVE ) ); 
            vars.push_back( var );
            lits.push_back( Literal( variableWithSourcePointer ) );
//            propagateSourcePointer( var, Literal( variableWithSourcePointer ) );
        }
    }
    
    for( unsigned int i = 0; i < getGUSData( variableWithSourcePointer ).auxVariablesSupportedByThis[ POSITIVE ].size(); i++ )
    {        
        Var var = getGUSData( variableWithSourcePointer ).auxVariablesSupportedByThis[ POSITIVE ][ i ];
        assert( solver.getComponent( var ) == this );
        if( solver.isFalse( var ) )
            continue;
        
        trace_msg( unfoundedset, 1, "Literal " << Literal( variableWithSourcePointer ) << " is a source pointer of " << Literal( var, POSITIVE ) );        
        assert_msg( !getGUSData( var ).isFounded(), "Variable " << Literal( var, POSITIVE ) << " is founded" );
        vars.push_back( var );
        lits.push_back( Literal( variableWithSourcePointer ) );
//        propagateSourcePointer( var, Literal( variableWithSourcePointer ) );        
    }
}

void
Component::lookForANewSourcePointer(
    Var var )
{
    if( getGUSData( var ).isAux() )
        return;
    
    Literal lit = Literal::null;
    vector< Literal >& externalLiterals = getGUSData( var ).externalLiterals;
    for( unsigned int i = 0; i < externalLiterals.size(); i++ )
    {
        lit = externalLiterals[ i ];
        if( solver.isFalse( lit ) )
            continue;
        trace_msg( unfoundedset, 1, "Literal " << lit << " is an external source pointer of " << Literal( var, POSITIVE ) );
        propagateSourcePointer( var, lit );
        return;
    }

    vector< Literal >& internalLiterals = getGUSData( var ).internalLiterals;
    for( unsigned int i = 0; i < internalLiterals.size(); i++ )
    {
        lit = internalLiterals[ i ];
        if( solver.isFalse( lit ) || !getGUSData( lit.getVariable() ).isFounded() )
            continue;
        trace_msg( unfoundedset, 1, "Literal " << lit << " is an internal source pointer of " << Literal( var, POSITIVE ) );
        propagateSourcePointer( var, lit );
        return;
    }
}

void
Component::addVariableSupportedByLiteral(
    Var variable,
    Literal literal )
{
    Var tmp = literal.getVariable();
    if( solver.inTheSameComponent( tmp, variable ) )
        getGUSData( tmp ).supportedByThisInternalRule[ literal.getSign() ].push_back( variable );
    else
        getGUSData( tmp ).supportedByThisExternalRule[ literal.getSign() ].push_back( variable );        
}

void
Component::onLearningForUnfounded(
    unsigned int id,
    Learning& learning )
{
    if( getGUSData( id ).isAux() )
        return;
    
    for( unsigned int i = 0; i < getGUSData( id ).externalLiterals.size(); i++ )
    {
        Literal literal = getGUSData( id ).externalLiterals[ i ];
        assert( solver.isFalse( literal ) );
        if( solver.getDecisionLevel( literal ) > 0 )
            learning.onNavigatingLiteralForUnfoundedSetLearning( literal );        
    }

    for( unsigned int i = 0; i < getGUSData( id ).internalLiterals.size(); i++ )
    {
        Literal literal = getGUSData( id ).internalLiterals[ i ];
        assert( solver.getComponent( literal.getVariable() ) == this );
        if( solver.getDecisionLevel( literal ) > 0 && !getGUSData( literal.getVariable() ).isInUnfoundedSet() ) //getGUSData( literal.getVariable() ).isFounded() )
        {
//            GUSData& element = getGUSData( literal.getVariable() );
            assert( solver.isFalse( literal ) );
            learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
//            if( !element.isAux() )
//            {
//                learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
//            }
//            else
//            {
//                bool ignore = false;
//                unsigned int min = UINT_MAX;
//                unsigned int minPos = UINT_MAX;
//                for( unsigned int j = 0; j < element.literals.size(); j++ )
//                {
//                    Literal lit = element.literals[ j ];
//                    if( lit.isPositive() && getGUSData( lit.getVariable() ).isInUnfoundedSet() )
//                    {
//                        ignore = true;
//                        break;
//                    }
//
//                    if( solver.isFalse( lit ) )
//                    {
//                        unsigned int dl = solver.getDecisionLevel( lit );
//                        if( dl == 0 )
//                        {
//                            ignore = true;
//                            break;
//                        }
//                        else if( dl < min )
//                        {
//                            min = dl;
//                            minPos = j;
//                        }
//                    }
//                }
//                if( ignore )
//                    continue;
//
//                if( minPos == UINT_MAX )
//                {
//                    learning.onNavigatingLiteralForUnfoundedSetLearning( literal );
//                }
//                else
//                {
//                    assert_msg( minPos < element.literals.size(), "Literal " << literal << " " << minPos << " >= " << element.literals.size() );
//                    learning.onNavigatingLiteralForUnfoundedSetLearning( element.literals[ minPos ] );
//                }
//            }
        }
    }    
}

bool
Component::propagateFalseForGUS(
    Literal lit )
{
    bool res1 = iterationOnSupportedByThisExternal( lit );
    
    bool res2 = false;
    if( solver.getComponent( lit.getVariable() ) == this )
        res2 = iterationOnSupportedByThisInternal( lit );
    
    return res1 || res2;
}

Clause*
Component::inferFalsityOfUnfoundedAtoms()
{
    Var variable;
    do
    {
        variable = unfoundedSet.back();        
        setFounded( variable );
        if( solver.isTrue( variable ) )
        {
            conflict = variable;
            return handleConflict();         
        }        
        unfoundedSet.pop_back();
        getGUSData( variable ).removeFromUnfoundedSet();

        if( solver.isFalse( variable ) )
            continue;        

        Clause* loopFormulaPointer = new Clause();
        Clause& loopFormula = *loopFormulaPointer;
        loopFormula.copyLiterals( *clauseToPropagate );
        if( solver.glucoseHeuristic() )
            loopFormula.setLbd( clauseToPropagate->lbd() );

        loopFormula.addLiteral( Literal( variable, NEGATIVE ) );
        unsigned int size = loopFormula.size();
        if( size >= 2 )
        {
            loopFormula.swapLiterals( 0, size - 1 );
            loopFormula.swapLiterals( 1, size - 1 );
        }

        trace_msg( unfoundedset, 2, "Adding loop formula: " << loopFormula );
        loopFormula.setLearned();                                
        solver.onLearningALoopFormulaFromGus();
        
        return loopFormulaPointer;        
    } while( !unfoundedSet.empty() );
    
    return NULL;
}

//void
//Component::conflictOnUnfoundedAtom(
//    Clause* loopFormula,
//    Var variable )
//{
//    if( solver.getDecisionLevel( variable ) > 0 )
//        loopFormula->addLiteral( Literal( variable, NEGATIVE ) );
//
//    if( loopFormula->size() > 1 )
//    {
//        unsigned int maxDecisionLevel = solver.getDecisionLevel( loopFormula->getAt( 1 ) );
//        unsigned int maxPosition = 1;
//
//        for( unsigned int i = 2; i < loopFormula->size(); i++ )
//        {
//            unsigned int dl = solver.getDecisionLevel( loopFormula->getAt( i ) );
//            if( dl > maxDecisionLevel )
//            {
//                maxDecisionLevel = dl;
//                maxPosition = i;
//            }
//        }
//        loopFormula->swapLiterals( 1, maxPosition );
//
//        if( solver.getDecisionLevel( loopFormula->getAt( 0 ) ) <= solver.getDecisionLevel( loopFormula->getAt( 1 ) ) )
//            loopFormula->swapLiterals( 0, 1 );
//
//        if( solver.glucoseHeuristic() )
//            loopFormula->setLbd( solver.computeLBD( *loopFormula ) );
//    }                                        
//
//    //unfoundedSet.clear();
//    reset();
//}

bool
Component::computeUnfoundedSet(
    Var variable )
{
    trace_msg( unfoundedset, 1, "Starting the computation of Unfounded Set from variable " << Literal( variable, POSITIVE ) );
    numberOfCalls++;
    vector< Var > toConsider;    
    assert( unfoundedSet.empty() );
    toConsider.push_back( variable );    
    visit( variable );
    for( unsigned int i = 0; i < toConsider.size(); i++ )
    {
        assert_msg( toConsider.size() <= solver.numberOfVariables(), "Loop!" );        
        Var next = toConsider[ i ];
        assert( solver.getComponent( next ) == this );
        assert( visited( next ) );
        
        assert( !solver.isFalse( next ) );
        if( getGUSData( next ).isFounded() )
            continue;
        
        if( getGUSData( next ).isAux() )
        {
            vector< Literal >& literals = getGUSData( next ).literals;
            #ifndef NDEBUG
            unsigned int count = 0;
            #endif

            for( unsigned int j = 0; j < literals.size(); j++ )
            {
                Literal currentLiteral = literals[ j ];
                Var currentVariable = currentLiteral.getVariable();
                assert( !solver.isFalse( currentLiteral ) );                
                if( currentLiteral.isNegative() || solver.getComponent( currentVariable ) != this || getGUSData( currentVariable ).isFounded() )
                    continue;
                assert( ++count );

                assert( solver.getComponent( currentVariable ) == this );
                if( visited( currentVariable ) )
                    continue;

                visit( currentVariable );
                toConsider.push_back( currentVariable );
            }
            assert_msg( count > 0 && count == getGUSData( next ).numberOfSupporting, count << " - " << getGUSData( next ).numberOfSupporting );
            unfoundedSet.push_back( next );
            continue;
        }

        bool hasSource = false;
        vector< Literal >& externalLiterals = getGUSData( next ).externalLiterals;        
        for( unsigned int j = 0; j < externalLiterals.size(); j++ )
        {
            Literal lit = externalLiterals[ j ];
            if( solver.isFalse( lit ) )
                continue;

            trace_msg( unfoundedset, 1, "Literal " << lit << " is an external source pointer of " << Literal( next, POSITIVE ) );
            propagateSourcePointer( next, lit );
            hasSource = true;
            break;
        }

        if( hasSource )
            continue;

        vector< Literal >& internalLiterals = getGUSData( next ).internalLiterals;
        for( unsigned int j = 0; j < internalLiterals.size(); j++ )
        {
            Literal lit = internalLiterals[ j ];
            assert( lit.isPositive() );
            if( solver.isFalse( lit ) )
                continue;

            Var var = lit.getVariable();
            assert( solver.getComponent( var ) == this );
            if( !getGUSData( var ).isFounded() )
            {
                if( visited( var ) )
                    continue;

                visit( var );
                toConsider.push_back( var );
            }
            else
            {
                trace_msg( unfoundedset, 1, "Literal " << lit << " is an internal source pointer of " << Literal( next, POSITIVE ) );
                propagateSourcePointer( next, lit );
                hasSource = true;
                break;
            }
        }

        if( hasSource )
            continue;

        assert( !unfoundedSet.existElement( next ) );
        unfoundedSet.push_back( next );
    }

    unsigned int j = 0;
    for( unsigned int i = 0; i < unfoundedSet.size(); i++ )
    {
        unfoundedSet[ j ] = unfoundedSet[ i ];
        if( getGUSData( unfoundedSet[ i ] ).isFounded() )
            continue;

        getGUSData( unfoundedSet[ i ] ).setInUnfoundedSet();
        j++;

        if( conflict || !solver.isTrue( unfoundedSet[ i ] ) )
            continue;

        assert( conflict == 0 );
        assert( unfoundedSet[ i ] != 0 );
        conflict = unfoundedSet[ i ];
    }
    unfoundedSet.shrink( j );

    return !unfoundedSet.empty();
}

void
Component::propagateSourcePointer(
    Var var,
    Literal literal )
{
    vector< Var > vars;
    vector< Literal > lits;
    vars.push_back( var );
    lits.push_back( literal );
    while( !vars.empty() )
    {
        Var var = vars.back();
        Literal literal = lits.back();
        
        vars.pop_back();
        lits.pop_back();
        assert( !solver.isFalse( var ) );
        if( getGUSData( var ).isFounded() )
            continue;
        if( getGUSData( var ).isAux() )
        {
            assert( find( getGUSData( var ).literals.begin(), getGUSData( var ).literals.end(), literal ) != getGUSData( var ).literals.end() );
            assert( getGUSData( var ).numberOfSupporting > 0 );
            getGUSData( var ).numberOfSupporting--;
            if( getGUSData( var ).numberOfSupporting > 0 )
                continue;
            assert( getGUSData( var ).numberOfSupporting == 0 );
        }
        else
        {
            getGUSData( var ).sourcePointer = literal;    
        }

        setFounded( var );
        foundSourcePointer( var, vars, lits );
        assert( vars.size() == lits.size() );
    }
}

Clause*
Component::handleConflict()
{
    assert( conflict != 0 );
    assert( unfoundedSet.existElement( conflict ) );
    assert( solver.isTrue( conflict ) );
    setFounded( conflict );                
    if( solver.getDecisionLevel( conflict ) > 0 )
        clauseToPropagate->addLiteralInLearnedClause( Literal( conflict, NEGATIVE ) );

    unsigned int size = clauseToPropagate->size();
    if( size > 1 )
    {
        unsigned int maxDecisionLevel = solver.getDecisionLevel( clauseToPropagate->getAt( 1 ) );
        unsigned int maxPosition = 1;

        for( unsigned int i = 2; i < size; i++ )
        {
            unsigned int dl = solver.getDecisionLevel( clauseToPropagate->getAt( i ) );
            if( dl > maxDecisionLevel )
            {
                maxDecisionLevel = dl;
                maxPosition = i;
            }
        }
        clauseToPropagate->swapLiterals( 1, maxPosition );

        if( solver.getDecisionLevel( clauseToPropagate->getAt( 0 ) ) <= solver.getDecisionLevel( clauseToPropagate->getAt( 1 ) ) )
            clauseToPropagate->swapLiterals( 0, 1 );

        if( solver.glucoseHeuristic() )
            clauseToPropagate->setLbd( solver.computeLBD( *clauseToPropagate ) );
    }

    Clause* c = clauseToPropagate;
    assert( !solver.conflictDetected() );    
    
    //clauseToPropagate is set to NULL in the reset;
    reset();
    assert( unfoundedSet.empty() );
    assert( clausesToDelete.size() >= 1 );
    assert( clausesToDelete.back() == c );
    clausesToDelete.pop_back();        
    
    conflict = 0;
    return c;
}

Var
Component::updateClauseToPropagate()
{
    assert( !solver.conflictDetected() );
    while( !unfoundedSet.empty() )
    {
        Var variable = unfoundedSet.back();
        unfoundedSet.pop_back();
        getGUSData( variable ).removeFromUnfoundedSet();
        setFounded( variable );

        if( solver.isFalse( variable ) )
            continue;

        assert_msg( solver.getDecisionLevel( clauseToPropagate->getAt( 1 ) ) == solver.getCurrentDecisionLevel(), "Literal " << clauseToPropagate->getAt( 1 ) << " - DL: " << solver.getDecisionLevel( clauseToPropagate->getAt( 1 ) ) << " != " << solver.getCurrentDecisionLevel() );
        trace_msg( unfoundedset, 2, "Using " << *clauseToPropagate << " for the inference of unfounded atom " << Literal( variable, POSITIVE ) );

        clauseToPropagate->setAt( 0, Literal( variable, NEGATIVE ) );
        return variable;
    }

    return 0;
}

bool
Component::hasToAddClause() const
{
    return !done;
}

#ifndef NDEBUG
bool
Component::checkSourcePointersStatus()
{
    for( unsigned int i = 0; i < variablesInComponent.size(); i++ )
    {
        unsigned int id = variablesInComponent[ i ];                
        assert( solver.getComponent( id )->getId() == this->id );
        if( !getGUSData( id ).isFounded() )
        {
            cerr << "Variable " << Literal( id, POSITIVE ) << " is not founded " << getGUSData( id ).numberOfSupporting << endl;
            return false;
        }

        if( getGUSData( id ).numberOfSupporting != 0 )
        {
            cerr << "Variable " << Literal( id, POSITIVE ) << " has a number of supporting greater than 0" << endl;
            return false;
        }
    }

    return true;
}

bool
Component::checkFoundedStatus()
{    
    for( unsigned int i = 0; i < variablesInComponent.size(); i++ )
    {
        unsigned int var = variablesInComponent[ i ];
        assert( solver.getComponent( var )->getId() == this->id );        
        assert_msg( !variablesWithoutSourcePointer.existElement( var ), "VariablesWithoutSourcePointer contains " << Literal( var, POSITIVE ) );
        assert_msg( getGUSData( var ).isFounded(), Literal( var, POSITIVE ) << " is not founded" );
        assert_msg( !getGUSData( var ).isInQueue(), Literal( var, POSITIVE ) << " is in queue" );

        assert_msg( !getGUSData( var ).isAux() || getGUSData( var ).numberOfSupporting == 0, Literal( var, POSITIVE ) << " - Number of supporting: " << getGUSData( var ).numberOfSupporting );
        if( getGUSData( var ).isAux() )
            continue;

        if( solver.isFalse( var ) )
            return true;
            
        assert_msg( getGUSData( var ).sourcePointer != Literal::null, Literal( var, POSITIVE ) << " has no source pointer" );
        assert_msg( !solver.isFalse( getGUSData( var ).sourcePointer ), getGUSData( var ).sourcePointer << " is a source pointer of " << Literal( var, POSITIVE ) << " but it is false" );
            
        if( solver.getComponent( getGUSData( var ).sourcePointer.getVariable() ) != this )
            return true;

        assert_msg( getGUSData( getGUSData( var ).sourcePointer.getVariable() ).isFounded(), getGUSData( var ).sourcePointer << " is a source pointer of " << Literal( var, POSITIVE ) << " but it is unfounded" );
    }

    //Return always true!
    return true;
}

void
Component::printVariablesWithoutSourcePointer()
{
    cout << "Variables:";
    for( unsigned int i = 0; i < variablesWithoutSourcePointer.size(); i++ )
        cout << " " << Literal( variablesWithoutSourcePointer[ i ], POSITIVE );
    cout << endl;
}
#endif