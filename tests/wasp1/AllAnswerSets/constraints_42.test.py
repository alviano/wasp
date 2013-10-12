input = """
% Inferences from constraints.

b v a.

x :- a.
y :- x.

:- not a, y.
"""

output = """
{a, x, y}
{b}
"""
