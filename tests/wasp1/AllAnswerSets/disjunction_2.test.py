input = """
a(X) v b(Y) :- v (X,Y) .
 v(v ,v) v w(v,w)v v(w, v ).
"""

output = """
{a(v), v(v,v)}
{a(w), v(w,v)}
{b(v), v(v,v)}
{b(v), v(w,v)}
{w(v,w)}
"""
