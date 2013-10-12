input = """
n(X1) :- n(X), succ(X,X1).
succ(0,1).
"""

output = """
{succ(0,1)}
"""
