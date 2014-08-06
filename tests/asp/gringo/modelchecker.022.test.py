input = """
a | b.
a :- b.
b :- a.
a :- not c.
a :- c.
c | d.
"""
output = """
{a, b, d}
{a, b, c}
"""
