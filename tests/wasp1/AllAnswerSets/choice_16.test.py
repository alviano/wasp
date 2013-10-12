input = """
:- not a.
a :- c.
c v b v e.
c v b.
d v e.
:- a, d, e.
"""

output = """
{a, c, d}
{a, c, e}
"""
