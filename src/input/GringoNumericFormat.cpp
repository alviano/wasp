#include "GringoNumericFormat.h"

#include "../util/Constants.h"
#include "../util/ErrorMessage.h"
#include "../Clause.h"
#include "../Aggregate.h"
#include "../util/Istream.h"
#include "../HCComponent.h"
#include "../ExternalPropagator.h"

#include <cassert>
#include <iostream>
#include <unordered_set>
#include <bitset>
#include <stdint.h>
using namespace std;

void
GringoNumericFormat::parse()
{
    Istream input( cin );
    this->parse( input );
}

void
GringoNumericFormat::parse(
    Istream& input )
{
    trace_msg( parser, 1, "Start parsing..." );
    bool loop = true;

    uint64_t value = 0;
    statistics( &solver, startParsing() );
    addExternalPropagators();
    while( loop )
    {
        unsigned int type;
        input.read( type );

        value = normalRules.size();
        if( value != 0 && value % 10000000 == 0 )
        {
            cleanData();
        }
        
        if( solver.callSimplifications() && solver.numberOfClauses() + value > 1000000 )
        {
            solver.turnOffSimplifications();
            if( !usedDictionary )
                bodiesDictionary.disable();
        }

        switch( type )
        {
        case GRINGO_NORMAL_RULE_ID:
            readNormalRule( input );
            break;

        case GRINGO_CHOICE_RULE_ID:
            readChoiceRule( input );
            break;

        case GRINGO_COUNT_CONSTRAINT_RULE_ID:
            readCount( input );
            break;
            
        case GRINGO_SUM_CONSTRAINT_RULE_ID:
            readSum( input );
            break;

        case GRINGO_DISJUNCTIVE_RULE_ID:
            readDisjunctiveRule( input );
            break;
            
        case GRINGO_OPTIMIZATION_RULE_ID:
            readOptimizationRule( input );
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
    propagate();

    solver.onFinishedParsing();
//    trace_msg( parser, 1, "Apply bimander to at-most-one constraints" );
//    for( unsigned i = 0; i < delayedAggregateRewriting.size(); ++i )
//        atMostOneBimander( delayedAggregateRewriting[ i ] );
//    delayedAggregateRewriting.clear();
    
    simplify();    
    
    bodiesDictionary.clear();

    if( solver.conflictDetected() )
        return;
        
//    cout << "cc" << endl;
    if( solver.numberOfClauses() + normalRules.size() > 1000000 )
        solver.turnOffSimplifications();
    
    statistics( &solver, endParsing() );    
    statistics( &solver, startSCCs() );
    computeSCCs();
    statistics( &solver, endSCCs() );
    if( !solver.tight() )
    {
        trace_msg( parser, 1, "Program is not tight" );
        computeGusStructures();
    }
    else
    {
        trace_msg( parser, 1, "Program is tight" );
    }    
    addWeightConstraints();
    addOptimizationRules();
    clearDataStructures();    
    statistics( &solver, startCompletion() );
    computeCompletion();    
    statistics( &solver, endCompletion() );
    //TODO: remove
//    cout << solver.numberOfVariables() << endl;
//    unsigned c[1024] = {0};
//    for( unsigned i = 0; i < solver.numberOfClauses(); i++ )
//        c[solver.clauseAt( i )->size()]++;
//    cout << "occs\n";
//    for( unsigned i = 1; i < 10; i++)
//        cout << c[i] << endl;
//    cout << "Solving..." << endl; 
    solver.endPreprocessing();
}

void
GringoNumericFormat::readChoiceRule(
    Istream& input )
{
    unsigned headSize, bodySize, negativeSize;
    input.read( headSize );
    unsigned head[ headSize ];
    for( unsigned i = 0; i < headSize; )
    {
        input.read( head[ i ] );
        createStructures( head[ i ] );
        if( solver.isFalse( head[ i ] ) )
            --headSize;
        else
            ++i;
    }
    readBodySize( input, bodySize, negativeSize );
    if( headSize == 0 )
    {
        skipLiterals( input, bodySize );
        return;
    }
    
    ++readNormalRule_numberOfCalls;
    NormalRule* rule = new NormalRule( head[ 0 ] );
    unsigned tmp;
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input.read( tmp );
        createStructures( tmp );
        
        if( atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isTrue( tmp ) )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else if( solver.isUndefined( tmp ) )
        {
            rule->addNegativeLiteral( tmp );
            atomData[ tmp ].readNormalRule_negativeLiterals = readNormalRule_numberOfCalls;
        }
    }
    while( bodySize-- > 0 )
    {
        input.read( tmp );
        createStructures( tmp );

        if( atomData[ tmp ].readNormalRule_positiveLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isFalse( tmp ) || tmp == head[ 0 ] || atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else 
        {
            if( solver.isUndefined( tmp ) )
                rule->addPositiveLiteral( tmp );
            else if( !atomData[ tmp ].isSupported() )
                rule->addPositiveTrueLiteral( tmp );
            atomData[ tmp ].readNormalRule_positiveLiterals = readNormalRule_numberOfCalls;
        }
    }
    rule->addDoubleNegLiteral( head[ 0 ] );

    for( unsigned i = 1; i < headSize; ++i )
    {
        if( atomData[ head[ i ] ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        NormalRule* r = new NormalRule( *rule );
        r->literals[ 0 ] = Literal::newPossiblySupportedHeadAtom( head[ i ] );
        r->literals.back() = Literal::newDoubleNegatedBodyLiteral( head[ i ] );
        add( r, 0 );
    }

    if( atomData[ head[ 0 ] ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        delete rule;
    else
        add( rule, 0 );
    
}

void
GringoNumericFormat::readDisjunctiveRule(
    Istream& input )
{
    unsigned headSize = 0;
    input.read( headSize );
    unsigned head[ headSize ];
    ++readNormalRule_numberOfCalls;    
    for( unsigned i = 0; i < headSize; )
    {        
        input.read( head[ i ] );
        createStructures( head[ i ] );
        if( solver.isFalse( head[ i ] ) )
            --headSize;        
        else if( atomData[ head[ i ] ].isSupported() )
        {
            skipLiterals( input, headSize - i - 1 );
            unsigned bodySize, negativeSize;
            readBodySize( input, bodySize, negativeSize );
            skipLiterals( input, bodySize );
            return;
        }
        else
        {            
            atomData[ head[ i ] ].readNormalRule_headAtoms = readNormalRule_numberOfCalls;
            ++i;
        }
    }
    if( headSize == 0 )
    {
        readConstraint( input );
        return;
    }
    
    unsigned bodySize, negativeSize;
    readBodySize( input, bodySize, negativeSize );
    NormalRule* rule = new NormalRule();
    unsigned tmp;
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input.read( tmp );
        createStructures( tmp );
        
        if( atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isTrue( tmp ) )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else if( solver.isUndefined( tmp ) )
        {
            rule->addNegativeLiteral( tmp );
            atomData[ tmp ].readNormalRule_negativeLiterals = readNormalRule_numberOfCalls;
        }
    }
    while( bodySize-- > 0 )
    {
        input.read( tmp );
        createStructures( tmp );

        if( atomData[ tmp ].readNormalRule_positiveLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isFalse( tmp ) || atomData[ tmp ].readNormalRule_headAtoms == readNormalRule_numberOfCalls || atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        {
            delete rule;
            skipLiterals( input, bodySize );
            return;
        }
        else 
        {
            if( solver.isUndefined( tmp ) )
                rule->addPositiveLiteral( tmp );
            else if( !atomData[ tmp ].isSupported() )
                rule->addPositiveTrueLiteral( tmp );
            atomData[ tmp ].readNormalRule_positiveLiterals = readNormalRule_numberOfCalls;
        }
    }

    unsigned numberOfTrueHeadAtoms = 0;    
    // remove head atoms appearing in negative body
    {
        unsigned j = 0;
        for( unsigned i = 0; i < headSize; ++i )
        {
            head[ j ] = head[ i ];
            if( atomData[ head[ i ] ].readNormalRule_negativeLiterals != readNormalRule_numberOfCalls )
            {
                if( solver.isTrue( head[ i ] ) )
                    numberOfTrueHeadAtoms++;
                ++j;
            }
        }
        headSize = j;
    }
    
    if( headSize == 0 )
    {
        bodyToConstraint( rule );
        delete rule;
    }
    else if( headSize == 1 )
    {
        if( rule->literals.size() == 0 )
        {
            addFact( head[ 0 ] );
            delete rule;
        }
        else
        {
            rule->addHeadAtom( head[ 0 ] );
            add( rule, numberOfTrueHeadAtoms );
        }
    }
    else
    {
        for( unsigned i = 0; i < headSize; ++i )
            rule->addHeadAtom( head[ i ] );
        add( rule, numberOfTrueHeadAtoms );
//        for( unsigned i = 0; i < headSize; ++i )
//        {
//            NormalRule* r = new NormalRule( *rule );
//            r->addHeadAtom( head[ i ] );
//            for( unsigned j = 0; j < headSize; ++j )
//                if( j != i )
//                    r->addNegativeLiteral( head[ j ] );
//            add( r );
//        }
//        delete rule;
    }
}

void
GringoNumericFormat::readOptimizationRule(
    Istream& input )
{
    unsigned int size, negativeSize, tmp = 0;
    uint64_t weight = 0;
    
    input.read( tmp );
    assert( tmp == 0 );
    
    input.read( size );
    input.read( negativeSize );    

    if( size < negativeSize )
        ErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
    
    unsigned int counter = 0;
    WeightConstraintRule* weightConstraintRule = new WeightConstraintRule( 0, UINT64_MAX );
    
    Vector< int64_t > tmps;
    Vector< uint64_t > weights;
    while( counter < negativeSize )
    {
        input.read( tmp );
        createStructures( tmp );
//        weightConstraintRule->addNegativeLiteral( tmp );
        ++counter;
        tmps.push_back( -tmp );
    }
    while( counter < size )
    {
        input.read( tmp );
        createStructures( tmp );
//        weightConstraintRule->addPositiveLiteral( tmp );
        tmps.push_back( tmp );
        ++counter;
    }

    uint64_t bound = 1;
    counter = 0;
    while( counter < negativeSize )
    {        
        input.read( weight );
//        weightConstraintRule->addNegativeLiteralWeight( weight );
        weights.push_back( weight );
        bound += weight;
        ++counter;
    }
    while( counter < size )
    {
        input.read( weight );
//        weightConstraintRule->addPositiveLiteralWeight( weight );
        weights.push_back( weight );
        bound += weight;
        ++counter;
    }
    
    assert( tmps.size() == weights.size() );
    for( unsigned int i = 0; i < tmps.size(); i++ )
    {
        int64_t id = tmps[ i ];
        assert( id != 0 );
        uint64_t weight = weights[ i ];
        if( weight > 0 )
        {
            if( id < 0 )
            {
                weightConstraintRule->addNegativeLiteral( -id );
                weightConstraintRule->addNegativeLiteralWeight( weight );
            }
            else
            {
                weightConstraintRule->addPositiveLiteral( id );
                weightConstraintRule->addPositiveLiteralWeight( weight );
            }
        }
//        else
//        {
//            //Quick fix
//            wasp::Options::stratification = false;
//        }
    }
        
    assert( weightConstraintRule->sameSizeOfInternalVectors() );
    weightConstraintRule->setBound( bound );
    
    addOptimizationRule( weightConstraintRule );
}

void
GringoNumericFormat::readNormalRule(
    Istream& input )
{
    unsigned head;
    input.read( head );
    createStructures( head );

    if( solver.isFalse( head ) )
        readConstraint( input );
    else
    {
        unsigned int bodySize, negativeSize;
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
    Istream& input,
    unsigned& bodySize,
    unsigned& negativeSize )
{
    input.read( bodySize );
    input.read( negativeSize );

    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
}

void
GringoNumericFormat::skipLiterals(
    Istream& input,
    unsigned howMany )
{
    unsigned tmp;
    while( howMany-- > 0 )
        input.read( tmp );
}

void
GringoNumericFormat::readNormalRule(
    Istream& input,
    unsigned head,
    unsigned bodySize,
    unsigned negativeSize )
{
    assert( !solver.isFalse( head ) );
    assert( !atomData[ head ].isSupported() );
    assert( bodySize >= negativeSize );
//    assert( negativeSize >= 0 );
    assert( bodySize >= 1 );

    readNormalRule_numberOfCalls++;
    assert( readNormalRule_numberOfCalls != 0 );

    bodiesDictionary.startInsertion();
    bodiesDictionary.addElement( head );    
    NormalRule* rule = new NormalRule( head );
    bool firing = true;
    
    unsigned tmp = 0;
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input.read( tmp );
        
        createStructures( tmp );
        bodiesDictionary.addElement( -tmp );
        
        if( atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isTrue( tmp ) )
        {
            delete rule;
            skipLiterals( input, bodySize );
            bodiesDictionary.endInsertion();
            return;
        }
        else if( solver.isUndefined( tmp ) )
        {
            rule->addNegativeLiteral( tmp );
            atomData[ tmp ].readNormalRule_negativeLiterals = readNormalRule_numberOfCalls;
            firing = false;
        }
    }
    while( bodySize-- > 0 )
    {
        input.read( tmp );
        createStructures( tmp );
        
        bodiesDictionary.addElement( tmp );
        
        if( atomData[ tmp ].readNormalRule_positiveLiterals == readNormalRule_numberOfCalls )
            continue;
        if( solver.isFalse( tmp ) || tmp == head || atomData[ tmp ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        {
            delete rule;
            skipLiterals( input, bodySize );
            bodiesDictionary.endInsertion();
            return;
        }
        else 
        {
            if( solver.isUndefined( tmp ) )
            {
                rule->addPositiveLiteral( tmp );
                firing = false;
            }
            else if( !atomData[ tmp ].isSupported() )
                rule->addPositiveTrueLiteral( tmp );
            atomData[ tmp ].readNormalRule_positiveLiterals = readNormalRule_numberOfCalls;
        }
    }

    if( bodiesDictionary.endInsertion() )
    {
        usedDictionary = true;
        delete rule;
        return;
    }
    
    if( atomData[ head ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
    {
        bodyToConstraint( rule );
        delete rule;
    }
    else if( rule->isFact() )
    {
        delete rule;
        addFact( head );
    }
    else
    {
        add( rule, solver.isTrue( head ) ? 1 : 0 );
        if( firing )
            solver.addClause( Literal( head, POSITIVE ) );
    }
}

void
GringoNumericFormat::readConstraint(
    Istream& input )
{
    unsigned  bodySize, negativeSize, tmp = 0;
    input.read( bodySize );
    input.read( negativeSize );

    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );

    Clause* clause = solver.newClause( bodySize );
    while( negativeSize-- > 0 )
    {
        --bodySize;
        input.read( tmp );
        createStructures( tmp );
        if( !addUndefinedLiteral( clause, Literal( tmp, POSITIVE ) ) )
        {
            solver.releaseClause( clause );
            skipLiterals( input, bodySize );
            return;
        }
    }
    while( bodySize-- > 0 )
    {
        input.read( tmp );
        createStructures( tmp );
        if( !addUndefinedLiteral( clause, Literal( tmp, NEGATIVE ) ) )
        {
            solver.releaseClause( clause );
            skipLiterals( input, bodySize );
            return;
        }
    }
    
    if( clause->size() >= 3 && clause->size() <= 5 && solver.isSubsumed( clause ) )
    {
        solver.releaseClause( clause );
        return;
    }
    trace_msg( parser, 2, "Adding clause " << *clause );
//    solver.addClause( clause );
    solver.cleanAndAddClause( clause );
}

void
GringoNumericFormat::readCount(
    Istream& input )
{
    unsigned int id, bound, size, negativeSize, tmp = 0;
    input.read( id );
    input.read( size );
    input.read( negativeSize );
    input.read( bound );
    
    createStructures( id );
    if( size < negativeSize )
    {
        ErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
    }
    
    if( size < bound )
    {
        while( size-- > 0 )
        {
            input.read( tmp );
            createStructures( tmp );
        }
        solver.addClause( Literal( id, NEGATIVE ) );
        return;
    }
    assert( size >= bound );

    WeightConstraintRule* weightConstraintRule = new WeightConstraintRule( id, bound );    
    while( negativeSize-- > 0 )
    {
        --size;
        input.read( tmp );
        createStructures( tmp );
        weightConstraintRule->addNegativeLiteralCount( tmp );
    }
    while( size-- > 0 )
    {
        input.read( tmp );
        createStructures( tmp );
        weightConstraintRule->addPositiveLiteralCount( tmp );
    }
    
    assert( weightConstraintRule->sameSizeOfInternalVectors() );
    add( weightConstraintRule );
}

void
GringoNumericFormat::readSum(
    Istream& input )
{
    unsigned int id, bound, size, negativeSize, tmp = 0;
    input.read( id );
    input.read( bound );
    input.read( size );
    input.read( negativeSize );

    createStructures( id );

    if( size < negativeSize )
    {
        ErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
    }
    
    WeightConstraintRule* weightConstraintRule = new WeightConstraintRule( id, bound );
    
    unsigned int counter = 0;
    unsigned int previousWeight = 0;
    
    while( counter < negativeSize )
    {
        input.read( tmp );
        createStructures( tmp );
        weightConstraintRule->addNegativeLiteral( tmp );
        ++counter;
    }
    while( counter < size )
    {
        input.read( tmp );
        createStructures( tmp );
        weightConstraintRule->addPositiveLiteral( tmp );
        ++counter;
    }
        
    counter = 0;
    while( counter < negativeSize )
    {
        input.read( tmp );
        if( tmp > bound )
            tmp = bound;
        weightConstraintRule->addNegativeLiteralWeight( tmp );
        ++counter;
        if( previousWeight == 0 )
            previousWeight = tmp;
        else if( previousWeight != tmp )
            previousWeight = MAXUNSIGNEDINT;            
    }
    while( counter < size )
    {
        input.read( tmp );
        if( tmp > bound )
            tmp = bound;
        weightConstraintRule->addPositiveLiteralWeight( tmp );
        ++counter;
        if( previousWeight == 0 )
            previousWeight = tmp;
        else if( previousWeight != tmp )
            previousWeight = MAXUNSIGNEDINT;
    }
  
    assert( weightConstraintRule->sameSizeOfInternalVectors() );
    add( weightConstraintRule );
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
            Var var = solver.getAssignedVariable( propagatedLiterals++ );
            if( solver.isTrue( var ) )
                propagateTrue( var );
            else
                propagateFalse( var );
        }
        while( !facts.empty() )
        {
            repeat = true;
            Var var = facts.back();
            facts.pop_back();
            propagateFact( var );
        }
    }while( repeat );
}

void
GringoNumericFormat::propagateTrue(
    Var var )
{
    assert( var != 0 );
    assert( solver.isTrue( var ) );

    assert( var < atomData.size() );
    AtomData& data = atomData[ var ];

    trace_msg( parser, 2, "Propagating " << var << " as true" << ( data.isSupported() ? " (supported)" : "" ) );

    if( data.numberOfHeadOccurrences == 1 )
        atomsWithSupportInference.push_back( var );

    unsigned j = 0;
    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        NormalRule* rule = data.posOccurrences[ i ];
        assert( rule != NULL );
        if( !shrinkPos( rule, var ) )
            data.posOccurrences[ j++ ] = data.posOccurrences[ i ];
    }
    data.posOccurrences.shrink( j );

    for( unsigned i = 0; i < data.negOccurrences.size(); ++i )
    {
        NormalRule* rule = data.negOccurrences[ i ];
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.negOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.doubleNegOccurrences.size(); ++i )
    {
        NormalRule* rule = data.doubleNegOccurrences[ i ];
        assert( rule != NULL );
        shrinkDoubleNeg( rule, var );
    }
    data.doubleNegOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        NormalRule* rule = data.headOccurrences[ i ];
        assert( rule != NULL );
        onTrueHeadAtom( rule, var );
    }
    
//    for( unsigned i = 0; i < data.negConstraints.size(); ++i )
//    {
//        NormalRule* rule = data.negConstraints[ i ];
//        assert( rule != NULL );
//        rule->remove();
//    }
//    data.negConstraints.clear();
//    
//    for( unsigned i = 0; i < data.posConstraints.size(); ++i )
//    {
//        NormalRule* rule = data.posConstraints[ i ];
//        assert( rule != NULL );
//        shrinkConstraint( rule, var->getId() );            
//    }
//    data.posConstraints.clear();

    for( unsigned int i = 0; i < data.negWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraintRule* weightConstraintRule = data.negWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateMaxPossibleValueWeightConstraint( weightConstraintRule, data.positionsInNegWeightConstraints[ i ] );
    }
    data.negWeightConstraintsOccurrences.clear();
    data.positionsInNegWeightConstraints.clear();
    
    for( unsigned int i = 0; i < data.posWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraintRule* weightConstraintRule = data.posWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );        
        updateCurrentValueWeightConstraint( weightConstraintRule, data.positionsInPosWeightConstraints[ i ] );
    }
    data.posWeightConstraintsOccurrences.clearAndDelete();
    data.positionsInPosWeightConstraints.clearAndDelete();
    
    if( data.isWeightConstraint() )
    {                
        if( data.weightConstraintRule->canBeAClause() && !data.weightConstraintRule->isTrue() )
        {
            atLeastOne( data.weightConstraintRule );
            data.weightConstraintRule->remove();
        }
        else
            weightConstraintIsTrue( data.weightConstraintRule );
    }
}

void
GringoNumericFormat::propagateFalse(
    Var var )
{
    assert( var != 0 );
    assert( solver.isFalse( var ) );

    assert( var < atomData.size() );
    AtomData& data = atomData[ var ];
    
    trace_msg( parser, 2, "Propagating " << var << " as false" );

    for( unsigned i = 0; i < data.doubleNegOccurrences.size(); ++i )
    {
        NormalRule* rule = data.doubleNegOccurrences[ i ];
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.doubleNegOccurrences.clearAndDelete();        
    
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
        shrinkNeg( rule, var );
    }
    data.negOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        NormalRule* rule = data.headOccurrences[ i ];
        assert( rule != NULL );
        if( rule->isRemoved() )
            continue;
                
        shrinkHead( rule, var );        
    }
    data.numberOfHeadOccurrences = 0;
    data.headOccurrences.clearAndDelete();
        
    for( unsigned int i = 0; i < data.negWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraintRule* weightConstraintRule = data.negWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateCurrentValueWeightConstraint( weightConstraintRule, data.positionsInNegWeightConstraints[ i ] );
    }
    data.negWeightConstraintsOccurrences.clearAndDelete();
    data.positionsInNegWeightConstraints.clearAndDelete();
    
    for( unsigned int i = 0; i < data.posWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraintRule* weightConstraintRule = data.posWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateMaxPossibleValueWeightConstraint( weightConstraintRule, data.positionsInPosWeightConstraints[ i ] );
    }
    data.posWeightConstraintsOccurrences.clearAndDelete();
    data.positionsInPosWeightConstraints.clearAndDelete();
    
    if( data.isWeightConstraint() )
    {
//        if( data.weightConstraintRule->weights.front() == data.weightConstraintRule->weights.back() && data.weightConstraintRule->bound / data.weightConstraintRule->weights.front() == 2 && data.weightConstraintRule->literals.size() > 8 && !data.weightConstraintRule->isFalse() )
//        {
//            atMostOne( data.weightConstraintRule );
//            data.weightConstraintRule->remove();
//        }
//        else
            weightConstraintIsFalse( data.weightConstraintRule );
    }
}

//bool
//GringoNumericFormat::ruleToConstraint(
//    NormalRule* rule )
//{
//    trace_msg( parser, 3, "Try to replace " << *rule << " by a constraint" ); 
//    assert( !rule->isRemoved() );
//    Clause* clause = solver.newClause( rule->size() );
//    for( unsigned j = 0; j < rule->size(); ++j )
//    {
//        if( !solver.isFalse( rule->literals[ j ] ) )
//        {
//            if( rule->literals[ j ].isHeadAtom() )
//            {
//                solver.releaseClause( clause );
//                trace_msg( parser, 4, "Not replaced" ); 
//                return false;
//            }
//            clause->addLiteral( rule->literals[ j ] );
//        }        
//    }
////    for( unsigned j = 0; j < rule->negBody.size(); ++j )
////        clause->addLiteral( Literal( rule->negBody[ j ], POSITIVE ) );
////    for( unsigned j = 0; j < rule->posBody.size(); ++j )
////        clause->addLiteral( Literal( rule->posBody[ j ], NEGATIVE ) );
////    for( unsigned j = 0; j < rule->doubleNegBody.size(); ++j )
////        clause->addLiteral( Literal( rule->doubleNegBody[ j ], NEGATIVE ) );
//    solver.cleanAndAddClause( clause );
//    trace_msg( parser, 4, "Replaced" );    
//    return true;
//}

void
GringoNumericFormat::bodyToConstraint(
    NormalRule* rule )
{
    trace_msg( parser, 3, "Replacing " << *rule << " by a constraint" );
    Clause* clause = solver.newClause( rule->size() );
    for( unsigned j = 0; j < rule->size(); ++j )
        if( !rule->literals[ j ].isToBeRemoved() && !rule->literals[ j ].isHeadAtom() )
            clause->addLiteral( rule->literals[ j ] );

    solver.cleanAndAddClause( clause );
}

void
GringoNumericFormat::propagateFact(
    Var var )
{
    assert( var < atomData.size() );
    AtomData& data = atomData[ var ];
    
    trace_msg( parser, 2, "Propagating fact " << var );

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
    data.headOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        NormalRule* rule = data.posOccurrences[ i ];
        assert( rule != NULL );
        shrinkPos( rule, var );
    }
    data.posOccurrences.clearAndDelete();
}

void
GringoNumericFormat::addFact(
    unsigned head )
{
    if( atomData[ head ].isSupported() )
        return;

    trace_msg( parser, 3, "Adding fact " << head );
    atomData[ head ].setSupported();
    solver.addClause( Literal( head, POSITIVE ) );
    facts.push_back( head );
}

void
GringoNumericFormat::addTrueVariable(
    unsigned int id )
{
    solver.addClause( Literal( id, POSITIVE ) );
}
    
void
GringoNumericFormat::addFalseVariable(
    unsigned int id )
{   
    solver.addClause( Literal( id, NEGATIVE ) );
}

void
GringoNumericFormat::readAtomsTable(
    Istream& input )
{
    unsigned int nextAtom;
    input.read( nextAtom );

    char name[ 1024 ];
    while( nextAtom != 0 )
    {
//        assert_msg( nextAtom < inputVarId.size(), "nextAtom = " << nextAtom << "; size = " << inputVarId.size() );
//        assert( inputVarId[ nextAtom ] > 1 );        
        createStructures( nextAtom );
        input.getline( name, 1024 );
        VariableNames::setName( nextAtom, name );
        solver.addedVarName( nextAtom );
        trace_msg( parser, 6, "Set name " << name << " for atom " << nextAtom );
        
        if( wasp::Options::queryAlgorithm != NO_QUERY )
            solver.setFrozen( nextAtom );
        input.read( nextAtom );        
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
    Istream& input )
{
    char b;

    input.read( b );
    assert( b == 'B' );
    input.read( b );
    assert( b == '+' );

    unsigned int nextAtom;
    input.read( nextAtom );

    createStructures( nextAtom );

    while( nextAtom != 0 )
    {        
        solver.addClause( Literal( nextAtom, POSITIVE ) );
        input.read( nextAtom );
    }
}

void
GringoNumericFormat::readFalseAtoms(
    Istream& input )
{
    char b;

    input.read( b );
    assert( b == 'B' );
    input.read( b );
    assert( b == '-' );

    unsigned int nextAtom;
    input.read( nextAtom );

    createStructures( nextAtom );

    while( nextAtom != 0 )
    {
        solver.addClause( Literal( nextAtom, NEGATIVE ) );
        input.read( nextAtom );
    }
}

void
GringoNumericFormat::readErrorNumber(
    Istream& input )
{
    char b;
    unsigned int errorNumber;
    input.read( b );
    if( b == 'E' )
    {
        unsigned int nextAtom;
        input.read( nextAtom );

        createStructures( nextAtom );

        while( nextAtom != 0 )
        {
            NormalRule* r = new NormalRule();
            r->literals.push_back( Literal::newPossiblySupportedHeadAtom( nextAtom ) );            
            r->literals.push_back( Literal::newDoubleNegatedBodyLiteral( nextAtom ) );
            add( r, 0 );
            input.read( nextAtom );
        }
        input.read( errorNumber );
    }
    else
    {
        errorNumber = b - '0';
    }    

    if( errorNumber != 1 ) {
        stringstream ss;
        ss << "read error message number " << errorNumber;
        ErrorMessage::errorDuringParsing( ss.str() );
        exit( 0 );
    }
}

void
GringoNumericFormat::processRecursivePositiveCrule(
    Clause* crule )
{
    assert( crule != NULL );
    assert( crule->getAt( 0 ).isPositive() );
    Var variable = crule->getAt( 0 ).getVariable();
    assert( solver.isInCyclicComponent( variable ) );

    trace_msg( parser, 2, "Creating GUS data structures for variable " << variable << " whose supporting rule has " << crule->size() - 1 << " literals" );
    solver.setFrozen( variable );

    Component* component = solver.getComponent( variable );
    assert( component != NULL );
    component->setAuxVariable( variable );

    for( unsigned int j = 1; j < crule->size(); j++ )
    {
        Literal currentLiteral = crule->getAt( j );
        Var currentVariable = currentLiteral.getVariable();
        solver.setFrozen( currentVariable );
        if( currentLiteral.isNegative() )
        {
            if( solver.inTheSameComponent( currentVariable, variable ) && variable != currentVariable /* FIXME: variable == ruleVar if it is a double negated literal. Clauses lack this information */ )
            {
                Literal lit = currentLiteral.getOppositeLiteral();
                trace_msg( parser, 3, "Adding " << lit << " to the supporting rule of " << variable );
                component->addAuxVariableSupportedByLiteral( variable, lit );
                component->addInternalLiteralForVariable( variable, lit );
            }
            else if( !solver.inTheSameComponent( currentVariable, variable ) )
            {
                component->addExternalLiteralForVariable( variable, currentLiteral.getOppositeLiteral() );
            }
        }
        else
        {
            component->addExternalLiteralForVariable( variable, currentLiteral.getOppositeLiteral() );
        }
    }
}

void
GringoNumericFormat::processRecursiveNegativeCrule(
    Clause* crule )
{
    assert( crule != NULL );
    assert( crule->getAt( 0 ).isNegative() );
    Var variable = crule->getAt( 0 ).getVariable();
    assert( solver.isInCyclicComponent( variable ) );

    trace_msg( parser, 2, "Creating GUS data structures for variable " << variable << ", which has " << crule->size() - 1 << " supporting rules" );

    Component* component = solver.getComponent( variable );
    assert( component != NULL );

    for( unsigned int j = 1; j < crule->size(); j++ )
    {
        Literal ruleLiteral = crule->getAt( j );
        Var ruleVar = ruleLiteral.getVariable();
        solver.setFrozen( ruleVar );
        if( literalsPostPropagator[ ruleLiteral.getSign() ][ ruleVar ].insert( component ).second )
        {
            Literal lit( ruleVar, ruleLiteral.getSign() );
            solver.addPostPropagator( lit, component );
        }

        if( ruleLiteral.isPositive() && solver.inTheSameComponent( variable, ruleVar ) && variable != ruleVar /* FIXME: variable == ruleVar if it is a double negated literal. Clauses lack this information */ )
        {
            trace_msg( parser, 3, "Adding " << ruleVar << " as internal rule for " << variable );
            component->addInternalLiteralForVariable( variable, Literal( ruleVar, POSITIVE ) );
            component->addVariablePossiblySupportedByLiteral( variable, Literal( ruleVar, POSITIVE ) );
        }
        else
        {
            trace_msg( parser, 3, "Adding " << ruleLiteral << " as external rule for " << variable );
            component->addExternalLiteralForVariable( variable, ruleLiteral );
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
//        gd->setVariable( i );
        solver.addGUSData( gd );
    }

    trace_msg( parser, 2, "Program is not tight. Number of cyclic components " << solver.getNumberOfCyclicComponents() );
    for( unsigned int i = 0; i < solver.getNumberOfCyclicComponents(); i++ )
    {
        Component* component = solver.getCyclicComponent( i );
        trace_msg( parser, 3, "Processing component " << ( i + 1 ) << " with size " << component->size() );
        trace_msg( parser, 7, "Component: " << *component );
        
        if( isHeadCycleFree( component ) )
        {
            trace_msg( parser, 4, "The component is HCF" );
            for( unsigned int j = 0; j < component->size(); j++ )
            {
                Var currentVariable = component->getVariable( j );
                assert( currentVariable <= solver.numberOfVariables() );
                trace_msg( parser, 2, "Variable " << currentVariable << " is in the cyclic component " << i );             
                if( solver.getComponent( currentVariable ) == NULL )
                    solver.setComponent( currentVariable, component );
                component->variableHasNoSourcePointer( currentVariable );
            }        
            solver.addPostPropagator( component );
        }
        else
        {
            trace_msg( parser, 4, "The component is non HCF" );            
            HCComponent* hcComponent = solver.createHCComponent( atomData.size() - 1 ); //new HCComponent( solver );

            hcComponent->setId( solver.numberOfHCComponents() );
            for( unsigned int j = 0; j < component->size(); j++ )
            {
                Var v = component->getVariable( j );
                solver.setFrozen( v );
                
                if( v >= atomData.size() )
                    continue;
                hcComponent->addHCVariable( v );                                
                solver.addPostPropagator( Literal( v, POSITIVE ), hcComponent );
                solver.addPostPropagator( Literal( v, NEGATIVE ), hcComponent );
                solver.setComponent( v, NULL );
                solver.setHCComponent( v, hcComponent );
            }
            statistics( &solver, removeComponent( component->getId() ) );            
            component->remove();
            statistics( &solver, addCyclicHCComponent( hcComponent->size() ) );            
            
            for( unsigned int j = 0; j < hcComponent->size(); j++ )
            {                
                Var v = hcComponent->getVariable( j );
                assert( v < atomData.size() );
                Vector< NormalRule* >& headOccs = atomData[ v ].headOccurrences;
                for( unsigned int t = 0; t < headOccs.size(); t++ )
                {
                    NormalRule* rule = headOccs[ t ];
                    assert( rule != NULL );
                    if( !rule->isRemoved() )
                    {                        
                        for( unsigned int k = 0; k < rule->size(); k++ )
                        {
                            assert( rule->literals[ k ].getVariable() < atomData.size() );
                            if( solver.getHCComponent( rule->literals[ k ].getVariable() ) != hcComponent )
                            {
                                if( hcComponent->addExternalLiteral( rule->literals[ k ] ) )
                                {
                                    solver.setFrozen( rule->literals[ k ].getVariable() );
                                    solver.addPostPropagator( rule->literals[ k ], hcComponent );
                                    solver.addPostPropagator( rule->literals[ k ].getOppositeLiteral(), hcComponent );
                                }
                            }
                            else if( rule->literals[ k ].isNegativeBodyLiteral() )
                            {
                                if( hcComponent->addExternalLiteralForInternalVariable( rule->literals[ k ] ) )
                                {
                                    solver.setFrozen( rule->literals[ k ].getVariable() );
                                    solver.addPostPropagator( rule->literals[ k ], hcComponent );
                                    solver.addPostPropagator( rule->literals[ k ].getOppositeLiteral(), hcComponent );
                                }
                            }
                        }
                        Clause* c = normalRuleToClause( rule );
                        hcComponent->addClauseToChecker( c, v );
                    }
                }
            }
            
            for( unsigned int j = 0; j < hcComponent->size(); j++ )
            {
                Var v = hcComponent->getVariable( j );
                if( solver.isTrue( v ) )
                    hcComponent->onLiteralFalse( Literal( v, NEGATIVE ) );
                else if( solver.isFalse( v ) )
                    hcComponent->onLiteralFalse( Literal( v, POSITIVE ) );
            }
            
            for( unsigned int j = 0; j < hcComponent->externalLiteralsSize(); j++ )
            {
                Literal lit = hcComponent->getExternalLiteral( j );
                if( solver.isTrue( lit ) )
                    hcComponent->onLiteralFalse( lit.getOppositeLiteral() );
                else if( solver.isFalse( lit ) )
                    hcComponent->onLiteralFalse( lit );
            }
            
            solver.addPostPropagator( hcComponent );
            solver.addHCComponent( hcComponent );
        }
    }

    unsigned int j = 0;
    for( unsigned int i = 0; i < crules.size(); i++ )
    {
        crules[ j ] = crules[ i ];                        
        Clause* crule = crules[ i ];        
        assert( crule != NULL );            
        
        if( crule->hasBeenDeleted() )
        {
            solver.releaseClause( crule );
            continue;
        }
        
        j++;
        // skip acyclic variables
        if( !solver.isInCyclicComponent( crule->getAt( 0 ).getVariable() ) )
            continue;

        if( crule->getAt( 0 ).isPositive() )
            processRecursivePositiveCrule( crule );
        else
            processRecursiveNegativeCrule( crule );
    }
    crules.shrink( j );    
}

void
GringoNumericFormat::createStructures(
    unsigned id )
{
    while( id >= atomData.size() )
    {
        solver.addVariable();
        solver.addEdgeInDependencyGraph( solver.numberOfVariables(), 0 );
        atomData.push_back( AtomData( false ) );
    }
}

void
GringoNumericFormat::simplify()
{
    trace_msg( parser, 1, "Simplify" );
    atomsWithSupportInference.clear();
    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        if( atomData[ i ].isSupported() || solver.isFalse( i ) )
            continue;
        if( atomData[ i ].numberOfHeadOccurrences == 0 || ( atomData[ i ].numberOfHeadOccurrences == 1 && solver.isTrue( i ) ) )
            atomsWithSupportInference.push_back( i );
    }
    
    while( !atomsWithSupportInference.empty() )
    {
        AtomData& data = atomData[ atomsWithSupportInference.back() ];
        trace_msg( parser, 2, "Considering support inference for atom " << atomsWithSupportInference.back() );
        if( !data.isSupported() && !solver.isFalse( atomsWithSupportInference.back() ) && !data.isWeightConstraint() )
        {
            if( data.numberOfHeadOccurrences == 0 )
            {
                trace_msg( parser, 2, "Infer falsity of variable " << atomsWithSupportInference.back() << " with no support" );
                solver.addClause( Literal( atomsWithSupportInference.back(), NEGATIVE ) );
                assert( solver.isFalse( atomsWithSupportInference.back() ) || solver.conflictDetected() );
            }
            else
            {
                assert( data.numberOfHeadOccurrences == 1 );
                assert( solver.isTrue( atomsWithSupportInference.back() ) );
                assert( !data.isSupported() );
                
                unsigned int i = 0;
                for( ; i < data.headOccurrences.size(); ++i )
                {
                    if( !data.headOccurrences[ i ]->isRemoved() && isSupporting( data.headOccurrences[ i ], atomsWithSupportInference.back() ) )
                    {
                        //data.headOccurrences[ 0 ] = data.headOccurrences[ i ];
                        break;
                    }
                }
                //data.headOccurrences.shrink( 1 );
                assert( i < data.headOccurrences.size() );
                NormalRule* rule = data.headOccurrences[ i ];
                assert( !rule->isRemoved() );
                trace_msg( parser, 2, "Infer truth of the body literals and falsity of the head atoms of " << *rule << " (unique support of true head)" );
                for( unsigned j = 0; j < rule->literals.size(); ++j )
                    if( rule->literals[ j ].getVariable() != atomsWithSupportInference.back() ) //if( !rule->literals[ i ].isHeadAtom() )
                        solver.addClause( rule->literals[ j ].getOppositeLiteral() );
            }
        }
        atomsWithSupportInference.pop_back();
        propagate();
        if( solver.conflictDetected() )
            return;
    }
}

void
GringoNumericFormat::createCrule(
    Literal head,
    Var varToSkip,
    NormalRule* rule )
{
    assert( head.isPositive() );
    Clause* crule = solver.newClause( rule->literals.size() );
    crule->addLiteral( head );
    for( unsigned k = 0; k < rule->literals.size(); ++k )
    {
        Literal lit = rule->literals[ k ];
        // TODO: MALVI: handle disjunctive rules
        if( lit.isHeadAtom() && lit.getVariable() == varToSkip )
            continue;
//        assert_msg( !lit.isHaedAtom(), "rule: " << *rule << "; head: " << head );

//        if( !lit.isToBeRemoved() )
            crule->addLiteral( lit );
        if( lit.isPositiveBodyLiteral() )
            solver.addEdgeInDependencyGraph( head.getVariable(), lit.getVariable() );
            
    }
    crules.push_back( crule );
    trace_msg( parser, 5, "New crule: " << *crule );
}

void
GringoNumericFormat::addDependencies(
    Literal head,
    NormalRule* rule )
{
    assert( head.isPositive() );
    for( unsigned k = 0; k < rule->literals.size(); ++k )
    {
        Literal lit = rule->literals[ k ];
        if( lit == head )
            continue;

        if( lit.isPositiveBodyLiteral() )
            solver.addEdgeInDependencyGraph( head.getVariable(), lit.getVariable() );
    }
}

void
GringoNumericFormat::computeSCCs()
{
    trace_msg( parser, 1, "Computing crules and SCCs" );
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    
    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        trace_msg( parser, 2, "Processing atom " << i );
        AtomData& data = atomData[ i ];
        Var var = i;
        if( data.isSupported() || solver.isFalse( var ) || data.isWeightConstraint() )
            continue;        
        assert( data.numberOfHeadOccurrences > 0 );
        
        if( data.numberOfHeadOccurrences == 1 )
        {
            trace_msg( parser, 3, "Only one defining rule" );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                if( data.headOccurrences[ j ]->isRemoved() )
                    continue;
                if( isSupporting( data.headOccurrences[ j ], var ) )
                    createCrule( Literal( i, POSITIVE ), var, data.headOccurrences[ j ] );
                else
                    addDependencies( Literal( i, POSITIVE ), data.headOccurrences[ j ] );
            }
        }
        else
        {
            trace_msg( parser, 3, "More than one defining rule" );            
            Clause* crule = solver.newClause( data.numberOfHeadOccurrences + 1 );            
            crule->addLiteral( Literal( i, NEGATIVE ) );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                if( data.headOccurrences[ j ]->isRemoved() )
                    continue;
                if( isSupporting( data.headOccurrences[ j ], var ) )
                {
                    // TODO: MALVI: handle disjunctive rules
                    if( data.headOccurrences[ j ]->literals.size() == 2 )
                    {
                        assert( data.headOccurrences[ j ]->literals[ 0 ].getVariable() == i || data.headOccurrences[ j ]->literals[ 1 ].getVariable() == i );
                        Literal lit = data.headOccurrences[ j ]->literals[ 0 ].getVariable() == i ? data.headOccurrences[ j ]->literals[ 1 ] : data.headOccurrences[ j ]->literals[ 0 ];
//                        if( !lit.isToBeRemoved() )
                            crule->addLiteral( lit.getOppositeLiteral() );
                        if( lit.isPositiveBodyLiteral() )
                            solver.addEdgeInDependencyGraph( i, lit.getVariable() );
                    }
                    else
                    {
                        solver.addVariable();
                        crule->addLiteral( Literal( solver.numberOfVariables(), POSITIVE ) );
                        solver.addEdgeInDependencyGraph( i, solver.numberOfVariables() );
                        createCrule( Literal( solver.numberOfVariables(), POSITIVE ), var, data.headOccurrences[ j ] );
                    }
                }
                else
                    addDependencies( Literal( i, POSITIVE ), data.headOccurrences[ j ] );
            }
            data.crule = crule;
            crules.push_back( crule );
            trace_msg( parser, 5, "New crule: " << *crule );
        }
    }

    solver.computeStrongConnectedComponents();
}

