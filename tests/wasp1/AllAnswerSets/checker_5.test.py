input = """
b :- f.

c :- a, not b.

d :- c.

e :- d.

c :- e.

f :- e.

g v a.

"""

output = """
{g}
"""
