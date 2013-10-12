input = """
true.
a :- true.
b :- a.

c :- not notc.
notc :- not c.
"""

output = """
{a, b, c, true}
{a, b, notc, true}
"""
