input = """
c(1) v c(2) v c(3).
a(X) :- c(X).

okay :- not #count{V :a(V)} > 2. 
ouch :- #count{X:a(X)} > 1, not #count{V :a(V)} > 3.
"""

output = """
{a(1), c(1), okay}
{a(2), c(2), okay}
{a(3), c(3), okay}
"""
