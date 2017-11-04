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
 
#ifndef WASP_VARIABLES_H
#define WASP_VARIABLES_H

#include <vector>

#include "Literal.h"
#include "outputBuilders/OutputBuilder.h"
#include "util/WaspAssert.h"
#include "util/VariableNames.h"
#include "stl/Vector.h"
#include "ReasonForBinaryClauses.h"
#include "util/WaspOptions.h"

using namespace std;
class Component;
class HCComponent;
class Reason;

struct VariableData
{
    Reason* implicant;
    Component* component;
    HCComponent* hcComponent;
    ReasonForBinaryClauses* reasonForBinaryClauses;
    
    unsigned int decisionLevel : 27;
    unsigned int isAssumptionPositive : 1;
    unsigned int isAssumptionNegative : 1;
    unsigned int frozen : 1;
    unsigned int signOfEliminatedVariable : 2;        
    
    unsigned int positionInTrail;
};

class Variables
{
    public:
        inline Variables();
        inline ~Variables();    
    
        inline void push_back();
        
        inline bool hasNextVariableToPropagate() const { return nextVariableToPropagate < assignedVariablesSize; }
        inline Var getNextVariableToPropagate();
        
        inline Literal getOppositeLiteralFromLastAssignedVariable();        
        inline Literal createLiteralFromAssignedVariable( Var variable );
        inline Literal createOppositeLiteralFromAssignedVariable( Var variable );

        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();
        
        inline Var unrollLastVariable();
        inline void resetLiteralsToPropagate() { nextVariableToPropagate = assignedVariablesSize; }
        
        inline unsigned numberOfAssignedLiterals() const { return assignedVariablesSize; }
        inline unsigned numberOfVariables() const { assert( numOfVariables > 0 ); return numOfVariables - 1; }
        
        inline Var getAssignedVariable( unsigned idx ) { assert( idx < assignedVariables.size() ); return assignedVariables[ idx ]; }        
        inline unsigned getPositionInTrail( Var variable ) const { assert( variable < variablesData.size() ); return variablesData[ variable ].positionInTrail; }
        
        inline void printAnswerSet( OutputBuilder* outputBuilder ) const;
        
        /**
         * Assign a literal with the truth value TRUE.
         * 
         * @param level the level of the inference.
         * @param literal the literal which is true.
         * @param implicant the clause which is the reason of the literal assignment.
         * @return true if no conflict occurs, false otherwise. 
         */
        inline bool assign( int level, Literal literal, Reason* implicant );
        inline bool assign( int level, Literal literal );
        inline bool assign( int level, Clause* implicant );
        inline void onEliminatingVariable( Var variable );
        
        inline void onUnroll();
        
        inline bool isTrue( Var v ) const { return getTruthValue( v ) == TRUE; }
        inline bool isFalse( Var v ) const { return getTruthValue( v ) == FALSE; }        
        inline bool isUndefined( Var v ) const { return getTruthValue( v ) == UNDEFINED; }
        
        inline bool isTrue( Literal lit ) const { return lit.isPositive() ? getTruthValue( lit.getVariable() ) == TRUE : getTruthValue( lit.getVariable() ) == FALSE; }
        inline bool isFalse( Literal lit ) const { return lit.isPositive() ? getTruthValue( lit.getVariable() ) == FALSE : getTruthValue( lit.getVariable() ) == TRUE; }
        inline bool isUndefined( Literal lit ) const { return getTruthValue( lit.getVariable() ) == UNDEFINED; }
        
        inline bool setTruthValue( Var v, TruthValue truth );
        inline void setUndefined( Var v );
        inline void setUndefinedBrutal( Var v ) { assigns[ v ] = UNDEFINED; }
        
        inline TruthValue getTruthValue( Var v ) const { return assigns[ v ] & UNROLL_MASK; }
        inline TruthValue getCachedTruthValue( Var v ) const { return assigns[ v ] >> UNROLL_MASK; }
        
