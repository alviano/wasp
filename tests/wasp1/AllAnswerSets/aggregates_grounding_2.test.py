input = """
a(1) v a(2).
b(1).
:- b(N), #count{ V : a(V) } > N, not #count{ T: a(T) } > N1, N1=N-1.
"""

output = """
{a(1), b(1)}
{a(2), b(1)}
"""
