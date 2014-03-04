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
        
        if( !consistent )
            return;
    }

    addSupportClauses();

    readAtomsTable( input );
    readTrueAtoms( input );
    readFalseAtoms( input );
    readErrorNumber( input );
    
//    solver.computeStrongConnectedComponents();
    
    if( !solver.tight() )
    {
        programIsNotTight();
    }
    
    //TODO: remove
//    cout << inputVarId.size() << " " << auxVarId.size() << endl;
//    unsigned sizes[1024] = {0};
//    unsigned occs[1024] = {0};
//    unsigned occs_[1024] = {0};
//    for( unsigned i = 0; i < auxVarId.size(); i++ )
//        if( supportVectorAuxVar[i] != NULL  )
//        {
//            sizes[  supportVectorAuxVar[i]->size() ]++;
//            
//            Literal lit = solver.getLiteral( auxVarId[ i ]);
//            lit.startIterationOverOccurrences();
//            unsigned count = 0;
//            while(lit.hasNextOccurrence())
//            {
//                Clause* c = lit.nextOccurence();
//                count++;
//                occs_[c->size()]++;
//            }
//            lit = lit.getOppositeLiteral();
//            lit.startIterationOverOccurrences();
//            while(lit.hasNextOccurrence())
//            {
//                Clause* c = lit.nextOccurence();
//                count++;
//                occs_[c->size()]++;
//            }
//            occs[count]++;
//        }
//    cout << "sizes\n";
//    for( unsigned i = 1; i < 10; i++)
//        cout << sizes[i] << endl;
//    cout << "occs\n";
//    for( unsigned i = 1; i < 10; i++)
//        cout << occs[i] << " " << occs_[i] << endl;
//        
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

    Clause* body = readBody( input );

    if( head == 1 )
        addConstraint( body );
    else if( body->size() == 0 )
        addFact( head, body );
    else if( body->size() == 1 )
    {
        addNormalRule( head, body->getAt( 0 ) );
        solver.releaseClause( body );
    }
    else
        addNormalRule( head, getBodyLiteral( body ) );
}

void
GringoNumericFormat::addConstraint(
    Clause* body )
{
    consistent = solver.addClause( body );
}


void
GringoNumericFormat::addFact(
    unsigned head,
    Clause* body )
{
    assert( body->size() == 0 );
    body->addLiteral( getLiteralForInputVar( head, POSITIVE ) );
    consistent = solver.addClause( body );
    supportVectorInputVar[ head ]->free();
    supportVectorInputVar[ head ]->addLiteral( getLiteralForInputVar( 1, NEGATIVE ) ); 
}

Literal
GringoNumericFormat::getBodyLiteral(
    Clause* body )
{
    assert( body->size() >= 2 );
    
    bodiesDictionary.startInsertion();
    for( unsigned i = 0; i < body->size(); ++i )
    {
        bodiesDictionary.addElement( body->getAt( i ).getId() );
    }
    unsigned id = bodiesDictionary.endInsertion();
    if( id < auxVarId.size() )
        solver.releaseClause( body );
    else
    {
        Literal lit = getLiteralForAuxVar( id, NEGATIVE );
        
        for( unsigned i = 0; i < body->size(); ++i )
        {
            Clause* clause = solver.newClause();
            clause->addLiteral( lit );
            clause->addLiteral( body->getAt( i ).getOppositeLiteral() );
            if( !solver.addClause( clause ) )
                consistent = false;
        }
        
        body->addLiteral( lit.getOppositeLiteral() );
        if( !solver.addClause( body ) )
            consistent = false;
        assert( supportVectorAuxVar[ id ] == NULL );
        supportVectorAuxVar[ id ] = body;
    }
    return getLiteralForAuxVar( id, NEGATIVE );
}

void
GringoNumericFormat::addNormalRule(
    unsigned head,
    Literal body )
{
    Clause* clause = solver.newClause();
    clause->addLiteral( getLiteralForInputVar( head, POSITIVE ) );
    clause->addLiteral( body );
    if( !solver.addClause( clause ) )
        consistent = false;
    
    supportVectorInputVar[ head ]->addLiteral( body.getOppositeLiteral() );
}

