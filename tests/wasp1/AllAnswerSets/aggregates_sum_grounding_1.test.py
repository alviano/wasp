input = """
#maxint = 2.
n(2).
a(X) :- #int(X), X > 0, X < #sum{ Z : n(Z) }.

"""

output = """
{a(1), n(2)}
"""
