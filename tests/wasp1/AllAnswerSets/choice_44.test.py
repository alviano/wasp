input = """
:- not a, x.
:- not e, y.

x v y.


l v a v e :- l.
a v e v l :- a.

nota :- y, not a.
a :- r, y, not nota.
note :- x, not e.
e :- r, x, not note.

r :- x, y.
"""

output = """
"""