void
GringoNumericFormat::computeCompletion()
{
    trace_msg( parser, 1, "Computing completion" );
    assert_msg( propagatedLiterals == solver.numberOfAssignedLiterals(), "difference is " << ( solver.numberOfAssignedLiterals() - propagatedLiterals ) );
    for( unsigned i = 0; i < crules.size(); ++i )
    {
        Clause* crule = crules[ i ];
        trace_msg( parser, 2, "Processing crule " << *crule );
        assert( crule != NULL );
        Literal lit = crule->getAt( 0 ).getOppositeLiteral();
        if( !solver.isTrue( lit ) )
        {
            for( unsigned j = 1; j < crule->size(); ++j )
            {
                Literal lit2 = crule->getAt( j ).getOppositeLiteral();
                if( solver.isTrue( lit2 ) || crule->getAt( j ) == lit )
                    continue;
                assert( solver.isUndefined( lit ) );
                assert( solver.isUndefined( lit2 ) );
                
                trace_msg( parser, 3, "Adding binary clause " << lit << " | " << lit2 );

                solver.addClause( lit, lit2 );
                assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
            }
        }
        trace_msg( parser, 3, "Adding clause " << *crule );
        solver.cleanAndAddClause( crule );
//        trace_msg( parser, 4, "Actually added " << *crule );
        
        assert_msg( propagatedLiterals == solver.numberOfAssignedLiterals() || ( propagatedLiterals + 1 == solver.numberOfAssignedLiterals() && solver.getAssignedVariable( propagatedLiterals ) == lit.getVariable() && lit.getVariable() >= atomData.size() ), "difference is " << ( solver.numberOfAssignedLiterals() - propagatedLiterals ) );
        propagatedLiterals = solver.numberOfAssignedLiterals();
    }    
}
//void
//GringoNumericFormat::computeCompletion()
//{
//    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
//    for( unsigned i = 0; i < crules.size(); ++i )
//    {
//        Clause* crule = crules[ i ];
//        assert( crule != NULL );
//        Literal lit = crule->getAt( 0 ).getOppositeLiteral();
//        if( lit.isTrue() )
//        {
//            solver.cleanAndAddClause( crule );
//        }
//        else if( crule->size() >= 5 )
//        {
//            lit.getVariable()->setFrozen();
//            Aggregate* aggregate = new Aggregate();
//
//            //The weight of this literal is computed in aggregate->updateBound()
//            aggregate->addLiteral( lit.getOppositeLiteral(), 0 );
//
//            for( unsigned int j = 1; j < crule->size(); j++ )
//            {
//                Literal lit2 = crule->getAt( j );
//
//                assert( !lit2.isTrue() );
//                if( lit2.isUndefined() )
//                {
//                    aggregate->addLiteral( lit2, 1 );
//                    lit2.getVariable()->setFrozen();
//                }
//            }
//
//            aggregate->attachAggregate();
//            assert( aggregate->size() > 0 );
//            aggregate->updateBound( 1 );
//            trace_msg( parser, 3, "Crule: " << *aggregate );
//
//            if( lit.isFalse() )
//            {
//                aggregate->onLiteralFalse( solver, lit, -1 );
//        //            solver.addPostPropagator( aggregate );
//            }
//            else if( lit.isTrue() )
//            {
//                aggregate->onLiteralFalse( solver, lit.getOppositeLiteral(), 1 );
//        //            solver.addPostPropagator( aggregate );
//            }            
//            
//            solver.addAggregate( aggregate );
//        }
//        else
//        {
//            for( unsigned j = 1; j < crule->size(); ++j )
//            {
//                Literal lit2 = crule->getAt( j ).getOppositeLiteral();
//                if( lit2.isTrue() || crule->getAt( j ) == lit )
//                    continue;
//                assert( lit.isUndefined() );
//                assert( lit2.isUndefined() );
//                   
//                Clause* bin = solver.newClause();
//                bin->addLiteral( lit );
//                bin->addLiteral( lit2 );
//                solver.addClause( bin );
//                assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
//            }
//            solver.cleanAndAddClause( crule );
//        }
//        assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
//    }    
//}

