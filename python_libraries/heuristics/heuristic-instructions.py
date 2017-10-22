import wasp

def onConflict():
    """
    Optional.
    A conflict happened during the solving
    """
    pass

def onDeletion():
    """
    Optional.
    The method for deleting clauses is invoked.
    """
    pass

def onLearningClause(lbd, size, *lits):
    """
    Optional.
    When a clause is learnt.
    :param lbd: the lbd value of the learnt clause
    :param size: the size of the learned clause
    :param lits: the literals in the learned clause
    """
    pass
    
def onLitInImportantClause(lit):
    """
    Optional.
    When a literal appears in special clauses, e.g. glue clauses.
    :param lit: the literal in the important clause.
    """
    pass    
    
def onLitInvolvedInConflict(lit):
    """
    Optional.
    When a literal is involved in the computation of the learned clause.
    :param lit: the literal involved in the conflict
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

def onNewClause(*clause):
    """
    Optional.
    All clauses left after the simplifications are sent to the heuristic using this method
    :param clause: the clause
    """
    
def onRestart():
    """
    Optional.
    When the solver performs a restart.
    """
    pass    

def onUnfoundedSet(*unfounded_set):
    """
    Optional.
    When an unfounded set is found.
    :param unfounded_set: all atoms in the unfounded set
    """
    pass

def initFallback():
    """
    Optional.
    Init the activities of variables in the fallback heuristic.
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

def selectLiteral():
    """
    Required.
    This method is invoked when a choice is needed. It can return a choice and special
    values for performing special actions.
    Special values:
     - wasp.restart() force the solver to perform a restart
     - wasp.fallback(n) use the fallback heuristic for n steps (n<=0 use always fallback heuristic) -> require the presence of the method fallback() in the script
     - wasp.unroll(v) unroll the truth value of the variable v
    :return: wasp.choice(l), where l is a literal
    """
    pass