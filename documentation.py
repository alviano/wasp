#var e' l'id dell'atomo e name e' il suo nome
def addedVarName(var,name):
    return

#invia come informazione se un atomo e' stato eliminato
def onAtomElimination(var):
    return

#restituisce i letterali per cui ti interessa la notifica del cambio di valore di verita', nVars rappresenta il numero di variabili nel solver
def getLiterals(nVars):
    lits = []
    return lits

#definisce gli atomi da non eliminare
def getAtomsToFreeze():
    atoms = []
    return atoms

#nofica quando un letterale tra quelli indicati in getLiterals() diventa falso
#restituisce un insieme di letterali da inferire come true
def onLiteralFalse(lit):
    output = []
    return output

#per ogni letterale inferito la ragione e' una clausola C. Il metodo restituisce C.
def getReason():
    clause = []
    return choices

#nofica quando un letterale tra quelli indicati in getLiterals() ritorna undefined
def onLiteralsUndefined(lits):
    return

#notifica quando un letterale e' inferito true al livello 0
def onLitAtLevelZero(lit):
    return
