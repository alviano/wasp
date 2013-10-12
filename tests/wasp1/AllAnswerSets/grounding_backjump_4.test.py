input = """
% By this example, I found the bug on the negative literals!
 
h(X) v t(X) :- q(X,Y), q(A,B),  not p(X), not r(A), s(A,B).

q(1,2).
q(3,4).
p(2).
p(3).
r(1).
s(3,4).
s(1,2).
"""

output = """
{h(1), p(2), p(3), q(1,2), q(3,4), r(1), s(1,2), s(3,4)}
{p(2), p(3), q(1,2), q(3,4), r(1), s(1,2), s(3,4), t(1)}
"""
