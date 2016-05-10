#optional
#description: this method is invoked while reading the name associated to a variable
#parameters: "var" the id of the variable, "name" the name associated to var
#output: void
sub addedVarName()
{
    ($var, $name) = @_;
}

#optional
#description: a conflict happened during the solving
#parameters: "choice" the id of the latest undefined choice according with the previous order, 0 if no choice is undefined
#output: void
sub onConflict()
{
    ($choice) = @_;
}

#optional
#description: the method for deleting clauses is invoked
#parameters: void
#output: void
sub onDeletion()
{
}

#optional
#description: when the solver starts the parsing of the input
#parameters: void
#output: void
sub onStartingParsing()
{
}

#optional
#description: when the solver finishes the parsing of the input
#parameters: void
#output: (optional) list of literals that must no be removed by initial simplifications
sub onFinishedParsing()
{
#    @lits = (-1,2,3);
#    return lits;
}

#optional
#description: starting simplifications
#parameters: void
#output: void
sub onStartingSimplifications()
{
}

#optional
#description: when all simplifications are completed
#parameters: void
#output: void
sub onFinishedSimplifications()
{
}

#optional
#description: all clauses left after the simplifications are sent to the heuristic using this method
#parameters: the clause
#output: void
sub onNewClause()
{
    @clause = @_;
}

#optional
#description: when a clause is learnt
#parameters: "lbd" the lbd value of the learnt clause, "size" the size of the learned clause, "lits" the literals in the learned clause
#output: void
sub onLearningClause()
{
    @elems = @_;
    $lbd = $elems[0];
    $size = $elems[1];
    @lits;
    for($i=0; $i < $size; $i++)
        push(@lits, $elems[$i]);
}

#optional
#description: when an unfounded set is found
#parameters: all atoms in the unfounded set
#output: void
sub onUnfoundedSet()
{
    @unfoundedset = @_;
}

#optional
#description: when a loop formula is learnt for an unfounde set
#parameters: "lbd" the lbd value of the loop formula, "size" the size of the loop formula, "lits" the literals in the loop formula
#output: void
sub onLoopFormula()
{
    @elems = @_;
    $lbd = $elems[0];
    $size = $elems[1];
    @lits;
    for($i=0; $i < $size; $i++)
        push(@lits, $elems[$i]);
}

#optional
#description: when a literal is inferred as deterministic consequence of the level 0
#parameters: "lit" the literal that has been derived at level 0
#output: void
sub onLitAtLevelZero()
{
    ($lit) = @_;
}

#optional
#description: when a literal is involved in the computation of the learned clause
#parameters: "lit" the literal involved in the conflict
#output: void
sub onLitInvolvedInConflict()
{
    ($lit) = @_;
}

#optional
#description: when the solver performs a restart
#parameters: void
#output: void
sub onRestart()
{
}

#optional
#description: when the solver found an answer set
#parameters: "AnswerSet" a list of literals which are in the answer set. "l" if the literal is true, "-l" if the literal is false
#output: void
sub onAnswerSet()
{
    @AnswerSet = @_;
    print "@AnswerSet\n";
}

#optional
#description: when the computation starts (after reading the input and performing the simplifications)
#parameters: "nVars" number of vars, "nClauses" number of clauses. The ids for variables start from 1.
#output: void
sub onStartingSolver()
{
   ($nVars, $nClauses) = @_;
}

#optional
#description: when a literal appears in special clauses
#parameters: "lit" the literal in the important clause.
#output:
sub onLitInImportantClause()
{
    ($lit) = @_;
}

#optional
#description: this method should be added if the heuristic wants to use the fallback heuristic at same point
#parameters: void
#output: void
sub fallback()
{
}

#optional
#description: init the activities of variables in the fallback heuristic (required fallback method)
#parameters: void
#output: list of pairs (v, i), the activity variable v is associated with i
sub initFallback()
{    
    @litsAct = (1,1,2,3);
    return litsAct;
}

#optional
#description: set the factor for the activities of variables in the fallback heuristic (required fallback method)
#parameters: void
#output: list of pairs (v, f), the factor f is associated to the variable v
sub factorFallback()
{
    @litsFactor = (1,1,2,3);
    return litsFactor;
}

#optional
#description: set the preferred polarity for variables in the fallback heuristic (required fallback method)
#parameters: void
#output: list of literals
sub signFallback()
{
    @signs = (1,-4,2,3);
    return signs;
}


#optional
#description: when a variable is removed by clause rewriting
#parameters: "var" the variable that has been removed
#output: void
sub onVariableElimination()
{
    ($var) = @_;
}

#optional
#description: when a set of literals is derived as true either by propagation and by choice
#parameters: "lits" the true literals
#output: void
sub onLiteralsTrue()
{
    (@lits) = @_;
}

#optional
#description: when a variable is set as undefined (by an unroll)
#parameters: "var" the undefined variable
#output: void
sub onVariableUndefined()
{
    ($var) = @_;
}

#required
#description: this method is invoked when a choice is needed. It can return a choice, a list of choices and special values for performing special actions.
#parameters: a partial interpretation if the method partialInterpretation() is added.
#output: a list of choices. Be careful, the first choice is the last element in the list
#special values:
    # - (1,0) force the solver to perform a restart
    # - (n,2,0) use the fallback heuristic for n steps (n<=0 use always fallback heuristic) -> require the presence of the method fallback() in the script
    # - (v,3,0) unroll the truthvalue of the variable v
    # - (4,0) force the solver to stop the computation returning incoherent
sub choiceVars()
{
    #(@interpretation) = @_;
    ##the order of the choices will be 3,2,-1
    @choices = (-1,2,3);

    #Force a restart
    #@choices = (1,0);

    #Fallback heuristic for 5 steps
    #@choices = (5,2,0);

    #Unroll variable with id 7
    #@choices = (7,3,0);

    #Trigger an incoherence
    #@choices = (4,0);

    return @choices;
}


#required if ignorePolarity has not been set
#description: this method is invoked when a choice cannot be inferred. If this method is added than ignorePolarity must not be added.
#parameters: "choice" the choice that cannot be inferred
#output: void
sub onChoiceContradictory()
{
    ($choice) = @_;
}

#required if onChoiceContradictory has not been set
#description: this method tells the solver to use the suggested polarity if possible, otherwise the solver infers the polarity in order to have no conflicts. If this method is added than onChoiceContradictory must not be added.
#parameters: void
#output: void
sub ignorePolarity()
{
}

#optional
#description: if enabled the method choiceVars receives as input also the partial interpretation
#parameters: void
#output: void
sub partialInterpretation()
{
}
