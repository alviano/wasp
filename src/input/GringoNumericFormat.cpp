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
    trace_msg( parser, 1, "Start parsing..." );
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

    simplify();

//    cout << "cc" << endl;
    computeSCCs();
    if( !solver.tight() )
    {
        trace_msg( parser, 1, "Program is not tight" );
        computeGusStructures();
    } else
    {
        trace_msg( parser, 1, "Program is tight" );
    }
    computeCompletion();

    //TODO: remove
//    cout << solver.numberOfVariables() << endl;
//    unsigned c[1024] = {0};
//    for( unsigned i = 0; i < solver.numberOfClauses(); i++ )
//        c[solver.clauseAt( i )->size()]++;
//    cout << "occs\n";
//    for( unsigned i = 1; i < 10; i++)
//        cout << c[i] << endl;
    cout << "Solving..." << endl;
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
    {
        int bodySize, negativeSize;
        readBodySize( input, bodySize, negativeSize );

        if( atomData[ head ].isSupported() )
            skipLiterals( input, bodySize );
        else if( bodySize == 0 )
            addFact( head );
        else
            readNormalRule( input, head, bodySize, negativeSize );
    }
}

void
GringoNumericFormat::readBodySize(
    istream& input,
    int& bodySize,
    int& negativeSize )
{
    input >> bodySize >> negativeSize;

    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
}

void
GringoNumericFormat::skipLiterals(
    istream& input,
    unsigned howMany )
{
    unsigned tmp;
    while( howMany-- > 0 )
        input >> tmp;
}

void
GringoNumericFormat::readNormalRule(
    istream& input,
    unsigned head,
    int bodySize,
    int negativeSize )
{
    assert( !solver.getVariable( head )->isFalse() );
    assert( !atomData[ head ].isSupported() );
    assert( bodySize >= negativeSize );
    assert( negativeSize >= 0 );
    assert( bodySize >= 1 );

    readNormalRule_numberOfCalls++;
    assert( readNormalRule_numberOfCalls != 0 );

    NormalRule* rule = new NormalRule( head );
    unsigned tmp;
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input >> tmp;
        createStructures( tmp );
        
        if( atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.getVariable( tmp )->isTrue() )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else if( solver.getVariable( tmp )->isUndefined() )
        {
            rule->addNegativeLiteral( tmp );
            atomData[ tmp ].readNormalRule_negativeLiterals = readNormalRule_numberOfCalls;
        }
    }
    while( bodySize-- > 0 )
    {
        input >> tmp;
        createStructures( tmp );

        if( atomData[ tmp ].readNormalRule_positiveLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.getVariable( tmp )->isFalse() || tmp == head || atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else 
        {
            if( solver.getVariable( tmp )->isUndefined() )
                rule->addPositiveLiteral( tmp );
            else
                rule->addPositiveTrueLiteral( tmp );
            atomData[ tmp ].readNormalRule_positiveLiterals = readNormalRule_numberOfCalls;
        }
    }

    if( rule->isFact() )
    {
        delete rule;
        addFact( head );
    }
    else
    {
        if( rule->isFiring() )
            solver.addClause( Literal( solver.getVariable( head ), POSITIVE ) );
        add( rule );
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

    Clause* clause = solver.newClause();
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input >> tmp;
        if( !clause->addUndefinedLiteral( Literal( solver.getVariable( tmp ), POSITIVE ) ) )
        {
            solver.releaseClause( clause );
            skipLiterals( input, bodySize );
            return;
        }
    }
    while( bodySize-- > 0 )
    {
        input >> tmp;
        if( !clause->addUndefinedLiteral( Literal( solver.getVariable( tmp ), NEGATIVE ) ) )
        {
            solver.releaseClause( clause );
            skipLiterals( input, bodySize );
            return;
        }
    }

    solver.addClause( clause );
}

void
GringoNumericFormat::propagate()
{
    bool repeat;
    do {
        repeat = false;
        while( propagatedLiterals < solver.numberOfAssignedLiterals() )
        {
            repeat = true;
            Variable* var = solver.getAssignedVariable( propagatedLiterals++ );
            if( var->isTrue() )
                propagateTrue( var );
            else
                propagateFalse( var );
        }
        while( !facts.empty() )
        {
            repeat = true;
            Variable* var = solver.getVariable( facts.back() );
            facts.pop_back();
            propagateFact( var );
        }
    }while( repeat );
}

