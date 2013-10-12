input = """
a v a.

b1 v b2.
b1 v b4.
b3 v b5.
b3 v b4.
b5 v b4.

b2 :- b5.
b2 :- b3.
b3 :- b1.

b4 :- a, b2.
b4 :- a, b5.

b5 :- b1, b4.
b1 :- b5, b3.

"""

output = """
{a, b2, b3, b4}
{a, b2, b4, b5}
"""
