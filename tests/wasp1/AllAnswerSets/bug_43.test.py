input = """
a :- not b.
b :- not a.

:- b, a.
"""

output = """
{a}
{b}
"""
