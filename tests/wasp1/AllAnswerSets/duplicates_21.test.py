input = """
a :- not b.
a :- not b.
a :- not b.

b :- not a.
"""

output = """
{a}
{b}
"""
