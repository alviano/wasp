input = """
h(X) :- f1(X), non f2(X), f3(X), non f4(X).
h(X) :- non f0(X), non f1(X), f2(X), f3(X).

const(a).
"""

output = """
{const(a)}
"""
