input = """
c(1) v c(2).
p(4).
a(1).
ouch(X) :- c(X),a(Z), 1<#count{Y:p(Y)}.
"""

output = """
{a(1), c(1), p(4)}
{a(1), c(2), p(4)}
"""
