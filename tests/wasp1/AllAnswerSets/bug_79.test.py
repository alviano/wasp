input = """
a(2). a(3).
b(X) :- X = #times{ Y : a(Y) }.
c(X) :- b(Y), b(X), X < Y.

"""

output = """
{a(2), a(3), b(6)}
"""
