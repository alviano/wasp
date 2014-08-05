input = """
a | b | c | d.
a :- b.
b :- a.
c :- d.
d :- c.
a :- c, not d.
c :- a, not b.
"""
output = """
{a, b}
{c, d}
"""
