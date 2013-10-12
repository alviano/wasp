input = """
dummy.
c(0,0).

d :- c(_,a_long__but_still_nice_constant), dummy.
"""

output = """
{c(0,0), dummy}
"""
