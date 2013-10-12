input = """
s :- a.
s :- b.

a :- true.
true.

false :- not true.
"""

output = """
{a, s, true}
"""
