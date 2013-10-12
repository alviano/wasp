input = """
#maxint = 3.
n(2).
a(X) :- #int(X), X > #times{ Z : n(Z) }.

"""

output = """
{a(3), n(2)}
"""
