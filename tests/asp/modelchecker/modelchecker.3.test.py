input = """
b | a | c | d | e.   % nur hier gibt es einen Unterschied

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
