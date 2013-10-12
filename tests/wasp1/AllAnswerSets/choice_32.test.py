input = """
a v b.
:- b.

a v x v y :- x.
x v y :- y.

:- not x, not y.
"""

output = """
"""
