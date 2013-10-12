input = """
reach(X,Y) :- arc(X,Y).
reach(X,Y) :- arc(X,Z), reach(Z,Y).
"""

output = """
{}
"""
