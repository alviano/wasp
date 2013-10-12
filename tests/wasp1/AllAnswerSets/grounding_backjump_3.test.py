input = """
%This is an example where we can not eliminate the duplicates of the rules.

#maxint=8.
 
p(X) v r(X) :- q(X,Y), q(A,B), A=X+1, #succ(Y,B).

q(1,3).
q(2,4).
q(2,7).
q(3,8).
q(3,5).
q(1,6)."""

output = """
{p(1), p(2), q(1,3), q(1,6), q(2,4), q(2,7), q(3,5), q(3,8)}
{p(1), q(1,3), q(1,6), q(2,4), q(2,7), q(3,5), q(3,8), r(2)}
{p(2), q(1,3), q(1,6), q(2,4), q(2,7), q(3,5), q(3,8), r(1)}
{q(1,3), q(1,6), q(2,4), q(2,7), q(3,5), q(3,8), r(1), r(2)}
"""
