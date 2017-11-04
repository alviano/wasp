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

#ifndef WASP_GRINGONUMERICFORMAT_H
#define WASP_GRINGONUMERICFORMAT_H

#include "../Solver.h"
#include "../stl/Trie.h"
#include "../util/Istream.h"
#include "Rule.h"
#include "WeightConstraint.h"
#include "AtomData.h"
#include <unordered_set>

using namespace std;

class GringoNumericFormat
{
public:
    inline GringoNumericFormat( Solver& s );
    inline ~GringoNumericFormat();

    /**
    * This function read instruction from standard input and
    * build the program.
    */
    void parse( istream& i );    
    inline bool isHeadCycleFree( Component* component );    
    
private:
    void parse( Istream& input );
    inline void readChoiceRule( Istream& input );
    inline void readNormalRule( Istream& input );
    inline void readNormalRule( Istream& input, unsigned head, unsigned bodySize, unsigned negativeSize );
    inline void readDisjunctiveRule( Istream& input );
    inline void readConstraint( Istream& input );
    inline void readCount( Istream& input );
    inline void readSum( Istream& input );
    inline void readOptimizationRule( Istream& input );
    inline void skipLiterals( Istream& input, unsigned howMany );
    inline void readBodySize( Istream& input, unsigned& bodySize, unsigned& negativeSize );
    void addFact( unsigned head );
    void addTrueVariable( unsigned int id );
    void addFalseVariable( unsigned int id );
//    void addRule( unsigned head, Literal body );
//    void addConstraint( Clause* body );
//    Clause* readBody( istream& input, vector< Variable* >& truePositiveLiterals );
//    Literal getBodyLiteral( Clause* body );
    
//    void addSupportClauses();
    void processRecursivePositiveCrule( Clause* crule );
    void processRecursiveNegativeCrule( Clause* crule );
    void computeGusStructures();
    void computeSCCs();
    void computeSCCsDisjunctive();
    void createClausesForShift( const Vector< Var >& headAtoms, Vector< Var >& auxVars, Literal bodyLiteral );
    void createPropagatorForDisjunction( const Vector< Var >& headAtoms, Vector< Var >& auxVars, Literal bodyLiteral );
    void normalizeHeads( const Vector< Var >& headAtoms, Vector< Var >& auxVars, Literal bodyLiteral );    
    void quadraticStrategy( const Vector< Var >& headAtoms, Vector< Var >& auxVars, Literal bodyLiteral );
    void quadraticStrategyAggregate( const Vector< Var >& headAtoms, Vector< Var >& auxVars, Literal bodyLiteral );
    void addBinaryImplication( Literal lit1, Literal lit2 );
    
    //add a ^ b <-> c
    void addTwoAndLiteralEquivalence( Literal lit1And, Literal lit2And, Literal litEquivalent );
    //add a <-> b v c
    inline void addLiteralTwoOrEquivalence( Literal litEquivalent, Literal lit1Or, Literal lit2Or ) { addTwoAndLiteralEquivalence( lit1Or.getOppositeLiteral(), lit2Or.getOppositeLiteral(), litEquivalent.getOppositeLiteral() ); }
    inline void addEquivalence( Literal lit1, Literal lit2 );
    void computeCompletion();
    void simplify();
    void removeSatisfiedLiterals( WeightConstraint* );
    
    void readAtomsTable( Istream& input );

    void readTrueAtoms( Istream& input );
    void readFalseAtoms( Istream& input );

    void readErrorNumber( Istream& input );
    
    void createStructures( unsigned id );
    
    void propagate();
    void propagateTrue( Var var );
    void propagateFalse( Var var );
    void propagateFact( Var var );
    
//    bool ruleToConstraint( Rule* rule );    
    void bodyToConstraint( Rule* rule );    
    
    void cleanData();
    
    void addExternalPropagators();
    Rule* getRule( unsigned int id ) { assert( id < normalRules.size() ); return normalRules[ id ]; }

//    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
//    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;    
    Trie bodiesDictionary;
    
//    vector< unsigned int > inputVarId;
//    vector< unsigned int > auxVarId;
    
    unsigned propagatedLiterals;
    
