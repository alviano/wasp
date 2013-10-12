input = """
% Test describing a bug in grounding.C
% An aggregate should be considered an
% assignment if isAssignment() holds and
% guards are unbound variables.
% Atom bug is wrongly derived true,
% as the second condition is not checked
% during grounding. 

a(1).
b(1,2).
b(2,2).
c(1).

bug(X,Y) :- c(Y), a(X), #count{V : b(V,2)} = Y.
"""

output = """
{a(1), b(1,2), b(2,2), c(1)}
"""
