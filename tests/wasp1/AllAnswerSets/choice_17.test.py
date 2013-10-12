input = """
a v b.
c v d.
a v c.

:- not c, a.
"""

output = """
{a, c}
{b, c}
"""
