input = """
a(X,Y) v b(X,Y) :- c(X,R,S), d(S,Y).

c(z,z,a).
c(z,r,a).
c(z,s,a).

d(a,y).

"""

output = """
{a(z,y), c(z,r,a), c(z,s,a), c(z,z,a), d(a,y)}
{b(z,y), c(z,r,a), c(z,s,a), c(z,z,a), d(a,y)}
"""
