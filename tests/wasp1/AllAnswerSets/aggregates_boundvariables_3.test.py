input = """
% Error:
% line 5: parse error.
% Aborting due to parser errors.
% This happened because the bound builtin equality
% was not recognized as safe.

c(C) :- b(C,N), #sum{N1 : q(C,C1,N1)} >K, K==50.
c1(C) :- b(C,N), #sum{N1 : q(C,C1,N1)} >K, K=50.

"""

output = """
{}
"""
