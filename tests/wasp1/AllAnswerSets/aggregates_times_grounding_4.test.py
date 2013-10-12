input = """
#maxint = 7.
n(2).
n(3).
a(X) :- #int(X), X > #times{ Z : n(Z) }.

"""

output = """
{a(7), n(2), n(3)}
"""
