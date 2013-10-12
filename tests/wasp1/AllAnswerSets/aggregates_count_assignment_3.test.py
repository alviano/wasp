input = """
% This rule is safe as X is 
% saved by an aggregate with assignment.
ok(X) :- #count{V:b(V)} = X,  not p(X).

b(1).
p(2).
"""

output = """
{b(1), ok(1), p(2)}
"""
