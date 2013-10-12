input = """
#maxint = 20.
n(2).
n(5).

a(X) :- #int(X), X < #times{ Z : n(Z) }.
b(X) :- #int(X), X > #times{ Z : n(Z) }.

"""

output = """
{a(0), a(1), a(2), a(3), a(4), a(5), a(6), a(7), a(8), a(9), b(11), b(12), b(13), b(14), b(15), b(16), b(17), b(18), b(19), b(20), n(2), n(5)}
"""
