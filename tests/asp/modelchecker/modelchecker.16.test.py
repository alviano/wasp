input = """
c | d.
a | b :- c.
a :- b.
b :- a.
"""
output = """
{d}
{c, a, b}
"""
