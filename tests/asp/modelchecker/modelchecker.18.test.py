input = """
{c}.
a | b :- c.
a :- b.
b :- a.
"""
output = """
{}
{c, a, b}
"""
