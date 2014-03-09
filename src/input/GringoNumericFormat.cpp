#include "GringoNumericFormat.h"

#include "../util/Constants.h"
#include "../util/ErrorMessage.h"
#include "../Clause.h"
//#include "../CRule.h"

#include <cassert>
#include <iostream>
#include <unordered_set>
using namespace std;

void
GringoNumericFormat::parse()
{
    this->parse( cin );
}

void
GringoNumericFormat::parse(
    istream& input )
{
    bool loop = true;
    
    while( loop )
    {
        unsigned int type;
        input >> type;
    
        switch( type )
        {
            case GRINGO_NORMAL_RULE_ID:
                readNormalRule( input );                
                break;
                
            case GRINGO_CHOICE_RULE_ID:
//                readChoiceRule( input );                
                ErrorMessage::errorDuringParsing( "Unsupported rule type." );
                break;
                
            case GRINGO_CONSTRAINT_RULE_ID:
//                readConstraintRule( input );
                ErrorMessage::errorDuringParsing( "Unsupported rule type." );
                break;
                
            case GRINGO_DISJUNCTIVE_RULE_ID:
//                readDisjunctiveRule( input );
                ErrorMessage::errorDuringParsing( "Unsupported rule type." );
                break;

            case GRINGO_LINE_SEPARATOR:
                loop = false;
                break;   
            
            default:
                ErrorMessage::errorDuringParsing( "Unsupported rule type." );
                break;
        }
        
        if( solver.conflictDetected() )
            return;

        propagate();
    }

    readAtomsTable( input );
    readTrueAtoms( input );
    readFalseAtoms( input );
    readErrorNumber( input );

//    cout << "cc" << endl;
    computeCompletion();
    if( !solver.tight() )
    {
        trace_msg( parser, 1, "Program is not tight" );
        programIsNotTight();
    }
    else
    {
        trace_msg( parser, 1, "Program is tight" );
    }
    
    //TODO: remove
//    cout << solver.numberOfVariables() << endl;
//    unsigned c[1024] = {0};
//    for( unsigned i = 0; i < solver.numberOfClauses(); i++ )
//        c[solver.clauseAt( i )->size()]++;
//    cout << "occs\n";
//    for( unsigned i = 1; i < 10; i++)
//        cout << c[i] << endl;
//    cout << "Solving..." << endl;
}

void
GringoNumericFormat::readNormalRule(
    istream& input )
{
    unsigned head;
    input >> head;

    createStructures( head );

    if( solver.getVariable( head )->isFalse() )
        readConstraint( input );
    else
        readNormalRule( input, head );
}

void
GringoNumericFormat::readNormalRule(
    istream& input,
    unsigned head )
{
    assert( !solver.getVariable( head )->isFalse() );
    
    int bodySize, negativeSize;
    unsigned tmp;
    input >> bodySize >> negativeSize;
 
    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
    
    if( bodySize == 0 )
    {
        addFact( head );
        return;
    }
    
    normalRules.push_back( vector< int >() );
    normalRules.back().push_back( head );

    numberOfHeadOccurrences[ head ]++;
    headOccurrences[ head ].push_back( normalRules.size() - 1 );
    
    bool remove = facts[ head ];
    while( negativeSize-- > 0 )
    {
        input >> tmp;
        createStructures( tmp );

        if( solver.getVariable( tmp )->isTrue() )
            remove = true;
        else if( solver.getVariable( tmp )->isUndefined() )
        {
            normalRules.back().push_back( -tmp );
            negOccurrences[ tmp ].push_back( normalRules.size() - 1 );
        }

        --bodySize;
    }
    while( bodySize-- > 0 )
    {
        input >> tmp;
        createStructures( tmp );

        if( solver.getVariable( tmp )->isFalse() || tmp == head )
            remove = true;
        else
        {
            normalRules.back().push_back( tmp );
            posOccurrences[ tmp ].push_back( normalRules.size() - 1 );
        }
    }
    
    if( remove )
    {
        normalRules.pop_back();
        numberOfHeadOccurrences[ head ]--;
        headOccurrences[ head ].pop_back();
        return;
    }

    if( normalRules.back().size() == 1 )
    {
        numberOfHeadOccurrences[ head ]--;
        normalRules.pop_back();
        headOccurrences[ head ].pop_back();
        addFact( head );
    }
}

