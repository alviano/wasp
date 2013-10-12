input = """
a v na.
b v nb.
c v nc.
d v nd.
x v nx.

x :- w.
nx :- w.

w :- na, nc, nx.
w :- nb, c, nx.
w :- b, nd.
w :- not w.
"""

output = """
{a, b, c, nd, nx, w, x}
{a, b, nc, nd, nx, w, x}
{b, c, na, nd, nx, w, x}
{b, na, nc, nd, nx, w, x}
"""
