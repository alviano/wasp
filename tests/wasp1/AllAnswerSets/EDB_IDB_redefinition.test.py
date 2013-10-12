input = """
f(a) v f(b).
f(1).
f(d) :- f(a).
f(d) :- f(b).
f(2).
"""

output = """
{f(1), f(2), f(a), f(d)}
{f(1), f(2), f(b), f(d)}
"""
