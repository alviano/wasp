input = """
b :- not c.
a :- b.
b :- a.
c :- b, not k.
e v e1.
e v e2.
e v e3.
e v e4.
e v e5.
:- not e, not a.
k v k1.
k v k2.
k v k3.
k v k4.
k v k5.
k v k6.
c v c1.
"""

output = """
{a, b, c1, e, k}
{a, b, c1, e1, e2, e3, e4, e5, k}
{c, e, k1, k2, k3, k4, k5, k6}
{c, e, k}
"""
