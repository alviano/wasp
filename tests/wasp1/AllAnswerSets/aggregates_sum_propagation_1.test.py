input = """
a(1) v a(2).     

% Count up the IDB predicates during the propagation.
b(X) :- a(X).

% If the count value is correct, the 'ok' predicate should be true.
ok1 :- not #sum{V : b(V)} > 1, #sum{T: b(T)} > 0.
ok2 :- not #sum{V : b(V)} > 2, #sum{T: b(T)} > 1.
"""

output = """
{a(1), b(1), ok1}
{a(2), b(2), ok2}
"""
