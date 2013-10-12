input = """
c(2).
d(1,2).
e(2,1).
okay1(X):- c(X), #count{V:d(V,X),e(X,Y)} = 1.
okay2(X):- c(X), #count{V:e(X,Y), d(V,X)} = 1.
:- #count{V:d(V,X), e(V,Y)} > 1.
:- #count{V:e(V,Y), d(V,X)} > 2.
:- #count{V:d(V,a), e(V,b)} > 1.
:- #count{V:e(V,b), d(V,a)} > 2.
"""

output = """
{c(2), d(1,2), e(2,1), okay1(2), okay2(2)}
"""
