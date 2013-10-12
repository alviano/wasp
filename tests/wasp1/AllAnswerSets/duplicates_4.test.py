input = """
b :- not a, not x.
a :- not b, not y.

x :- not b.
y :- not a.
"""

output = """
{a, x}
{b, y}
{x, y}
"""
