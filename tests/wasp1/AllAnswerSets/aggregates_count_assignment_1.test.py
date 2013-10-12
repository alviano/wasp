input = """
% Atom bug shouldn't be derived, as the body of the rule
% should be false. Auxiliary atoms shouldn't be printed
% out, as they are censored. 

d(1).
d(2).
d(3).


bug :- 1 < #count{V : d(V)} <= 2.
"""

output = """
{d(1), d(2), d(3)}
"""
