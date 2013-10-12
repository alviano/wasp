input = """
mysum(X) :- not #sum{P: c(P)} > X, #sum{T: c(T)} > X1, X1=X-1, dom(X).
mycount(X) :- not #count{P: c(P)} > X, #count{T:c(T)} > X1, X1=X-1, dom(X).

dom(0). dom(1).
c(X) v d(X) :- dom(X).

:- not c(0).
:- not c(1).
"""

output = """
{c(0), c(1), dom(0), dom(1), mysum(1)}
"""
