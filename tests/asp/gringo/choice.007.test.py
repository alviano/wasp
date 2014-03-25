input = """
{a,b} :- c.
c | d.
:- not a.
"""

output = """
{a, c}
{a, c, b}
"""
