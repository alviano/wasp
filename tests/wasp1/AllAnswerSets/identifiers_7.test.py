input = """
% This would generate =strange= models with versions before 1999-06-09.

true.
b("") :- true.
a("") :- true.

c(X) :- b(X).
"""

output = """
{a(""), b(""), c(""), true}
"""