void
GringoNumericFormat::propagateTrue(
    Variable* var)
{
    assert( var != NULL );
    assert( var->isTrue() );

    assert( var->getId() < atomData.size() );
    AtomData& data = atomData[ var->getId() ];

    trace_msg( parser, 2, "Propagating " << *var << " as true" << ( data.isSupported() ? " (supported)" : "" ) );

    unsigned j = 0;
    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        NormalRule* rule = data.posOccurrences[ i ];
        assert( rule != NULL );
        if( !shrinkPos( rule, var->getId() ) )
            data.posOccurrences[ j++ ] = data.posOccurrences[ i ];
    }
    data.posOccurrences.resize( j );

    for( unsigned i = 0; i < data.negOccurrences.size(); ++i )
    {
        NormalRule* rule = data.negOccurrences[ i ];
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.negOccurrences.clear();
}

void
GringoNumericFormat::propagateFalse(
    Variable* var )
{
    assert( var != NULL );
    assert( var->isFalse() );

    assert( var->getId() < atomData.size() );
    AtomData& data = atomData[ var->getId() ];
    
    trace_msg( parser, 2, "Propagating " << *var << " as false" );
    
    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        NormalRule* rule = data.headOccurrences[ i ];
        assert( rule != NULL );
        if( rule->isRemoved() )
            continue;
        trace_msg( parser, 3, "Replacing " << *rule << " by a constraint" );
        Clause* clause = solver.newClause();
        for( unsigned j = 0; j < rule->negBody.size(); ++j )
            clause->addLiteral( Literal( solver.getVariable( rule->negBody[ j ] ), POSITIVE ) );
        for( unsigned j = 0; j < rule->posBody.size(); ++j )
            clause->addLiteral( Literal( solver.getVariable( rule->posBody[ j ] ), NEGATIVE ) );
        solver.cleanAndAddClause( clause );
        rule->remove();
    }
    data.numberOfHeadOccurrences = 0;
    data.headOccurrences.clear();

    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        NormalRule* rule = data.posOccurrences[ i ];
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.posOccurrences.clear();

    for( unsigned i = 0; i < data.negOccurrences.size(); ++i )
    {
        NormalRule* rule = data.negOccurrences[ i ];
        assert( rule != NULL );
        shrinkNeg( rule, var->getId() );
    }
    data.negOccurrences.clear();
}

void
GringoNumericFormat::propagateFact(
    Variable* var )
{
    assert( var->getId() < atomData.size() );
    AtomData& data = atomData[ var->getId() ];
    
    trace_msg( parser, 2, "Propagating fact " << *var );

    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        NormalRule* rule = data.headOccurrences[ i ];
        assert( rule != NULL );
        if( rule->isRemoved() )
            continue;
        trace_msg( parser, 3, "Removing rule " << *rule );
        rule->remove();
    }
    data.numberOfHeadOccurrences = 0;
    data.headOccurrences.clear();
    
    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        NormalRule* rule = data.posOccurrences[ i ];
        assert( rule != NULL );
        shrinkPos( rule, var->getId() );
    }
    data.posOccurrences.clear();
}

void
GringoNumericFormat::addFact(
    unsigned head )
{
    if( atomData[ head ].isSupported() )
        return;

    trace_msg( parser, 3, "Adding fact " << head );
    atomData[ head ].setSupported();
    solver.addClause( Literal( solver.getVariable( head ), POSITIVE ) );
    facts.push_back( head );
}

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
        solver.addClause( Literal( solver.getVariable( nextAtom ), POSITIVE ) );
        input >> nextAtom;
    }
}