void
GringoNumericFormat::add(
    NormalRule* rule,
    unsigned int numberOfTrueHeadAtoms )
{
    assert( rule != NULL );
    assert( !rule->isFact() );

    trace_msg( parser, 2, "Adding rule " << *rule );
    normalRules.push_back( rule );

    for( unsigned i = 0; i < rule->literals.size(); ++i )
    {
        Literal lit = rule->literals[ i ];
        if( lit.isPossiblySupportedHeadAtom() )
        {
            Var v = lit.getVariable();
            AtomData& headData = atomData[ v ];
            headData.headOccurrences.push_back( rule );
            if( numberOfTrueHeadAtoms == 0 || ( numberOfTrueHeadAtoms == 1 && solver.isTrue( v ) ) )
                headData.numberOfHeadOccurrences++;
            else
                rule->literals[ i ].setUnsupportedHeadAtom();            
        }
        else if( lit.isPositiveBodyLiteral() )
            atomData[ lit.getVariable() ].posOccurrences.push_back( rule );
        else if( lit.isNegativeBodyLiteral() )
            atomData[ lit.getVariable() ].negOccurrences.push_back( rule );
        else if( lit.isDoubleNegatedBodyLiteral() )
            atomData[ lit.getVariable() ].doubleNegOccurrences.push_back( rule );
    }
}

