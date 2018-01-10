#include "GringoNumericFormat.h"

#include "../util/WaspConstants.h"
#include "../util/WaspErrorMessage.h"
#include "../Clause.h"
#include "../propagators/Aggregate.h"
#include "../util/Istream.h"
#include "../propagators/HCComponent.h"
#include "../propagators/ExternalPropagator.h"

#include <cassert>
#include <iostream>
#include <unordered_set>
#include <bitset>
#include <stdint.h>
using namespace std;

void
GringoNumericFormat::parse( istream& i )
{
    Istream input( i );
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
            WaspErrorMessage::errorDuringParsing( "Unsupported rule type." );
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

    simplify();    
    
    bodiesDictionary.clear();

    if( solver.conflictDetected() )
        return;
        
    if( solver.numberOfClauses() + normalRules.size() > 1000000 )
        solver.turnOffSimplifications();
    
    statistics( &solver, endParsing() );    
    statistics( &solver, startSCCs() );
    if( numberOfDisjunctiveRules == 0 || wasp::Options::shiftStrategy == SHIFT_NAIVE )
        computeSCCs();
    else
        computeSCCsDisjunctive();    
    statistics( &solver, endSCCs() );
    trace_msg( parser, 1, "Program is " << ( solver.tight() ? "tight" : "not tight" ) );
    
    if( !solver.tight() )
        computeGusStructures();        
    addWeightConstraints();
    addOptimizationRules();
    clearDataStructures();    
    statistics( &solver, startCompletion() );
    computeCompletion();    
    statistics( &solver, endCompletion() );
    solver.endPreprocessing();
    for( unsigned int i = 0; i < multiAggregates.size(); i++ )
        multiAggregates[ i ]->finalize( solver );   
    for( unsigned int i = 0; i < multiAggregates.size(); i++ )
        solver.attachMultiAggregate( *multiAggregates[ i ] );
    for( unsigned int i = 0; i < equivalences.size(); i++ )
    {
        trace_msg( parser, 1, "Adding equivalence " << equivalences[ i ].first << " <=> " << equivalences[ i ].second );
        addEquivalence( equivalences[ i ].first, equivalences[ i ].second );
    }        
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
    Rule* rule = new Rule( head[ 0 ] );
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

    statistics( &solver, readChoiceRule() );
    for( unsigned i = 1; i < headSize; ++i )
    {
        if( atomData[ head[ i ] ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
            continue;
        Rule* r = new Rule( *rule );
        r->literals[ 0 ] = Literal::newPossiblySupportedHeadAtom( head[ i ] );
        r->literals.back() = Literal::newDoubleNegatedBodyLiteral( head[ i ] );
        add( r, true, 0 );
    }

    if( atomData[ head[ 0 ] ].readNormalRule_negativeLiterals == readNormalRule_numberOfCalls )
        delete rule;
    else
        add( rule, true, 0 );
    
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
            if( atomData[ head[ i ] ].readNormalRule_headAtoms == readNormalRule_numberOfCalls )
                --headSize;
            else
            {
                atomData[ head[ i ] ].readNormalRule_headAtoms = readNormalRule_numberOfCalls;
                ++i;
            }
        }
    }
    if( headSize == 0 )
    {
        readConstraint( input );
        return;
    }
    
    unsigned bodySize, negativeSize;
    readBodySize( input, bodySize, negativeSize );
    Rule* rule = new Rule();
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
        statistics( &solver, readConstraint() );
    }
    else if( headSize == 1 )
    {
        if( rule->literals.size() == 0 )
        {
            addFact( head[ 0 ] );
            delete rule;
            statistics( &solver, readDisjunctiveRule() );
        }
        else
        {
            rule->addHeadAtom( head[ 0 ] );
            add( rule, false, numberOfTrueHeadAtoms );
            statistics( &solver, readDisjunctiveRule() );
        }
    }
    else
    {
        for( unsigned i = 0; i < headSize; ++i )
            rule->addHeadAtom( head[ i ] );        
        add( rule, false, numberOfTrueHeadAtoms );
        numberOfDisjunctiveRules++;
        statistics( &solver, readDisjunctiveRule() );
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
        WaspErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
    
    unsigned int counter = 0;
    WeightConstraint* weightConstraintRule = new WeightConstraint( 0, UINT64_MAX );
    
    Vector< int64_t > tmps;
    Vector< uint64_t > weights;
    while( counter < negativeSize )
    {
        input.read( tmp );
        createStructures( tmp );
        ++counter;
        tmps.push_back( -tmp );
    }
    while( counter < size )
    {
        input.read( tmp );
        createStructures( tmp );
        tmps.push_back( tmp );
        ++counter;
    }

    uint64_t bound = 1;
    counter = 0;
    while( counter < negativeSize )
    {        
        input.read( weight );
        weights.push_back( weight );
        bound += weight;
        ++counter;
    }
    while( counter < size )
    {
        input.read( weight );
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
    statistics( &solver, readWeakConstraints( size ) );
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
        WaspErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
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
    assert( bodySize >= 1 );

    readNormalRule_numberOfCalls++;
    assert( readNormalRule_numberOfCalls != 0 );

    bodiesDictionary.startInsertion();
    bodiesDictionary.addElement( head );    
    Rule* rule = new Rule( head );
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
        statistics( &solver, readConstraint() );
        delete rule;
    }
    else if( rule->isFact() )
    {
        delete rule;
        addFact( head );
        statistics( &solver, readNormalRule() );
    }
    else
    {
        add( rule, false, solver.isTrue( head ) ? 1 : 0 );
        if( firing )
            solver.addClause( Literal( head, POSITIVE ) );
        statistics( &solver, readNormalRule() );
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
        WaspErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );

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
    solver.cleanAndAddClause( clause );
    statistics( &solver, readConstraint() );
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
        WaspErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
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

    WeightConstraint* weightConstraintRule = new WeightConstraint( id, bound );    
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
    if( solver.isTrue( id ) )
        weightConstraintIsTrue( weightConstraintRule );
    else if( solver.isFalse( id ) )
        weightConstraintIsFalse( weightConstraintRule );
    statistics( &solver, readCount() );
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
        WaspErrorMessage::errorDuringParsing( "Size must be greater than or equal to negative size." );
    }
    
    WeightConstraint* weightConstraintRule = new WeightConstraint( id, bound );
    
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
    if( solver.isTrue( id ) )
        weightConstraintIsTrue( weightConstraintRule );
    else if( solver.isFalse( id ) )
        weightConstraintIsFalse( weightConstraintRule );
    statistics( &solver, readSum() );
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
            if( solver.conflictDetected() ) return;
        }
        while( !facts.empty() )
        {
            repeat = true;
            Var var = facts.back();
            facts.pop_back();
            propagateFact( var );
            if( solver.conflictDetected() ) return;
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
        Rule* rule = getRule( data.posOccurrences[ i ].getId() );
        assert( rule != NULL );
        if( !shrinkPos( rule, var ) )
            data.posOccurrences[ j++ ] = data.posOccurrences[ i ];
    }
    data.posOccurrences.shrink( j );

    for( unsigned i = 0; i < data.negOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.negOccurrences[ i ].getId() );
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.negOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.doubleNegOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.doubleNegOccurrences[ i ].getId() );
        assert( rule != NULL );
        shrinkDoubleNeg( rule, var );
    }
    data.doubleNegOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.headOccurrences[ i ].getId() );
        assert( rule != NULL );
        onTrueHeadAtom( rule, var );
    }    

    for( unsigned int i = 0; i < data.negWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraint* weightConstraintRule = data.negWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateMaxPossibleValueWeightConstraint( weightConstraintRule, data.positionsInNegWeightConstraints[ i ] );
    }
    data.negWeightConstraintsOccurrences.clear();
    data.positionsInNegWeightConstraints.clear();
    
    for( unsigned int i = 0; i < data.posWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraint* weightConstraintRule = data.posWeightConstraintsOccurrences[ i ];
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
        Rule* rule = getRule( data.doubleNegOccurrences[ i ].getId() );
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.doubleNegOccurrences.clearAndDelete();        
    
    for( unsigned i = 0; i < data.posOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.posOccurrences[ i ].getId() );
        assert( rule != NULL );
        removeAndCheckSupport( rule );
    }
    data.posOccurrences.clear();

    for( unsigned i = 0; i < data.negOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.negOccurrences[ i ].getId() );
        assert( rule != NULL );
        shrinkNeg( rule, var );
    }
    data.negOccurrences.clearAndDelete();
    
    for( unsigned i = 0; i < data.headOccurrences.size(); ++i )
    {
        Rule* rule = getRule( data.headOccurrences[ i ].getId() );
        assert( rule != NULL );
        if( rule->isRemoved() )
            continue;
                
        shrinkHead( rule, var );        
    }
    data.numberOfHeadOccurrences = 0;
    data.headOccurrences.clearAndDelete();
        
    for( unsigned int i = 0; i < data.negWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraint* weightConstraintRule = data.negWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateCurrentValueWeightConstraint( weightConstraintRule, data.positionsInNegWeightConstraints[ i ] );
    }
    data.negWeightConstraintsOccurrences.clearAndDelete();
    data.positionsInNegWeightConstraints.clearAndDelete();
    
    for( unsigned int i = 0; i < data.posWeightConstraintsOccurrences.size(); i++ )
    {
        WeightConstraint* weightConstraintRule = data.posWeightConstraintsOccurrences[ i ];
        assert( weightConstraintRule != NULL );
        updateMaxPossibleValueWeightConstraint( weightConstraintRule, data.positionsInPosWeightConstraints[ i ] );
    }
    data.posWeightConstraintsOccurrences.clearAndDelete();
    data.positionsInPosWeightConstraints.clearAndDelete();
    
    if( data.isWeightConstraint() )
        weightConstraintIsFalse( data.weightConstraintRule );    
}

