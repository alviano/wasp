input = """
a(1) v a(2).

a(3) v a(4).

ok :- #count{V : a(V)} >=1, not #count{T : a(T)} >1.
"""

output = """
{a(1), a(3)}
{a(1), a(4)}
{a(2), a(3)}
{a(2), a(4)}
"""
