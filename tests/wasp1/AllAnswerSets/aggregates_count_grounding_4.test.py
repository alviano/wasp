input = """
b(1). b(2).
c(a). c(b).
a(A,B) v na(A,B) :- c(A), b(B).
:- 1 < #count{A:a(A,B)}, b(B).
"""

output = """
{a(a,1), a(a,2), b(1), b(2), c(a), c(b), na(b,1), na(b,2)}
{a(a,1), a(b,2), b(1), b(2), c(a), c(b), na(a,2), na(b,1)}
{a(a,1), b(1), b(2), c(a), c(b), na(a,2), na(b,1), na(b,2)}
{a(a,2), a(b,1), b(1), b(2), c(a), c(b), na(a,1), na(b,2)}
{a(a,2), b(1), b(2), c(a), c(b), na(a,1), na(b,1), na(b,2)}
{a(b,1), a(b,2), b(1), b(2), c(a), c(b), na(a,1), na(a,2)}
{a(b,1), b(1), b(2), c(a), c(b), na(a,1), na(a,2), na(b,2)}
{a(b,2), b(1), b(2), c(a), c(b), na(a,1), na(a,2), na(b,1)}
{b(1), b(2), c(a), c(b), na(a,1), na(a,2), na(b,1), na(b,2)}
"""
