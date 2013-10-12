input = """
#maxint = 2.
n(1).
a(X) :- #int(X), X > 0, X = #count{Z : n(Z) }.

"""

output = """
{a(1), n(1)}
"""
