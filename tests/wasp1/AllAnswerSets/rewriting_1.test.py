input = """
a.
b.
c.

q :- p.
p :- q.

sth :- a,p,q,b.
sth :- undefined.
sth :- not defined.
sth :- not q, not b, not a, not p.
sth :- not defined1, not defined2, not defined3.
sth :- a, not defined1.
sth :- not defined1, b, not defined2. 
"""

output = """
{a, b, c, sth}
"""