    void add( Rule* rule, bool isChoice, unsigned int numberOfTrueHeadAtoms );
    void add( WeightConstraint* rule );
    bool addUndefinedLiteral( Clause* clause, Literal lit );
    void removeAndCheckSupport( Rule* rule );
    bool shrinkPos( Rule* rule, unsigned lit );
    void shrinkNeg( Rule* rule, unsigned lit );
    void shrinkHead( Rule* rule, unsigned lit );
    void shrinkDoubleNeg( Rule* rule, unsigned lit );
    void onShrinkingBody( Rule* rule );
    void onShrinkingHead( Rule* rule );
    void onTrueHeadAtom( Rule* rule, Var headAtom );
    bool isSupporting( Rule* rule, Var headAtom );
    
    void updateMaxPossibleValueWeightConstraint( WeightConstraint* rule, unsigned int position );
    void updateCurrentValueWeightConstraint( WeightConstraint* rule, unsigned int position );
    void weightConstraintIsTrue( WeightConstraint* rule );
    void weightConstraintIsFalse( WeightConstraint* rule );
    void atLeastOne( WeightConstraint* rule );
    void atMostOne( WeightConstraint* rule );
    void atMostOnePairwise( WeightConstraint* rule );
    void atMostOneBimander( WeightConstraint* rule );
    void atMostOneSequential( WeightConstraint* rule );
    void atMostOneBisequential( WeightConstraint* rule );
    Aggregate* weightConstraintToAggregate( WeightConstraint* rule );
    void addWeightConstraints();
    void cleanWeightConstraint( WeightConstraint* rule );
    void addOptimizationRules();
    void addOptimizationRule( WeightConstraint* rule );
//    void computeLinearCostsForOptimizationRules( vector< uint64_t >& maxCostOfLevelOfOptimizationRules, vector< int >& literals, vector< uint64_t >& weights, vector< unsigned int >& levels );
    void addOptimizationRules( vector< int >& literals, vector< uint64_t >& weights, vector< unsigned int >& levels );
    
    void createCrule( Literal head, Var varToSkip, Rule* rule );
    void addDependencies( Literal head, Rule* rule );
    void clearDataStructures();
    void cleanRules();
    
    Literal createAuxForBody( Rule* rule, Vector< Var >& headAtoms );
    
    Clause* normalRuleToClause( Rule* );
    
    Aggregate* createPseudoBooleanConstraint( Vector< Literal >& literals, Vector< uint64_t > & weights, uint64_t bound );
    Aggregate* createAggregate( Vector< Literal >& literals, Vector< uint64_t > & weights, uint64_t bound, Literal aggregateLiteral );    

    Vector< Rule* > normalRules;    
    Vector< WeightConstraint* > weightConstraintRules;
    Vector< WeightConstraint* > delayedAggregateRewriting;
    Vector< WeightConstraint* > optimizationRules;
    vector< AtomData > atomData;
    Vector< Clause* > crules;
    Vector< pair< Var, Literal > > auxs;
    //vector< Rule* > constraints;
    
    unsigned readNormalRule_numberOfCalls;
    Vector< unsigned > atomsWithSupportInference;
    Vector< unsigned > facts;
    unordered_map< Var, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];
    
    vector< pair< Literal, Literal > > equivalences;
    vector< MultiAggregate* > multiAggregates;    
    
    bool usedDictionary;
    unsigned int numberOfDisjunctiveRules;
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), propagatedLiterals( 0 ), readNormalRule_numberOfCalls( 0 ), usedDictionary( false ), numberOfDisjunctiveRules( 0 )
{
    atomData.push_back( AtomData( false ) );
    createStructures( 1 );
    solver.addClause( Literal( 1, NEGATIVE ) );
}

GringoNumericFormat::~GringoNumericFormat()
{
    while( !normalRules.empty() )
    {
        delete normalRules.back();
        normalRules.pop_back();
    }
    
    while( !optimizationRules.empty() )
    {
        delete optimizationRules.back();
        optimizationRules.pop_back();
    }
    
    while( !weightConstraintRules.empty() )
    {
        delete weightConstraintRules.back();
        weightConstraintRules.pop_back();
    }
}

void
GringoNumericFormat::addEquivalence(
    Literal lit1,
    Literal lit2 )
{
    addBinaryImplication( lit1, lit2 );
    addBinaryImplication( lit2, lit1 );
}

#endif
