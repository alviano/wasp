input = """
x :- not y.
y :- not x.

m :- not x.
x :- not m.
"""

output = """
{m, y}
{x}
"""
