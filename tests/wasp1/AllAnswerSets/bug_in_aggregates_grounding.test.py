input = """
a(2).
b(1,3).
e(2).
s(1).
c(X) :- not a(X), not e(Y), b(X,Y), not #count{V:s(V)} = 1.
"""

output = """
{a(2), b(1,3), e(2), s(1)}
"""