void
GringoNumericFormat::bodyToConstraint(
    Rule* rule )
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
        Rule* rule = getRule( data.headOccurrences[ i ].getId() );
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
        Rule* rule = getRule( data.posOccurrences[ i ].getId() );
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

    if( wasp::Options::printAtomTable )
        cout << "% Atoms table: start" << endl;        
    
    char name[ 1024 ];
    while( nextAtom != 0 )
    {
        createStructures( nextAtom );
        input.getline( name, 1024 );
        VariableNames::setName( nextAtom, name );
        solver.addedVarName( nextAtom );
        if( wasp::Options::printAtomTable )
            cout << nextAtom << " " << name << endl;
        trace_msg( parser, 6, "Set name " << name << " for atom " << nextAtom );
        
        if( wasp::Options::queryAlgorithm != NO_QUERY )
            solver.setFrozen( nextAtom );
        input.read( nextAtom );        
    }
    
    if( wasp::Options::printAtomTable )
        cout << "% Atoms table: end" << endl; 
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
            Rule* r = new Rule();
            r->literals.push_back( Literal::newPossiblySupportedHeadAtom( nextAtom ) );            
            r->literals.push_back( Literal::newDoubleNegatedBodyLiteral( nextAtom ) );
            add( r, true, 0 );
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
        WaspErrorMessage::errorDuringParsing( ss.str() );
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

    trace_msg( parser, 2, "Creating GUS data structures for variable " << Literal( variable, POSITIVE ) << " whose supporting rule has " << crule->size() - 1 << " literals" );
    solver.setFrozen( variable );

    Component* component = solver.getComponent( variable );
    assert( component != NULL );
    component->setAuxVariable( variable );

    for( unsigned int j = 1; j < crule->size(); j++ )
    {
        Literal currentLiteral = crule->getAt( j );
        Var currentVariable = currentLiteral.getVariable();
        solver.setFrozen( currentVariable );
        
        if( currentLiteral.isDoubleNegatedBodyLiteral() )
            continue;
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
                trace_msg( parser, 3, "Adding " << currentLiteral.getOppositeLiteral() << " as external rule for " << Literal( variable, POSITIVE ) );
                component->addExternalLiteralForVariable( variable, currentLiteral.getOppositeLiteral() );
            }
        }
        else
        {
            trace_msg( parser, 3, "Adding " << currentLiteral.getOppositeLiteral() << " as external rule for " << Literal( variable, POSITIVE ) );
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

    trace_msg( parser, 2, "Creating GUS data structures for variable " << Literal( variable, POSITIVE ) << ", which has " << crule->size() - 1 << " supporting rules" );

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
            trace_msg( parser, 3, "Adding " << Literal( ruleVar, POSITIVE ) << " as internal rule for " << Literal( variable, POSITIVE ) );
            component->addInternalLiteralForVariable( variable, Literal( ruleVar, POSITIVE ) );
            component->addVariablePossiblySupportedByLiteral( variable, Literal( ruleVar, POSITIVE ) );
        }
        else
        {
            trace_msg( parser, 3, "Adding " << ruleLiteral << " as external rule for " << Literal( variable, POSITIVE ) );
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
        solver.addGUSData( gd );
    }

    unsigned int nbOfCallsHCC = 0;
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
                trace_msg( parser, 5, "Variable " << Literal( currentVariable, POSITIVE ) << " is in the cyclic component " << i );             
                if( solver.getComponent( currentVariable ) == NULL )
                    solver.setComponent( currentVariable, component );
                component->variableHasNoSourcePointer( currentVariable );
            }        
            solver.addPostPropagator( component );
        }
        else
        {
            nbOfCallsHCC++;
            trace_msg( parser, 4, "The component is non HCF" );            
            HCComponent* hcComponent = solver.createHCComponent( atomData.size() - 1 ); //new HCComponent( solver );

            hcComponent->setId( solver.numberOfHCComponents() );
            for( unsigned int j = 0; j < component->size(); j++ )
            {
                Var v = component->getVariable( j );
                solver.setFrozen( v );
                if( v >= atomData.size() )
                    continue;
                
                trace_msg( parser, 5, "Variable " << Literal( v, POSITIVE ) << " is in the cyclic component " << i );                
                hcComponent->addHCVariable( v );                
            }
            statistics( &solver, removeComponent( component->getId() ) );            
            component->remove();
            statistics( &solver, addCyclicHCComponent( hcComponent->size() ) );            
            
            for( unsigned int j = 0; j < hcComponent->size(); j++ )
            {                
                Var v = hcComponent->getVariable( j );
                assert( v < atomData.size() );
                Vector< AbstractRule >& headOccs = atomData[ v ].headOccurrences;
                for( unsigned int t = 0; t < headOccs.size(); t++ )
                {
                    Rule* rule = getRule( headOccs[ t ].getId() );
                    assert( rule != NULL );
                    if( rule->isRemoved() )
                        continue;
                    
                    if( rule->isHandledForModelChecker( nbOfCallsHCC ) )
                        continue;
                    
                    rule->setHandledForModelChecker( nbOfCallsHCC );
                    hcComponent->processRule( rule );
                }
            }
            
            hcComponent->processComponentBeforeStarting();            
            
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
    
    
    for( unsigned int i = 0; i < auxs.size(); i++ )
    {        
        Var variable = auxs[ i ].first;
        Component* component = solver.getComponent( variable );
        if( component == NULL )
            continue;        
        assert( component != NULL );
        component->setAuxVariable( variable );
        Literal literal = auxs[ i ].second;
        trace_msg( parser, 2, "Creating GUS data structures for variable " << Literal( variable, POSITIVE ) << ", which has 1 supporting literal: " << literal.getOppositeLiteral() );        
        assert( solver.inTheSameComponent( literal.getVariable(), variable ) );
        component->addAuxVariableSupportedByLiteral( variable, literal.getOppositeLiteral() );
        component->addInternalLiteralForVariable( variable, literal.getOppositeLiteral() );        
    }
    auxs.clearAndDelete();
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
        statistics( &solver, addedAtom() );
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
                    Rule* r = getRule( data.headOccurrences[ i ].getId() );
                    if( !r->isRemoved() && isSupporting( r, atomsWithSupportInference.back() ) )
                        break;                    
                }
                //data.headOccurrences.shrink( 1 );
                assert( i < data.headOccurrences.size() );
                Rule* rule = getRule( data.headOccurrences[ i ].getId() );
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
    Rule* rule )
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
    Rule* rule )
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
GringoNumericFormat::computeSCCsDisjunctive()
{
    assert( wasp::Options::shiftStrategy != SHIFT_NAIVE );
    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        AtomData& data = atomData[ i ];
        Var var = i;
        if( data.isSupported() || solver.isFalse( var ) || data.isWeightConstraint() )
            continue;
        
        Clause* crule = new Clause();
        crule->addLiteral( Literal( i, NEGATIVE ) );
        data.crule = crule;        
    }
    
    for( unsigned i = 2; i < atomData.size(); ++i )
    {
        trace_msg( parser, 2, "Processing atom " << Literal( i, POSITIVE ) << " with id " << i );
        AtomData& data = atomData[ i ];
        Var var = i;
        if( data.isSupported() || solver.isFalse( var ) || data.isWeightConstraint() )
            continue;
        assert( data.numberOfHeadOccurrences > 0 );
        {
            trace_msg( parser, 3, "Creating crule" );            
            Clause* crule = data.crule;
            assert( crule != NULL && crule->size() > 0 && crule->getAt( 0 ) == Literal( i, NEGATIVE ) );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                Rule* rule = getRule( data.headOccurrences[ j ].getId() );
                if( rule->isRemoved() )
                    continue;
                if( isSupporting( rule, var ) )
                {
                    if( rule->isHandled() )
                        continue;
                    rule->setHandled();
                    Vector< Var > headAtoms;
                    Literal addedLit;
                    if( !wasp::Options::compactReasonsForHCC || !rule->isBodyCreated() )
                    {
                        rule->setBodyCreated();
                        addedLit = createAuxForBody( rule, headAtoms );
                        if( wasp::Options::compactReasonsForHCC )
                            rule->setBodyAux( addedLit.getId() );
                    }
                    else
                    {
                        const Vector< Literal >& myLiterals = rule->literals;    
                        for( unsigned k = 0; k < myLiterals.size(); ++k )
                        {
                            Literal myLit = myLiterals[ k ];
                            if( myLit.isHeadAtom() )
                                headAtoms.push_back( myLit.getVariable() );
                        }
                        addedLit = rule->getBodyAux() != 0 ? solver.getLiteral( rule->getBodyAux() ) : Literal::null;
                    }
                    Var addedVar = addedLit.getVariable();
                    assert( !headAtoms.empty() );
                    if( headAtoms.size() == 1 || data.headOccurrences[ j ].isChoice() )
                    {                        
                        trace_msg( parser, 5, "Head contains one atom or the rule is a choice rule: add body in crule" );
                        for( unsigned k = 0; k < headAtoms.size(); k++ )
                        {
                            if( addedVar != 0 )
                            {
                                atomData[ headAtoms[ k ] ].crule->addLiteral( addedLit.getOppositeLiteral() );
                                if( addedLit.isPositiveBodyLiteral() )
                                    solver.addEdgeInDependencyGraph( headAtoms[ k ], addedVar );
                            }
                            else
                            {
                                assert( data.headOccurrences[ j ].isChoice() );
                                //Create a tautology:
                                atomData[ headAtoms[ k ] ].crule->addLiteral( Literal( headAtoms[ k ], POSITIVE ) );
                            }
                        }
                    }
                    else
                    {
                        trace_msg( parser, 5, "Rule contains a disjunction" );
                        solver.cleanAndAddClause( normalRuleToClause( rule ) );                        
                        Vector< Var > auxVars;
                        
                        switch( wasp::Options::shiftStrategy )
                        {
                            case SHIFT_PROPAGATOR:
                                createPropagatorForDisjunction( headAtoms, auxVars, addedLit );
                                break;
                            case SHIFT_LEFT_RIGHT:
                                createClausesForShift( headAtoms, auxVars, addedLit );
                                break;
                            case SHIFT_NORMALIZE:
                                normalizeHeads( headAtoms, auxVars, addedLit );
                                break;
                            case SHIFT_QUADRATIC:
                                quadraticStrategy( headAtoms, auxVars, addedLit );
                                break;
                            case SHIFT_QUADRATIC_AGGREGATE:
                                quadraticStrategyAggregate( headAtoms, auxVars, addedLit );
                                break;
                            case SHIFT_AUTO:
                                if( headAtoms.size() <= 4 )
                                    normalizeHeads( headAtoms, auxVars, addedLit );
                                else
                                    createPropagatorForDisjunction( headAtoms, auxVars, addedLit );
                                break;
                            default:
                                WaspErrorMessage::errorDuringParsing( "Unknown shift strategy" );
                        }                        
                            
                        assert( headAtoms.size() == auxVars.size() );
                        for( unsigned k = 0; k < auxVars.size(); k++ )
                        {
                            atomData[ headAtoms[ k ] ].crule->addLiteral( Literal( auxVars[ k ], POSITIVE ) );
                            if( addedLit.isPositiveBodyLiteral() )
                            {
                                solver.addEdgeInDependencyGraph( auxVars[ k ], addedVar );
                                solver.addEdgeInDependencyGraph( headAtoms[ k ], auxVars[ k ] );
                                
                                auxs.push_back( pair< Var, Literal >( auxVars[ k ], addedLit ) );
                            }
                        }
                    }
                }
                else
                {
                    if( wasp::Options::compactReasonsForHCC && !rule->isBodyCreated() )
                    {
                        rule->setBodyCreated();
                        Vector< Var > headAtoms;
                        rule->setBodyAux( createAuxForBody( rule, headAtoms ).getId() );
                    }
                    addDependencies( Literal( i, POSITIVE ), rule );
                }
            }
            crules.push_back( crule );
            trace_msg( parser, 5, "New crule: " << *crule );
        }
    }
    
    solver.propagateAtLevelZero();
    solver.computeStrongConnectedComponents();    
}

