input = """
% Bug:
% line 4: Aggregate conjunction is not safe.
% Aborting due to parser errors.
% This happened because of a faulty binding check
% of the TERMS occurring inside the aggregate
% w.r.t. literals q, r, and s.

:- q(X), not r(X,W), s(W),#count{Y: p(Y,Z), Z >= X} < 1.
"""

output = """
{}
"""
