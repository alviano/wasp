input = """
% (x1 OR NOT x2 OR x3) AND (x2 OR NOT x3 OR x4) AND (NOT x2 OR NOT x3 OR NOT x4)

x1 :- not x2_, not x3.
x2_ :- not x1, not x3.
%x3 :- not x1, not x2_.

x2 :- not x3_, not x4.
%x3_ :- not x2, not x4.
x4 :- not x2, not x3_.

x2_ :- not x3_, not x4_.
%x3_ :- not x2_, not x4_.
x4_ :- not x2_, not x3_.

%ok :- x1, x1_, not ok.
%ok :- x2, x2_, not ok.
%ok :- x3, x3_, not ok.
%ok :- x4, x4_, not ok.
"""

output = """
{x1, x2, x4_}
{x1, x4, x4_}
{x2, x2_}
{x2_, x4}
"""
