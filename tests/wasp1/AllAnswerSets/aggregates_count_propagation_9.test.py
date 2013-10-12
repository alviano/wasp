input = """
c(1) v c(2).
d(1) v d(2).
okay1 :- #count{ V: c(V) } > 0.
okay2 :- #count{ V: d(V) } > 0.

"""

output = """
{c(1), d(1), okay1, okay2}
{c(1), d(2), okay1, okay2}
{c(2), d(1), okay1, okay2}
{c(2), d(2), okay1, okay2}
"""
