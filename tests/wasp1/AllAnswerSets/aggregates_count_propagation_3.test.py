input = """
a(1) v a(2).

a(3) v a(4).

ok :- not #count{T: a(T)} > 2, #count{V : a(V)} > 1.
"""

output = """
{a(1), a(3), ok}
{a(1), a(4), ok}
{a(2), a(3), ok}
{a(2), a(4), ok}
"""
