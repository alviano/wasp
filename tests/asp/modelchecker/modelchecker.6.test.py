input = """
a:-b.
b:-a.
a | b.

:- not a.
:- not b.
"""
output = """
{a, b}
"""
