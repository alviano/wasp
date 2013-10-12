input = """
P(a). LongPredicate(a).
Q. QuasiPredicate.

TRUE :- P(a), LongPredicate(a), Q, QuasiPredicate.
"""

output = """
{LongPredicate(a), P(a), Q, QuasiPredicate, TRUE}
"""
