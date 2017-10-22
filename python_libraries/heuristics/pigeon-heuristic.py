import wasp

interpretation = []; atoms=[]
TRUE = 1; FALSE = -1; UNDEFINED = 0
myvars = {1: 'false', 'false': 1}
P = []  # list of all pigeon-constants
H = []  # list of all hole-constants
assigned = 1

def addedVarName(var, name):
    global atoms, myvars, P, H
    atoms.append(var)
    myvars.update({var: name, name: var})
    if name.startswith("pigeon"):
        P.append(wasp.getTerms("pigeon",name)[0]) #only one term
    if name.startswith("hole"):
        H.append(wasp.getTerms("hole",name)[0])

def getVariablesToFreeze():
    #freeze all atoms
    return atoms

def checkAnswerSet(*answer_set):
    return wasp.coherent()

def getReasonsForCheckFailure():
    #always coherent
    return []

def simplifyAtLevelZero():
    if len(P) > len(H):
        return [1] # force incoherence
    else:
        return []

def selectLiteral():
    global var, H, P, assigned
    v = myvars["inHole(%s,%s)" % (assigned, assigned)]
    assigned += 1
    return wasp.choice(v)
