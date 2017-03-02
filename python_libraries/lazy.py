import wasp
import grounder
import sys


constraints = []
atom2id={}
id2atom={}

filename="constraint.asp"

atomsInConstraint=[]
idAtomsInConstraint=[]

scriptDir=""

def setScriptDirectory(id, directory):
    scriptDir = directory

def addedVarName(var, name):
    global atom2id, id2atom, atomsInConstraint
    if name.startswith("__atoms_in_constraint("):
        atomsInConstraint.append(name[:-1].replace("__atoms_in_constraint(",""))
    else:    
        atom2id[str(name)]=int(var)
        id2atom[int(var)]=str(name)

def getVariablesToFreeze():
    global idAtomsInConstraint
    idAtomsInConstraint = [atom2id[l] for l in atomsInConstraint]
    return idAtomsInConstraint

def checkAnswerSet(*answer_set):
    global constraints, scriptDir
    answer=[]
    for i in idAtomsInConstraint:
        if answer_set[i] > 0:
            answer.append("%s." % id2atom[i])
        #else:
        #    answer.append("__not_%s." % id2atom[i])

    print("calling grounder")
    sys.stdout.flush()
    output = grounder.launchGrounder(scriptDir, filename, answer)
    print("...end! Produced %s constraints" % len(output))
    if len(output) > 0:
        constraints = output
        return wasp.incoherent()
    return wasp.coherent()

def getReasonsForCheckFailure():
    #In case of failure compute the clause
    global constraints
    reasons=[]
    for c in constraints:
        reason = []
        for l in c:
            if l.startswith("__not_"):
                name=l.replace("__not_", "")
                if name in atom2id:
                    reason.append(atom2id[name])
            else:
                reason.append(-atom2id[l])
        reasons.append(reason)
    return wasp.createReasonsForCheckFailure(reasons)
