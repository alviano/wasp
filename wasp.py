def createReasonsForCheckFailure(clauses):
    output=[]  
    for clause in clauses:
        output.append(0)
        for l in clause:
            output.append(l)

    if len(output) == 0:
        output.append(0)
    output.append(0)
    return output

def createWeakConstraints(weak):
    output=[]
    for w in weak:
        output.append(0)
        for l in w:
            output.append(l)

    if len(output) == 0:
        output.append(0)
    output.append(0)
    return output
    

def fromNogood(conj):
    clause=[]
    for l in conj:
        clause.append(-l)
    return clause

#modeling lit -> l1 ^ ... ^ ln (for l1,...,ln in conj)
#return a list of clauses c1 ^ ... ^ cn, where ci = -lit v li (for i = 1,...,n)
def fromLitImplConj(lit, conj):
    clauses=[]
    for l in conj:
        clause=[]
        clause.append(-lit)
        clause.append(l)
        clauses.append(clause)
    return clauses

#modeling lit -> l1 v ... v ln (for l1,...,ln in disj)
#return a clause -lit v l1 v ... v ln
def fromLitImplDisj(lit, disj):
    clause=[]
    clause.append(-lit)
    for l in disj:
        clause.append(l)
    return clause

#modeling l1 ^ ... ^ ln -> lit (for l1,...,ln in conj)
#return a clause -l1 v ... v -ln v lit (for l1,...,ln in conj)
def fromConjImplLit(lit, conj):
    clause=[]
    clause.append(lit)
    for l in conj:
        clause.append(-l)
    return clause

#modeling l1 v ... v ln -> lit (for l1,...,ln in disj)
#return a list of clauses c1 ^ ... ^ cn, where ci = lit v -li (for i = 1,...,n)
def fromDisjImplLit(lit, disj):
    clauses=[]
    for l in disj:
        clause=[]
        clause.append(lit)
        clause.append(-l)
        clauses.append(clause)
    return clauses