Literal
GringoNumericFormat::createAuxForBody(
    Rule* rule,
    Vector< Var >& headAtoms )
{
    const Vector< Literal >& literals = rule->literals;    
    Vector< Literal > body;
    for( unsigned k = 0; k < literals.size(); ++k )
    {
        Literal lit = literals[ k ];
//        if( lit.isDoubleNegatedBodyLiteral() )
//            continue;
        if( lit.isHeadAtom() )
            headAtoms.push_back( lit.getVariable() );
        else
            body.push_back( lit );                
    }
    
    if( body.empty() )
        return Literal::null;
    if( body.size() == 1 )
        return body[ 0 ];
    
    assert( body.size() > 1 );
    solver.addVariable();
    Var addedVar = solver.numberOfVariables();    
    trace_msg( parser, 4, "Creating aux var with id " << addedVar << " for body of " << *rule );
    Clause* cruleForBody = new Clause( literals.size() );
    cruleForBody->addLiteral( Literal( addedVar, POSITIVE ) );
    for( unsigned k = 0; k < body.size(); ++k )
    {
        Literal lit = body[ k ];
        cruleForBody->addLiteral( lit );
        if( lit.isPositiveBodyLiteral() )
            solver.addEdgeInDependencyGraph( addedVar, lit.getVariable() );        
    }
    trace_msg( parser, 5, " which is " << *cruleForBody );
    crules.push_back( cruleForBody );    
    return Literal::newUndefinedPositiveBodyLiteral( addedVar );
}

