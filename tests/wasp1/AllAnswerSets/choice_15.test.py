input = """
% Enforce that <MUST_BE_TRUE,a> is in the queue before <TRUE,a>
:- x.
:- y.

x :- not a.
a :- not y.

% A rule with w and y in the head, such that they are not optimised away.
w v y v z.

% If <MUST_BE_TRUE,a> is in the queue before <TRUE,a>, the undefPosBody counter
% in this constraint should be decremented exactly once.
% If it is decremented twice, a bogus inconsistency is detected.
:- a, w.

"""

output = """
{a, z}
"""
