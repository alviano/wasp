input = """
% Inferences from constraints.

b v a.

x :- a.
y :- x.

:- a, y.
"""

output = """
{b}
"""
