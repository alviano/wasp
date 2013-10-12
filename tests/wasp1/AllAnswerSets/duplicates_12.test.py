input = """
a v b.

s :- not a.
s :- not b.
"""

output = """
{a, s}
{b, s}
"""