void
GringoNumericFormat::addBinaryImplication(
    Literal lit1,
    Literal lit2 )
{
    trace_msg( parser, 7, "Adding " << lit1 << " -> " << lit2 );
    Clause* clause = new Clause( 2 );
    clause->addLiteral( lit1.getOppositeLiteral() );
    clause->addLiteral( lit2 );
    solver.cleanAndAddClause( clause );
}

void
GringoNumericFormat::addTwoAndLiteralEquivalence(
    Literal lit1,
    Literal lit2,
    Literal lit3 )
{
    trace_msg( parser, 7, "Adding " << lit1 << " ^ " << lit2 << " <-> " << lit3 );
    Clause* clause = new Clause( 3 );
    clause->addLiteral( lit1.getOppositeLiteral() );
    clause->addLiteral( lit2.getOppositeLiteral() );
    clause->addLiteral( lit3 );
    solver.cleanAndAddClause( clause );
    
    Clause* clause2 = new Clause( 2 );
    clause2->addLiteral( lit1 );
    clause2->addLiteral( lit3.getOppositeLiteral() );
    solver.cleanAndAddClause( clause2 );
    
    Clause* clause3 = new Clause( 2 );
    clause3->addLiteral( lit2 );
    clause3->addLiteral( lit3.getOppositeLiteral() );
    solver.cleanAndAddClause( clause3 );
}

void
GringoNumericFormat::createPropagatorForDisjunction(
    const Vector< Var >& headAtoms,
    Vector< Var >& auxVars,
    Literal bodyLiteral )
{
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    Var firstTrue = 0;
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
        if( solver.isTrue( headAtoms[ i ] ) )
        {
            firstTrue = headAtoms[ i ];
            break;
        }
    
    trace_msg( parser, 4, "Creating propagator for disjunction" );
    DisjunctionPropagator* disjunctionPropagator;
    trace_msg( parser, 5, "Adding body literal " << bodyLiteral );

    Vector< Literal > lits;
    Vector< uint64_t > weights;
    
    if( bodyLiteral == Literal::null )
        disjunctionPropagator = new DisjunctionPropagator( Literal::null );
    else
    {
        solver.setFrozen( bodyLiteral.getVariable() );
        disjunctionPropagator = new DisjunctionPropagator( bodyLiteral.getOppositeLiteral() );
        lits.push_back( bodyLiteral );
        weights.push_back( 2 );
    }
        
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        solver.addVariable();
        Var v = solver.numberOfVariables();
//        VariableNames::setName( v, "S" + VariableNames::getName( headAtoms[ i ] ) );
        auxVars.push_back( v );
        Literal aux = Literal( v, POSITIVE );
        Literal headLit = Literal( headAtoms[ i ], POSITIVE );
        trace_msg( parser, 5, "Adding " << headLit << " and " << aux );
        disjunctionPropagator->addOriginalAndAuxLiterals( headLit, aux );
        solver.setFrozen( v );
        solver.setFrozen( headLit.getVariable() );
        addBinaryImplication( aux, headLit );
        lits.push_back( headLit );
        weights.push_back( 1 );
        lits.push_back( aux );
        weights.push_back( 1 );
        if( firstTrue != 0 && firstTrue != headAtoms[ i ] )
        {
            assert( !solver.isTrue( headAtoms[ i ] ) );
            solver.addClause( aux.getOppositeLiteral() );
        }
    }
    solver.addDisjunctionPropagator( disjunctionPropagator );
    trace_msg( parser, 5, "Finalizing propagator" );
    disjunctionPropagator->finalize( solver );
    trace_msg( parser, 6, "...Done" );
    Aggregate* aggregate = createPseudoBooleanConstraint( lits, weights, 2 );
    solver.addAggregate( aggregate );
    solver.attachAggregate( *aggregate );
    trace_msg( parser, 5, "Adding aggregate " << *aggregate );
    propagatedLiterals = solver.numberOfAssignedLiterals();
}

