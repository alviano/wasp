input = """
{a} :- b.
b :- a.
b | c.
"""

output = """
{c}
{b}
{b, a}
"""