void
GringoNumericFormat::readConstraint(
    istream& input )
{
    unsigned  bodySize, negativeSize, tmp;
    input >> bodySize >> negativeSize;
 
    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
   
    bool remove = false;
    Clause* clause = solver.newClause();
    while( negativeSize-- > 0 )
    {
        input >> tmp;
        if( !clause->addUndefinedLiteral( Literal( solver.getVariable( tmp ), POSITIVE ) ) )
            remove = true;
        --bodySize;
    }
    while( bodySize-- > 0 )
    {
        input >> tmp;
        if( !clause->addUndefinedLiteral( Literal( solver.getVariable( tmp ), NEGATIVE ) ) )
            remove = true;
    }
    
    if( remove )
        solver.releaseClause( clause );
    else
        solver.addClause( clause );
}

//void
//GringoNumericFormat::addConstraint(
//    Clause* body )
//{
//    consistent = solver.addClause( body );
//}
//

void
GringoNumericFormat::propagate()
{
    while( propagatedLiterals < solver.numberOfAssignedLiterals() )
    {
        propagate( solver.getAssignedVariable( propagatedLiterals++ ) );
    }
}

void
GringoNumericFormat::propagate(
    Variable* var )
{
    assert( !var->isUndefined() );
    
    vector< unsigned > facts;
    
    if( var->isFalse() )
    {
        assert( var->getId() < headOccurrences.size() );
        vector< unsigned >& ho = headOccurrences[ var->getId() ];
        for( unsigned i = 0; i < ho.size(); ++i )
        {
            trace_msg( parser, 3, "Replacing " << ho[ i ] << " by a constraint" );
            vector< int >& rule = normalRules[ ho[ i ] ];
            if( rule.empty() )
                continue;
            Clause* clause = solver.newClause();
            for( unsigned j = 1; j < rule.size(); ++j )
                if( rule[ j ] > 0 )
                    clause->addLiteral( Literal( solver.getVariable( rule[ j ] ), NEGATIVE ) );
                else
                    clause->addLiteral( Literal( solver.getVariable( -rule[ j ] ), POSITIVE ) );
            solver.cleanAndAddClause( clause );
            rule.clear();
        }
        numberOfHeadOccurrences[ var->getId() ] = 0;
        ho.clear();
        
        assert( var->getId() < posOccurrences.size() );
        vector< unsigned >& po = posOccurrences[ var->getId() ];
        for( unsigned i = 0; i < po.size(); ++i )
        {
            trace_msg( parser, 3, "Removing rule " << po[ i ] );
            vector< int >& rule = normalRules[ po[ i ] ];
            if( !rule.empty() )
            {
                numberOfHeadOccurrences[ rule[ 0 ] ]--;
                rule.clear();
            }
        }
        po.clear();

        assert( var->getId() < negOccurrences.size() );
        vector< unsigned >& no = negOccurrences[ var->getId() ];
        for( unsigned i = 0; i < no.size(); ++i )
        {
            trace_msg( parser, 3, "Shrinking rule " << no[ i ] );
            vector< int >& rule = normalRules[ no[ i ] ];
            if( rule.empty() )
                continue;
            unsigned k = 1;
            for( unsigned j = 1; j < rule.size(); ++j )
            {
                if( rule[ j ] != -static_cast< int >( var->getId() ) )
                    rule[ k++ ] = rule[ j ];
            }
            if( k > 1 )
                rule.resize( k );
            else
                facts.push_back( rule[ 0 ] );
        }
        no.clear();
    }
    else
    {
        assert( var->getId() < posOccurrences.size() );
        vector< unsigned >& po = posOccurrences[ var->getId() ];
        for( unsigned i = 0; i < po.size(); ++i )
        {
            trace_msg( parser, 3, "Shrinking rule " << po[ i ] );
            vector< int >& rule = normalRules[ po[ i ] ];
            if( rule.empty() )
                continue;
            unsigned k = 1;
            for( unsigned j = 1; j < rule.size(); ++j )
            {
                if( rule[ j ] != -static_cast< int >( var->getId() ) )
                    rule[ k++ ] = rule[ j ];
            }
            if( k > 1 )
                rule.resize( k );
            else
                facts.push_back( rule[ 0 ] );
        }
        po.clear();

        assert( var->getId() < negOccurrences.size() );
        vector< unsigned >& no = negOccurrences[ var->getId() ];
        for( unsigned i = 0; i < no.size(); ++i )
        {
            trace_msg( parser, 3, "Removing rule " << no[ i ] );
            vector< int >& rule = normalRules[ no[ i ] ];
            if( !rule.empty() )
            {
                numberOfHeadOccurrences[ rule[ 0 ] ]--;
                rule.clear();
            }
        }
        no.clear();
    }
    
    while( !facts.empty() )
    {
        addFact( facts.back() );
        facts.pop_back();
    }
}