void
GringoNumericFormat::add(
    WeightConstraintRule* weightConstraintRule )
{
    assert( weightConstraintRule != NULL );
    
    trace_msg( parser, 2, "Adding weight constraint rule " << *weightConstraintRule );    
    removeSatisfiedLiterals( weightConstraintRule );
    weightConstraintRule->sort();
    trace_msg( parser, 2, "Sorted weight constraint rule " << *weightConstraintRule );
    weightConstraintRules.push_back( weightConstraintRule );

    atomData[ weightConstraintRule->getId() ].setWeightConstraint( weightConstraintRule );
    for( unsigned i = 0; i < weightConstraintRule->size(); ++i )
    {
        int lit = weightConstraintRule->getLiteral( i );
        if( lit < 0 )
        {
            atomData[ -lit ].negWeightConstraintsOccurrences.push_back( weightConstraintRule );
            atomData[ -lit ].positionsInNegWeightConstraints.push_back( i );
        }
        else
        {
            atomData[ lit ].posWeightConstraintsOccurrences.push_back( weightConstraintRule );
            atomData[ lit ].positionsInPosWeightConstraints.push_back( i );
        }
    }
    
    if( weightConstraintRule->isTrue() )
        addTrueVariable( weightConstraintRule->getId() );
    else if( weightConstraintRule->isFalse() )
        addFalseVariable( weightConstraintRule->getId() );
}

