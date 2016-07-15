interpretation = []
TRUE = 1
FALSE = -1
UNDEFINED = 0
countTrue = 0
countFalse = 0

def getLiterals(*lits):
    global interpretation
    #The first position of lits contains the 
    #number of variables used in wasp
    for i in range(0, lits[0]+1):
        interpretation.append(UNDEFINED)
    #Attached to all changes of the truth values
    l = []
    for i in range(2,6):
        l.append(i)
        l.append(-i)
    return l

def getVariablesToFreeze():
    #Freeze all variables
    lits = [2,3,4,5]
    return lits
    
def onLiteralTrue(lit, dl):
    global interpretation, countTrue, countFalse
    #Count true and false and propagate if needed
    l = abs(lit)
    if lit >= 0:
        countTrue = countTrue + 1
        interpretation[l]=TRUE
    else:
        countFalse = countFalse + 1
        interpretation[l]=FALSE
    output = []
    if countTrue == 2:
        for i in range(2,6):
            if interpretation[i] == UNDEFINED:
                output.append(-i)
    elif countFalse == 2:
        for i in range(2,6):
            if interpretation[i] == UNDEFINED:
                output.append(i)
    return output

def getReason():
    #Let T={a,b} be the true atoms then the reason for 
    #the assignment is a ^ b.
    #Thus, a ^ b -> c and a ^ b -> d.
    #The reason is a clause thus literals are flipped.
    reason=[]
    if countTrue == 2:
        for i in range(2,6):
            if interpretation[i] == TRUE:
                reason.append(-i)
    else:
        for i in range(2,6):
            if interpretation[i] == FALSE:
                reason.append(i)
    return reason

def onLiteralsUndefined(*lits):
    global interpretation, countTrue, countFalse
    #If a literal is again undefined 
    #the interpretation is restored.
    for i, l in enumerate(lits):
    	if(i==0):
    		continue    
        if(l>=0):
            countTrue = countTrue -1
        else:
            countFalse = countFalse - 1
        interpretation[abs(l)] = UNDEFINED
    return