Clause*
GringoNumericFormat::readBody( istream& input )
{
    unsigned  bodySize, negativeSize, tmp;
    input >> bodySize >> negativeSize;
 
    if( bodySize < negativeSize )
        ErrorMessage::errorDuringParsing( "Body size must be greater than or equal to negative size." );
   
    Clause* clause = solver.newClause();
    while( negativeSize-- > 0 )
    {
        input >> tmp;
        clause->addLiteral( getLiteralForInputVar( tmp, POSITIVE ) );
        --bodySize;
    }
    while( bodySize-- > 0 )
    {
        input >> tmp;
        clause->addLiteral( getLiteralForInputVar( tmp, NEGATIVE ) );
    }
    return clause;
}

Literal
GringoNumericFormat::getLiteralForInputVar( 
    unsigned int id,
    unsigned int sign )
{
    while( inputVarId.size() <= id )
    {
        solver.addVariable();
        inputVarId.push_back( solver.numberOfVariables() );
        supportVectorInputVar.push_back( solver.newClause() );
    }
    return Literal( solver.getVariable( inputVarId[ id ] ), sign );
}

Literal
GringoNumericFormat::getLiteralForAuxVar( 
    unsigned int id, 
    unsigned int sign )
{
    while( auxVarId.size() <= id )
    {
        solver.addVariable();
        auxVarId.push_back( solver.numberOfVariables() );
        supportVectorAuxVar.push_back( NULL );
    }
    return Literal( solver.getVariable( auxVarId[ id ] ), sign );
}

void
GringoNumericFormat::addSupportClauses()
{
    for( unsigned i = 1; i < inputVarId.size(); ++i )
    {
        Clause* clause = supportVectorInputVar[ i ];
        if( clause->getAt( 0 ) == getLiteralForInputVar( 1, NEGATIVE ) )
        {
            solver.releaseClause( clause );
            solver.addEdgeInDependencyGraph( inputVarId[ i ], 0 );
            continue;
        }
        
        for( unsigned j = 0; j < clause->size(); ++j )
            if( clause->getAt( j ).isPositive() )
                solver.addEdgeInDependencyGraph( inputVarId[ i ], clause->getAt( j ).getVariable()->getId() );
            
        clause->addLiteral( getLiteralForInputVar( i, NEGATIVE ) );
        if( clause->removeDuplicatesAndFalseAndCheckIfTautological() )
        {
            solver.releaseClause( clause );
            continue;
        }
        
        consistent = solver.addClause( clause );
        if( !consistent )
            return;
    }
    
    for( unsigned i = 1; i < auxVarId.size(); ++i )
    {
        Clause* clause = supportVectorAuxVar[ i ];
        for( unsigned j = 0; j < clause->size(); ++j )
            if( clause->getAt( j ).isNegative() )
                solver.addEdgeInDependencyGraph( auxVarId[ i ], clause->getAt( j ).getVariable()->getId() );
    }
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
        input.getline( name, 1024 );

        VariableNames::setName( solver.getVariable( inputVarId[ nextAtom ] ), name+1 );
        input >> nextAtom;
    }
    
    #ifdef TRACE_ON
    for( unsigned int i = 1; i <= solver.numberOfVariables(); i++ )
    {
        if( VariableNames::isHidden( solver.getVariable( i ) ) )
        {
            stringstream s;
            s << "#hidden" << i;
            string name = s.str();
            VariableNames::setName( solver.getVariable( i ), name );
        }
    }
    #endif
}

void
GringoNumericFormat::readTrueAtoms(
    istream& input )
{
    char b;
    
    input >> b;
    input >> b;
    
    unsigned int nextAtom;
    input >> nextAtom;
    
    while( nextAtom != 0 )
    {
        Literal lit = getLiteralForInputVar( nextAtom, NEGATIVE );
        Clause* clause = solver.newClause();
        clause->addLiteral( lit );
        consistent = solver.addClause( clause );
        if( !consistent )
            return;
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
        Literal lit = getLiteralForInputVar( nextAtom, NEGATIVE );
        Clause* clause = solver.newClause();
        clause->addLiteral( lit );
        consistent = solver.addClause( clause );
        if( !consistent )
            return;
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
        cout << ERRORPARSING << endl;
        exit( 0 );
    }
    else
    {
    }
}

