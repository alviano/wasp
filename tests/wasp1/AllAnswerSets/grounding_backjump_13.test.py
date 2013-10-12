input = """
num(2).
node(a).
p(N) :- num(N), #count{Y:node(Y)} = N1, <=(N,N1).
"""

output = """
{node(a), num(2)}
"""
