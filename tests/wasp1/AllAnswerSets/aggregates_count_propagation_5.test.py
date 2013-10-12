input = """
% Perform a backward propagation upon #count{V : a(V,2)} =1.
% The aggregate predicate should be F, so the undefined aux predicate
% inside it should be derived as F in order to violate the aggregate.

a(1,1) v a(1,2).

:- #count{V : a(V,2)} >=1, not #count{T: a(T,2)} > 1.

bug(X,2) :- a(X,2).
"""

output = """
{a(1,1)}
"""
