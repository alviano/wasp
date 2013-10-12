input = """
a(1,1,x).
a(1,2,x).
b(x).
ok :- #count{ X: a(X,Y,Z) } = 1, b(Z).

"""

output = """
{a(1,1,x), a(1,2,x), b(x), ok}
"""