void
GringoNumericFormat::createClausesForShift(
    const Vector< Var >& headAtoms,
    Vector< Var >& auxVars,
    Literal bodyLiteral )
{
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    Vector< Var > left;
    Vector< Var > right;
    Vector< Literal > lits;
    Vector< uint64_t > weights;    
    if( bodyLiteral != Literal::null )
    {
        lits.push_back( bodyLiteral );
        weights.push_back( 2 );
    }
    
    solver.addVariable();
    Var aggrId = solver.numberOfVariables();
    Vector< Literal > moreLits;
    Vector< uint64_t > moreWeights;
    
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        solver.addVariable();
        Var v = solver.numberOfVariables();        
        auxVars.push_back( v );
        
//        VariableNames::setName( v, "S" + VariableNames::getName( headAtoms[ i ] ) );
        moreLits.push_back( Literal( headAtoms[ i ], POSITIVE ) );
        moreWeights.push_back( 1 );
        
        lits.push_back( Literal( headAtoms[ i ], POSITIVE ) );
        weights.push_back( 1 );
        lits.push_back( Literal( v, POSITIVE ) );
        weights.push_back( 1 );
        //not Body -> not aux
        if( bodyLiteral != Literal::null )
            addBinaryImplication( bodyLiteral, Literal( v, NEGATIVE ) );
        
        //aux -> head
        addBinaryImplication( Literal( v, POSITIVE ), Literal( headAtoms[ i ], POSITIVE ) );
        
        if( i != 0 )
        {
            solver.addVariable();
//            VariableNames::setName( solver.numberOfVariables(), "r" + VariableNames::getName( headAtoms[ i ] ) );
            right.push_back( solver.numberOfVariables() );

            solver.addVariable();
//            VariableNames::setName( solver.numberOfVariables(), "l" + VariableNames::getName( headAtoms[ i - 1 ] ) );
            left.push_back( solver.numberOfVariables() );
        }        
    }
    
    Aggregate* moreThanOne = createAggregate( moreLits, moreWeights, 2, Literal( aggrId, POSITIVE ) );
    assert( solver.isUndefined( aggrId ) );
    solver.addAggregate( moreThanOne );
    solver.attachAggregate( *moreThanOne );
    trace_msg( parser, 5, "Adding aggregate " << *moreThanOne );
    
    assert( right.size() == left.size() && !right.empty() );
    addBinaryImplication( Literal( aggrId, POSITIVE ), Literal( right[ 0 ], NEGATIVE ) );
    addBinaryImplication( Literal( aggrId, POSITIVE ), Literal( left.back(), NEGATIVE ) );
    
    assert( left.size() == right.size() );
    assert( left.size() == headAtoms.size() - 1 );
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        Var head = headAtoms[ i ];
        if( i != headAtoms.size() - 1 )
        {
            //head -> not r_{head+1}
            addBinaryImplication( Literal( head, POSITIVE ), Literal( right[ i ], NEGATIVE ) );
            //not r_{head+1} -> not aux_{head+1}
            addBinaryImplication( Literal( right[ i ], NEGATIVE ), Literal( auxVars[ i + 1 ], NEGATIVE ) );            
        }
        if( i != 0 )
        {
            //head -> not l_{head-1}
            addBinaryImplication( Literal( head, POSITIVE ), Literal( left[ i - 1 ], NEGATIVE ) );
            //not l_{head-1} -> not aux_{head-1}
            addBinaryImplication( Literal( left[ i - 1 ], NEGATIVE ), Literal( auxVars[ i - 1 ], NEGATIVE ) );
        }
    }
    
    assert( left.size() == right.size() );
    unsigned int i = 0;
    for( ; i < right.size() - 1; i++ )
    {
        //not r_{head} -> not r_{head+1}
        addBinaryImplication( Literal( right[ i ], NEGATIVE ), Literal( right[ i + 1 ], NEGATIVE ) );
        //not l_{head} -> not l_{head-1}
        addBinaryImplication( Literal( left[ i + 1 ], NEGATIVE ), Literal( left[ i ], NEGATIVE ) );
        if( bodyLiteral != Literal::null )
        {
            addBinaryImplication( bodyLiteral, Literal( right[ i ], NEGATIVE ) );
            addBinaryImplication( bodyLiteral, Literal( left[ i ], NEGATIVE ) );
        }
    }

    if( bodyLiteral != Literal::null )
    {
        addBinaryImplication( bodyLiteral, Literal( right[ i ], NEGATIVE ) );
        addBinaryImplication( bodyLiteral, Literal( left[ i ], NEGATIVE ) );
    }
    Aggregate* aggregate = createPseudoBooleanConstraint( lits, weights, 2 );
    solver.addAggregate( aggregate );
    solver.attachAggregate( *aggregate );
    trace_msg( parser, 5, "Adding aggregate " << *aggregate );
    propagatedLiterals = solver.numberOfAssignedLiterals();
}

void
GringoNumericFormat::normalizeHeads(
    const Vector< Var >& headAtoms,
    Vector< Var >& auxVars,
    Literal bodyLiteral )
{    
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    Vector< Var > auxs;
    Vector< Var > sAuxs;
//    static unsigned int count = 0;
//    count++;
    
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        solver.addVariable();
        Var v = solver.numberOfVariables();        
        auxVars.push_back( v );
        
        addBinaryImplication( Literal( v, POSITIVE ), Literal( headAtoms[ i ], POSITIVE ) );
//        VariableNames::setName( v, "s" + VariableNames::getName( headAtoms[ i ] ) + to_string( count ) );
                
        if( i == 0 || i == 1 )
            continue;
        
//        string name = "";
//        for( unsigned int j = i - 1; j < headAtoms.size(); j++ )
//            name += VariableNames::getName( headAtoms[ j ] );
//        name += to_string( count );
        
        solver.addVariable();
        Var aux = solver.numberOfVariables();
        auxs.push_back( aux );
//        VariableNames::setName( aux, name );
        
        solver.addVariable();
        Var sAux = solver.numberOfVariables();
        sAuxs.push_back( sAux );
//        VariableNames::setName( sAux, "s" + name );
        addBinaryImplication( Literal( aux, NEGATIVE ), Literal( sAux, NEGATIVE ) );
    }        
    
    if( headAtoms.size() > 2 )
    {
        for( unsigned int i = 0; i < headAtoms.size() - 1; i++ )
        {
            if( i == 0 )
            {
                if( bodyLiteral == Literal::null )
                {
                    addEquivalence( Literal( auxs[ i ], NEGATIVE ), Literal( auxVars[ i ], POSITIVE ) );
                    addEquivalence( Literal( headAtoms[ i ], NEGATIVE ), Literal( sAuxs[ i ], POSITIVE ) );
                }
                else
                {
                    addTwoAndLiteralEquivalence( bodyLiteral.getOppositeLiteral(), Literal( auxs[ i ], NEGATIVE ), Literal( auxVars[ i ], POSITIVE ) );
                    addTwoAndLiteralEquivalence( bodyLiteral.getOppositeLiteral(), Literal( headAtoms[ i ], NEGATIVE ), Literal( sAuxs[ i ], POSITIVE ) );
                }
            }
            else if( i == headAtoms.size() - 2 )
            {
                addTwoAndLiteralEquivalence( Literal( sAuxs[ i - 1 ], POSITIVE ), Literal( headAtoms[ i ], NEGATIVE ), Literal( auxVars[ i + 1 ], POSITIVE ) );
                addTwoAndLiteralEquivalence( Literal( sAuxs[ i - 1 ], POSITIVE ), Literal( headAtoms[ i + 1 ], NEGATIVE ), Literal( auxVars[ i ], POSITIVE ) );
                addLiteralTwoOrEquivalence( Literal( auxs[ i - 1 ], POSITIVE ), Literal( headAtoms[ i ], POSITIVE ), Literal( headAtoms[ i + 1 ], POSITIVE ) );
            }
            else
            {
                addTwoAndLiteralEquivalence( Literal( sAuxs[ i - 1 ], POSITIVE ), Literal( auxs[ i ], NEGATIVE ), Literal( auxVars[ i ], POSITIVE ) );
                addTwoAndLiteralEquivalence( Literal( sAuxs[ i - 1 ], POSITIVE ), Literal( headAtoms[ i ], NEGATIVE ), Literal( sAuxs[ i ], POSITIVE ) );
                addLiteralTwoOrEquivalence( Literal( auxs[ i - 1 ], POSITIVE ), Literal( headAtoms[ i ], POSITIVE ), Literal( auxs[ i ], POSITIVE ) );
            }            
        }
    }
    else
    {
        assert( headAtoms.size() == 2 );
        if( bodyLiteral == Literal::null )
        {
            addEquivalence( Literal( headAtoms[ 0 ], NEGATIVE ), Literal( auxVars[ 1 ], POSITIVE ) );
            addEquivalence( Literal( headAtoms[ 1 ], NEGATIVE ), Literal( auxVars[ 0 ], POSITIVE ) );
        }
        else
        {
            addTwoAndLiteralEquivalence( bodyLiteral.getOppositeLiteral(), Literal( headAtoms[ 0 ], NEGATIVE ), Literal( auxVars[ 1 ], POSITIVE ) );
            addTwoAndLiteralEquivalence( bodyLiteral.getOppositeLiteral(), Literal( headAtoms[ 1 ], NEGATIVE ), Literal( auxVars[ 0 ], POSITIVE ) );
        }
    }
   
    propagatedLiterals = solver.numberOfAssignedLiterals();
}

