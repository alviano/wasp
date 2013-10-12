input = """
p(a,b).

i(X) :- p(X,Y).

r(X,Y,X) :- i(X), i(Y).
"""

output = """
{i(a), p(a,b), r(a,a,a)}
"""