void
GringoNumericFormat::addFact(
    unsigned head )
{
    if( facts[ head ] )
        return;

    trace_msg( parser, 3, "Adding fact " << head );
    facts[ head ] = true;

    vector< unsigned > stack;
    stack.push_back( head );
    
    do{
        head = stack.back();
        stack.pop_back();
        solver.addClause( Literal( solver.getVariable( head ), POSITIVE ) );
        assert( solver.getVariable( head )->isTrue() || solver.conflictDetected() );
            
        assert( head < headOccurrences.size() );
        vector< unsigned >& ho = headOccurrences[ head ];
        for( unsigned i = 0; i < ho.size(); ++i )
        {
            if( normalRules[ ho[ i ] ].empty() )
                continue;
            trace_msg( parser, 4, "Removing rule " << ho[ i ] );
            assert( normalRules[ ho[ i ] ].size() >= 2 );
            normalRules[ ho[ i ] ].clear();
        }
        numberOfHeadOccurrences[ head ] = 0;
        ho.clear();
        
        assert( head < posOccurrences.size() );
        vector< unsigned >& po = posOccurrences[ head ];
        for( unsigned i = 0; i < po.size(); ++i )
        {
            trace_msg( parser, 4, "Shrinking rule " << po[ i ] );
            assert( po[ i ] < normalRules.size() );
            vector< int >& rule = normalRules[ po[ i ] ];
            if( rule.empty() )
                continue;
            unsigned k = 1;
            for( unsigned j = 1; j < rule.size(); ++j )
            {
                if( rule[ j ] != static_cast< int >( head ) )
                    rule[ k++ ] = rule[ j ];
            }
            if( k == 1 )
            {
                if( !facts[ rule[ 0 ] ] )
                {
                    trace_msg( parser, 5, "New fact: " << rule[ 0 ] );
                    facts[ rule[ 0 ] ] = true;
                    stack.push_back( rule[ 0 ] );
                }
            }
            else
                rule.resize( k );
        }
        po.clear();
        
        assert( head < negOccurrences.size() );
        vector< unsigned >& no = negOccurrences[ head ];
        for( unsigned i = 0; i < no.size(); ++i )
        {
            trace_msg( parser, 4, "Shrinking rule " << no[ i ] );
            assert( no[ i ] < normalRules.size() );
            vector< int >& rule = normalRules[ no[ i ] ];
            if( rule.empty() )
                continue;
            unsigned k = 1;
            for( unsigned j = 1; j < rule.size(); ++j )
            {
                if( rule[ j ] != -static_cast< int >( head ) )
                    rule[ k++ ] = rule[ j ];
            }
            if( k == 1 )
            {
                if( !facts[ rule[ 0 ] ] )
                {
                    trace_msg( parser, 5, "New fact: " << rule[ 0 ] );
                    facts[ rule[ 0 ] ] = true;
                    stack.push_back( rule[ 0 ] );
                }
            }
            else
                rule.resize( k );
        }
        no.clear();
    }while( !stack.empty() );
}

