input = """
a | b | c | d.
a :- b.
b :- a.
c :- d.
d :- c.
:- not a.
:- not c.
"""
output = """
"""
