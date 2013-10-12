input = """
a(1,1).
a(1,2).
ok :- #count{ X: a(X,Y) } = 1.

"""

output = """
{a(1,1), a(1,2), ok}
"""
