input = """
d(1).

okay(Y) :- Y = #count{V : d(V)}. 
"""

output = """
{d(1), okay(1)}
"""