void
GringoNumericFormat::quadraticStrategy(
    const Vector< Var >& headAtoms,
    Vector< Var >& auxVars,
    Literal bodyLiteral )
{    
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    
    Vector< Literal > lits;
    Vector< uint64_t > weights;
    
    if( bodyLiteral != Literal::null )
    {
        lits.push_back( bodyLiteral );
        weights.push_back( 2 );
    }
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        solver.addVariable();
        Var v = solver.numberOfVariables();
        auxVars.push_back( v );
        addBinaryImplication( Literal( v, POSITIVE ), Literal( headAtoms[ i ], POSITIVE ) );
        lits.push_back( Literal( headAtoms[ i ], POSITIVE ) );
        weights.push_back( 1 );
        lits.push_back( Literal( v, POSITIVE ) );
        weights.push_back( 1 );
    }        
    
    assert( headAtoms.size() == auxVars.size() );
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        Var head = headAtoms[ i ];
        for( unsigned int j = 0; j < auxVars.size(); j++ )
        {
            if( i == j )
                continue;
            Var aux = auxVars[ j ];                
            addBinaryImplication( Literal( head, POSITIVE ), Literal( aux, NEGATIVE ) );
        }
        
        if( bodyLiteral != Literal::null )
            addBinaryImplication( bodyLiteral, Literal( auxVars[ i ], NEGATIVE ) );
    }
    
    Aggregate* aggregate = createPseudoBooleanConstraint( lits, weights, 2 );
    solver.addAggregate( aggregate );
    solver.attachAggregate( *aggregate );
    trace_msg( parser, 5, "Adding aggregate " << *aggregate );
    propagatedLiterals = solver.numberOfAssignedLiterals();
}

void
GringoNumericFormat::quadraticStrategyAggregate(
    const Vector< Var >& headAtoms,
    Vector< Var >& auxVars,
    Literal bodyLiteral )
{    
    assert( propagatedLiterals == solver.numberOfAssignedLiterals() );
    
    Vector< Literal > lits;
    Vector< uint64_t > weights;
    
    if( bodyLiteral != Literal::null )
    {
        lits.push_back( bodyLiteral );
        weights.push_back( 2 );
    }
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        solver.addVariable();
        Var v = solver.numberOfVariables();
        auxVars.push_back( v );
        addBinaryImplication( Literal( v, POSITIVE ), Literal( headAtoms[ i ], POSITIVE ) );
        lits.push_back( Literal( headAtoms[ i ], POSITIVE ) );
        weights.push_back( 1 );
        lits.push_back( Literal( v, POSITIVE ) );
        weights.push_back( 1 );
    }        
    
    assert( headAtoms.size() == auxVars.size() );
    for( unsigned int i = 0; i < headAtoms.size(); i++ )
    {
        Var head = headAtoms[ i ];
        Vector< Literal > ls;
        Vector< uint64_t > wt;
        
        ls.push_back( Literal( head, NEGATIVE ) );
        wt.push_back( headAtoms.size() - 1 );
        
        for( unsigned int j = 0; j < auxVars.size(); j++ )
        {
            if( i == j )
                continue;
            Var aux = auxVars[ j ];                
            ls.push_back( Literal( aux, NEGATIVE ) );
            wt.push_back( 1 );
        }                
        
        if( bodyLiteral != Literal::null )
            addBinaryImplication( bodyLiteral, Literal( auxVars[ i ], NEGATIVE ) );
        
        Aggregate* aggr = createPseudoBooleanConstraint( ls, wt, headAtoms.size() - 1 );
        solver.addAggregate( aggr );
        solver.attachAggregate( *aggr );
        trace_msg( parser, 5, "Adding aggregate " << *aggr );
    }
    
    Aggregate* aggregate = createPseudoBooleanConstraint( lits, weights, 2 );
    solver.addAggregate( aggregate );
    solver.attachAggregate( *aggregate );
    trace_msg( parser, 5, "Adding aggregate " << *aggregate );
    propagatedLiterals = solver.numberOfAssignedLiterals();
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
        
        if( !wasp::Options::oneDefShift && data.numberOfHeadOccurrences == 1 )
        {
            trace_msg( parser, 3, "Only one defining rule" );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                Rule* r = getRule( data.headOccurrences[ j ].getId() );
                if( r->isRemoved() )
                    continue;
                if( isSupporting( r, var ) )
                    createCrule( Literal( i, POSITIVE ), var, r );
                else
                    addDependencies( Literal( i, POSITIVE ), r );
            }
        }
        else
        {
            trace_msg( parser, 3, "More than one defining rule" );            
            Clause* crule = solver.newClause( data.numberOfHeadOccurrences + 1 );            
            crule->addLiteral( Literal( i, NEGATIVE ) );
            for( unsigned j = 0; j < data.headOccurrences.size(); ++j )
            {
                Rule* r = getRule( data.headOccurrences[ j ].getId() );
                if( r->isRemoved() )
                    continue;
                if( isSupporting( r, var ) )
                {
                    // TODO: MALVI: handle disjunctive rules
                    if( r->literals.size() == 2 )
                    {
                        assert( r->literals[ 0 ].getVariable() == i || r->literals[ 1 ].getVariable() == i );
                        Literal lit = r->literals[ 0 ].getVariable() == i ? r->literals[ 1 ] : r->literals[ 0 ];
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
                        createCrule( Literal( solver.numberOfVariables(), POSITIVE ), var, r );
                    }
                }
                else
                    addDependencies( Literal( i, POSITIVE ), r );
            }
//            data.crule = crule;
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
        
        assert_msg( propagatedLiterals == solver.numberOfAssignedLiterals() || ( propagatedLiterals + 1 == solver.numberOfAssignedLiterals() && solver.getAssignedVariable( propagatedLiterals ) == lit.getVariable() && lit.getVariable() >= atomData.size() ), "difference is " << ( solver.numberOfAssignedLiterals() - propagatedLiterals ) );
        propagatedLiterals = solver.numberOfAssignedLiterals();
    }    
}

void
GringoNumericFormat::add(
    Rule* rule,
    bool isChoice,
    unsigned int numberOfTrueHeadAtoms )
{
    assert( rule != NULL );
    assert( !rule->isFact() );

    AbstractRule abstractRule( normalRules.size(), isChoice );
    for( unsigned i = 0; i < rule->literals.size(); ++i )
    {        
        Literal lit = rule->literals[ i ];
        if( lit.isPossiblySupportedHeadAtom() )
        {            
            Var v = lit.getVariable();
            AtomData& headData = atomData[ v ];
            headData.headOccurrences.push_back( abstractRule );
            if( numberOfTrueHeadAtoms == 0 || ( numberOfTrueHeadAtoms == 1 && solver.isTrue( v ) ) )
                headData.numberOfHeadOccurrences++;
            else
                rule->literals[ i ].setUnsupportedHeadAtom();            
        }
        else if( lit.isPositiveBodyLiteral() )
            atomData[ lit.getVariable() ].posOccurrences.push_back( abstractRule );
        else if( lit.isNegativeBodyLiteral() )
            atomData[ lit.getVariable() ].negOccurrences.push_back( abstractRule );
        else if( lit.isDoubleNegatedBodyLiteral() )
            atomData[ lit.getVariable() ].doubleNegOccurrences.push_back( abstractRule );
    }
    trace_msg( parser, 2, "Adding rule " << *rule );
    normalRules.push_back( rule );    
}

