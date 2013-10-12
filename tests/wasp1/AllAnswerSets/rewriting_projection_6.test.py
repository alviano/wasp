input = """
% If the IDB is ground, rewrite Constraints nevertheless.

f(1,2).
g(2,3).
h(3,4).

%x(C) :- f(A,B), g(B,C), h(C,D).

:- f(A,B), g(B,C), h(C,D), true.

true v false.
"""

output = """
{f(1,2), false, g(2,3), h(3,4)}
"""
