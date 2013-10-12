input = """
c(2).
p(1).
a(2).
d(2,2,1).
okay(X):- c(X), #count{V:a(V),d(V,X,1)} = 1.
ouch(X):- p(X), #count{V:a(V),d(V,X,1)} = 1.
"""

output = """
{a(2), c(2), d(2,2,1), okay(2), p(1)}
"""
