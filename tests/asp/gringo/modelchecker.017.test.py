input = """
a | b | c.
a :- b.
b :- a.
"""
output = """
{a, b}
{c}
"""
