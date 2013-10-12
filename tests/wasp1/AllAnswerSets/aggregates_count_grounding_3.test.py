input = """
b(1). b(2).
c(a). c(b).
a(A,B) v na(A,B) :- c(A), b(B).
:- #count{A:a(A,B)} > 1, b(B) .
ad(A) :- a(A,_).
:- c(A), not ad(A).
"""

output = """
{a(a,1), a(b,2), ad(a), ad(b), b(1), b(2), c(a), c(b), na(a,2), na(b,1)}
{a(a,2), a(b,1), ad(a), ad(b), b(1), b(2), c(a), c(b), na(a,1), na(b,2)}
"""
