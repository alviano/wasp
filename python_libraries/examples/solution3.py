import wasp

answer = []; atoms = []

def addedVarName(var, name):
    global atoms
    atoms.append(var)

def addWeakConstraints(*answer_set):
    global answer
    answer = answer_set
    soft = [[i] for i in atoms if answer_set[i] < 0]
    return wasp.createWeakConstraints(soft)

def addWeightsForWeakConstraints():
    global answer
    weights=[1 for i in atoms if answer[i] < 0]
    return wasp.createWeights(weights)
