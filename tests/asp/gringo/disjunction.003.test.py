input = """
a | b :- c.
c | d.
"""

output = """
{a, c}
{b, c}
{d}
"""
