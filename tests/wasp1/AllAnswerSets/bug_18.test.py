input = """
bad :- r1, r2.
bad :- g1, g2.
bad :- b1, b2.
bad :- r2, r3.
bad :- g2, g3.
bad :- b2, b3.
bad :- r3, r1.
bad :- g3, g1.
bad :- b3, b1.
r1 :- not g1, not b1.
g1 :- not b1, not r1.
b1 :- not r1, not g1.
r2 :- not g2, not b2.
g2 :- not b2, not r2.
b2 :- not r2, not g2.
r3 :- not g3, not b3.
g3 :- not b3, not r3.
b3 :- not r3, not g3.
a :- bad, not a.


true.
r1 :- true.
"""

output = """
{b2, g3, r1, true}
{b3, g2, r1, true}
"""
