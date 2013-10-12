input = """
#maxint=10.

f(a).

g(2..4).
h(1..3).

f(b).

intersect(X) :- g(X), h(X).
"""

output = """
{f(a), f(b), g(2), g(3), g(4), h(1), h(2), h(3), intersect(2), intersect(3)}
"""
