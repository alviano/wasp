input = """
f(a) :- not f(b).
f(b) :- not f(a).

edb(a).
f(c) :- edb(a).
"""

output = """
{edb(a), f(a), f(c)}
{edb(a), f(b), f(c)}
"""
