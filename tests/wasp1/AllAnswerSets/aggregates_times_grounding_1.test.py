input = """
#maxint = 2.
n(2).
a(X) :- #int(X), X < #times{ Z : n(Z) }.

"""

output = """
{a(0), a(1), n(2)}
"""
