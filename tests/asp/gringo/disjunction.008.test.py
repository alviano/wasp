input = """
{d}.
a | c :- d.
a | c :- b.
a :- b.
b :- a.
"""
output = """
{}
{c, d}
{a, b, d}
"""
