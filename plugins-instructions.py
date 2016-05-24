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
    return

def onAtomElimination(var):
    """
    Optional.
    When an atom is removed by clause rewriting.
    :param var: the atom that has been removed
    """
    return

def getLiterals(num_vars):
    """
    Required.
    Returns the literals whose truth will be notified
    :return: The list of literals
    """
    return lits

def getAtomsToFreeze():
    """
    Required.
    Returns a set of atoms that must be not removed by atom simplifications
    :return: The list of atoms
    """
    return atoms

def onLiteralTrue(lit, pos):
    """
    Required.
    When a literal is derived as true either by propagation and by choice.
    :param lit: the true literal
    :param pos: each literal is associated to a position for fast access
    :return: A list of literals to infer as true. Afterward, the method getReasons is invoked.
    """
    global interpretation, TRUE, FALSE
    interpretation[lit] = TRUE if lit >= 0 else FALSE
    return output

def getReason():
    """
    Required.
    Returns a clause modeling the reason for the assignments A={a1,...,am} made by onLiteralTrue.
    The clause is of the form l1 ^ ... ^ ln -> a (for each a in A).
    :return: A list of literals not l1, ..., not ln
    """
    return reasons

def onLiteralsUndefined(*lits):
    """
    Required.
    When a set of literals is set as undefined (by an unroll).
    :param lits: the undefined literals
    """
    global interpretation, UNKNOWN
    for l in lits:
        interpretation[abs(l)] = UNKNOWN
    return

def onLitAtLevelZero(lit):
    """
    Optional.
    When a literal is inferred as deterministic consequence of the level 0.
    :param lit: the literal that has been derived at level 0
    """
    return

def isProgramIncoherent():
    """
    Optional.
    Before starting checks whether the program is coherent.
    :return: 0 if the program is coherent, !=0 otherwise
    """
    return 0
    
def onAnswerSet(*answer_set):
    """
    Optional.
    When the solver found an answer set.
    :param answer_set: A list of literals which are in the answer set. "l" if the literal is true, "-l" if the literal is false
    """
    return
    
def simplifyAtLevelZero():
    """
    Optional.
    Returns a list of literals to be inferred at level as deterministic consequence.
    :return: A list of literals
    """
    return out

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
