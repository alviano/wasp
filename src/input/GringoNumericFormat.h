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
    
    inline bool isHeadCycleFree( Component* component );

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
        
        inline bool isFact() const { assert( !literals.empty() ); return literals.size() == 1 && literals.back().isPossiblySupportedHeadAtom(); }
//        inline bool isFiring() const { return posBody.empty() && negBody.empty() && doubleNegBody.empty(); }
        inline unsigned size() const { return literals.size(); }        
        
        inline void addHeadAtom( unsigned id ) { literals.push_back( Literal::newPossiblySupportedHeadAtom( id ) ); }
        inline void addNegativeLiteral( unsigned id ) { literals.push_back( Literal::newNegativeBodyLiteral( id ) ); }
        inline void addPositiveLiteral( unsigned id ) { literals.push_back( Literal::newUndefinedPositiveBodyLiteral( id ) ); }
        inline void addPositiveTrueLiteral( unsigned id ) { literals.push_back( Literal::newTruePositiveBodyLiteral( id ) ); }
        inline void addDoubleNegLiteral( unsigned id ) { literals.push_back( Literal::newDoubleNegatedBodyLiteral( id ) ); }
        
        inline unsigned int sizeOf()
        {
            return literals.capacity() * sizeof( Literal ) + sizeof( Vector< Literal > );
        }
    };
    
    class WeightConstraintRule
    {
        friend ostream& operator<<( ostream& out, const WeightConstraintRule& rule );
        public:            
            inline WeightConstraintRule( unsigned int id_, uint64_t bound_ ) : id( id_ ), bound( bound_ ), currentValue( 0 ), maxPossibleValue( 0 ), umax( 0 ) {}
            
            inline bool sameSizeOfInternalVectors() const
            {
                return literals.size() == weights.size();
            }
            
            inline void setBound( uint64_t bound_ )
            {
                bound = bound_;
            }                        
            
            inline uint64_t getBound() const
            {
                return bound;
            }
            
            inline unsigned int getId() const
            {
                return id;
            }
            
            inline uint64_t getMaxPossibleValue() const
            {
                return maxPossibleValue;
            }
            
            inline uint64_t getCurrentValue() const
            {
                return currentValue;
            }
            
            inline unsigned int size() const
            {
                return literals.size();
            }
            
            inline void resize( unsigned int newSize )
            {
                literals.resize( newSize );
                weights.resize( newSize );
            }
            
            inline void overwrite( unsigned int toChange, unsigned int newPosition )
            {
                assert( toChange < literals.size() );
                assert( literals.size() == weights.size() );
                literals[ toChange ] = literals[ newPosition ];
                weights[ toChange ] = weights[ newPosition ];
            }
            
            inline void incrementMaxPossibleValue( uint64_t value )
            {
                maxPossibleValue += value;
            }
            
            inline void decrementMaxPossibleValue( uint64_t value )
            {
                maxPossibleValue -= value;
            }
            
            inline void incrementCurrentValue( uint64_t value )
            {
                currentValue += value;
            }            
            
            inline void decrementBound( uint64_t value )
            {
                assert( bound >= value );
                bound -= value;
            }
            
            inline int getLiteral( unsigned int pos ) const
            {
                assert( pos < literals.size() );
                return literals[ pos ];
            }
            
            inline uint64_t getWeight( unsigned int pos ) const
            {
                assert( pos < weights.size() );
                return weights[ pos ];
            }
            
            inline void setWeight( unsigned int pos, uint64_t value )
            {
                assert( pos < weights.size() );
                weights[ pos ] = value;
            }
            
            inline bool canBeAClause() const
            {
                return ( weights.front() == weights.back() ) && ( weights.front() != 0 ) && ( bound / weights.front() == 1 );
            }
            
            inline void addNegativeLiteralCount( unsigned int idLit )
            {
                addNegativeLiteral( idLit );
                addNegativeLiteralWeight( 1 );
            }

            inline void addPositiveLiteralCount( unsigned int idLit )
            {
                addPositiveLiteral( idLit );
                addPositiveLiteralWeight( 1 );
            }

            inline void addNegativeLiteral( unsigned int idLit ) { literals.push_back( -idLit ); }
            inline void addPositiveLiteral( unsigned int idLit ) { literals.push_back( idLit ); }            
            
            inline void addNegativeLiteralWeight( uint64_t weightLit )
            {
                if( weightLit > bound )
                    weightLit = bound;
                weights.push_back( weightLit );
            }

            inline void addPositiveLiteralWeight( uint64_t weightLit )
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
            vector< int > literals;
            vector< uint64_t > weights;

            unsigned int id;
            uint64_t bound;
            uint64_t currentValue;
            uint64_t maxPossibleValue;
            unsigned int umax;
            
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
                uint64_t* auxWeights = new uint64_t[ right + 1 ];

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
        unsigned supported : 1;
        unsigned numberOfHeadOccurrences : 31;
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
        Clause* crule;

        inline AtomData( bool supported_ = false ) : supported( supported_ ), numberOfHeadOccurrences( 0 ), readNormalRule_headAtoms( 0 ), readNormalRule_negativeLiterals( 0 ), readNormalRule_positiveLiterals( 0 ), weightConstraintRule( NULL ), crule( NULL ) {}
        
        inline bool isSupported() const { return supported; }
        inline void setSupported() { supported = true; }
        
        inline bool isWeightConstraint() const { return weightConstraintRule != NULL; }
        inline void setWeightConstraint( WeightConstraintRule* rule ) { assert( rule != NULL ); weightConstraintRule = rule; }
        
//        inline unsigned int sizeOf()
//        {
//            return
//            (
//                headOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( headOccurrences ) +                
//                posOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( posOccurrences ) +
//                negOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( negOccurrences )+
//                negWeightConstraintsOccurrences.capacity() * sizeof( WeightConstraintRule* ) + sizeof( negWeightConstraintsOccurrences ) +
//                positionsInNegWeightConstraints.capacity() * sizeof( unsigned ) + sizeof( positionsInNegWeightConstraints ) +
//                posWeightConstraintsOccurrences.capacity() * sizeof( WeightConstraintRule* ) + sizeof( posWeightConstraintsOccurrences ) +
//                positionsInPosWeightConstraints.capacity() * sizeof( unsigned ) + sizeof( positionsInPosWeightConstraints ) +
//                doubleNegOccurrences.capacity() * sizeof( NormalRule* ) + sizeof( doubleNegOccurrences ) +
////                    negConstraints.capacity() * sizeof( NormalRule* ) + sizeof( negConstraints ) +
////                    posConstraints.capacity() * sizeof( NormalRule* ) + sizeof( posConstraints ) +
//                sizeof( unsigned ) * 3 + sizeof( bool ) + sizeof( WeightConstraintRule* )
//            );
//        }
        
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

        AtomData( const AtomData& init ) : supported( init.supported ), numberOfHeadOccurrences( init.numberOfHeadOccurrences ), readNormalRule_headAtoms( init.readNormalRule_headAtoms ), readNormalRule_negativeLiterals( init.readNormalRule_negativeLiterals ), readNormalRule_positiveLiterals( init.readNormalRule_positiveLiterals ), weightConstraintRule( init.weightConstraintRule ), crule( init.crule )
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

        AtomData& operator=( const AtomData& right ) 
        {
            supported = right.supported;
            numberOfHeadOccurrences = right.numberOfHeadOccurrences;
            readNormalRule_headAtoms = right.readNormalRule_headAtoms;
            readNormalRule_negativeLiterals = right.readNormalRule_negativeLiterals;
            readNormalRule_positiveLiterals = right.readNormalRule_positiveLiterals;
            weightConstraintRule = right.weightConstraintRule;
            crule = right.crule;
            
            headOccurrences.clearAndDelete();
            posOccurrences.clearAndDelete();
            negOccurrences.clearAndDelete();
            doubleNegOccurrences.clearAndDelete();
            negWeightConstraintsOccurrences.clearAndDelete();
            positionsInNegWeightConstraints.clearAndDelete();
            posWeightConstraintsOccurrences.clearAndDelete();
            positionsInPosWeightConstraints.clearAndDelete();
            
            headOccurrences.initFrom( right.headOccurrences );
            posOccurrences.initFrom( right.posOccurrences );
            negOccurrences.initFrom( right.negOccurrences );
            doubleNegOccurrences.initFrom( right.doubleNegOccurrences );
            negWeightConstraintsOccurrences.initFrom( right.negWeightConstraintsOccurrences );
            positionsInNegWeightConstraints.initFrom( right.positionsInNegWeightConstraints );
            posWeightConstraintsOccurrences.initFrom( right.posWeightConstraintsOccurrences );
            positionsInPosWeightConstraints.initFrom( right.positionsInPosWeightConstraints );
            
            return *this;
        }
    };
    
