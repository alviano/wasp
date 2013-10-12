input = """

a(X,Y) :- c(X,R,S), d(S,T,Y), not na(X,Y).
na(X,Y) :- c(X,R,S), d(S,T,Y), not a(X,Y).

c(z,r,a).
c(z,s,a).

d(a,r,y).
d(a,s,y).

"""

output = """
{a(z,y), c(z,r,a), c(z,s,a), d(a,r,y), d(a,s,y)}
{c(z,r,a), c(z,s,a), d(a,r,y), d(a,s,y), na(z,y)}
"""
