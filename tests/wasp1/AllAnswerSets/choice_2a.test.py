input = """
f(a) :- not f(b).
f(b) :- not f(a).
"""

output = """
{f(a)}
{f(b)}
"""
