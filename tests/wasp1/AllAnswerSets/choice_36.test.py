input = """
a v b :- c.
a v b :- d.

c v d.

:- c, not d.
"""

output = """
{a, d}
{b, d}
"""
