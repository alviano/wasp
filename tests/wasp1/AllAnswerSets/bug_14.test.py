input = """
true.

w :- true.
w :- not w.
"""

output = """
{true, w}
"""
