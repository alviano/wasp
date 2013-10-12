input = """
b(1) v b(2).
b(1) v b(3).
b(1) v b(4).
x v y.
:- x, #count{Y:b(Y)} > 0, not #count{X:b(X)}>1.
:- #count{Y:b(Y)} > 0, not #count{X:b(X)}>1, y.
"""

output = """
{b(2), b(3), b(4), x}
{b(2), b(3), b(4), y}
"""