private:
    void computeCost();
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
    
//    bool ruleToConstraint( NormalRule* rule );    
    void bodyToConstraint( NormalRule* rule );    
    
    void cleanData();
    
    void addExternalPropagators();
//    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
//    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;
    
    Trie bodiesDictionary;
    
//    vector< unsigned int > inputVarId;
//    vector< unsigned int > auxVarId;
    
    unsigned propagatedLiterals;
    
    void add( NormalRule* rule, unsigned int numberOfTrueHeadAtoms );
    void add( WeightConstraintRule* rule );
    bool addUndefinedLiteral( Clause* clause, Literal lit );
    void removeAndCheckSupport( NormalRule* rule );
    bool shrinkPos( NormalRule* rule, unsigned lit );
    void shrinkNeg( NormalRule* rule, unsigned lit );
    void shrinkHead( NormalRule* rule, unsigned lit );
    void shrinkDoubleNeg( NormalRule* rule, unsigned lit );
    void onShrinkingBody( NormalRule* rule );
    void onShrinkingHead( NormalRule* rule );
    void onTrueHeadAtom( NormalRule* rule, Var headAtom );
    bool isSupporting( NormalRule* rule, Var headAtom );
    
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
//    void computeLinearCostsForOptimizationRules( vector< uint64_t >& maxCostOfLevelOfOptimizationRules, vector< int >& literals, vector< uint64_t >& weights, vector< unsigned int >& levels );
    void addOptimizationRules( vector< int >& literals, vector< uint64_t >& weights, vector< unsigned int >& levels );
    
    void createCrule( Literal head, Var varToSkip, NormalRule* rule );
    void addDependencies( Literal head, NormalRule* rule );
    void clearDataStructures();
    void cleanNormalRules();
    
    Clause* normalRuleToClause( NormalRule* );

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
    
    bool usedDictionary;
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), propagatedLiterals( 0 ), readNormalRule_numberOfCalls( 0 ), usedDictionary( false )
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