void
GringoNumericFormat::addOptimizationRule(
    WeightConstraintRule* weightConstraintRule )
{
    assert( weightConstraintRule != NULL );
    
    trace_msg( parser, 2, "Adding weight constraint rule " << *weightConstraintRule );
//    unsigned int originalBound = weightConstraintRule->bound;
//    removeSatisfiedLiterals( weightConstraintRule );
//
//    assert( originalBound >= weightConstraintRule->bound );
//    weightConstraintRule->cost = originalBound - weightConstraintRule->bound;     
//    
    weightConstraintRule->sort();
    trace_msg( parser, 2, "Sorted weight constraint rule " << *weightConstraintRule );
    optimizationRules.push_back( weightConstraintRule );   
}

void
GringoNumericFormat::removeSatisfiedLiterals(
    WeightConstraintRule* weightConstraintRulePointer )
{
    assert( weightConstraintRulePointer != NULL );
    assert( weightConstraintRulePointer->sameSizeOfInternalVectors() );
    
    WeightConstraintRule& weightConstraintRule = *weightConstraintRulePointer;
    
    unsigned int j = 0;
    for( unsigned int i = 0; i < weightConstraintRule.size(); i++ )
    {
        weightConstraintRule.overwrite( j, i );
        
        int literalId = weightConstraintRule.getLiteral( i );
        Literal lit = solver.getLiteral( literalId );
        if( solver.isTrue( lit ) )
        {
            if( weightConstraintRule.getBound() >= weightConstraintRule.getWeight( i ) )
                weightConstraintRule.decrementBound( weightConstraintRule.getWeight( i ) );
            else
                weightConstraintRule.setBound( 0 );
        }
        else if( !solver.isFalse( lit ) )
        {
            weightConstraintRule.incrementMaxPossibleValue( weightConstraintRule.getWeight( i ) );
            j++;
        }
    }
    
    weightConstraintRule.resize( j );
}


