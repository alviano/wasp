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

#ifndef GRINGONUMERICFORMAT_H
#define GRINGONUMERICFORMAT_H

#include "../Solver.h"
#include "../stl/Trie.h"
#include "../util/Istream.h"
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
    void parse();

    /**
    * This function read instruction from input and
    * build the program.
    *
    * @param input The istream input.
    */
    void parse( Istream& input );    

    class NormalRule
    {
        friend ostream& operator<<( ostream& out, const NormalRule& rule );
    public:
        Vector< Literal > literals;

        inline NormalRule() {}
        inline NormalRule( unsigned head_ ) { addHeadAtom( head_ ); }
        NormalRule( const NormalRule& init ) { literals.initFrom( init.literals ); }
        
        inline bool isRemoved() const { return literals.empty(); }
        inline void remove() { literals.clearAndDelete(); }
        
        inline bool isFact() const { assert( !literals.empty() ); assert( literals.size() != 1 || literals.back().isHeadAtom() ); return literals.size() == 1; }
//        inline bool isFiring() const { return posBody.empty() && negBody.empty() && doubleNegBody.empty(); }
        inline unsigned size() const { return literals.size(); }
        
        inline void addHeadAtom( unsigned id ) { literals.push_back( Literal::newHeadAtom( id ) ); }
        inline void addNegativeLiteral( unsigned id ) { literals.push_back( Literal::newNegativeBodyLiteral( id ) ); }
        inline void addPositiveLiteral( unsigned id ) { literals.push_back( Literal::newUndefinedPositiveBodyLiteral( id ) ); }
        inline void addPositiveTrueLiteral( unsigned id ) { literals.push_back( Literal::newTruePositiveBodyLiteral( id ) ); }
        inline void addDoubleNegLiteral( unsigned id ) { literals.push_back( Literal::newDoubleNegatedBodyLiteral( id ) ); }
        
//        inline unsigned int sizeOf()
//        {
//            return ( negBody.capacity() * sizeof( unsigned ) + sizeof( negBody ) +
//            posBody.capacity() * sizeof( unsigned ) + sizeof( posBody ) +
//            posBodyTrue.capacity() * sizeof( unsigned ) + sizeof( posBodyTrue )+
//            doubleNegBody.capacity() * sizeof( unsigned ) + sizeof( doubleNegBody ) +
//            sizeof( unsigned ) );
//        }
    };
    
    class WeightConstraintRule
    {
        friend ostream& operator<<( ostream& out, const WeightConstraintRule& rule );
        public:
            vector< int > literals;
            vector< unsigned int > weights;

            unsigned int id;
            unsigned int bound;
            unsigned int currentValue;
            unsigned int maxPossibleValue;
            unsigned int umax;
            
            inline WeightConstraintRule( unsigned int id_, unsigned int bound_ ) : id( id_ ), bound( bound_ ), currentValue( 0 ), maxPossibleValue( 0 ), umax( 0 ) {}

            inline void addNegativeLiteral( unsigned int idLit, unsigned int weightLit )
            {
                addNegativeLiteral( idLit );
                addNegativeLiteralWeight( weightLit );
            }

            inline void addPositiveLiteral( unsigned int idLit, unsigned int weightLit )
            {
                addPositiveLiteral( idLit );
                addPositiveLiteralWeight( weightLit );
            }

            inline void addNegativeLiteral( unsigned int idLit ) { literals.push_back( -idLit ); }
            inline void addPositiveLiteral( unsigned int idLit ) { literals.push_back( idLit ); }            
            
            inline void addNegativeLiteralWeight( unsigned int weightLit )
            {
                if( weightLit > bound )
                    weightLit = bound;
                weights.push_back( weightLit );
            }

            inline void addPositiveLiteralWeight( unsigned int weightLit )
            {
                if( weightLit > bound )
                    weightLit = bound;
                weights.push_back( weightLit );
            }
            
            inline bool isTrue() { return currentValue >= bound; }
            
            inline bool isFalse() { return maxPossibleValue < bound; }
            
            inline void sort() { mergesort( 0, literals.size() - 1 ); }
            
            inline unsigned int sizeOf()
            {
                return ( literals.capacity() * sizeof( int ) + sizeof( literals ) +
                weights.capacity() * sizeof( unsigned ) + sizeof( weights ) +
                sizeof( unsigned ) * 4 );
            }
            
            inline void remove(){ umax = MAXUNSIGNEDINT; }            
            inline bool isRemoved() const { return umax == MAXUNSIGNEDINT; }
            
        private:
            void mergesort( int left, int right )
            {
                if( left < right )
                {
                    int center = ( left + right ) / 2;
                    mergesort( left, center );
                    mergesort( center + 1, right );
                    merge( left, center, right );
                }
            }

            void merge( int left, int center, int right )
            {
                int* auxLiterals = new int[ right + 1 ];
                unsigned int* auxWeights = new unsigned int[ right + 1 ];

                int i, j;
                for( i = center + 1; i > left; i-- )
                {                    
                    auxLiterals[ i - 1 ] = literals[ i - 1 ];
                    auxWeights[ i - 1 ] = weights[ i - 1 ];
                }
                for( j = center; j < right; j++ )
                {
                    auxLiterals[ right + center - j ] = literals[ j + 1 ];
                    auxWeights[ right + center - j ] = weights[ j + 1 ];
                }
                for( int k = left; k <= right; k++ )
                {
                    if( auxWeights[ j ] > auxWeights[ i ] )
                    {
                        literals[ k ] = auxLiterals[ j ];
                        weights[ k ] = auxWeights[ j-- ];
                    }
                    else
                    {
                        literals[ k ] = auxLiterals[ i ];
                        weights[ k ] = auxWeights[ i++ ];
                    }
                }

                delete [] auxWeights;
                delete [] auxLiterals;
            }
    };
    
    class AtomData
    {
    public:        
        unsigned supported:1;
        unsigned numberOfHeadOccurrences:31;
        Vector< NormalRule* > headOccurrences;
        Vector< NormalRule* > posOccurrences;
        Vector< NormalRule* > negOccurrences;
        Vector< NormalRule* > doubleNegOccurrences;
        Vector< WeightConstraintRule* > negWeightConstraintsOccurrences;
        Vector< unsigned int > positionsInNegWeightConstraints;
        Vector< WeightConstraintRule* > posWeightConstraintsOccurrences;
        Vector< unsigned int > positionsInPosWeightConstraints;
//        vector< NormalRule* > negConstraints;
//        vector< NormalRule* > posConstraints;
        
        unsigned readNormalRule_headAtoms;
        unsigned readNormalRule_negativeLiterals;
        unsigned readNormalRule_positiveLiterals;
        
        WeightConstraintRule* weightConstraintRule;
        
        inline AtomData( bool supported_ = false ) : supported( supported_ ), numberOfHeadOccurrences( 0 ), readNormalRule_negativeLiterals( 0 ), readNormalRule_positiveLiterals( 0 ), weightConstraintRule( NULL ) {}
        
        inline bool isSupported() const { return supported; }
        inline void setSupported() { supported = true; }
        
        inline bool isWeightConstraint() const { return weightConstraintRule != NULL; }
        inline void setWeightConstraint( WeightConstraintRule* rule ) { assert( rule != NULL ); weightConstraintRule = rule; }
        
        inline unsigned int sizeOf()
        {
            return
            (
                    headOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( headOccurrences ) +
                    posOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( posOccurrences ) +
                    negOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( negOccurrences )+
                    negWeightConstraintsOccurrences.capacity() * sizeof( WeightConstraintRule* ) + sizeof( negWeightConstraintsOccurrences ) +
                    positionsInNegWeightConstraints.capacity() * sizeof( unsigned ) + sizeof( positionsInNegWeightConstraints ) +
                    posWeightConstraintsOccurrences.capacity() * sizeof( WeightConstraintRule* ) + sizeof( posWeightConstraintsOccurrences ) +
                    positionsInPosWeightConstraints.capacity() * sizeof( unsigned ) + sizeof( positionsInPosWeightConstraints ) +
                    doubleNegOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( doubleNegOccurrences ) +
//                    negConstraints.capacity() * sizeof( NormalRule* ) + sizeof( negConstraints ) +
//                    posConstraints.capacity() * sizeof( NormalRule* ) + sizeof( posConstraints ) +
                    sizeof( unsigned ) * 3 + sizeof( bool ) + sizeof( WeightConstraintRule* )
            );
        }
        
        inline void clear()
        {
            headOccurrences.clearAndDelete();
            posOccurrences.clearAndDelete();
            negOccurrences.clearAndDelete();
            doubleNegOccurrences.clearAndDelete();
            negWeightConstraintsOccurrences.clearAndDelete();
            positionsInNegWeightConstraints.clearAndDelete();
            posWeightConstraintsOccurrences.clearAndDelete();
            positionsInPosWeightConstraints.clearAndDelete();
//            negConstraints.swap( tmpNegConstraints );
//            posConstraints.swap( tmpPosConstraints );
        }

        AtomData( const AtomData& init ) : supported( init.supported ), numberOfHeadOccurrences( init.numberOfHeadOccurrences ), readNormalRule_negativeLiterals( init.readNormalRule_negativeLiterals ), readNormalRule_positiveLiterals( init.readNormalRule_positiveLiterals ), weightConstraintRule( init.weightConstraintRule )
        {
            headOccurrences.initFrom( init.headOccurrences );
            posOccurrences.initFrom( init.posOccurrences );
            negOccurrences.initFrom( init.negOccurrences );
            doubleNegOccurrences.initFrom( init.doubleNegOccurrences );
            negWeightConstraintsOccurrences.initFrom( init.negWeightConstraintsOccurrences );
            positionsInNegWeightConstraints.initFrom( init.positionsInNegWeightConstraints );
            posWeightConstraintsOccurrences.initFrom( init.posWeightConstraintsOccurrences );
            positionsInPosWeightConstraints.initFrom( init.positionsInPosWeightConstraints );
        }

//        AtomData& operator=( const AtomData& right ) 
//        {
//            supported = right.supported;
//            numberOfHeadOccurrences = right.numberOfHeadOccurrences;
//            readNormalRule_negativeLiterals = right.readNormalRule_negativeLiterals;
//            readNormalRule_positiveLiterals = right.readNormalRule_positiveLiterals;
//            weightConstraintRule = right.weightConstraintRule;
//            
//            headOccurrences.clearAndDelete();
//            posOccurrences.clearAndDelete();
//            negOccurrences.clearAndDelete();
//            doubleNegOccurrences.clearAndDelete();
//            negWeightConstraintsOccurrences.clearAndDelete();
//            positionsInNegWeightConstraints.clearAndDelete();
//            posWeightConstraintsOccurrences.clearAndDelete();
//            positionsInPosWeightConstraints.clearAndDelete();
//            
//            headOccurrences.initFrom( right.headOccurrences );
//            posOccurrences.initFrom( right.posOccurrences );
//            negOccurrences.initFrom( right.negOccurrences );
//            doubleNegOccurrences.initFrom( right.doubleNegOccurrences );
//            negWeightConstraintsOccurrences.initFrom( right.negWeightConstraintsOccurrences );
//            positionsInNegWeightConstraints.initFrom( right.positionsInNegWeightConstraints );
//            posWeightConstraintsOccurrences.initFrom( right.posWeightConstraintsOccurrences );
//            positionsInPosWeightConstraints.initFrom( right.positionsInPosWeightConstraints );
//            
//            return *this;
//        }
    };
    
