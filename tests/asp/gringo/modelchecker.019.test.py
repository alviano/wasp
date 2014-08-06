input = """
a | b | c | d.
a :- b.
b :- a.
c :- d.
d :- c.
"""
output = """
{a, b}
{c, d}
"""
