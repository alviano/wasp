input = """
true.
a v b :- true, 1 < 2.
:- a.
"""

output = """
{b, true}
"""
