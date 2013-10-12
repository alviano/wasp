input = """
% This shows the necessity to invoke the Model Checker even for
% non-disjunctive input in the case of positive PTs.

a :- not b.
c :- a.
a :- c.
b :- c.
"""

output = """
"""