void
GringoNumericFormat::readFalseAtoms(
    istream& input )
{
    char b;

    input >> b;
    assert( b == 'B' );
    input >> b;
    assert( b == '-' );

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

    if( errorNumber != 1 ) {
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
GringoNumericFormat::processRecursivePositiveCrule(
    Clause* crule )
{
    assert( crule != NULL );
    assert( crule->getAt( 0 ).isPositive() );
    Variable* variable = crule->getAt( 0 ).getVariable();
    assert( variable->isInCyclicComponent() );

    trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " whose supporting rule has " << crule->size()-1 << " literals" );
    variable->setFrozen();

    Component* component = variable->getComponent();
    assert( component != NULL );
    component->setAuxVariable( variable->getId() );

    for( unsigned int j = 1; j < crule->size(); j++ )
    {
        crule->getAt( j ).getVariable()->setFrozen();
        if( crule->getAt( j ).isNegative() )
        {
            if( crule->getAt( j ).getVariable()->inTheSameComponent( variable ) )
            {
                Literal lit = crule->getAt( j ).getOppositeLiteral();
                trace_msg( parser, 3, "Adding " << lit << " to the supporting rule of " << *variable );
                component->addAuxVariableSupportedByLiteral( variable, lit );
                component->addInternalLiteralForVariable( variable->getId(), lit );
            }
        }
    }
}

void
GringoNumericFormat::processRecursiveNegativeCrule(
    Clause* crule )
{
    assert( crule != NULL );
    assert( crule->getAt( 0 ).isNegative() );
    Variable* variable = crule->getAt( 0 ).getVariable();
    assert( variable->isInCyclicComponent() );

    trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << ", which has " << crule->size()-1 << " supporting rules" );

    Component* component = variable->getComponent();
    assert( component != NULL );

    for( unsigned int j = 1; j < crule->size(); j++ )
    {
        Variable* ruleVar = crule->getAt( j ).getVariable();
        ruleVar->setFrozen();
        if( literalsPostPropagator[ crule->getAt( j ).getSign() ][ ruleVar ].insert( component ).second )
            Literal( ruleVar, crule->getAt( j ).getSign() ).addPostPropagator( component );

        if( crule->getAt( j ).isPositive() && variable->inTheSameComponent( ruleVar ) )
        {
            trace_msg( parser, 3, "Adding " << *ruleVar << " as internal rule for " << *variable );
            component->addInternalLiteralForVariable( variable->getId(), Literal( ruleVar, POSITIVE ) );
            component->addVariablePossiblySupportedByLiteral( variable, Literal( ruleVar, POSITIVE ) );
        }
        else
        {
            trace_msg( parser, 3, "Adding " << crule->getAt( j ) << " as external rule for " << *variable );
            component->addExternalLiteralForVariable( variable->getId(), crule->getAt( j ) );
        }
    }    
}

void
GringoNumericFormat::computeGusStructures()
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

    for( unsigned int i = 0; i < crules.size(); i++ )
    {
        Clause* crule = crules[ i ];
        assert( crule != NULL );

        // skip acyclic variables
        if( !crule->getAt( 0 ).getVariable()->isInCyclicComponent() )
            continue;

        if( crule->getAt( 0 ).isPositive() )
            processRecursivePositiveCrule( crule );
        else
            processRecursiveNegativeCrule( crule );
    }
    
//    unsigned count = 0;
//    for( unsigned int i = 0; i < solver.getNumberOfCyclicComponents(); i++ )
//    {
//        bool found = false;
//        Component* component = solver.getCyclicComponent( i );
//        for( unsigned int j = 0; j < component->size(); j++ )
//        {
//            unsigned int varId = component->getVariable( j );
//            if( !component->isAuxVariable( varId ) )
//            {
//                cout << varId << endl;
//                found = true;
//                break;
//            }
//        }
//        if( found ) count++;
//    }
//    cout << count << " " << solver.getNumberOfCyclicComponents() << endl;
}

void
GringoNumericFormat::createStructures(
    unsigned id )
{
    while( id >= atomData.size() ) {
        solver.addVariable();
        solver.addEdgeInDependencyGraph( solver.numberOfVariables(), 0 );
        atomData.push_back( AtomData( false ) );
    }
}

