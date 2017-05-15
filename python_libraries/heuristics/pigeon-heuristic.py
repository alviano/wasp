# global data structures
var = {1: 'false', 'false': 1}
P = []  # list of all pigeon-constants
H = []  # list of all hole-constants

def addedVarName(v, name):
    #invoked when WASP parses the atom table of the gringo numeric format
    global var, H, P
    var.update({v: name, name: v})
    if name.startswith("pigeon"):
        P.append(name[7:-1])
    if name.startswith("hole"):
        H.append(name[5:-1])

def onFinishedParsing():
    # disable simplifications of variables
    return [v for v in var.keys() if isinstance(v, int)]

def choiceVars(): #event onChoiceRequired, invoked when a choice is needed
    global var, H, P
    if len(P) > len(H):
        return [4, 0]  # force incoherence
    # assign pigeon i to hole i
    return [var["inHole(%s,%s)" % (i, i)] for i in range(1, len(P))]

def onChoiceContradictory(choice): #event onIncoChoice
    pass  # no choice can be contradictory
