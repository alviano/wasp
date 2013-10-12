input = """
f(X3) :- d(X1,X2,X3,X4,X5,X6,X7,X8,X9,X10,X11,_),

u(X1),u(X2),u(X3),u(X4),u(X5),u(X6),u(X7),u(X8),u(X9),u(X10),u(X11).

g(X3) :- d(_,_,X3,X4,X5,X6,X7,X8,X9,X10,X11,X12),
         u(X3),u(X4),u(X5),u(X6),u(X7),u(X8),u(X9),u(X10),u(X11),u(X12).

d(1,2,3,4,5,6,7,8,9,10,11,12) :- true.
u(1) :- true.

true v false.

"""

output = """
{d(1,2,3,4,5,6,7,8,9,10,11,12), true, u(1)}
{false}
"""