private:
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
//    void addNormalRule( unsigned head, Literal body );
//    void addConstraint( Clause* body );
//    Clause* readBody( istream& input, vector< Variable* >& truePositiveLiterals );
//    Literal getBodyLiteral( Clause* body );
    
//    void addSupportClauses();
    void processRecursivePositiveCrule( Clause* crule );
    void processRecursiveNegativeCrule( Clause* crule );
    void computeGusStructures();
    void computeSCCs();
    void computeCompletion();
    void simplify();
    void removeSatisfiedLiterals( WeightConstraintRule* );
    
    void readAtomsTable( Istream& input );

    void readTrueAtoms( Istream& input );
    void readFalseAtoms( Istream& input );

    void readErrorNumber( Istream& input );
    
    void createStructures( unsigned id );
    
    void propagate();
    void propagateTrue( Var var );
    void propagateFalse( Var var );
    void propagateFact( Var var );
    
    void bodyToConstraint( NormalRule* rule );    
    
    void cleanData();
    
//    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
//    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;
    
    Trie bodiesDictionary;
    
//    vector< unsigned int > inputVarId;
//    vector< unsigned int > auxVarId;
    
    unsigned propagatedLiterals;
    
    void add( NormalRule* rule );
    void add( WeightConstraintRule* rule );
    bool addUndefinedLiteral( Clause* clause, Literal lit );
    void removeAndCheckSupport( NormalRule* rule );
    bool shrinkPos( NormalRule* rule, unsigned lit );
    void shrinkNeg( NormalRule* rule, unsigned lit );
    void shrinkDoubleNeg( NormalRule* rule, unsigned lit );
    void onShrinking( NormalRule* rule );
    
    void updateMaxPossibleValueWeightConstraint( WeightConstraintRule* rule, unsigned int position );
    void updateCurrentValueWeightConstraint( WeightConstraintRule* rule, unsigned int position );
    void weightConstraintIsTrue( WeightConstraintRule* rule );
    void weightConstraintIsFalse( WeightConstraintRule* rule );
    void atLeastOne( WeightConstraintRule* rule );
    void atMostOne( WeightConstraintRule* rule );
    void atMostOnePairwise( WeightConstraintRule* rule );
    void atMostOneBimander( WeightConstraintRule* rule );
    void atMostOneSequential( WeightConstraintRule* rule );
    void atMostOneBisequential( WeightConstraintRule* rule );
    Aggregate* weightConstraintToAggregate( WeightConstraintRule* rule );
    void addWeightConstraints();
    void cleanWeightConstraint( WeightConstraintRule* rule );
    void addOptimizationRules();
    void addOptimizationRule( WeightConstraintRule* rule );
    void computeLinearCostsForOptimizationRules( vector< unsigned int >& maxCostOfLevelOfOptimizationRules, vector< int >& literals, vector< unsigned int >& weights, vector< unsigned int >& levels, unsigned int& bound );
    
    void createCrule( Literal head, NormalRule* rule );
    void clearDataStructures();    

    Vector< NormalRule* > normalRules;
    Vector< WeightConstraintRule* > weightConstraintRules;
    Vector< WeightConstraintRule* > delayedAggregateRewriting;
    Vector< WeightConstraintRule* > optimizationRules;
    vector< AtomData > atomData;
    Vector< Clause* > crules;
    //vector< NormalRule* > constraints;
    
    unsigned readNormalRule_numberOfCalls;
    Vector< unsigned > atomsWithSupportInference;
    Vector< unsigned > facts;
    unordered_map< Var, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), propagatedLiterals( 0 ), readNormalRule_numberOfCalls( 0 )
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

#endif
