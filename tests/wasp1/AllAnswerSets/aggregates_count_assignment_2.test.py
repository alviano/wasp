input = """
a(2).
b(1).
c(2,2).
p(2).
q(2).

bug :- p(M),q(N), #count{ V:a(M),b(N),c(M,V) } = N.
"""

output = """
{a(2), b(1), c(2,2), p(2), q(2)}
"""
