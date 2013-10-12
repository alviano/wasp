input = """
c(1) v c(2) v c(3).
d(1) v d(2) v d(3). 
e(1) v e(2) v e(3).
a(X) :- c(X).

b(Y) :- d(Y).
okay :- #count{V :a(V)} > 0. 
okay1 :- #count{V: a(V)} > 0, not #count{T :a(T)} > 2.
okay2 :- #count{V :b(V)} > 0.
okay3 :- #count{V :e(V)} > 0.
"""

output = """
{a(1), b(1), c(1), d(1), e(1), okay, okay1, okay2, okay3}
{a(1), b(1), c(1), d(1), e(2), okay, okay1, okay2, okay3}
{a(1), b(1), c(1), d(1), e(3), okay, okay1, okay2, okay3}
{a(1), b(2), c(1), d(2), e(1), okay, okay1, okay2, okay3}
{a(1), b(2), c(1), d(2), e(2), okay, okay1, okay2, okay3}
{a(1), b(2), c(1), d(2), e(3), okay, okay1, okay2, okay3}
{a(1), b(3), c(1), d(3), e(1), okay, okay1, okay2, okay3}
{a(1), b(3), c(1), d(3), e(2), okay, okay1, okay2, okay3}
{a(1), b(3), c(1), d(3), e(3), okay, okay1, okay2, okay3}
{a(2), b(1), c(2), d(1), e(1), okay, okay1, okay2, okay3}
{a(2), b(1), c(2), d(1), e(2), okay, okay1, okay2, okay3}
{a(2), b(1), c(2), d(1), e(3), okay, okay1, okay2, okay3}
{a(2), b(2), c(2), d(2), e(1), okay, okay1, okay2, okay3}
{a(2), b(2), c(2), d(2), e(2), okay, okay1, okay2, okay3}
{a(2), b(2), c(2), d(2), e(3), okay, okay1, okay2, okay3}
{a(2), b(3), c(2), d(3), e(1), okay, okay1, okay2, okay3}
{a(2), b(3), c(2), d(3), e(2), okay, okay1, okay2, okay3}
{a(2), b(3), c(2), d(3), e(3), okay, okay1, okay2, okay3}
{a(3), b(1), c(3), d(1), e(1), okay, okay1, okay2, okay3}
{a(3), b(1), c(3), d(1), e(2), okay, okay1, okay2, okay3}
{a(3), b(1), c(3), d(1), e(3), okay, okay1, okay2, okay3}
{a(3), b(2), c(3), d(2), e(1), okay, okay1, okay2, okay3}
{a(3), b(2), c(3), d(2), e(2), okay, okay1, okay2, okay3}
{a(3), b(2), c(3), d(2), e(3), okay, okay1, okay2, okay3}
{a(3), b(3), c(3), d(3), e(1), okay, okay1, okay2, okay3}
{a(3), b(3), c(3), d(3), e(2), okay, okay1, okay2, okay3}
{a(3), b(3), c(3), d(3), e(3), okay, okay1, okay2, okay3}
"""
