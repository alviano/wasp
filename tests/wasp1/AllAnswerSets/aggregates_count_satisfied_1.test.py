input = """
c(1) v c(2).
a(X) :- c(X).
p(4).
okay(X) :- p(Y), c(X), #count{V : a(V)} <Y. 
"""

output = """
{a(1), c(1), okay(1), p(4)}
{a(2), c(2), okay(2), p(4)}
"""
