input = """
% Most of this example is due to Nicola Leone.
% It is a counterexample to why single-pass "positional" PT literal computation
% is not sound.

d v f.
q(1) :- d,f.
q(2) v p(2):- d,f.
v(Y) v p(Y) :- a(X,Y), q(X).
q(X) v v(X) :- a(X,_), p(X).
p(1) :- d,f.
t v p(4).

p(3) v p(1) :- t. 
 

a(1,2). 

"""

output = """
{a(1,2), d, p(1), p(2), q(1), t}
{a(1,2), d, p(1), q(1), t, v(2)}
{a(1,2), d, p(1), t, v(1)}
{a(1,2), d, p(3), t}
{a(1,2), d, p(4)}
{a(1,2), f, p(1), p(2), q(1), t}
{a(1,2), f, p(1), q(1), t, v(2)}
{a(1,2), f, p(1), t, v(1)}
{a(1,2), f, p(3), t}
{a(1,2), f, p(4)}
"""
