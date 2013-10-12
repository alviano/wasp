input = """
a(1,1).
a(2,1).
p(Z):- #count{X:a(X,Y)} = Z, #sum{W:a(W,Y)}<=Z.

"""

output = """
{a(1,1), a(2,1)}
"""
