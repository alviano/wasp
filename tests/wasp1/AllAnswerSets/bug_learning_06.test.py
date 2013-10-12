input = """
a v n_a.
b v n_b.
c v n_c.
d v n_d.

e :- a.
f :- b.
h :- d.
i :- a.
j :- b.
k :- a.
l :- b.
m :- c.
n :- d.
k :- o, not i.
l :- k, not j.
p :- not i. 
n :- c.
q :- not i.
r :- m. 
:- f, m. 
:- s, t.
:- not i, u.
:- h, f.
:- not n.
:- not l.
"""

output = """
{a, c, d, e, h, i, k, l, m, n, n_b, r}
{a, c, e, i, k, l, m, n, n_b, n_d, r}
{a, d, e, h, i, k, l, n, n_b, n_c}
"""
