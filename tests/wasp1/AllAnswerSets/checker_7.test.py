input = """
a :- b.
b :- a.
c v d.  
a :- not c.

a v z.

:- not a.
:- not z.

z :- y.
y :- z.
x v w.
z :- not x.
"""

output = """
{a, b, d, w, y, z}
"""