//Literal
//GringoNumericFormat::getBodyLiteral(
//    Clause* body )
//{
//    assert( body->size() >= 2 );
//    assert( body->allUndefined() );
//    
//    bodiesDictionary.startInsertion();
//    for( unsigned i = 0; i < body->size(); ++i )
//    {
//        bodiesDictionary.addElement( body->getAt( i ).getId() );
//    }
//    unsigned id = bodiesDictionary.endInsertion();
//    if( id < auxVarId.size() )
//        solver.releaseClause( body );
//    else
//    {
//        Literal lit = getLiteralForAuxVar( id, NEGATIVE );
//        
//        for( unsigned i = 0; i < body->size(); ++i )
//        {
//            Clause* clause = solver.newClause();
//            clause->addLiteral( lit );
//            clause->addLiteral( body->getAt( i ).getOppositeLiteral() );
//            if( !solver.addClause( clause ) )
//                consistent = false;
//        }
//        
//        body->addLiteral( lit.getOppositeLiteral() );
//        if( !solver.addClause( body ) )
//            consistent = false;
//        assert( supportVectorAuxVar[ id ] == NULL );
//        supportVectorAuxVar[ id ] = body;
//    }
//    return getLiteralForAuxVar( id, NEGATIVE );
//}
//
//void
//GringoNumericFormat::addNormalRule(
//    unsigned head,
//    Literal body )
//{
//    Clause* clause = solver.newClause();
//    if( clause->addUndefinedLiteral( getLiteralForInputVar( head, POSITIVE ) ) && clause->addUndefinedLiteral( body ) )
//    {
//        if( !solver.addClause( clause ) )
//            consistent = false;
//    }
//    else
//    {
//        solver.releaseClause( clause );
//    }
//    
//    supportVectorInputVar[ head ]->addLiteral( body.getOppositeLiteral() );
//}
//
//Clause*
//GringoNumericFormat::readBody( 
//    istream& input,
//    vector< Variable* >& truePositiveLiterals )
//{
//    unsigned  bodySize, negativeSize, tmp;
//    input >> bodySize >> negativeSize;
// 
//    if( bodySize < negativeSize )
//        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
//   
//    bool remove = false;
//    Clause* clause = solver.newClause();
//    while( negativeSize-- > 0 )
//    {
//        input >> tmp;
//        if( !clause->addUndefinedLiteral( getLiteralForInputVar( tmp, POSITIVE ) ) )
//            remove = true;
//        --bodySize;
//    }
//    while( bodySize-- > 0 )
//    {
//        input >> tmp;
//        if( !clause->addUndefinedLiteral( getLiteralForInputVar( tmp, NEGATIVE ) ) )
//            remove = true;
//        if( solver.getVariable( inputVarId[ tmp ] )->isTrue() )
//            truePositiveLiterals.push_back( solver.getVariable( inputVarId[ tmp ] ) );
//    }
//    
//    if( remove )
//    {
//        solver.releaseClause( clause );
//        return NULL;
//    }
//    
//    return clause;
//}
//
//Literal
//GringoNumericFormat::getLiteralForInputVar( 
//    unsigned int id,
//    unsigned int sign )
//{
//    assert( id != 0 );
//    while( inputVarId.size() <= id )
//    {
//        solver.addVariable();
//        inputVarId.push_back( solver.numberOfVariables() );
//        supportVectorInputVar.push_back( solver.newClause() );
//        trueVariablesRemovedFromSupportVectorInputVar.push_back( vector< Variable* >() );
//    }
//    return Literal( solver.getVariable( inputVarId[ id ] ), sign );
//}
//
//Literal
//GringoNumericFormat::getLiteralForAuxVar( 
//    unsigned int id, 
//    unsigned int sign )
//{
//    while( auxVarId.size() <= id )
//    {
//        solver.addVariable();
//        auxVarId.push_back( solver.numberOfVariables() );
//        supportVectorAuxVar.push_back( NULL );
//        trueVariablesRemovedFromSupportVectorAuxVar.push_back( vector< Variable* >() );
//    }
//    return Literal( solver.getVariable( auxVarId[ id ] ), sign );
//}
//
//void
//GringoNumericFormat::addSupportClauses()
//{
//    for( unsigned i = 1; i < inputVarId.size(); ++i )
//    {
//        Clause* clause = supportVectorInputVar[ i ];
//        if( clause->size() > 0 && clause->getAt( 0 ) == getLiteralForInputVar( 1, NEGATIVE ) )
//        {
//            solver.releaseClause( clause );
//            continue;
//        }
//        
//        if( !clause->addUndefinedLiteral( getLiteralForInputVar( i, NEGATIVE ) ) || clause->removeDuplicatesAndFalseAndCheckIfTautological() )
//        {
//            solver.releaseClause( clause );
//            continue;
//        }
//        
//        consistent = solver.addClause( clause );
//        if( !consistent )
//            return;
//    }
//}

