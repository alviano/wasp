input = """
:- a.
:- not b.
a :- b.

a v b :- c.
c v d.
"""

output = """
"""
