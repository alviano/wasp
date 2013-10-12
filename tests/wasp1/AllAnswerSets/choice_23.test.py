input = """
b :- not a, not x. 
a :- not b, not y.

x :- not b, not p.
x :- not b, not q.
y :- not a, not p.
y :- not a, not q.

a v b.

"""

output = """
{a, x}
{b, y}
"""
