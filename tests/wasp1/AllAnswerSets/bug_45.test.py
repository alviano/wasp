input = """
:- a.
b :- not a.
a :- not b.
:- not a, b.
"""

output = """
"""