void
GringoNumericFormat::readAtomsTable(
    istream& input )
{
    unsigned int nextAtom;
    input >> nextAtom;

    char name[ 1024 ];
    while( nextAtom != 0 )
    {
//        assert_msg( nextAtom < inputVarId.size(), "nextAtom = " << nextAtom << "; size = " << inputVarId.size() );
//        assert( inputVarId[ nextAtom ] > 1 );
        
        input.getline( name, 1024 );
        VariableNames::setName( solver.getVariable( nextAtom ), name+1 );
        trace_msg( parser, 6, "Set name " << name+1 << " for atom " << nextAtom );
        input >> nextAtom;
    }
    
//    #ifdef TRACE_ON
//    for( unsigned i = 1; i < inputVarId.size(); ++i )
//    {
//        Variable* variable = solver.getVariable( inputVarId[ i ] );
//        if( VariableNames::isHidden( variable ) )
//        {
//            stringstream s;
//            s << "#hidden" << i;
//            string name = s.str();
//            VariableNames::setName( variable, name );
//        }
//    }
//    for( unsigned i = 1; i < auxVarId.size(); ++i )
//    {
//        Variable* variable = solver.getVariable( auxVarId[ i ] );
//        if( VariableNames::isHidden( variable ) )
//        {
//            stringstream s;
//            s << "#aux" << i;
//            string name = s.str();
//            VariableNames::setName( variable, name );
//        }
//    }
//    #endif
}

void
GringoNumericFormat::readTrueAtoms(
    istream& input )
{
    char b;
    
    input >> b;
    assert( b == 'B' );
    input >> b;
    assert( b == '+' );
    
    unsigned int nextAtom;
    input >> nextAtom;
    
    while( nextAtom != 0 )
    {
        cout << nextAtom << endl;
        solver.addClause( Literal( solver.getVariable( nextAtom ), NEGATIVE ) );
        input >> nextAtom;
    }
}

void
GringoNumericFormat::readFalseAtoms(
    istream& input )
{
    char b;
    
    input >> b;
    input >> b;
    
    unsigned int nextAtom;
    input >> nextAtom;

    while( nextAtom != 0 )
    {
        solver.addClause( Literal( solver.getVariable( nextAtom ), NEGATIVE ) );
        input >> nextAtom;
    }
}

void
GringoNumericFormat::readErrorNumber(
    istream& input )
{
    unsigned int errorNumber;
    input >> errorNumber;
    
    if( errorNumber != 1 )
    {
        stringstream ss;
        ss << "read error message number " << errorNumber;
        ErrorMessage::errorDuringParsing( ss.str() );
        exit( 0 );
    }
    else
    {
    }
}

