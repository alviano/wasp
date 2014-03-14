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
    void parse( istream& input );    

    class NormalRule
    {
        friend ostream& operator<<( ostream& out, const NormalRule& rule );
    public:
        unsigned head;
        vector< unsigned > negBody;
        vector< unsigned > posBody;
        vector< unsigned > posBodyTrue;
        
        inline NormalRule( unsigned head_ ) : head( head_ ) {}
        
        inline bool isRemoved() const { return head == 0; }
        inline void remove() { head = 0; }
        
        inline bool isFact() const { return posBody.empty() && negBody.empty() && posBodyTrue.empty(); }
        inline bool isFiring() const { return posBody.empty() && negBody.empty(); }
        inline unsigned size() const { return negBody.size() + posBody.size() + posBodyTrue.size(); }
        
        inline void addNegativeLiteral( unsigned id ) { negBody.push_back( id ); }
        inline void addPositiveLiteral( unsigned id ) { posBody.push_back( id ); }
        inline void addPositiveTrueLiteral( unsigned id ) { posBodyTrue.push_back( id ); }
    };
    
    class AtomData
    {
    public:
        bool supported;
        unsigned numberOfHeadOccurrences;
        vector< NormalRule* > headOccurrences;
        vector< NormalRule* > posOccurrences;
        vector< NormalRule* > negOccurrences;
        unsigned readNormalRule_negativeLiterals;
        unsigned readNormalRule_positiveLiterals;
        
        inline AtomData( bool supported_ ) : supported( supported_ ), numberOfHeadOccurrences( 0 ), readNormalRule_negativeLiterals( 0 ), readNormalRule_positiveLiterals( 0 ) {}
        
        inline bool isSupported() const { return supported; }
        inline void setSupported() { supported = true; }
    };
    
private:
    void readNormalRule( istream& input );
    void readNormalRule( istream& input, unsigned head, int bodySize, int negativeSize );
    void readConstraint( istream& input );
    void skipLiterals( istream& input, unsigned howMany );
    void readBodySize( istream& input, int& bodySize, int& negativeSize );
    void addFact( unsigned head );
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
    
    void readAtomsTable( istream& input );

    void readTrueAtoms( istream& input );
    void readFalseAtoms( istream& input );

    void readErrorNumber( istream& input );
    
    void createStructures( unsigned id );
    
    void propagate();
    void propagateTrue( Variable* var );
    void propagateFalse( Variable* var );
    void propagateFact( Variable* var );

    
//    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
//    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;
    
//    Trie bodiesDictionary;
    
//    vector< unsigned int > inputVarId;
//    vector< unsigned int > auxVarId;
    
    unsigned propagatedLiterals;
    
    void add( NormalRule* rule );
    void removeAndCheckSupport( NormalRule* rule );
    bool shrinkPos( NormalRule* rule, unsigned lit );
    void shrinkNeg( NormalRule* rule, unsigned lit );
    void onShrinking( NormalRule* rule );
    
    void createCrule( Literal head, NormalRule* rule );

    vector< NormalRule* > normalRules;
    vector< AtomData > atomData;
    vector< Clause* > crules;
    
    unsigned readNormalRule_numberOfCalls;
    vector< unsigned > atomsWithSupportInference;
    vector< unsigned > facts;
    unordered_map< Variable*, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), propagatedLiterals( 0 ), readNormalRule_numberOfCalls( 0 )
{
    atomData.push_back( AtomData( false ) );
    createStructures( 1 );
    solver.addClause( Literal( solver.getVariable( 1 ), NEGATIVE ) );
}

GringoNumericFormat::~GringoNumericFormat()
{
    for( unsigned i = 0; i < normalRules.size(); ++i )
        delete normalRules[ i ];
}

#endif
