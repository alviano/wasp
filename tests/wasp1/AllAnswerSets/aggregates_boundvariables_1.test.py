input = """
% Bug:
% Aggregate conjunction is not safe.
% Aborting due to parser errors.
% This happened because of a faulty binding check
% of q(X) w.r.t. literal p(Y,Z).

:- q(X), #count{Y: p(Y,Z), Z >= X} < 1.
"""

output = """
{}
"""