void
GringoNumericFormat::add(
    WeightConstraint* weightConstraintRule )
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
    WeightConstraint* weightConstraintRule )
{
    assert( weightConstraintRule != NULL );
    
    trace_msg( parser, 2, "Adding weight constraint rule " << *weightConstraintRule );
    weightConstraintRule->sort();
    trace_msg( parser, 2, "Sorted weight constraint rule " << *weightConstraintRule );
    optimizationRules.push_back( weightConstraintRule );   
}

void
GringoNumericFormat::removeSatisfiedLiterals(
    WeightConstraint* weightConstraintRulePointer )
{
    assert( weightConstraintRulePointer != NULL );
    assert( weightConstraintRulePointer->sameSizeOfInternalVectors() );
    
    WeightConstraint& weightConstraintRule = *weightConstraintRulePointer;
    
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
    Rule* rule )
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
    Rule* rule, 
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
    Rule* rule )
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
    Rule* rule, 
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
    Rule* rule )
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
    Rule* rule, 
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
    WeightConstraint* weightConstraintRule,
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
            weightConstraintIsTrue( weightConstraintRule );
        else if( solver.isFalse( weightConstraintRule->getId() ) )
            weightConstraintIsFalse( weightConstraintRule );        
    }    
}

void
GringoNumericFormat::updateCurrentValueWeightConstraint(
    WeightConstraint* weightConstraintRule,
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
            weightConstraintIsTrue( weightConstraintRule );
        else if( solver.isFalse( weightConstraintRule->getId() ) )
            weightConstraintIsFalse( weightConstraintRule );        
    }    
}

void
GringoNumericFormat::weightConstraintIsTrue(
    WeightConstraint* weightConstraintRule )
{    
    assert( weightConstraintRule != NULL );
    assert( !weightConstraintRule->isRemoved() );
    trace_msg( parser, 4, "WeightConstraint " << *weightConstraintRule << " is true" );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "...and it is satisfied" );
        return;
    }

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
    WeightConstraint* weightConstraintRule )
{    
    assert( weightConstraintRule != NULL );
    assert( !weightConstraintRule->isRemoved() );
    trace_msg( parser, 4, "WeightConstraint " << *weightConstraintRule << " is false" );
    if( weightConstraintRule->isFalse() || weightConstraintRule->isTrue() )
    {
        trace_msg( parser, 4, "...and it is satisfied" );
        return;
    }

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
GringoNumericFormat::atLeastOne( WeightConstraint* rule )
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
GringoNumericFormat::atMostOne( WeightConstraint* rule )
{    
    trace_msg( parser, 3, "Replacing " << *rule << " (at most one)" );
    trace_msg( parser, 4, "Delayed" );
    delayedAggregateRewriting.push_back( rule );
}

void
GringoNumericFormat::atMostOnePairwise( WeightConstraint* rule )
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
GringoNumericFormat::atMostOneBimander( WeightConstraint* rule )
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
GringoNumericFormat::atMostOneSequential( WeightConstraint* rule )
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
GringoNumericFormat::atMostOneBisequential( WeightConstraint* rule )
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
    WeightConstraint* weightConstraintRule )
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
    unordered_map< unsigned int, unsigned int > used;
    Trie aggregatesTrie;
    trace_msg( parser, 1, "Adding weight constraints (if any)" );
    for( unsigned int i = 0; i < weightConstraintRules.size(); i++ )
    {
        WeightConstraint* weightConstraintRule = weightConstraintRules[ i ];
        if( weightConstraintRule->isRemoved() )
            continue;
        trace_msg( parser, 2, "Considering weight constraint " << *weightConstraintRule );

        if( weightConstraintRule->isFalse() )
        {
            trace_msg( parser, 3, "... which is false: skipping it." );
            assert( solver.isFalse( weightConstraintRule->getId() ) );                        
            continue;
        }
        else if( weightConstraintRule->isTrue() )
        {
            trace_msg( parser, 3, "... which is true: skipping it." );
            assert( solver.isTrue( weightConstraintRule->getId() ) );
            continue;
        }
        
        cleanWeightConstraint( weightConstraintRule );
        solver.notifyAggregate( weightConstraintRule );
        
        if( !wasp::Options::multiAggregates )
        {
            Aggregate* aggregate = weightConstraintToAggregate( weightConstraintRule );
            solver.addAggregate( aggregate );            
            trace_msg( parser, 2, "Adding aggregate " << *aggregate );  
            continue;
        }
        
        aggregatesTrie.startInsertion();
        for( unsigned int j = 0; j < weightConstraintRule->size(); j++ )
        {
            aggregatesTrie.addElement( weightConstraintRule->getLiteral( j ) );
            aggregatesTrie.addElement( weightConstraintRule->getWeight( j ) );
        }
        
        unsigned int id = aggregatesTrie.endInsertionGetId();
        if( id != UINT_MAX )
        {
            assert_msg( ( id - 1 ) < multiAggregates.size(), ( id - 1 ) << " >= " << multiAggregates.size() );
            MultiAggregate* multi = multiAggregates[ id - 1 ];
            unsigned int pos = multi->hasBound( weightConstraintRule->getBound() );
            if( pos == UINT_MAX )
            {
                multi->addBound( solver.getLiteral( weightConstraintRule->getId() ), weightConstraintRule->getBound() );
                used[ id - 1 ] = UINT_MAX;
            }
            else
            {
                //addEquivalence( solver.getLiteral( weightConstraintRule->getId() ), multi->getId( pos ) );
                equivalences.push_back( pair< Literal, Literal >( solver.getLiteral( weightConstraintRule->getId() ), multi->getId( pos ) ) );
                weightConstraintRule->remove();
            }
        }
        else
        {
            MultiAggregate* multi = new MultiAggregate();
            for( unsigned int j = 0; j < weightConstraintRule->size(); j++ )
                multi->addLiteral( solver.getLiteral( weightConstraintRule->getLiteral( j ) ), weightConstraintRule->getWeight( j ) );
            multi->addBound( solver.getLiteral( weightConstraintRule->getId() ), weightConstraintRule->getBound() );
            used[ multiAggregates.size() ] = i;
            multiAggregates.push_back( multi );
        }
    }    
    
    unsigned int j = 0;
    for( unsigned int i = 0; i < multiAggregates.size(); i++ )
    {
        multiAggregates[ j ] = multiAggregates[ i ];
        unsigned int pos = used[ i ];
        if( pos == UINT_MAX )
        {
            trace_msg( parser, 2, "Adding multi aggregate " << *multiAggregates[ i ] );
            solver.addMultiAggregate( multiAggregates[ i ] );
            j++;
            continue;
        }
        delete multiAggregates[ i ];
        assert( pos < weightConstraintRules.size() );
        if( weightConstraintRules[ pos ]->isRemoved() ) continue;
        Aggregate* aggregate = weightConstraintToAggregate( weightConstraintRules[ pos ] );
        solver.addAggregate( aggregate );
        trace_msg( parser, 2, "Adding aggregate " << *aggregate );        
    }
    multiAggregates.resize( j );
}

