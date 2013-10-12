input = """
a(1) v a(2).
okay :- 0 <= #count{V:a(V)} <= 2. 
"""

output = """
{a(1), okay}
{a(2), okay}
"""
