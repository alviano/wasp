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

#ifndef WASP_WASPFACADE_H
#define WASP_WASPFACADE_H

#include <cassert>
#include <vector>
#include <unordered_map>
using namespace std;

#include "util/WaspConstants.h"
#include "Solver.h"
#include "input/Dimacs.h"
#include "weakconstraints/WeakInterface.h"
#include "weakconstraints/Mgd.h"
#include "weakconstraints/One.h"
#include "weakconstraints/Opt.h"
#include "weakconstraints/PMRes.h"
#include "weakconstraints/OneBB.h"
#include "weakconstraints/K.h"
#include "heuristic/ExternalHeuristic.h"
#include "weakconstraints/LazyInstantiation.h"
#include "outputBuilders/NoopOutputBuilder.h"
#include "outputBuilders/WaspOutputBuilder.h"

class WaspFacade
{
    public:
        inline WaspFacade();
        inline ~WaspFacade(){ delete outputBuilder; delete tmpOutputBuilder; }
        
        /**
         * Reads the input (gringo or dimacs format).
         * @param i
         */        
        void readInput( istream& i );
        
        /**
         * Default solving method, solving algorithm is selected according to the input program (computation of answer sets, computation of cautious consequences, computation of optimum answer sets).
         */
        void solve();
        
        /**
         * Incremental solving method. Computes an answer set under assumptions, if it exists. Otherwise, an unsatisfiable core is stored in conflict.
         * Returns COHERENT, INCOHERENT, or INTERRUPTED.
         * @param assumptions
         * @param conflicts
         * @return 
         */
        unsigned int solve(const vector<Literal>& assumptions, vector<Literal>& conflict);
        
        /**
         * Adds a clause to the clause DB. The method performs a full restart to the level 0.
         * @param lits
         * @return 
         */
        inline bool addClause(const vector<Literal>& lits) {
            if(solver.conflictDetected() || !ok_) return false;
            solver.unrollToZero();
            if( lits.empty() ) { ok_ = false; return false; }
            Clause* clause = new Clause( lits.size() );
            for(unsigned int i = 0; i < lits.size(); i++) {
                addVariables(lits[i].getVariable());
                if(solver.hasBeenEliminated(lits[i].getVariable())) WaspErrorMessage::errorGeneric("Trying to add a deleted variable to clause.");
                if(solver.isFalse(lits[i])) continue;                  
                if(solver.isTrue(lits[i])) { delete clause; return true; }
                clause->addLiteral(lits[i]);
            }
            if(clause->size() == 0) { ok_ = false; return false; }
            if(clause->size() > 1) {
                //Tautology: do not add.
                if(clause->removeDuplicatesAndCheckIfTautological()) return true;
            }
            bool res = runtime_ ? solver.addClauseRuntime(clause) : solver.addClause(clause);
            if(!res) ok_ = false;
            return ok_;
        }
        
        
        inline bool addClause(Literal lit) { vector<Literal> lits; lits.push_back(lit); return addClause(lits); }
        inline bool addClause(Literal l1, Literal l2) { vector<Literal> lits; lits.push_back(l1); lits.push_back(l2); return addClause(lits); }
        
        /**
         * Adds a pseudoBoolean constraint to the DB. The method performs a full restart to the level 0.         
         * @param lits
         * @param weights
         * @return 
         */        
        bool addPseudoBooleanConstraint(vector<Literal>& lits, vector<uint64_t>& weights, uint64_t bound);
        
        
        /**
         * Add a cardinality constraint to the DB. The method performs a full restart to the level 0.
         * @param lits
         * @param weights
         * @param bound
         * @return 
         */
        inline bool addCardinalityConstraint(vector<Literal>& lits, uint64_t bound) {
            if(bound==1) return addClause(lits);            
            vector<uint64_t> weights;
            for(unsigned int i = 0; i < lits.size(); i++) weights.push_back(1);
            return addPseudoBooleanConstraint(lits, weights, bound);
        }
        
