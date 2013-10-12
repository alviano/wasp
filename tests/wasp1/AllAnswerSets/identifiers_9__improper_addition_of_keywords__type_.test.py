input = """
% This broke with a proposed patch for an extension (ODBC, I believe)
% which tried to add "type" as a keyword.  Make sure to catch such attempts
% in the future.

type :- a.
a v b.  
"""

output = """
{a, type}
{b}
"""