void
GringoNumericFormat::simplify()
{
    trace_msg( parser, 1, "Simplify" );
    atomsWithoutSupport.clear();
    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        if( !atomData[ i ].isSupported() && !solver.getVariable( i )->isFalse() && atomData[ i ].numberOfHeadOccurrences == 0 )
            atomsWithoutSupport.push_back( i );
    }
    
    while( !atomsWithoutSupport.empty() )
    {
        trace_msg( parser, 2, "Infer falsity of variable " << *solver.getVariable( atomsWithoutSupport.back() ) << " with no support" );
        solver.addClause( Literal( solver.getVariable( atomsWithoutSupport.back() ), NEGATIVE ) );
        assert( solver.getVariable( atomsWithoutSupport.back() )->isFalse() || solver.conflictDetected() );
        atomsWithoutSupport.pop_back();
        propagate();
        if( solver.conflictDetected() )
            return;
    }
}

void
GringoNumericFormat::createCrule(
    Literal head,
    NormalRule* rule )
{
    assert( head.isPositive() );
    Clause* crule = solver.newClause();
    crule->addLiteral( head );
    for( unsigned k = 0; k < rule->negBody.size(); ++k )
        crule->addLiteral( Literal( solver.getVariable( rule->negBody[ k ] ), POSITIVE ) );
    for( unsigned k = 0; k < rule->posBody.size(); ++k )
    {
        crule->addLiteral( Literal( solver.getVariable( rule->posBody[ k ] ), NEGATIVE ) );
        solver.addEdgeInDependencyGraph( head.getVariable()->getId(), rule->posBody[ k ] );
    }
    for( unsigned k = 0; k < rule->posBodyTrue.size(); ++k )
    {
        crule->addLiteral( Literal( solver.getVariable( rule->posBodyTrue[ k ] ), NEGATIVE ) );
        solver.addEdgeInDependencyGraph( head.getVariable()->getId(), rule->posBodyTrue[ k ] );
    }
    crules.push_back( crule );
}

void
GringoNumericFormat::computeSCCs()
{
    trace_msg( parser, 1, "Computing crules and SCCs" );

    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        AtomData& data = atomData[ i ];
        Variable* var = solver.getVariable( i );
        if( data.isSupported() || var->isFalse() )
            continue;
        assert( data.numberOfHeadOccurrences > 0 );
        
//        if( data.numberOfHeadOccurrences == 1 )
//        {
//            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
//            {
//                if( data.headOccurrences[ j ]->isRemoved() )
//                    continue;
//                createCrule( Literal( solver.getVariable( i ), POSITIVE ), data.headOccurrences[ j ] );
//                break;
//            }
//        }
//        else
        {
            Clause* crule = solver.newClause();
            crule->addLiteral( Literal( solver.getVariable( i ), NEGATIVE ) );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                if( data.headOccurrences[ j ]->isRemoved() )
                    continue;
                if( data.headOccurrences[ j ]->size() == 1 )
                {
                    if( data.headOccurrences[ j ]->negBody.size() == 1 )
                        crule->addLiteral( Literal( solver.getVariable( data.headOccurrences[ j ]->negBody[ 0 ] ), NEGATIVE ) );
                    else if( data.headOccurrences[ j ]->posBody.size() == 1 )
                    {
                        crule->addLiteral( Literal( solver.getVariable( data.headOccurrences[ j ]->posBody[ 0 ] ), POSITIVE ) );
                        solver.addEdgeInDependencyGraph( i, data.headOccurrences[ j ]->posBody[ 0 ] );
                    }
                    else
                    {
                        assert( data.headOccurrences[ j ]->posBodyTrue.size() == 1 );
                        crule->addLiteral( Literal( solver.getVariable( data.headOccurrences[ j ]->posBodyTrue[ 0 ] ), POSITIVE ) );
                        solver.addEdgeInDependencyGraph( i, data.headOccurrences[ j ]->posBodyTrue[ 0 ] );
                    }
                }
                else
                {
                    solver.addVariable();
                    crule->addLiteral( Literal( solver.getVariable( solver.numberOfVariables() ), POSITIVE ) );
                    solver.addEdgeInDependencyGraph( i, solver.numberOfVariables() );
                    createCrule( Literal( solver.getVariable( solver.numberOfVariables() ), POSITIVE ), data.headOccurrences[ j ] );
                    
                }
            }
            crules.push_back( crule );
        }
    }

    solver.computeStrongConnectedComponents();
}

