input = """
a(1,1).
a(2,1).
q(1).
q(2).
p(Z):- #count{X:a(X,Y)} = Z, q(Y), #sum{W:a(W,Y)}<=Z.
"""

output = """
{a(1,1), a(2,1), p(0), q(1), q(2)}
"""
