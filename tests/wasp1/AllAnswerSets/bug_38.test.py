input = """
b v a v c v d v e.   % nur hier gibt es einen Unterschied

:- a.
:- a, e.
:- a, b.

c :- e.
b :- c.
e :- b.
c :- d.
"""

output = """
{b, c, e}
"""
