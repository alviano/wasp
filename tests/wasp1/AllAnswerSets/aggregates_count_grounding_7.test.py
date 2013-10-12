input = """
b(1).
c(a).
a(A,B) v na(A,B) :- c(A), b(B).
:- 0 < #count{A:a(A,1)}.

"""

output = """
{b(1), c(a), na(a,1)}
"""
