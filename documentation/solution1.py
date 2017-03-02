interpretation = []; atoms=[]
TRUE = 1; FALSE = -1; UNDEFINED = 0
countTrue = 0; countFalse = 0

def addedVarName(var, name):
    global atoms
    atoms.append(var)

def getLiterals(*lits):
    global interpretation
    #The first position of lits contains the number of atoms used in wasp
    interpretation = [UNDEFINED for i in range(0, lits[0]+1)]
    #Attached to all changes of the truth values
    l = [-i for i in atoms]
    l.extend(atoms)
    return l

def getVariablesToFreeze():
    #Freeze all atoms
    return atoms
    
def onLiteralTrue(lit, dl):
    global interpretation, countTrue, countFalse
    #Count true and false atoms and propagate if needed.
    interpretation[abs(lit)] = lit / abs(lit); #1 TRUE, -1 FALSE    
    if lit >= 0:
        countTrue += 1
    else:
        countFalse += 1
    output = []
    if countTrue == 2:
        output = [-i for i in atoms if interpretation[i]==UNDEFINED]
    elif countFalse == 2:
        output = [i for i in atoms if interpretation[i]==UNDEFINED]
    return output

def getReason():
    #Let T={a,b} be the true atoms, then the reason for the assignment is a ^ b (clause -a v -b).
    reason=[]    
    if countTrue == 2:
        reason=[-i for i in atoms if interpretation[i]==TRUE]
    else:
        reason=[i for i in atoms if interpretation[i]==FALSE]
    return reason

def onLiteralsUndefined(*lits):
    global interpretation, countTrue, countFalse
    #If a literal is again undefined the interpretation is restored.
    for i, l in enumerate(lits):
        if i == 0:    continue
        if l >= 0:    countTrue -= 1
        else:    countFalse -= 1
        interpretation[abs(l)] = UNDEFINED
    return