void
GringoNumericFormat::programIsNotTight()
{
    //Add two fake positions
    solver.addGUSData( NULL );
    solver.addGUSData( NULL );
    for( unsigned int i = 2; i <= solver.numberOfVariables(); i++ )
    {
        GUSData* gd = new GUSData();
        gd->variable = solver.getVariable( i );
        solver.addGUSData( gd );        
    }
    
    trace_msg( parser, 2, "Program is not tight. Number of cyclic components " << solver.getNumberOfCyclicComponents() );
    for( unsigned int i = 0; i < solver.getNumberOfCyclicComponents(); i++ )
    {
        Component* component = solver.getCyclicComponent( i );
        
        for( unsigned int j = 0; j < component->size(); j++ )
        {
            unsigned int varId = component->getVariable( j );
            assert( varId <= solver.numberOfVariables() );
            trace_msg( parser, 2, "Variable " << *solver.getVariable( varId ) << " is in the cyclic component " << i );
            Variable* currentVariable = solver.getVariable( varId );
            currentVariable->setComponent( component );
            component->variableHasNoSourcePointer( currentVariable );
        }
        
        solver.addPostPropagator( component );
    }                
    
    unordered_map< Variable*, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];    
    
    for( unsigned int i = 2; i < headOccurrences.size(); i++ )
    {
        assert( i <= solver.numberOfVariables() );
        Variable* variable = solver.getVariable( i );

        vector< unsigned >& ho = headOccurrences[ i ];
        // skip acyclic variables
        if( !variable->isInCyclicComponent() || numberOfHeadOccurrences[ variable->getId() ] == 0 )
            continue;

        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << ", which has " << numberOfHeadOccurrences[ variable->getId() ] << " supporting rules" );
        
        Component* component = variable->getComponent();
        assert( component != NULL );
        
        variable->setFrozen();
        
        for( unsigned int j = 0; j < ho.size(); j++ )
        {
            if( normalRules[ ho[ j ] ].empty() )
                continue;
            assert( normalRules[ ho[ j ] ].size() > 1 );
            int id = normalRules[ ho[ j ] ][ 0 ];
            if( id > 0 )
            {
                Variable* ruleVar = solver.getVariable( id );
                if( variable->inTheSameComponent( ruleVar ) )
                {
                    trace_msg( parser, 3, "Adding " << *ruleVar << " as internal rule for " << *variable );
                    component->addInternalLiteralForVariable( variable->getId(), Literal( ruleVar, POSITIVE ) );
                    component->addVariablePossiblySupportedByLiteral( variable, Literal( ruleVar, POSITIVE ) );
                }
                else
                {
                    trace_msg( parser, 3, "Adding " << *ruleVar << " as external rule for " << *variable );
                    component->addExternalLiteralForVariable( variable->getId(), Literal( ruleVar, POSITIVE ) );
                }
                
                if( literalsPostPropagator[ POSITIVE ][ ruleVar ].insert( component ).second )            
                    Literal( ruleVar, POSITIVE ).addPostPropagator( component );
                
                ruleVar->setFrozen();
            }
            else
            {
                Variable* ruleVar = solver.getVariable( -id );
                trace_msg( parser, 3, "Adding not " << *ruleVar << " as external rule for " << *variable );
                component->addExternalLiteralForVariable( variable->getId(), Literal( ruleVar, NEGATIVE ) );
                
                if( literalsPostPropagator[ NEGATIVE ][ ruleVar ].insert( component ).second )            
                    Literal( ruleVar, NEGATIVE ).addPostPropagator( component );
            
                ruleVar->setFrozen();
            }
        }
    }    
    
    for( unsigned int i = 0; i < normalRules.size(); i++ )
    {
        if( normalRules[ i ].empty() )
            continue;
        vector< int >& rule = normalRules[ i ];
        assert( rule.size() > 1 );
        if( rule[ 0 ] < 0 || ( rule[ 0 ] < static_cast< int >( headOccurrences.size() ) && numberOfHeadOccurrences[ rule[ 0 ] ] > 0 ) ) 
            continue;
        assert( static_cast< unsigned >( rule[ 0 ] ) <= solver.numberOfVariables() );
        Variable* variable = solver.getVariable( rule[ 0 ] );
        
        // skip acyclic variables, false variables and facts
        if( !variable->isInCyclicComponent() )
            continue;

        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " whose supporting rule has " << rule.size()-1 << " literals" );

        variable->setFrozen();
        
        Component* component = variable->getComponent();
        component->setAuxVariable( variable->getId() );
        
        assert( component != NULL );

        for( unsigned int j = 1; j < rule.size(); j++ )
        {
            if( rule[ j ] > 0 )
            {
                assert( static_cast< unsigned >( rule[ j ] ) <= solver.numberOfVariables() );
                if( solver.getVariable( rule[ j ] )->inTheSameComponent( variable ) )
                {
                    Literal lit( solver.getVariable( rule[ j ] ), POSITIVE );
                    trace_msg( parser, 3, "Adding " << lit << " to the supporting rule of " << *variable );
                    component->addAuxVariableSupportedByLiteral( variable, lit );
                    component->addInternalLiteralForVariable( variable->getId(), lit );
                    solver.getVariable( rule[ j ] )->setFrozen();
                }
            }
            else
            {
                assert( static_cast< unsigned >( -rule[ j ] ) <= solver.numberOfVariables() );
                solver.getVariable( -rule[ j ] )->setFrozen();
            }
        }
    }    
}

