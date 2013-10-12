input = """
%  Bug:
%  aux#_0~11_0~12_0_1, first used with arity 2, now seen with arity 1.

:- p(X), #count{Y:a(Y),b(Y,X)} > 5.

:- #count{Y:a(Y),b(Y,X)} > 1.
"""

output = """
{}
"""