void
GringoNumericFormat::removeAndCheckSupport(
    NormalRule* rule )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Removing rule " << *rule );
    for( unsigned i = 0; i < rule->literals.size(); ++i )
    {
        Literal lit = rule->literals[ i ];
        if( !lit.isPossiblySupportedHeadAtom() )
            continue;
        assert( atomData[ lit.getVariable() ].numberOfHeadOccurrences > 0 );
        if( --atomData[ lit.getVariable() ].numberOfHeadOccurrences == 0 || ( atomData[ lit.getVariable() ].numberOfHeadOccurrences == 1 && solver.isTrue( lit.getVariable() ) ) )
            atomsWithSupportInference.push_back( lit.getVariable() );
    }
    rule->remove();
}

bool
GringoNumericFormat::shrinkPos( 
    NormalRule* rule, 
    Var var )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return true;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->literals.size(); ++j )
    {
        Literal& lit = rule->literals[ j ];
        if( lit.getVariable() != var )
            continue;
        if( lit.isUndefinedPositiveBodyLiteral() )
        {
            if( atomData[ var ].isSupported() )
            {
                lit = rule->literals.back();
                rule->literals.pop_back();
                onShrinkingBody( rule );
                return true;
            }
            else
            {
                lit = Literal::newTruePositiveBodyLiteral( lit.getVariable() );
                onShrinkingBody( rule );
                return false;
            }
        }
        else if( lit.isTruePositiveBodyLiteral() )
        {
            assert( atomData[ var ].isSupported() );
            lit = rule->literals.back();
            rule->literals.pop_back();
            onShrinkingBody( rule );
            return true;
        }
    }
    assert( 0 );
    return false;
}

void
GringoNumericFormat::onShrinkingBody(
    NormalRule* rule )
{
    assert( rule != NULL );
    trace_msg( parser, 4, "After shrink: " << *rule );
    if( rule->isFact() )
    {
        assert( rule->literals.size() == 1 && rule->literals.back().isPossiblySupportedHeadAtom() );
        addFact( rule->literals.back().getVariable() );
        rule->remove();
    }
    else
    {
        Var head = 0;
        unsigned headCount = 0;
        for( unsigned i = 0; i < rule->literals.size(); ++i )
        {
            Literal lit = rule->literals[ i ];
            if( lit.isHeadAtom() )
            {
                head = lit.getVariable();
                ++headCount;
            }
            else if( !lit.isTruePositiveBodyLiteral() )
                return;
        }
        if( headCount == 1 )
            solver.addClause( Literal( head, POSITIVE ) );
    }
}

void
GringoNumericFormat::shrinkNeg( 
    NormalRule* rule, 
    Var var )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->literals.size(); ++j )
    {
        Literal& lit = rule->literals[ j ];
        if( lit.getVariable() != var )
            continue;
        
        //TODO: Malvi: Check if this should be an assert.
        //if( lit.isNegativeBodyLiteral() )
        {
            lit = rule->literals.back();
            rule->literals.pop_back();
            onShrinkingBody( rule );
            return;
        }
    }
    
    assert( 0 );
}

void
GringoNumericFormat::onShrinkingHead(
    NormalRule* rule )
{
    assert( rule != NULL );
    trace_msg( parser, 4, "After shrink: " << *rule );
    if( rule->isFact() )
    {
        assert( rule->literals.size() == 1 && rule->literals.back().isPossiblySupportedHeadAtom() );
        addFact( rule->literals.back().getVariable() );
        rule->remove();
    }
    else if( rule->size() == 1 )
    {
        assert( !rule->literals.back().isHeadAtom() );
        solver.addClause( rule->literals.back() );
    }
    else
    {
        Var head = 0;
        unsigned headCount = 0;
        
        bool hasUndefined = false;
        
        for( unsigned i = 0; i < rule->literals.size(); ++i )
        {
            Literal lit = rule->literals[ i ];
            if( lit.isHeadAtom() )
            {
                headCount++;
//                assert_msg( !solver.isFalse( lit.getVariable() ), lit.getVariable() << " is false" );
                head = lit.getVariable();
            }
            else if( !lit.isTruePositiveBodyLiteral() )
                hasUndefined = true;
        }
        
        if( headCount == 0 )
        {
            bodyToConstraint( rule );
            rule->remove();
        }
        else if( headCount == 1 && !hasUndefined )
        {
            solver.addClause( Literal( head, POSITIVE ) );
        }                
    }
}

void
GringoNumericFormat::shrinkHead( 
    NormalRule* rule, 
    Var var )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->literals.size(); ++j )
    {
        Literal& lit = rule->literals[ j ];
        if( lit.getVariable() != var )
            continue;
        
        //TODO: Malvi: Check if this should be an assert.
        //if( lit.isNegativeBodyLiteral() )
        {
            lit = rule->literals.back();
            rule->literals.pop_back();
            onShrinkingHead( rule );
            return;
        }
    }
    
    assert( 0 );
}

void
GringoNumericFormat::updateMaxPossibleValueWeightConstraint(
    WeightConstraintRule* weightConstraintRule,
    unsigned int position )
{
    assert( weightConstraintRule != NULL );
    if( weightConstraintRule->isRemoved() )
        return;
    trace_msg( parser, 4, "Updating values for weightConstraintRule " << *weightConstraintRule );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "It is satisfied: skipping" );
        return;
    }
    
    weightConstraintRule->decrementMaxPossibleValue( weightConstraintRule->getWeight( position ) );
    
    assert( !weightConstraintRule->isTrue() );
    if( weightConstraintRule->isFalse() )
    {
        trace_msg( parser, 4, "Inferring (weight constraint) variable " << weightConstraintRule->getId() << " as false" );
        addFalseVariable( weightConstraintRule->getId() );
    }
    else
    {
        if( solver.isTrue( weightConstraintRule->getId() ) )
        {
            weightConstraintIsTrue( weightConstraintRule );
        }
        else if( solver.isFalse( weightConstraintRule->getId() ) )
        {
            weightConstraintIsFalse( weightConstraintRule );
        }
    }    
}

void
GringoNumericFormat::updateCurrentValueWeightConstraint(
    WeightConstraintRule* weightConstraintRule,
    unsigned int position )
{
    assert( weightConstraintRule != NULL );
    if( weightConstraintRule->isRemoved() )
        return;
    trace_msg( parser, 4, "Updating values for weightConstraintRule " << *weightConstraintRule );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "It is satisfied: skipping" );
        return;
    }
    
    weightConstraintRule->incrementCurrentValue( weightConstraintRule->getWeight( position ) );
    
    assert( !weightConstraintRule->isFalse() );
    if( weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "Inferring (weight constraint) variable " << weightConstraintRule->getId() << " as true" );
        addTrueVariable( weightConstraintRule->getId() );
    }
    else
    {
        if( solver.isTrue( weightConstraintRule->getId() ) )
        {
            weightConstraintIsTrue( weightConstraintRule );
        }
        else if( solver.isFalse( weightConstraintRule->getId() ) )
        {
            weightConstraintIsFalse( weightConstraintRule );
        }
    }    
}

void
GringoNumericFormat::weightConstraintIsTrue(
    WeightConstraintRule* weightConstraintRule )
{    
    assert( weightConstraintRule != NULL );
    assert( !weightConstraintRule->isRemoved() );
    trace_msg( parser, 4, "WeightConstraintRule " << *weightConstraintRule << " is true" );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "...and it is satisfied" );
        return;
    }
        
//    unsigned int maxPossibleValue = weightConstraintRule->maxPossibleValue;
    unsigned int i = 0;
    while( i < weightConstraintRule->size() )
    {
        int litId = weightConstraintRule->getLiteral( i );
        Literal lit = solver.getLiteral( litId );
        if( solver.isUndefined( lit ) )
        {            
            if( ( weightConstraintRule->getMaxPossibleValue() - weightConstraintRule->getWeight( i ) ) < weightConstraintRule->getBound() )
            {                
                assert( litId != 0 );
                if( litId > 0 )
                {
                    trace_msg( parser, 4, "Inferring " << litId << " as true" );
                    addTrueVariable( litId );
                }
                else
                {
                    trace_msg( parser, 4, "Inferring " << -litId << " as false" );
                    addFalseVariable( -litId );
                } 
            }
            else
                break;
        }        
        
        i++;
    }
}

void
GringoNumericFormat::weightConstraintIsFalse(
    WeightConstraintRule* weightConstraintRule )
{    
    assert( weightConstraintRule != NULL );
    assert( !weightConstraintRule->isRemoved() );
    trace_msg( parser, 4, "WeightConstraintRule " << *weightConstraintRule << " is false" );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "...and it is satisfied" );
        return;
    }

//    unsigned int currentValue = weightConstraintRule->currentValue;
    unsigned int i = 0;
    while( i < weightConstraintRule->size() )
    {
        int litId = weightConstraintRule->getLiteral( i );
        Literal lit = solver.getLiteral( litId );
        if( solver.isUndefined( lit ) )
        {
            if( ( weightConstraintRule->getCurrentValue() + weightConstraintRule->getWeight( i ) ) >= weightConstraintRule->getBound() )
            {
                assert( litId != 0 );
                if( litId > 0 )
                {
                    trace_msg( parser, 4, "Inferring " << litId << " as false" );
                    addFalseVariable( litId );
                }
                else
                {
                    trace_msg( parser, 4, "Inferring " << -litId << " as true" );
                    addTrueVariable( -litId );
                }
            }
            else
                break;
        }

        i++;
    }
}

