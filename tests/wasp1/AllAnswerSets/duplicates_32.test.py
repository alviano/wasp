input = """
a :- not b.
a :- not c.
b :- not a.
c :- not a.
"""

output = """
{a}
{b, c}
"""
