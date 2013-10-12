input = """
% If the falsity of b is derived (after assuming the truth of either p or q),
% a loses a supporting rule the body of which consists only of atoms of the
% same component as a. Nevertheless, this component has become unfounded.

% This is a counterexample to the hypothesis that satisfaction of a rule
% consisting only of atoms of the same component cannot create an unfounded
% set.

b v x :- r.
a :- b.
b :- a,c.
a :- c.
c :- a.

p v q.
r :- p.
r :- q.
:- b,r.

"""

output = """
{p, r, x}
{q, r, x}
"""
