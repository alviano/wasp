input = """
a v b :- w(X).
w(1) v w(2).
"""

output = """
{a, w(1)}
{a, w(2)}
{b, w(1)}
{b, w(2)}
"""
