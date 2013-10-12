input = """
a(0).
b(1).
b(2).
b(3).

okay(X) :- a(X), X < #count{V : b(V)}.
"""

output = """
{a(0), b(1), b(2), b(3), okay(0)}
"""
