hole(1..k).
pigeon(1..k).

{inHole(X,Y) : hole(Y)} = 1 :- pigeon(X).

:- hole(Y), #count{X : inHole(X,Y)} > 1.
