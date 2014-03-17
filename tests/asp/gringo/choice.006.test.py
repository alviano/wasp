input = """
{a,b} :- c.
c | d.
:- a.
"""

output = """
{c}
{c, b}
{d}
"""
