input = """
a(2).
b(1,2).
c(2).

q(X,Y) :- a(X), c(Y).
p(X,Y,Z) :- a(X), q(Y,Z), m(X,Z).
m(X,Y) :- a(Z), p(Z,X,Y).

"""

output = """
{a(2), b(1,2), c(2), q(2,2)}
"""
