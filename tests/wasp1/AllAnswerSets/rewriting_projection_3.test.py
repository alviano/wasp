input = """
f(X,1) :- a(X,Y),
          g(A),g(B),
          not f(1,X).

a(X,Y) :- g(X),g(Y).

g(x1).
g(x2).

"""

output = """
{a(x1,x1), a(x1,x2), a(x2,x1), a(x2,x2), f(x1,1), f(x2,1), g(x1), g(x2)}
"""