        /**
         * Prints the latest found answer set. If the answer set has not been computed the method is useless.
         */
        inline void printAnswerSet() { if(!solver.conflictDetected() && !solver.hasUndefinedLiterals()) solver.printAnswerSet(); }
        inline void printIncoherence() { solver.foundIncoherence(); }
        inline bool isTrue(Literal lit) const { return solver.isTrue(lit); }        
        inline bool isFalse(Literal lit) const { return solver.isFalse(lit); }
        inline bool isUndefined(Literal lit) const { return solver.isUndefined(lit); }
        inline unsigned int decisionLevel(Literal lit) const { return solver.getDecisionLevel(lit); }
        
        inline bool isTrue(Var v) const { return solver.isTrue(v); }
        inline bool isFalse(Var v) const { return solver.isFalse(v); }
        inline bool isUndefined(Var v) const { return solver.isUndefined(v); }
        inline unsigned int decisionLevel(Var v) const { return solver.getDecisionLevel(v); }
        inline void freeze(Var var) { solver.setFrozen(var); }
        
        /**
         * Notification of safe termination.
         */
        inline void onFinish() { statistics( &solver, endSolving() ); estatistics( &solver, endSolving() ); statistics( &solver, printTime() ); solver.onFinish(); }

        /**
         * Notification of kill.
         */
        inline void onKill() { statistics( &solver, endSolving() ); estatistics( &solver, endSolving() ); statistics( &solver, printTime() ); solver.onKill(); }        
        inline unsigned int numberOfLevels() const { return solver.numberOfLevels(); }
        
        inline void getOptLiterals(unsigned int level, vector< OptimizationLiteralData >& optLits) {
            optLits.clear();            
            for(unsigned int i = 0; i < solver.numberOfOptimizationLiterals(level); i++) {
                //copy
                optLits.push_back(solver.getOptimizationLiteral(level, i));
            }
        }
        
        /**
         * Prints the cost of the answer set.
         */
        inline void printOptimizationValue()
        { 
            if(solver.conflictDetected() || solver.hasUndefinedLiterals()) return;
            Vector< uint64_t > costs;
            solver.computeCostOfModel( costs );
            solver.printOptimizationValue( costs );
        }
        
        /**
         * Prints that the optimum solution has been found.
         */
        inline void printOptimumFound() { solver.optimumFound(); }
        
        /**
         * Remove all budgets.
         */
        inline void budgetOff() { solver.setMaxNumberOfChoices( UINT_MAX ); solver.setMaxNumberOfRestarts( UINT_MAX ); solver.setMaxNumberOfSeconds( UINT_MAX ); }
        
        /**
         * Set the budget. Type can be BUDGET_CHOICES, BUDGET_RESTARTS, BUDGET_TIME (in seconds).
         * @param value
         * @param type
         */
        inline void setBudget(unsigned int type, unsigned int value)
        {
            switch( type )
            {
                case BUDGET_CHOICES:
                    solver.setMaxNumberOfChoices(value);
                    break;
                    
                case BUDGET_RESTARTS:
                    solver.setMaxNumberOfRestarts(value);
                    break;
                    
                case BUDGET_TIME:
                    solver.setMaxNumberOfSeconds(value);
                    break;
                    
                default:
                    WaspErrorMessage::errorGeneric("Budget can be set on choices, restarts or time.");
                    return;
            }
        }
        
        /**
         * Add a new variable to the solver
         * @param frozen
         * @return 
         */
        inline Var addVariable(bool frozen = false) {
            runtime_ ? solver.addVariableRuntime() : solver.addVariable();
            Var id = solver.numberOfVariables();
            if(frozen) solver.setFrozen(id);
            return id;
        }
        inline unsigned int numberOfVariables() const { return solver.numberOfVariables(); }
        
        inline void setPreferredChoices(const vector<Literal>& prefChoices) { solver.removePrefChoices(); solver.addPrefChoices(prefChoices); }
        
        inline void setMinisatPolicy() { solver.setMinisatHeuristic(); }
        void setOutputPolicy( OUTPUT_POLICY );
        void setRestartsPolicy( RESTARTS_POLICY, unsigned int threshold );