void
GringoNumericFormat::programIsNotTight()
{
//    unordered_map< Variable*, Component* > variableComponent;
//    
//    //Add two fake positions
//    solver.addGUSData( NULL );
//    solver.addGUSData( NULL );
//    for( unsigned int i = 2; i <= solver.numberOfVariables(); i++ )
//    {
//        GUSData* gd = new GUSData();
//        gd->variable = solver.getVariable( i );
//        solver.addGUSData( gd );        
//    }
//    
//    trace_msg( parser, 2, "Program is not tight. Number of cyclic components " << solver.getNumberOfCyclicComponents() );
//    for( unsigned int i = 0; i < solver.getNumberOfCyclicComponents(); i++ )
//    {
//        Component* component = solver.getCyclicComponent( i );
//        
//        for( unsigned int j = 0; j < component->size(); j++ )
//        {
//            unsigned int varId = component->getVariable( j );
//            trace_msg( parser, 2, "Variable " << *solver.getVariable( varId ) << " is in the cyclic component " << i );
//            Variable* currentVariable = solver.getVariable( varId );
//            currentVariable->setComponent( component );
//            variableComponent[ currentVariable ] = component;
//            component->variableHasNoSourcePointer( currentVariable );
//        }
//        
//        solver.addPostPropagator( component );
//    }                
//    
//    unordered_map< Variable*, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];    
//    
//    for( unsigned int i = 2; i < supportVectorInputVar.size(); i++ )
//    {
//        unsigned int atomId = getIdInSolver( i, false );
//        Variable* variable = getVariable( atomId );
//
//        if( !variable->isInCyclicComponent() || supportedVariables.find( variable ) != supportedVariables.end() )
//            continue;
//        
//        Clause* lits = supportVector[ i ];
//        if( lits->size() == 0 )
//            continue;
//
//        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " which has " << lits->size() << " supporting rules" );
//        
//        Component* component = variableComponent[ variable ];
//        assert( component != NULL );
//        
//        variable->setFrozen();
//        
//        unordered_set< Variable* > tmp;
//        for( unsigned int j = 0; j < lits->size(); j++ )
//        {
//            if( !tmp.insert( lits->getAt( j ).getVariable() ).second )
//                continue;
//
//            if( lits->getAt( j ).isPositive() && variable->inTheSameComponent( lits->getAt( j ).getVariable() ) )
//            {
//                trace_msg( parser, 3, "Adding " << lits[ j ] << " as internal rule for " << *variable );
//                component->addInternalLiteralForVariable( variable->getId(), lits->getAt( j ) );
//            }
//            else
//            {
//                trace_msg( parser, 3, "Adding " << lits[ j ] << " as external rule for " << *variable );
//                component->addExternalLiteralForVariable( variable->getId(), lits->getAt( j ) );
//            }
//            
//            component->addVariablePossiblySupportedByLiteral( variable, lits->getAt( j ) );
//            
//            unsigned int sign = lits->getAt( j ).getSign();
//            if( literalsPostPropagator[ sign ][ lits->getAt( j ).getVariable() ].insert( component ).second )            
//                lits->getAt( j ).addPostPropagator( component );
//            
//            lits->getAt( j ).getVariable()->setFrozen();
//        }
//    }    
//    
//    for( unsigned int i = 1; i < supportVectorForAuxVariables.size(); i++ )
//    {
//        unsigned int atomId = getIdInSolver( i, true );
//        Variable* variable = getVariable( atomId );
//        
//        if( !variable->isInCyclicComponent() || supportedVariables.find( variable ) != supportedVariables.end() )
//            continue;
//
//        Clause* lits = supportVectorForAuxVariables[ i ];
//        if( lits->size() == 0 )
//            continue;
//
//        trace_msg( parser, 2, "Creating GUS data structures for variable " << *variable << " which has " << lits.size() << " supporting rules" );
//
//        variable->setFrozen();
//        
//        Component* component = variableComponent[ variable ];
//        component->setAuxVariable( variable->getId() );
//        
//        assert( component != NULL );
//
//        unordered_set< Variable* > tmp;
//        for( unsigned int j = 0; j < lits->size(); j++ )
//        {
//            if( !tmp.insert( lits->getAt( j ).getVariable() ).second )
//                continue;
//            trace_msg( parser, 3, "Adding " << lits->getAt( j ) << " as supporting rule for " << *variable );
//            if( lits->getAt( j ).isPositive() && lits->getAt( j ).getVariable()->inTheSameComponent( variable ) )
//            {
//                component->addAuxVariableSupportedByLiteral( variable, lits->getAt( j ) );
//                component->addInternalLiteralForVariable( variable->getId(), lits->getAt( j ) );
//            }
//            
//            lits->getAt( j ).getVariable()->setFrozen();
//        }
//    }    
}