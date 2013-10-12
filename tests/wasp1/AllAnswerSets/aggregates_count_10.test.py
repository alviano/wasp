input = """

% No more than bound1() Instances of a(X) should be true.
:- bound1(N), #count{V : a(V)} > N.
% No more than bound2() Instances of a(X) should be true.
:- bound2(N), #count{V : a(V)} > N.

% Exactly 2 Instances of a(X) are true.
a(first).
a(second).

% There should be no more than 2 Instances of a(X).
bound1(2).
% There should be no more than 3 Instances of a(X).
bound2(3).

"""

output = """
{a(first), a(second), bound1(2), bound2(3)}
"""
