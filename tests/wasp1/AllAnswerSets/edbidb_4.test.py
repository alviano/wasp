input = """
g(1,1):- true. true.
% Some edges of an Input graph
g(1,2).
g(2,3).
g(3,4).
g(7,6).

% The graph is undirected for X > 6.
g(Y,X):- X>6, g(X,Y).

g(4,2).

% Transitive Closure
g(X,Y):- g(X,Z), g(Z,Y).

g(6,5).
g(3,1).
"""

output = """
{g(1,1), g(1,2), g(1,3), g(1,4), g(2,1), g(2,2), g(2,3), g(2,4), g(3,1), g(3,2), g(3,3), g(3,4), g(4,1), g(4,2), g(4,3), g(4,4), g(5,5), g(5,6), g(5,7), g(6,5), g(6,6), g(6,7), g(7,5), g(7,6), g(7,7), true}
"""
