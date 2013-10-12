input = """
arc(a,b,2).
arc(a,c,3).
arc(b,d,3).
arc(c,d,2).
arc(d,a,2).

% maxint is deliberately set too large to detect wrong minima.

#maxint=20.
path(X,Y,C) :- arc(X,Y,C).
path(X,Y,C) :- arc(X,Z,C1), path(Z,Y,C2), C = C1 + C2.

minpath(X,Y,C) :- path(X,Y,C), C = #min{ C1: path(X,Y,C1) }.

"""

output = """
{arc(a,b,2), arc(a,c,3), arc(b,d,3), arc(c,d,2), arc(d,a,2), minpath(a,a,7), minpath(a,b,2), minpath(a,c,3), minpath(a,d,5), minpath(b,a,5), minpath(b,b,7), minpath(b,c,8), minpath(b,d,3), minpath(c,a,4), minpath(c,b,6), minpath(c,c,7), minpath(c,d,2), minpath(d,a,2), minpath(d,b,4), minpath(d,c,5), minpath(d,d,7), path(a,a,14), path(a,a,7), path(a,b,16), path(a,b,2), path(a,b,9), path(a,c,10), path(a,c,17), path(a,c,3), path(a,d,12), path(a,d,19), path(a,d,5), path(b,a,12), path(b,a,19), path(b,a,5), path(b,b,14), path(b,b,7), path(b,c,15), path(b,c,8), path(b,d,10), path(b,d,17), path(b,d,3), path(c,a,11), path(c,a,18), path(c,a,4), path(c,b,13), path(c,b,20), path(c,b,6), path(c,c,14), path(c,c,7), path(c,d,16), path(c,d,2), path(c,d,9), path(d,a,16), path(d,a,2), path(d,a,9), path(d,b,11), path(d,b,18), path(d,b,4), path(d,c,12), path(d,c,19), path(d,c,5), path(d,d,14), path(d,d,7)}
"""
