input = """
#maxint=2.
a(1).
b(1).
dummy :- a(A), b(B), +(A,B,C), #int(A).
"""

output = """
{a(1), b(1), dummy}
"""