void
GringoNumericFormat::cleanWeightConstraint(
    WeightConstraint* weightConstraintRule )
{
    unsigned int j = 0;
    for( unsigned int i = 0; i < weightConstraintRule->size(); i++ )
    {
        weightConstraintRule->overwrite( j, i );
        Literal lit = solver.getLiteral( weightConstraintRule->getLiteral( i ) );

        if( weightConstraintRule->getWeight( i ) > weightConstraintRule->getBound() )
            weightConstraintRule->setWeight( i, weightConstraintRule->getBound() );
        assert_msg( weightConstraintRule->getWeight( i ) <= weightConstraintRule->getBound(), weightConstraintRule->getWeight( i ) << " > " << weightConstraintRule->getBound() );
        if( solver.isTrue( lit ) )
            weightConstraintRule->decrementBound( weightConstraintRule->getWeight( i ) );
        else if( !solver.isFalse( lit ) )
            ++j;        
    }    
    weightConstraintRule->resize( j );
    trace_msg( parser, 3, "...after clean: " << *weightConstraintRule );
}

void
GringoNumericFormat::addOptimizationRules()
{
    trace_msg( parser, 1, "Program contains " << optimizationRules.size() << " optimization rule" << ( optimizationRules.size() == 1 ? "s" : "" ) );
    if( optimizationRules.empty() )
        return;
    
    vector< int > literals;
    vector< uint64_t > weights;
    vector< unsigned int > levels;
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
}

void
GringoNumericFormat::addOptimizationRules(
    vector< int >& literals,
    vector< uint64_t >& weights,
    vector< unsigned int >& levels )
{
    for( unsigned int i = 0; i < optimizationRules.size(); i++ )
    {
        WeightConstraint* optimizationRule = optimizationRules[ i ];
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
    Rule* rule, 
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
            if( !getRule( data.headOccurrences[ j ].getId() )->isRemoved() )
                data.headOccurrences[ k++ ] = data.headOccurrences[ j ];            
        }
        data.headOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.negOccurrences.size(); j++ )
        {
            if( !getRule( data.negOccurrences[ j ].getId() )->isRemoved() )
                data.negOccurrences[ k++ ] = data.negOccurrences[ j ];            
        }
        data.negOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.posOccurrences.size(); j++ )
        {
            if( !getRule( data.posOccurrences[ j ].getId() )->isRemoved() )
                data.posOccurrences[ k++ ] = data.posOccurrences[ j ];            
        }
        data.posOccurrences.shrink( k );
        
        k = 0;
        for( unsigned int j = 0; j < data.doubleNegOccurrences.size(); j++ )
        {
            if( !getRule( data.doubleNegOccurrences[ j ].getId() )->isRemoved() )
                data.doubleNegOccurrences[ k++ ] = data.doubleNegOccurrences[ j ];            
        }
        data.doubleNegOccurrences.shrink( k );        
    }
    
//    unsigned int j = 0;
//    for( unsigned int i = 0; i < normalRules.size(); i++ )
//    {
//        if( !normalRules[ i ]->isRemoved() )
//            normalRules[ j++ ] = normalRules[ i ];
//        else
//            delete normalRules[ i ];
//    }
//    normalRules.shrink( j );        
}

bool
GringoNumericFormat::addUndefinedLiteral(
    Clause* clause,
    Literal lit )
{
    if( solver.isTrue( lit ) )
        return false;
    if( solver.isUndefined( lit ) )
        clause->addLiteral( lit );
    return true;
}

bool
GringoNumericFormat::isHeadCycleFree(
    Component* componentPointer )
{
    Component& component = *componentPointer;
    unordered_set< Rule* > visited;

    for( unsigned int i = 0; i < component.size(); i++ )
    {
        Var v = component.getVariable( i );
        if( v >= atomData.size() )
            continue;

        Vector< AbstractRule >& headOccs = atomData[ v ].headOccurrences;
        for( unsigned int j = 0; j < headOccs.size(); j++ )
        {
            Rule* rule = getRule( headOccs[ j ].getId() );
            assert( rule != NULL );
            if( !rule->isRemoved() && !visited.insert( rule ).second )
                return false;            
        }
    }

    return true;
}

Clause*
GringoNumericFormat::normalRuleToClause(
    Rule* normalRule )
{
    trace_msg( parser, 3, "Converting normalRule " << *normalRule << " to clause" );
    Clause* c = new Clause( normalRule->size() );
    for( unsigned int i = 0; i < normalRule->size(); i++ )
        c->addLiteral( normalRule->literals[ i ] );
    return c;
}

void
GringoNumericFormat::onTrueHeadAtom(
    Rule* rule,
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
    Rule* rule,
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
        ExternalPropagator* prop = new ExternalPropagator( filename.c_str(), wasp::Options::interpreter, solver, wasp::Options::scriptDirectory );
        solver.addExternalPropagator( prop );
    }        
    
    if( wasp::Options::heuristic_scriptname != NULL )
    {
        string filename = wasp::Options::heuristic_scriptname; 
        ExternalPropagator* prop = new ExternalPropagator( filename.c_str(), wasp::Options::interpreter, solver, wasp::Options::scriptDirectory );
        solver.addExternalPropagator( prop );
    }
}

Aggregate*
GringoNumericFormat::createPseudoBooleanConstraint(
    Vector< Literal >& literals,
    Vector< uint64_t >& weights,
    uint64_t bound )
{
    assert( literals.size() == weights.size() );
    Literal aggregateLit( 1, NEGATIVE );
    Aggregate* aggregate = createAggregate( literals, weights, bound, aggregateLit );
    assert( solver.isTrue( aggregateLit ) );
    aggregate->onLiteralFalse( solver, aggregateLit.getOppositeLiteral(), 1 );    
    return aggregate;
}

Aggregate*
GringoNumericFormat::createAggregate(
    Vector<Literal>& literals,
    Vector<uint64_t>& weights,
    uint64_t bound,
    Literal aggregateLit )
{
    assert( literals.size() == weights.size() );
    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral( aggregateLit.getOppositeLiteral(), 0 );
    #ifndef NDEBUG
    uint64_t previousWeight = UINT64_MAX;
    #endif    
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        if( solver.isFalse( literals[ i ] ) )
            continue;
        
        if( solver.isTrue( literals[ i ] ) )
        {
            assert( bound >= weights[ i ] );
            bound -= weights[ i ];
            continue;
        }
        
        aggregate->addLiteral( literals[ i ], weights[ i ] );
        solver.setFrozen( literals[ i ].getVariable() );
        assert_msg( previousWeight >= weights[ i ], "Literals must be sorted in increasing order" );
        assert( previousWeight = weights[ i ] );
    }
    #ifndef NDEBUG
    bool res =
    #endif
    aggregate->updateBound( solver, bound );
    assert( res );    
    return aggregate;
}