        inline bool setTrue( Literal lit );        
        
        inline bool isImplicant( Var v, const Clause* clause ) const { return !isUndefined( v ) && variablesData[ v ].implicant == clause; }
        inline bool hasImplicant( Var v ) const { return variablesData[ v ].implicant != NULL; }
        inline void setImplicant( Var v, Reason* clause ) { variablesData[ v ].implicant = clause; }
        inline Reason* getImplicant( Var v ) { return variablesData[ v ].implicant; }

        inline unsigned int getDecisionLevel( Var v ) const { return variablesData[ v ].decisionLevel; }
        inline unsigned int getDecisionLevel( Literal lit ) const { return getDecisionLevel( lit.getVariable() ); }
        inline void setDecisionLevel( Var v, unsigned int decisionLevel ) { variablesData[ v ].decisionLevel = decisionLevel; }
        
        inline const Clause* getDefinition( Var v ) const { return ( Clause* ) variablesData[ v ].implicant; }
        inline void setEliminated( Var v, unsigned int value, Clause* definition );
        inline unsigned int getSignOfEliminatedVariable( Var v ) const { return variablesData[ v ].signOfEliminatedVariable; }
        inline bool hasBeenEliminated( Var v ) const { return variablesData[ v ].signOfEliminatedVariable != NOT_ELIMINATED; }
        inline bool hasBeenEliminatedByDistribution( Var v ) const { return variablesData[ v ].signOfEliminatedVariable == ELIMINATED_BY_DISTRIBUTION; } 
        
        inline bool inTheSameComponent( Var v1, Var v2 ) const { return variablesData[ v1 ].component != NULL && variablesData[ v1 ].component == variablesData[ v2 ].component; }
        inline bool inTheSameHCComponent( Var v1, Var v2 ) const { return variablesData[ v1 ].hcComponent != NULL && variablesData[ v1 ].hcComponent == variablesData[ v2 ].hcComponent; }
        inline bool isInCyclicComponent( Var v ) const { return variablesData[ v ].component != NULL; }
        inline bool isInCyclicHCComponent( Var v ) const { return variablesData[ v ].hcComponent != NULL; }
        inline void setComponent( Var v, Component* c ){ assert( variablesData[ v ].component == NULL || c == NULL ); variablesData[ v ].component = c; }
        inline void setHCComponent( Var v, HCComponent* c ){ assert( variablesData[ v ].hcComponent == NULL || c == NULL ); variablesData[ v ].hcComponent = c; }
        inline Component* getComponent( Var v ) { return variablesData[ v ].component; }
        inline HCComponent* getHCComponent( Var v ) { return variablesData[ v ].hcComponent; }
        
        inline ReasonForBinaryClauses* getReasonForBinaryClauses( Var v ) { return variablesData[ v ].reasonForBinaryClauses; }
        
        inline bool isFrozen( Var v ) const { return variablesData[ v ].frozen; }
        inline void setFrozen( Var v ) { variablesData[ v ].frozen = 1; }
        inline void printInterpretation() const;
        
        inline void setAssumption( Literal lit, bool isAssumption ) {
            Var v = lit.getVariable();
            if( lit.isPositive() )
                variablesData[ v ].isAssumptionPositive = isAssumption;
            else
                variablesData[ v ].isAssumptionNegative = isAssumption;
        }
        inline bool isAssumption( Var v ) const { return variablesData[ v ].isAssumptionPositive || variablesData[ v ].isAssumptionNegative; }   
        inline bool isAssumption( Literal lit ) const { return lit.isPositive() ? variablesData[ lit.getVariable() ].isAssumptionPositive : variablesData[ lit.getVariable() ].isAssumptionNegative; }
        
    private:
        vector< Var > assignedVariables;
        Vector< TruthValue > assigns;
        
        Vector< VariableData > variablesData;
        unsigned assignedVariablesSize;
        int iteratorOnAssignedVariables;
        unsigned nextVariableToPropagate;
        unsigned noUndefinedBefore;
        