void
GringoNumericFormat::createStructures(
    unsigned id )
{
    while( id >= headOccurrences.size() )
    {
        solver.addVariable();
        solver.addEdgeInDependencyGraph( solver.numberOfVariables(), 0 );
        facts.push_back( false );
        numberOfHeadOccurrences.push_back( 0 );
        headOccurrences.push_back( vector< unsigned >() );
        posOccurrences.push_back( vector< unsigned >() );
        negOccurrences.push_back( vector< unsigned >() );
    }
}

void
GringoNumericFormat::computeCompletion()
{
    trace_msg( parser, 2, "Computing completion" );
    
    trace_msg( parser, 3, "Simplify" );
    bool simplify = true;
    do{
        simplify = false;
        for( unsigned i = 2; i < headOccurrences.size(); ++i )
        {
            if( !facts[ i ] && !solver.getVariable( i )->isFalse() && numberOfHeadOccurrences[ i ] == 0 )
            {
                trace_msg( parser, 4, "Infer falsity of variable " << *solver.getVariable( i ) << " with no support" );
                solver.addClause( Literal( solver.getVariable( i ), NEGATIVE ) );
                assert( solver.getVariable( i )->isFalse() || solver.conflictDetected() );
                simplify = true;
                propagate();
            }
        }
        if( solver.conflictDetected() )
            return;
    }while( simplify );
    
    trace_msg( parser, 3, "Processing normal rules" );
    for( unsigned i = 0; i < normalRules.size(); ++i )
    {
        vector< int >& rule = normalRules[ i ];
        if( rule.empty() )
            continue;
            
        assert( rule.size() >= 2 );
        assert( numberOfHeadOccurrences[ rule[ 0 ] ] >= 1 );
        
        if( solver.getVariable( rule[ 0 ] )->isFalse() )
        {
            Clause* clause = solver.newClause();
            for( unsigned j = 1; j < rule.size(); ++j )
                if( rule[ j ] > 0 )
                    clause->addLiteral( Literal( solver.getVariable( rule[ j ] ), NEGATIVE ) );
                else
                    clause->addLiteral( Literal( solver.getVariable( -rule[ j ] ), POSITIVE ) );
            trace_msg( parser, 4, "Adding clause " << *clause );
            solver.cleanAndAddClause( clause );
            trace_msg( parser, 5, "Actually added clause " << *clause );
            rule.clear();
            continue;
        }
        
        if( rule.size() == 2 )
        {
            Clause* clause = solver.newClause();
            clause->addLiteral( Literal( solver.getVariable( rule[ 0 ] ), POSITIVE ) );
            if( rule[ 1 ] > 0 )
            {
                clause->addLiteral( Literal( solver.getVariable( rule[ 1 ] ), NEGATIVE ) );
                solver.addEdgeInDependencyGraph( rule[ 0 ], rule[ 1 ] );
            }
            else
                clause->addLiteral( Literal( solver.getVariable( -rule[ 1 ] ), POSITIVE ) );
            trace_msg( parser, 4, "Adding clause " << *clause );
            solver.cleanAndAddClause( clause );
            trace_msg( parser, 5, "Actually added clause " << *clause );
            rule[ 0 ] = rule[ 1 ];
            continue;
        }
        
        if( numberOfHeadOccurrences[ rule[ 0 ] ] > 1 )
        {
            solver.addVariable();
            trace_msg( parser, 3, "Adding variable " << solver.numberOfVariables() << " for rule " << i );
            
            assert( !solver.getVariable( rule[ 0 ] )->isFalse() );
            if( solver.getVariable( rule[ 0 ] )->isUndefined() )
            {
                Clause* clause = solver.newClause();
                clause->addLiteral( Literal( solver.getVariable( rule[ 0 ] ), POSITIVE ) );
                clause->addLiteral( Literal( solver.getVariable( solver.numberOfVariables() ), NEGATIVE ) );
                trace_msg( parser, 4, "Adding clause " << *clause );
                solver.cleanAndAddClause( clause );
                trace_msg( parser, 5, "Actually added clause " << *clause );
            }
            solver.addEdgeInDependencyGraph( rule[ 0 ], solver.numberOfVariables() );
            rule[ 0 ] = solver.numberOfVariables();
        }
        else
            numberOfHeadOccurrences[ rule[ 0 ] ] = 0;
        
        Clause* clause = solver.newClause();
        clause->addLiteral( Literal( solver.getVariable( rule[ 0 ] ), POSITIVE ) );
        for( unsigned j = 1; j < rule.size(); ++j )
        {
            if( rule[ j ] > 0 )
            {
                clause->addLiteral( Literal( solver.getVariable( rule[ j ] ), NEGATIVE ) );
                
                Clause* binClause = solver.newClause();
                binClause->addLiteral( Literal( solver.getVariable( rule[ 0 ] ), NEGATIVE ) );
                binClause->addLiteral( Literal( solver.getVariable( rule[ j ] ), POSITIVE ) );
                trace_msg( parser, 4, "Adding clause " << *binClause );
                solver.cleanAndAddClause( binClause );
                trace_msg( parser, 5, "Actually added clause " << *binClause );
                solver.addEdgeInDependencyGraph( rule[ 0 ], rule[ j ] );
            }
            else
            {
                clause->addLiteral( Literal( solver.getVariable( -rule[ j ] ), POSITIVE ) );

                Clause* binClause = solver.newClause();
                binClause->addLiteral( Literal( solver.getVariable( rule[ 0 ] ), NEGATIVE ) );
                binClause->addLiteral( Literal( solver.getVariable( -rule[ j ] ), NEGATIVE ) );
                trace_msg( parser, 4, "Adding clause " << *binClause );
                solver.cleanAndAddClause( binClause );
                trace_msg( parser, 5, "Actually added clause " << *binClause );
            }
        }
        trace_msg( parser, 4, "Adding clause " << *clause );
        solver.cleanAndAddClause( clause );
        trace_msg( parser, 5, "Actually added clause " << *clause );
    }
    
    trace_msg( parser, 3, "Processing input variables" );
    for( unsigned i = 2; i < headOccurrences.size(); ++i )
    {
        if( facts[ i ] || solver.getVariable( i )->isFalse() )
        {
            assert( !facts[ i ] || solver.getVariable( i )->isTrue() );
            assert( !facts[ i ] || headOccurrences[ i ].empty() );
            continue;
        }
        
//        assert( numberOfHeadOccurrences[ i ] > 0 );
        if( numberOfHeadOccurrences[ i ] == 0 )
        {
            continue;
        }

        vector< unsigned >& ho = headOccurrences[ i ];
        Clause* clause = solver.newClause();
        clause->addLiteral( Literal( solver.getVariable( i ), NEGATIVE ) );
        for( unsigned j = 0; j < ho.size(); ++j )
        {
            if( normalRules[ ho[ j ] ].empty() )
                continue;
            int id = normalRules[ ho[ j ] ][ 0 ];
            if( id > 0 )
                clause->addLiteral( Literal( solver.getVariable( id ), POSITIVE ) );
            else
                clause->addLiteral( Literal( solver.getVariable( -id ), NEGATIVE ) );
        }
        trace_msg( parser, 4, "Adding clause " << *clause );
        solver.cleanAndAddClause( clause );
        trace_msg( parser, 5, "Actually added clause " << *clause );
    }
    
    trace_msg( parser, 3, "Computing SSCs" );
    solver.computeStrongConnectedComponents();
}