void
GringoNumericFormat::atLeastOne( WeightConstraintRule* rule )
{    
    trace_msg( parser, 3, "Replacing " << *rule << " by a clause" );
    Clause* clause = solver.newClause( rule->size() );
    for( unsigned int j = 0; j < rule->size(); j++ )
    {
        assert( rule->getLiteral( j ) != 0 );
        Literal lit = solver.getLiteral( rule->getLiteral( j ) );
        assert( !solver.isTrue( lit ) );
        if( solver.isUndefined( lit ) )
            clause->addLiteral( lit );
    }
    solver.cleanAndAddClause( clause );
}

void
GringoNumericFormat::atMostOne( WeightConstraintRule* rule )
{    
    trace_msg( parser, 3, "Replacing " << *rule << " (at most one)" );
    trace_msg( parser, 4, "Delayed" );
    delayedAggregateRewriting.push_back( rule );
}

void
GringoNumericFormat::atMostOnePairwise( WeightConstraintRule* rule )
{
    cleanWeightConstraint( rule );
    trace_msg( parser, 4, "Pairwise rewriting of " << *rule );
    for( unsigned i = 0; i < rule->size(); ++i )
    {
        int lit1 = rule->getLiteral( i );
        for( unsigned j = i+1; j < rule->size(); ++j )
        {
            int lit2 = rule->getLiteral( j );
            Clause* clause = solver.newClause( 2 );
            clause->addLiteral( solver.getLiteral( -lit1 ) );
            clause->addLiteral( solver.getLiteral( -lit2 ) );
            trace_msg( parser, 5, "Adding clause: " << *clause );
            solver.addClause( clause );
        }
    }
}

void
GringoNumericFormat::atMostOneBimander( WeightConstraintRule* rule )
{
    cleanWeightConstraint( rule );
    trace_msg( parser, 4, "Bimander rewriting of " << *rule );
    
    for( unsigned i = 0; i < rule->size(); ++i )
    {
        Var v = abs( rule->getLiteral( i ) );
        solver.setFrozen( v );
    }
    
    unsigned last = rule->size() - rule->size() % 2;
    for( unsigned i = 0; i < last; i += 2 )
    {
        Clause* clause = solver.newClause( 2 );
        int lit = rule->getLiteral( i );
        int lit1 = rule->getLiteral( i + 1 );
        clause->addLiteral( solver.getLiteral( -lit ) );
        clause->addLiteral( solver.getLiteral( -lit1 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }
    
    unsigned nAux = ceil( log2( rule->size() ) ) - 1;
    unsigned firstAux = solver.numberOfVariables() + 1;
    for( unsigned i = 0; i < nAux; ++i )
    {
        solver.addVariable();
        solver.setFrozen( solver.numberOfVariables() );
    }
    for( unsigned i = 0; i < rule->size(); ++i )
    {
        int lit = rule->getLiteral( i );
        for( unsigned j = 0; j < nAux; ++j )
        {
            Clause* clause = solver.newClause( 2 );
            clause->addLiteral( solver.getLiteral( -lit ) );
            clause->addLiteral( solver.getLiteral( ( i / 2 ) & ( 1 << j ) ? firstAux + j : -firstAux - j ) );
            trace_msg( parser, 5, "Adding clause: " << *clause );
            solver.addClause( clause );
        }
    }
}

void
GringoNumericFormat::atMostOneSequential( WeightConstraintRule* rule )
{
    cleanWeightConstraint( rule );
    trace_msg( parser, 4, "Bimander rewriting of " << *rule );
    
    for( unsigned i = 0; i < rule->size(); ++i )
    {
        Var v = abs( rule->getLiteral( i ) );
        solver.setFrozen( v );
    }
    
    unsigned nAux = rule->size() - 1;
    unsigned firstAux = solver.numberOfVariables() + 1;
    for( unsigned i = 0; i < nAux; ++i )
    {
        solver.addVariable();
        solver.setFrozen( solver.numberOfVariables() );
    }
    for( unsigned i = 0; i < rule->size() - 1; ++i )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i ) ) );
        clause->addLiteral( solver.getLiteral( firstAux + i ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
        
        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i+1 ) ) );
        clause->addLiteral( solver.getLiteral( -firstAux - i ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }
    for( unsigned i = 1; i < nAux; ++i )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -firstAux - i + 1 ) );
        clause->addLiteral( solver.getLiteral( firstAux + i ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }
}

void
GringoNumericFormat::atMostOneBisequential( WeightConstraintRule* rule )
{
    cleanWeightConstraint( rule );
    trace_msg( parser, 4, "Bimander rewriting of " << *rule );
    
    for( unsigned i = 0; i < rule->size(); ++i )
    {
        Var v = abs( rule->getLiteral( i ) );
        solver.setFrozen( v );
    }
    
    unsigned nAux = ( rule->size() + 1 ) / 2 - 1;
    unsigned firstAux = solver.numberOfVariables() + 1;
    for( unsigned i = 0; i < nAux; ++i )
    {
        solver.addVariable();
        solver.setFrozen( solver.numberOfVariables() );
    }

    unsigned last = rule->size() - rule->size() % 2;
    for( unsigned i = 0; i < last; i += 2 )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i ) ) );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i + 1 ) ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }

    for( unsigned i = 0; i < last - 2; i += 2 )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i ) ) );
        clause->addLiteral( solver.getLiteral( firstAux + i/2 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );

        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i + 1 ) ) );
        clause->addLiteral( solver.getLiteral( firstAux + i/2 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
        
        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i + 2 ) ) );
        clause->addLiteral( solver.getLiteral( -firstAux - i/2 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
        
        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( i + 3 ) ) );
        clause->addLiteral( solver.getLiteral( -firstAux - i/2 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }
    if( last != rule->size() )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( rule->size() - 3 ) ) );
        clause->addLiteral( solver.getLiteral( firstAux + nAux - 1 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );

        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( rule->size() - 2 ) ) );
        clause->addLiteral( solver.getLiteral( firstAux + nAux - 1 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );

        clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -rule->getLiteral( rule->size() - 1 ) ) );
        clause->addLiteral( solver.getLiteral( -firstAux - nAux + 1 ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }

    for( unsigned i = 1; i < nAux; ++i )
    {
        Clause* clause = solver.newClause( 2 );
        clause->addLiteral( solver.getLiteral( -firstAux - i + 1 ) );
        clause->addLiteral( solver.getLiteral( firstAux + i ) );
        trace_msg( parser, 5, "Adding clause: " << *clause );
        solver.addClause( clause );
    }
}

Aggregate*
GringoNumericFormat::weightConstraintToAggregate( 
    WeightConstraintRule* weightConstraintRule )
{
    Literal aggregateLiteral = solver.getLiteral( weightConstraintRule->getId() );

    solver.setFrozen( aggregateLiteral.getVariable() );
    Aggregate* aggregate = new Aggregate();

    //The weight of this literal is computed in aggregate->updateBound()
    aggregate->addLiteral( aggregateLiteral.getOppositeLiteral(), 0 );

    for( unsigned int j = 0; j < weightConstraintRule->size(); j++ )
    {
        if( weightConstraintRule->getWeight( j ) > weightConstraintRule->getBound() )
            weightConstraintRule->setWeight( j, weightConstraintRule->getBound() );

        Literal lit = solver.getLiteral( weightConstraintRule->getLiteral( j ) );
        assert( solver.isUndefined( lit ) );         
        assert_msg( weightConstraintRule->getWeight( j ) <= weightConstraintRule->getBound(), weightConstraintRule->getWeight( j ) << ">" << weightConstraintRule->getBound() );
        aggregate->addLiteral( lit, weightConstraintRule->getWeight( j ) );
        solver.setFrozen( lit.getVariable() );
    }

    solver.attachAggregate( *aggregate ); 
    assert( aggregate->size() > 0 );
    aggregate->updateBound( solver, weightConstraintRule->getBound() );

    if( solver.isFalse( aggregateLiteral ) )
        aggregate->onLiteralFalse( solver, aggregateLiteral, -1 );
    else if( solver.isTrue( aggregateLiteral ) )
        aggregate->onLiteralFalse( solver, aggregateLiteral.getOppositeLiteral(), 1 );

    return aggregate;
}

void
GringoNumericFormat::addWeightConstraints()
{
    trace_msg( parser, 4, "Adding weight constraints (if any)" );
    for( unsigned int i = 0; i < weightConstraintRules.size(); i++ )
    {
        WeightConstraintRule* weightConstraintRule = weightConstraintRules[ i ];
        if( weightConstraintRule->isRemoved() )
            continue;
        trace_msg( parser, 4, "Considering weight constraint " << *weightConstraintRule );

        if( weightConstraintRule->isFalse() )
        {
            trace_msg( parser, 5, "... which is false: skipping it." );
            assert( solver.isFalse( weightConstraintRule->getId() ) );                        
            continue;
        }
        else if( weightConstraintRule->isTrue() )
        {
            trace_msg( parser, 5, "... which is true: skipping it." );
            assert( solver.isTrue( weightConstraintRule->getId() ) );
            continue;
        }
        
        cleanWeightConstraint( weightConstraintRule );
        
        Aggregate* aggregate = weightConstraintToAggregate( weightConstraintRule );
        solver.addAggregate( aggregate );
        trace_msg( parser, 4, "Adding aggregate " << *aggregate );
    }
}

void
GringoNumericFormat::cleanWeightConstraint(
    GringoNumericFormat::WeightConstraintRule* weightConstraintRule )
{
    unsigned int j = 0;
    for( unsigned int i = 0; i < weightConstraintRule->size(); i++ )
    {
        weightConstraintRule->overwrite( j, i );
        Literal lit = solver.getLiteral( weightConstraintRule->getLiteral( i ) );

        assert( weightConstraintRule->getWeight( i ) <= weightConstraintRule->getBound() );
        if( solver.isTrue( lit ) )
            weightConstraintRule->decrementBound( weightConstraintRule->getWeight( i ) );
        else if( !solver.isFalse( lit ) )
            ++j;        
    }    
    weightConstraintRule->resize( j );
}