        unsigned int numOfVariables;               
        
        inline bool checkVariableHasBeenAssigned( Var var );
};

Variables::Variables()
:
    assignedVariablesSize( 0 ),
    nextVariableToPropagate( 0 ),
    noUndefinedBefore( 1 ),
    numOfVariables( 1 )
{  
    assigns.push_back( UNDEFINED );
    variablesData.push_back( VariableData() );    
}

Variables::~Variables()
{
    for( unsigned int i = 1; i < numOfVariables; i++ )
    {
        if( hasBeenEliminated( i ) && !hasBeenEliminatedByDistribution( i ) )
            delete variablesData[ i ].implicant;
        delete variablesData[ i ].reasonForBinaryClauses;
    }
}

void
Variables::push_back()
{
    assignedVariables.push_back( 0 ); 
    numOfVariables++;
    variablesData.push_back( VariableData() );
    
    VariableData& vd = variablesData.back();
    vd.implicant = NULL;
    vd.decisionLevel = 0;
    vd.component = NULL;
    vd.signOfEliminatedVariable = NOT_ELIMINATED;
    vd.frozen = 0;
    vd.isAssumptionPositive = 0;
    vd.isAssumptionNegative = 0;    
    vd.reasonForBinaryClauses = new ReasonForBinaryClauses( variablesData.size() - 1 );
    vd.positionInTrail = UINT_MAX;
    
    assigns.push_back( UNDEFINED );
}

Var
Variables::getNextVariableToPropagate()
{ 
    assert( hasNextVariableToPropagate() );
    return assignedVariables[ nextVariableToPropagate++ ];
}

Var
Variables::unrollLastVariable()
{
    assert( assignedVariablesSize > 0 && assignedVariablesSize <= assignedVariables.size() );
    Var variable = assignedVariables[ --assignedVariablesSize ];
    setUndefined( variable );
    return variable;
}

Literal
Variables::getOppositeLiteralFromLastAssignedVariable()
{
    assert( iteratorOnAssignedVariables >= 0 && static_cast< unsigned >( iteratorOnAssignedVariables ) < assignedVariablesSize );    
    return createOppositeLiteralFromAssignedVariable( assignedVariables[ iteratorOnAssignedVariables-- ] );    
}

bool
Variables::hasNextAssignedVariable() const
{
    return iteratorOnAssignedVariables >= 0;
}

void
Variables::startIterationOnAssignedVariable()
{
    assert( assignedVariablesSize > 0 );
    iteratorOnAssignedVariables = assignedVariablesSize - 1;
}

Literal
Variables::createLiteralFromAssignedVariable(
    Var v )
{
    assert( TRUE == 2 && FALSE == 1 );
    return Literal( v, getTruthValue( v ) & 1 );
}

Literal
Variables::createOppositeLiteralFromAssignedVariable(
    Var v )
{
    assert( TRUE == 2 && FALSE == 1 );
    return Literal( v, ~( getTruthValue( v ) ) & 1 );
}

void
Variables::printAnswerSet(
    OutputBuilder* outputBuilder ) const
{
    if( wasp::Options::silent >= 1 )
    {        
        outputBuilder->foundModel();
        return;
    }
        
    outputBuilder->startModel();
    for( unsigned int i = 0; i < assignedVariablesSize; ++i )
    {
        Var v = assignedVariables[ i ];        
        outputBuilder->printVariable( v, isTrue( v ) );
    }
    outputBuilder->endModel();
}

bool
Variables::assign( 
    int level, 
    Literal literal,
    Reason* implicant )
{
    Var variable = literal.getVariable();
    assert( assignedVariablesSize < numOfVariables );
    if( setTrue( literal ) )
    {
        assert_msg( !checkVariableHasBeenAssigned( variable ), "The variable " << variable << " has been already assigned." );
        variablesData[ variable ].positionInTrail = assignedVariablesSize;
        assignedVariables[ assignedVariablesSize++ ] = variable;        
        setDecisionLevel( variable, level );
        setImplicant( variable, implicant );
        return true;
    }
    return false;
}

