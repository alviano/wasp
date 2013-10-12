input = """
% Bug.
% line 1: Constraint is not safe.
% Aborting due to parser errors.

:- p(3), X==5.
:- p(3), X=5.

"""

output = """
{}
"""
