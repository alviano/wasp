input = """
b(1).
a(X) :- b(X).
:- p(X).
"""

output = """
{a(1), b(1)}
"""
