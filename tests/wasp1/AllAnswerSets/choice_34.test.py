input = """
a v b :- d.
a v b :- c.

d :- not c.
c :- not d.

:- d, not c.
"""

output = """
{a, c}
{b, c}
"""
