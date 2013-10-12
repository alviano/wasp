input = """
a.
b.
c.

q :- p, not a.
q :- not p.
p :- c.

"""

output = """
{a, b, c, p}
"""
