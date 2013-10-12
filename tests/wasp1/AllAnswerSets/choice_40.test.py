input = """
a :- not b.
b :- not a.

c :- a.
c :- b.

"""

output = """
{a, c}
{b, c}
"""
