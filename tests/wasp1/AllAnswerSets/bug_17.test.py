input = """
color(X,red) v color(X,green) v color(X,blue) :- node(X).

:- edge(X,Y), color(X,C), color(Y,C).

node(T) :- edge(T,Gamma).
node(T) :- edge(Gamma,T).

edge(a,b).
edge(a,c).
edge(b,c).
color(a,red) :- node(a).
color(b,green) :- node(b).

"""

output = """
{color(a,red), color(b,green), color(c,blue), edge(a,b), edge(a,c), edge(b,c), node(a), node(b), node(c)}
"""
