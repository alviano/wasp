input = """
a(1) v a(2).     

% Count up the IDB predicates during the propagation.
b(X) :- a(X).

% If the count value is correct, the 'ok' predicate should be true.
ok :- #count{V : b(V)} > 0.
"""

output = """
{a(1), b(1), ok}
{a(2), b(2), ok}
"""
