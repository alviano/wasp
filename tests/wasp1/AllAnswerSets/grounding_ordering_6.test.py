input = """
#maxint=0.
f(Z) :-  Z=X+Y, #int(X), #int(Y).
"""

output = """
{f(0)}
"""
