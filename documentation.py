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

#prima di partire controlla se il programma e' incoerente per via del plugin
#0 se il programma non e' incoerente
#!= 0 se il programma e' incoerente
def isProgramIncoherent():
    return 0
#    return 1

#nofica quando un letterale tra quelli indicati in getLiterals() diventa true
#lit è il letterale, pos è la posizione nella lista lits del metodo getLiterals
#output: 3 possibili casi:
#output=[] non fa inferenza e non ha notifiche di onLiteralsUndefined
#output=[0] non fa inferenza ma riceve notifiche di onLiteralsUndefined
#output=[l1,l2,...,ln] per inferire l1, l2, ..., ln come veri e ricevere le notifiche di onLiteralsUndefined
def onLiteralTrue(lit, pos):
    output = []
    return output

#per ogni letterale inferito la ragione e' una clausola C. Il metodo restituisce C.
def getReason():
    clause = []
    return choices

#nofica quando un letterale tra quelli indicati in getLiterals() ritorna undefined
#il parametro è una lista di letterali
def onLiteralsUndefined(*lits):
    return

#notifica quando un letterale e' inferito true al livello 0
def onLitAtLevelZero(lit):
    return
