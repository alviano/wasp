input = """
a(a).
b(1).
b(2).

cap(1).

all(X,Y) v nall(X,Y) :- a(X), b(Y).

:- #count{Y:all(a,Y)} > C, cap(C).
%:- #count{Y:all(a,Y)} < C, cap(C).

"""

output = """
{a(a), all(a,1), b(1), b(2), cap(1), nall(a,2)}
{a(a), all(a,2), b(1), b(2), cap(1), nall(a,1)}
{a(a), b(1), b(2), cap(1), nall(a,1), nall(a,2)}
"""
