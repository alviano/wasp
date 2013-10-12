input = """
% Bug about safety of builtin equality:
% the builtin appearing in the rule is safe
% but an error is printed because of a faulty
% check of its binding w.r.t. literals b, and c.

c(1,1).
b(1).

:- X==5, #count{Y:c(X,Y),b(Y)} > 1.
:- X=5, #count{Y:c(X,Y),b(Y)} > 1.

"""

output = """
{b(1), c(1,1)}
"""
