input = """
a:-b.
b:-a.
a v b.

:- not a.
:- not b.
"""

output = """
{a, b}
"""
