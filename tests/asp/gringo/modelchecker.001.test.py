input = """
a | a.

b1 | b2.
b1 | b4.
b3 | b5.
b3 | b4.
b5 | b4.

b2 :- b5.
b2 :- b3.
b3 :- b1.

b4 :- a, b2.
b4 :- a, b5.

b5 :- b1, b4.
b1 :- b5, b3.
"""
output = """
{a, b2, b4, b3}
{a, b2, b4, b5}
"""
