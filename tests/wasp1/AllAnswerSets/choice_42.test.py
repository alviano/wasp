input = """
a v b.
b v c.

d :- not e.
e :- a, not c.
e :- c.
:- not d, a.
:- not d, c.
"""

output = """
{b, d}
"""
