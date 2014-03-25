input = """
{a,b} :- c.
c | d.
:- not a.
:- b.
"""

output = """
{a, c}
"""
