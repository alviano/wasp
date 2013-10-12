input = """
% This is also interesting for the model generator, due to the strange contraint.

b :- not c.
c :- not b.

a :- b.

:- a, not a.
"""

output = """
{a, b}
{c}
"""