        inline void setMinimizeUnsatCore( bool value ) { solver.setMinimizeUnsatCore( value ); }        
        
        /**
         * Return the cost of the answer set for a specific level
         * @param level
         * @return UINT64_MAX if the program is known to be incoherent or if the interpretation is partial.
         */
        inline uint64_t computeCostOfAnswerSet( unsigned int level ) {
            if(solver.conflictDetected() || solver.hasUndefinedLiterals()) return UINT64_MAX;
            return solver.computeCostOfModel(level);
        }
        
        inline void attachAnswerSetListener( AnswerSetListener* listener ) { solver.attachAnswerSetListener( listener ); }
        inline void removeAnswerSetListener( AnswerSetListener* listener ) { solver.removeAnswerSetListener( listener ); }
        inline void attachProgramListener( ProgramListener* listener ) { solver.attachProgramListener( listener ); }
        inline void removeProgramListener( ProgramListener* listener ) { solver.removeProgramListener( listener ); }
        
        inline OutputBuilder* getOutputBuilder() const { return outputBuilder; }
        inline void enableOutput() { if( tmpOutputBuilder != NULL ) { solver.setOutputBuilder( outputBuilder ); delete tmpOutputBuilder; tmpOutputBuilder = NULL; } }
        inline void disableOutput() { if( tmpOutputBuilder == NULL ) { tmpOutputBuilder = new NoopOutputBuilder(); solver.setOutputBuilder( tmpOutputBuilder ); } }        
        
        inline void disableVariableElimination() { disableVE_ = true; };
        
    private:
        Solver solver;
        bool runtime_;
        bool ok_;
        bool disableVE_;
        
        OutputBuilder* outputBuilder;
        OutputBuilder* tmpOutputBuilder;
        
        void handleWeakConstraints();
        inline unsigned int solveWithWeakConstraints();
        inline void greetings(){ solver.greetings(); }
        inline void addVariables(Var addedVar);
};

WaspFacade::WaspFacade() : runtime_(false), ok_(true), disableVE_(false), tmpOutputBuilder(NULL)
{   
    outputBuilder = new WaspOutputBuilder();
    solver.setOutputBuilder(outputBuilder);    
    if( wasp::Options::interpreter != NO_INTERPRETER && wasp::Options::heuristic_scriptname != NULL )
        solver.setChoiceHeuristic( new ExternalHeuristic( solver, wasp::Options::heuristic_scriptname, wasp::Options::interpreter, wasp::Options::scriptDirectory ) );
    estatistics( &solver, checkSolver( &solver ) );
}

unsigned int
WaspFacade::solveWithWeakConstraints()
{
    WeakInterface* w = NULL;    
    switch( wasp::Options::weakConstraintsAlg )
    {
        case MGD:
            w = new Mgd( solver );
            break;

        case OPT:
            w = new Opt( solver );
            break;

        case BB:
            w = new Opt( solver, true );
            break;

        case PMRES:            
            w = new PMRes( solver );
            break;

        case ONEBB:
            w = new OneBB( solver );
            break;

        case ONEBBREST:
            w = new OneBB( solver, true );
            break;
            
        case BBBT:
            w = new Opt( solver, true );            
            break;
            
        case KALG:
            w = new K( solver );
            break;
        
        case LAZY:
            w = new LazyInstantiation( solver, solver.getExternalPropagatorForLazyWeakConstraints() );
            break;
            
        case ONE:
        default:            
            w = new One( solver );
            break;
    }
    
    w->setDisjCoresPreprocessing( wasp::Options::disjCoresPreprocessing );
    unsigned int res = w->solve();    
    delete w;
    return res;
}

void
WaspFacade::addVariables(Var addedVar) {   
    if(runtime_) {
        while(addedVar > solver.numberOfVariables())
            solver.addVariableRuntime();        
    }
    else {
        while(addedVar > solver.numberOfVariables())
            solver.addVariable();
    }
}

#endif
