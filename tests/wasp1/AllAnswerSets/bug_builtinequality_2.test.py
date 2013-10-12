input = """
% Bug.
% line 2: Rule is not safe.
% Aborting due to parser errors.

a(5).
p(X) :- a(Y), X == 5.
p1(X) :- a(Y), X = 5.
"""

output = """
{a(5), p(5), p1(5)}
"""
