input = """
true.

g(0) :- true.
i(0) :- true.

:- i(X), g(X).

"""

output = """
"""
