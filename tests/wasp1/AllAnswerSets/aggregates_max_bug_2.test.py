input = """
% Only true auxiliary atoms.
a(0).
a(1).
a(2).
a(4).

ok1 :- #max{V:a(V)} = 4.

% Only a(0) true.
b(0) :- a(0).
ok2 :- #max{V:b(V)} = 0. 
ouch :- #max{V:b(V)} = 1.
"""

output = """
{a(0), a(1), a(2), a(4), b(0), ok1, ok2}
"""
