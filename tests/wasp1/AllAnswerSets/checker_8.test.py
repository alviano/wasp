input = """
a :- b.
b :- a.
c v d.  
a v c.

a v z.

:- not a.
:- not z.

z :- y.
y :- z.
x v w.
z v x.
"""

output = """
{a, b, d, w, y, z}
"""
