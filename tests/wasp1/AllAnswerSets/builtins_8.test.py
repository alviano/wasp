input = """
% We should not warn or fail for builtins with inappropriate parameters,
% just not ground them.

#maxint=1.

true :- #int(1).
false :- #int(c).
"""

output = """
{true}
"""