void
GringoNumericFormat::addOptimizationRules()
{
    if( optimizationRules.empty() )
    {
        trace_msg( parser, 4, "Program has no optimization rule" );
        return;
    }
 
    trace_msg( parser, 4, "Adding " << optimizationRules.size() << " optimization rules" );
//    vector< uint64_t > maxCostOfLevelOfWeakConstraints;
    vector< int > literals;
    vector< uint64_t > weights;
    vector< unsigned int > levels;
//    computeLinearCostsForOptimizationRules( maxCostOfLevelOfWeakConstraints, literals, weights, levels );
    addOptimizationRules( literals, weights, levels );
    solver.setLevels( optimizationRules.size() );
    for( unsigned int j = 0; j < literals.size(); j++ )
    {        
        Literal lit = solver.getLiteral( literals[ j ] );
        if( solver.isTrue( lit ) )
        {
            assert( lit != Literal::null );
            solver.addOptimizationLiteral( lit, weights[ j ], levels[ j ], false );
        }
        else
        {
            solver.setFrozen( lit.getVariable() );
            assert( lit != Literal::null );
            solver.addOptimizationLiteral( lit, weights[ j ], levels[ j ], false );
        }
    }
//    solver.setMaxCostOfLevelOfOptimizationRules( maxCostOfLevelOfWeakConstraints );
}

//void
//GringoNumericFormat::computeLinearCostsForOptimizationRules(
//    vector< uint64_t >& maxCostOfLevelOfOptimizationRules,
//    vector< int >& literals,
//    vector< uint64_t >& weights,
//    vector< unsigned int >& levels )
//{
//    maxCostOfLevelOfOptimizationRules.push_back( 1 );
//    uint64_t currentMaxCost = 1;
//    for( unsigned int i = 0; i < optimizationRules.size(); i++ )
//    {
//        WeightConstraintRule* optimizationRule = optimizationRules[ i ];
//        for( unsigned int j = 0; j < optimizationRule->size(); j++ )
//        {
//            uint64_t newWeight = optimizationRule->getWeight( j ) * maxCostOfLevelOfOptimizationRules.back();
//            currentMaxCost += newWeight;
//
//            literals.push_back( optimizationRule->getLiteral( j ) );
//            weights.push_back( newWeight );
//            levels.push_back( i + 1 );            
//        }
//        assert_msg( currentMaxCost >= maxCostOfLevelOfOptimizationRules.back(), currentMaxCost << " < " << maxCostOfLevelOfOptimizationRules.back() );
//        maxCostOfLevelOfOptimizationRules.push_back( currentMaxCost );
//    }
//}

void
GringoNumericFormat::addOptimizationRules(
    vector< int >& literals,
    vector< uint64_t >& weights,
    vector< unsigned int >& levels )
{
    for( unsigned int i = 0; i < optimizationRules.size(); i++ )
    {
        WeightConstraintRule* optimizationRule = optimizationRules[ i ];
        for( unsigned int j = 0; j < optimizationRule->size(); j++ )
        {
            literals.push_back( optimizationRule->getLiteral( j ) );
            weights.push_back( optimizationRule->getWeight( j ) );
            levels.push_back( i );            
        }        
    }
}

void
GringoNumericFormat::shrinkDoubleNeg( 
    NormalRule* rule, 
    Var var )
{
    assert( rule != NULL );
    if( rule->isRemoved() )
        return;
    trace_msg( parser, 3, "Shrinking rule " << *rule );
    for( unsigned j = 0; j < rule->literals.size(); ++j )
    {
        Literal& lit = rule->literals[ j ];
        if( lit.getVariable() != var )
            continue;
        if( lit.isDoubleNegatedBodyLiteral() )
        {
            lit = rule->literals.back();
            rule->literals.pop_back();
            onShrinkingBody( rule );
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
    out << "{head (possibly supported): [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isPossiblySupportedHeadAtom() )
            out << rule.literals[ i ].getVariable() << " ";
    out << "], head (unsupported): [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isUnsupportedHeadAtom() )
            out << rule.literals[ i ].getVariable() << " ";
    out << "], negBody: [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isNegativeBodyLiteral() )
            out << rule.literals[ i ].getVariable() << " ";
    out << "], posBody: [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isUndefinedPositiveBodyLiteral() )
            out << rule.literals[ i ].getVariable() << " ";
    out << "], posBodyTrue: [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isTruePositiveBodyLiteral() )
            out << rule.literals[ i ].getVariable() << " ";
    out << "], doubleNegBody: [ ";
    for( unsigned i = 0; i < rule.literals.size(); ++i )
        if( rule.literals[ i ].isDoubleNegatedBodyLiteral() )
            out << rule.literals[ i ].getVariable() << " ";
    return out << "]}";
}

ostream&
operator<<( 
    ostream& out, 
    const GringoNumericFormat::WeightConstraintRule& rule )
{
    out << "{id: " << rule.id << ". " << " [";
    if( rule.literals.size() > 0 )
    {
        unsigned i = 0;
        for( ; i < rule.literals.size() - 1; ++i )
            out << rule.literals[ i ] << "=" << rule.weights[ i ] << ",";
        out << rule.literals[ i ] << "=" << rule.weights[ i ];
    }
    return out << "] >= " << rule.bound << "}";
}

void
GringoNumericFormat::clearDataStructures()
{
    while( !normalRules.empty() )
    {
        delete normalRules.back();
        normalRules.pop_back();
    }    
    
    while( !weightConstraintRules.empty() )
    {
        delete weightConstraintRules.back();
        weightConstraintRules.pop_back();
    }
    
    while( !optimizationRules.empty() )
    {
        delete optimizationRules.back();
        optimizationRules.pop_back();
    }
    
    while( !atomData.empty() )
    {
        atomData.back().clear();
        atomData.pop_back();
    }       
}

void
GringoNumericFormat::cleanData()
{
    for( unsigned int i = 2; i < atomData.size(); i++ )
    {
        AtomData& data = atomData[ i ];        
        
        unsigned int k = 0;
        for( unsigned int j = 0; j < data.headOccurrences.size(); j++ )
        {
            if( !data.headOccurrences[ j ]->isRemoved() )
                data.headOccurrences[ k++ ] = data.headOccurrences[ j ];            
        }
        data.headOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.negOccurrences.size(); j++ )
        {
            if( !data.negOccurrences[ j ]->isRemoved() )
                data.negOccurrences[ k++ ] = data.negOccurrences[ j ];            
        }
        data.negOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.posOccurrences.size(); j++ )
        {
            if( !data.posOccurrences[ j ]->isRemoved() )
                data.posOccurrences[ k++ ] = data.posOccurrences[ j ];            
        }
        data.posOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.doubleNegOccurrences.size(); j++ )
        {
            if( !data.doubleNegOccurrences[ j ]->isRemoved() )
                data.doubleNegOccurrences[ k++ ] = data.doubleNegOccurrences[ j ];            
        }
        data.doubleNegOccurrences.shrink( k );        
    }
    
    unsigned int j = 0;
    for( unsigned int i = 0; i < normalRules.size(); i++ )
    {
        if( !normalRules[ i ]->isRemoved() )
            normalRules[ j++ ] = normalRules[ i ];
        else
            delete normalRules[ i ];
    }
    normalRules.shrink( j );        
}

bool
GringoNumericFormat::addUndefinedLiteral(
    Clause* clause,
    Literal lit )
{
    if( solver.isTrue( lit ) )
        return false;
    if( solver.isUndefined( lit ) )
    {
        clause->addLiteral( lit );
    }
    return true;
}

void
GringoNumericFormat::computeCost()
{
//    uint64_t sum = 0;
//    for( unsigned int i = 0; i < normalRules.size(); i++ )
//    {
//        sum += normalRules[ i ]->sizeOf();
//    }
//    cout << "Normal Rules " << normalRules.size() << " " << ( sum / ( 1024 * 1024 ) ) << " " << ( ( normalRules.capacity() * sizeof( NormalRule* ) ) / ( 1024 * 1024 ) ) << endl;
//       
//    sum = 0;
//    for( unsigned int i = 0; i < atomData.size(); i++ )
//    {
//        sum += atomData[ i ].sizeOf();
//    }
//    cout << "AtomData " << atomData.size() << " " << ( sum / ( 1024 * 1024 ) ) << " " << ( ( atomData.capacity() * sizeof( AtomData ) ) / ( 1024 * 1024 ) ) << endl;       
}

bool
GringoNumericFormat::isHeadCycleFree(
    Component* componentPointer )
{
    Component& component = *componentPointer;
    unordered_set< NormalRule* > visited;

    for( unsigned int i = 0; i < component.size(); i++ )
    {
        Var v = component.getVariable( i );
        if( v >= atomData.size() )
            continue;

        Vector< NormalRule* >& headOccs = atomData[ v ].headOccurrences;
        for( unsigned int j = 0; j < headOccs.size(); j++ )
        {
            NormalRule* rule = headOccs[ j ];
            assert( rule != NULL );
            if( !rule->isRemoved() && !visited.insert( rule ).second )
                return false;            
        }
    }

    return true;
}

Clause*
GringoNumericFormat::normalRuleToClause(
    NormalRule* normalRule )
{
    trace_msg( parser, 3, "Converting normalRule " << *normalRule << " to clause" );
    Clause* c = new Clause( normalRule->size() );
    for( unsigned int i = 0; i < normalRule->size(); i++ )
        c->addLiteral( normalRule->literals[ i ] );
    return c;
}

void
GringoNumericFormat::onTrueHeadAtom(
    NormalRule* rule,
    Var headAtom )
{
    trace_msg( parser, 3, "On true head atom " << headAtom << " in " << *rule );
    for( unsigned int i = 0; i < rule->literals.size(); i++ )
    {        
        Literal& lit = rule->literals[ i ];     
        Var v = lit.getVariable();
        if( lit.isPossiblySupportedHeadAtom() && v != headAtom )
        {            
            lit.setUnsupportedHeadAtom();   
            if( --atomData[ v ].numberOfHeadOccurrences == 0 || ( atomData[ v ].numberOfHeadOccurrences == 1 && solver.isTrue( v ) ) )
                atomsWithSupportInference.push_back( v );
        }
    }
    trace_msg( parser, 4, "Results to " << *rule );
}

bool
GringoNumericFormat::isSupporting(
    NormalRule* rule,
    Var headAtom )
{
    for( unsigned int i = 0; i < rule->literals.size(); i++ )
        if( rule->literals[ i ].getVariable() == headAtom && rule->literals[ i ].isPossiblySupportedHeadAtom() )
            return true;

    return false;
}

void
GringoNumericFormat::addExternalPropagators()
{
    for( unsigned int i = 0; i < wasp::Options::pluginsFilenames.size(); i++ )
    {
        string filename = wasp::Options::pluginsFilenames[ i ];        
        ExternalPropagator* prop = new ExternalPropagator( filename.c_str(), wasp::Options::plugins_interpreter );
        solver.addExternalPropagator( prop );
    }
}