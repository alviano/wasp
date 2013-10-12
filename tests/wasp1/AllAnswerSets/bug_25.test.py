input = """
g(X) :- e(X), f(X).
f(0).

:- d(0).
"""

output = """
{f(0)}
"""