void
GringoNumericFormat::computeCompletion()
{
    for( unsigned i = 0; i < crules.size(); ++i )
    {
        Clause* crule = crules[ i ];
        assert( crule != NULL );
        Literal lit = crule->getAt( 0 ).getOppositeLiteral();
        for( unsigned j = 1; j < crule->size(); ++j )
        {
            if( lit.isTrue() )
                break;
                
            Literal lit2 = crule->getAt( j ).getOppositeLiteral();
            if( lit2.isTrue() )
                continue;
                
            Clause* bin = solver.newClause();
            if( lit.isUndefined() )
                bin->addLiteral( lit );
            if( lit2.isUndefined() )
                bin->addLiteral( lit2 );
            assert( bin->size() >= 1 );
            solver.addClause( bin );
        }
        solver.cleanAndAddClause( crule );
    }
}

void
GringoNumericFormat::add(
    NormalRule* rule )
{
    assert( rule != NULL );
    assert( !rule->isFact() );

    trace_msg( parser, 2, "Adding rule " << *rule );
    normalRules.push_back( rule );

    AtomData& headData = atomData[ rule->head ];
    headData.headOccurrences.push_back( rule );
    headData.numberOfHeadOccurrences++;
    for( unsigned i = 0; i < rule->negBody.size(); ++i )
        atomData[ rule->negBody[ i ] ].negOccurrences.push_back( rule );
    for( unsigned i = 0; i < rule->posBody.size(); ++i )
        atomData[ rule->posBody[ i ] ].posOccurrences.push_back( rule );
    for( unsigned i = 0; i < rule->posBodyTrue.size(); ++i )
        atomData[ rule->posBodyTrue[ i ] ].posOccurrences.push_back( rule );
}

void
GringoNumericFormat::removeAndCheckSupport(
    NormalRule* rule )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Removing rule " << *rule );
    if( --atomData[ rule->head ].numberOfHeadOccurrences == 0 )
        atomsWithoutSupport.push_back( rule->head );
    rule->remove();    
}

bool
GringoNumericFormat::shrinkPos( 
    NormalRule* rule, 
    unsigned lit )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return true;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->posBody.size(); ++j )
    {
        if( rule->posBody[ j ] == lit )
        {
            rule->posBody[ j ] = rule->posBody.back();
            rule->posBody.pop_back();
            if( !atomData[ lit ].isSupported() )
            {
                rule->posBodyTrue.push_back( lit );
                onShrinking( rule );
                return false;
            }
            onShrinking( rule );
            return true;
        }
    }
    assert( atomData[ lit ].isSupported() );
    for( unsigned j = 0; j < rule->posBodyTrue.size(); ++j )
    {
        if( rule->posBodyTrue[ j ] == lit )
        {
            rule->posBodyTrue[ j ] = rule->posBodyTrue.back();
            rule->posBodyTrue.pop_back();
            onShrinking( rule );
            return true;
        }
    }
    assert( 0 );
    return false;
}

void
GringoNumericFormat::onShrinking(
    NormalRule* rule )
{
    assert( rule != NULL );
    trace_msg( parser, 4, "After shrink: " << *rule );
    if( rule->isFact() )
    {
        addFact( rule->head );
        rule->remove();
    }
    else if( rule->isFiring() )
        solver.addClause( Literal( solver.getVariable( rule->head ), POSITIVE ) );
}

void
GringoNumericFormat::shrinkNeg( 
    NormalRule* rule, 
    unsigned lit )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->negBody.size(); ++j )
    {
        if( rule->negBody[ j ] == lit )
        {
            rule->negBody[ j ] = rule->negBody.back();
            rule->negBody.pop_back();
            onShrinking( rule );
            return;
        }
    }
    assert( 0 );
}

ostream&
operator<<( 
    ostream& out, 
    const GringoNumericFormat::NormalRule& rule )
{
    out << "{head: " << rule.head << ", negBody: [ ";
    for( unsigned i = 0; i < rule.negBody.size(); ++i )
        out << rule.negBody[ i ] << " ";
    out << "], posBody: [ ";
    for( unsigned i = 0; i < rule.posBody.size(); ++i )
        out << rule.posBody[ i ] << " ";
    out << "], posBodyTrue: [ ";
    for( unsigned i = 0; i < rule.posBodyTrue.size(); ++i )
        out << rule.posBodyTrue[ i ] << " ";
    return out << "]}";
}