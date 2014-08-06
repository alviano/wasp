input = """
a1 | a2 | a3.
a2 | a3 :- a1, b1, b2.
a1 | a3 :- a2, b1, b2.
a1 | a2 :- a3, b1, b2.
b1 | b2 :- a.
c11 :- a1.
c21 :- a2.
c22 :- a2.
c31 :- a3.
c32 :- a3.
c33 :- a3.
a :- c11.
a :- c21, c22.
a :- c31, c32, c33.
"""
output = """
{a3, b2, a, c31, c32, c33}
{a3, b1, a, c31, c32, c33}
{a2, b2, a, c21, c22}
{a2, b1, a, c21, c22}
{a1, b2, a, c11}
{a1, b1, a, c11}
"""
