input = """
% Bug in rewrinting. Count is eliminated as it is considered isolated.
% Instead it must be considered as a false literal as the aggregate is
% always violated, and bug shouldn't be true. 

c(1).
b(1).

bug :- c(Y), Y < #count{ V:b(V) }.
"""

output = """
{b(1), c(1)}
"""
