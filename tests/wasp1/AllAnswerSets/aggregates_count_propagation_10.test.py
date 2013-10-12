input = """
a(1) v a(2).
b(1) v b(2).
okay :- not #count{X:a(X),b(X)}>1, #count{V:a(V),b(V)}>0.
"""

output = """
{a(1), b(1), okay}
{a(1), b(2)}
{a(2), b(1)}
{a(2), b(2), okay}
"""
