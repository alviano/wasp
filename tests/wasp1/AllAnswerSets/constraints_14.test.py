input = """
g(X) :- f(X).

i(0).

:- i(X), g(X).
"""

output = """
{i(0)}
"""
