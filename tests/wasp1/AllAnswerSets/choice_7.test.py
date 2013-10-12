input = """
a v b.
c v d :- not a.
"""

output = """
{a}
{b, c}
{b, d}
"""
