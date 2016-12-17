interpretation = []
TRUE = 1
FALSE = -1
UNKNOWN = 0


def addedVarName(var, name):
    """
    Optional.
    This method is invoked while reading the name associated to a variable.
    :param var: the id of the variable
    :param name: the name associated to var
    """
    pass


def onConflict(choice):
    """
    Optional.
    A conflict happened during the solving
    :param choice: the id of the latest undefined choice according with the previous order, 0 if no choice is undefined
    """
    pass


def onDeletion():
    """
    Optional.
    The method for deleting clauses is invoked.
    """
    pass


def onStartingParsing():
    """
    Optional.
    When the solver starts the parsing of the input.
    """
    pass


def onFinishedParsing():
    """
    Optional.
    When the solver finishes the parsing of the input
    :return: (optional) list of literals that must no be removed by initial simplifications
    """


def onStartingSimplifications():
    """
    Optional.
    Starting simplifications.
    """
    pass


def onFinishedSimplifications():
    """
    Optional.
    When all simplifications are completed.
    """


def onNewClause(*clause):
    """
    Optional.
    All clauses left after the simplifications are sent to the heuristic using this method
    :param clause: the clause
    """


def onLearningClause(lbd, size, *lits):
    """
    Optional.
    When a clause is learnt.
    :param lbd: the lbd value of the learnt clause
    :param size: the size of the learned clause
    :param lits: the literals in the learned clause
    """
    pass


def onUnfoundedSet(*unfounded_set):
    """
    Optional.
    When an unfounded set is found.
    :param unfounded_set: all atoms in the unfounded set
    """
    pass


def onLoopFormula(lbd, size, *lits):
    """
    Optional.
    When a loop formula is learnt for an unfounded set.
    :param lbd: the lbd value of the loop formula
    :param size: the size of the loop formula
    :param lits: the literals in the loop formula
    """
    pass


def onLitAtLevelZero(lit):
    """
    Optional.
    When a literal is inferred as deterministic consequence of the level 0.
    :param lit: the literal that has been derived at level 0
    """
    pass


def onLitInvolvedInConflict(lit):
    """
    Optional.
    When a literal is involved in the computation of the learned clause.
    :param lit: the literal involved in the conflict
    """
    pass


def onRestart():
    """
    Optional.
    When the solver performs a restart.
    """
    pass


def onAnswerSet(*answer_set):
    """
    Optional.
    When the solver found an answer set.
    :param answer_set: A list of literals which are in the answer set. "l" if the literal is true, "-l" if the literal is false
    """
    pass


def onStartingSolver(num_vars, num_clauses):
    """
    Optional.
    When the computation starts (after reading the input and performing the simplifications).
    :param num_vars: number of vars.
    :param num_clauses: number of clauses. The ids for variables start from 1.
    """
    # set-up interpretation as everything is unknown
    global interpretation, UNKNOWN
    interpretation = [UNKNOWN] * (num_vars + 1)


def onLitInImportantClause(lit):
    """
    Optional.
    When a literal appears in special clauses.
    :param lit: the literal in the important clause.
    """
    pass


def fallback():
    """
    Optional.
    This method should be added if the heuristic wants to use the fallback heuristic at same point.
    """
    pass


def initFallback():
    """
    Optional.
    Init the activities of variables in the fallback heuristic (required fallback method).
    :return: List of pairs (v, i), the activity variable v is associated with i.
    """
    pass


def factorFallback():
    """
    Optional.
    Set the factor for the activities of variables in the fallback heuristic (required fallback method).
    :return: list of pairs (v, f), the factor f is associated to the variable v.
    """
    pass


def signFallback():
    """
    Optional.
    Set the preferred polarity for variables in the fallback heuristic (required fallback method).
    :return: list of literals
    """
    pass


def onVariableElimination(var):
    """
    Optional.
    When a variable is removed by clause rewriting.
    :param var: the variable that has been removed
    """
    pass


def onLiteralsTrue(*lits):
    """
    Optional.
    When a set of literals is derived as true either by propagation and by choice.
    :param lits: the true literals
    """
    global interpretation, TRUE, FALSE
    for l in lits:
        interpretation[abs(l)] = TRUE if l >= 0 else FALSE


def onVariableUndefined(var):
    """
    Optional.
    When a variable is set as undefined (by an unroll).
    :param var: the undefined variable
    """
    global interpretation, UNKNOWN
    interpretation[var] = UNKNOWN


def choiceVars():
    """
    Required.
    This method is invoked when a choice is needed. It can return a choice, a list of choices and special
    values for performing special actions.
    Special values:
     - [1, 0] force the solver to perform a restart
     - [n, 2, 0] use the fallback heuristic for n steps (n<=0 use always fallback heuristic) -> require the presence of the method fallback() in the script
     - [v, 3, 0] unroll the truth value of the variable v
     - [4, 0] force the solver to stop the computation returning incoherent
    :return: A list of choices. Be careful, the first choice is the last element in the list.
    """
    pass


def onChoiceContradictory(choice):
    """
    Required if ignorePolarity has not been set.
    This method is invoked when a choice cannot be inferred. If this method is added than ignorePolarity must not be added.
    :param choice: the choice that cannot be inferred
    """
    pass


def ignorePolarity():
    """
    Required if onChoiceContradictory has not been set.
    This method tells the solver to use the suggested polarity if possible, otherwise the solver infers the polarity in order to have no conflicts. If this method is added than onChoiceContradictory must not be added.
    """
    pass
