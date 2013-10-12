input = """
a :- b, c.
b :- c, a.
c :- a, b.

c | d.
a | d.

"""

output = """
{a, b, c}
{d}
"""
