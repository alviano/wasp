input = """
q(a,b,c).
q(b,c,a).
q(c,b,c).
q(b,b,c).
q(a,b,b).
q(c,a,a).

s(c,b).
s(a,b).
s(a,c).
s(c,c).

t(a).
t(b).

w(b,c).
w(b,b).
w(a,a).

p(X,Y) :- q(X,b,Z), r(Z,b,Y), not r(X,Y,Z).
m(X,Y) :- u(a,X,Y,Y,c,X).
v(X) :- s(a,X), not t(X).
n(X,X) :- q(X,b,X).
r(X,Y,Z) :- t(a), s(X,Z), w(X,Y), not p(Z,Y).
"""

output = """
{n(c,c), q(a,b,b), q(a,b,c), q(b,b,c), q(b,c,a), q(c,a,a), q(c,b,c), r(a,a,b), r(a,a,c), s(a,b), s(a,c), s(c,b), s(c,c), t(a), t(b), v(c), w(a,a), w(b,b), w(b,c)}
"""