void
Variables::onEliminatingVariable(
    Var variable )
{
    assert( variable != 0 );
    assert( assignedVariablesSize < numOfVariables );
    assert_msg( !checkVariableHasBeenAssigned( variable ), "The variable " << variable << " has been already assigned." );
    variablesData[ variable ].positionInTrail = assignedVariablesSize;
    assignedVariables[ assignedVariablesSize++ ] = variable;        
}

bool
Variables::assign( 
    int level, 
    Literal literal )
{
    return assign( level, literal, NULL );
}

bool
Variables::assign( 
    int level, 
    Clause* implicant )
{
    assert( implicant != NULL );
    return assign( level, implicant->getAt( 0 ), implicant );
}

void
Variables::onUnroll()
{
    resetLiteralsToPropagate();
    
    noUndefinedBefore = 1;
}

bool
Variables::checkVariableHasBeenAssigned( 
    Var var )
{
    for( unsigned int i = 0; i < assignedVariablesSize; i++ )
    {
        if( assignedVariables[ i ] == var )
            return true;
    }
    return false;
}


bool
Variables::setTruthValue(
    Var v,
    TruthValue truth )
{
//    assert_msg( ( truth == TRUE || truth == FALSE ), "Truth Value parameter is neither true nor false. It is " << truth << "." );
    if( getTruthValue( v ) == UNDEFINED )
    {
        assigns[ v ] = truth;
        return true;
    }
    
    /*  
     * If truthValue is not undefined we assume that there is a conflict.
     * The solver should check if the variable has been already assigned.
     */
    assert_msg( assigns[ v ] != truth, "A variable is assigned twice with the same truthValue."  );
    return false;
}

void
Variables::setUndefined(
    Var v )
{
    assert_msg( assigns[ v ] != UNDEFINED, "This assert is not strictly necessary. By the way, this assignment is useless." );
    assert( ( ( assigns[ v ] & ~UNROLL_MASK ) & UNROLL_MASK ) == UNDEFINED );
    assert( getTruthValue( v ) == TRUE ? ( assigns[ v ] & ~UNROLL_MASK ) == CACHE_TRUE : ( assigns[ v ] & ~UNROLL_MASK ) == CACHE_FALSE );
    assigns[ v ] &= ~UNROLL_MASK;
    variablesData[ v ].positionInTrail = UINT_MAX;
}

bool
Variables::setTrue(
    Literal lit )
{
    assert( "Variable has not been set." && lit.getVariable() != 0 );
    TruthValue truth = ( TRUE | CACHE_TRUE ) >> lit.getSign();    
    return setTruthValue( lit.getVariable(), truth );
}

void
Variables::setEliminated(
    Var v,
    unsigned int value,
    Clause* def )
{
    assert_msg( value <= 2, "The sign must be 0 or 1. Found value " << value );
    assert( def != NULL || value == ELIMINATED_BY_DISTRIBUTION );
    variablesData[ v ].signOfEliminatedVariable = value;
    variablesData[ v ].implicant = def;
    
    #ifndef NDEBUG
    bool result = 
    #endif
    setTruthValue( v, TRUE );    
    assert( result );
}

void
Variables::printInterpretation() const
{
    cout << "Interpretation: [";
    for( unsigned int i = 2; i <= this->numberOfVariables(); i++ )
    {        
        if( hasBeenEliminated( i ) || hasBeenEliminatedByDistribution( i ) )
            cout << " R" << Literal( i, POSITIVE );            
        else if( isTrue( i ) )
            cout << " T" << Literal( i, POSITIVE );
        else if( isFalse( i ) )
            cout << " F" << Literal( i, POSITIVE );
        else
            cout << " U" << Literal( i, POSITIVE );
    }
    cout << " ]" << endl;
}

#endif
