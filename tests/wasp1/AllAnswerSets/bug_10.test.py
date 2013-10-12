input = """
fatto.
pos(0,1,b):- fatto.
pos(0,2,b):- fatto.
pos(0,3,x):- fatto.
pos(0,4,x):- fatto.
pos(0,5,x):- fatto.
pos(0,6,x):- fatto.
pos(0,7,y):- fatto.
pos(0,8,y):- fatto.
pos(0,9,y):- fatto.
pos(0,10,y):- fatto.
prec(0,1).
prec(1,2).
prec(2,3).
prec(3,4).
prec(4,5).
prec(5,6).
prec(6,7).
prec(7,8).
prec(8,9).
prec(9,10).
num_valido(1).
num_valido(2).
num_valido(3).
num_valido(4).
uguale(X,X):-pos(0,X,Y).
diff(X,Y):- uguale(X,X), uguale(Y,Y), not uguale(X,Y).
ok:- pos(4,1,x),pos(4,2,y),pos(4,3,x),pos(4,4,y),pos(4,5,x),pos(4,6,y),
     pos(4,7,x),pos(4,8,y),pos(4,9,b),pos(4,10,b).
%  mossa(#mossa, new.Pos.S, new.Pos.S, S, S, old.Pos S, old.Pos.S)
mossa(N,P,P,S,S,V,V):-num_valido(N), prec(M,N), pos(M,P,b), pos(M,P,b),
                          pos(M,V,S), pos(M,V,S), prec(P,P), prec(V,V), 
                          not diff_mossa(N,P,P,V,V).
diff_mossa(N,P,P,V,V):- mossa(N,P,P,S,S,V,V),
                            diff(V,V), diff(V,V).
modificato(N,P,S):-mossa(N,P,X,S,X,X,X).
modificato(N,P,S):-mossa(N,X,P,X,S,X,X).
modificato(N,P,b):-mossa(N,X,X,X,X,P,X5).
modificato(N,P,b):-mossa(N,X,X,X,X,X,P).
pos(N,P,S):- modificato(N,P,S).
variazione(N,P):- modificato(N,P,S).
pos(N,P,S):- num_valido(N), not variazione(N,P), 
             pos(M,P,S), prec(M,N).
no_stable:- not ok, not no_stable.

"""

output = """
"""
