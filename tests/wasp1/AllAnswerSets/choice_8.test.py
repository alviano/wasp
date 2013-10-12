input = """
c v d :- not a, not g.
f :- a, not f.
a v b.
g :- c.
"""

output = """
{b, d}
"""
