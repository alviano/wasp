input = """
b(1).
c(a).
a(A,B) v na(A,B) :- c(A), b(B).
:- 1 < #count{A:a(A,1)}.

"""

output = """
{a(a,1), b(1), c(a)}
{b(1), c(a), na(a,1)}
"""
