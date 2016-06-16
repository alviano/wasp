input = """
{a} :- b.
{a} :- c.
b :- a.
c :- a.
{d}.
b :- d.
c :- d.
"""
output = """
{}
{d, c, b}
{d, c, b, a}
"""
