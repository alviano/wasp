input = """
p(1).
q(4).
q(5).
a(1).
a(2).
ouch(P,M) :- p(M), q(P), M = #count{V:a(V)}.
"""

output = """
{a(1), a(2), p(1), q(4), q(5)}
"""
