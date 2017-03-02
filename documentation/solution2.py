import wasp

answer = []; atoms = []

def addedVarName(var, name):
    global atoms
    atoms.append(var)

def checkAnswerSet(*answer_set):
    global answer
    #Check whether the answer set has exactly two atoms true
    count = sum([1 for i in atoms if answer_set[i] > 0])
    if count != 2:
        answer = answer_set
        return wasp.incoherent()
    return wasp.coherent()

def getReasonsForCheckFailure():
    #In case of failure compute the clause
    global answer
    reasons=[[-answer[i] for i in atoms]]
    return wasp.createReasonsForCheckFailure(reasons)
