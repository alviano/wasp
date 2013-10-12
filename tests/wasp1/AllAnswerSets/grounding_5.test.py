input = """
n(X1) :- n(X), succ(X,X1).
n(X):- n0(X).

n0(0).
succ(0,1).
succ(1,2).
succ(2,3).
succ(3,4).
succ(4,5).
"""

output = """
{n(0), n(1), n(2), n(3), n(4), n(5), n0(0), succ(0,1), succ(1,2), succ(2,3), succ(3,4), succ(4,5)}
"""
