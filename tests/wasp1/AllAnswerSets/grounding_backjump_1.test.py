input = """
% by this example we can check
% the correctness of the jumps-arrays;
% the correctness of the solvedPredicates (in fact 
% we can jump the literal t(F,C) but not t(F,D) because
% D appear in the head). 
p(A) v p(D) :- q(F,A), r(A,C), t(F,D),t(F,C), s(C,D).

r(1,2).
r(1,3).
r(2,3).
s(2,7).
s(3,4).
s(3,5).
q(5,2).
q(3,1).
t(5,4).
t(5,3).
q(6,2).
t(6,4).




"""

output = """
{p(2), q(3,1), q(5,2), q(6,2), r(1,2), r(1,3), r(2,3), s(2,7), s(3,4), s(3,5), t(5,3), t(5,4), t(6,4)}
{p(4), q(3,1), q(5,2), q(6,2), r(1,2), r(1,3), r(2,3), s(2,7), s(3,4), s(3,5), t(5,3), t(5,4), t(6,4)}
"""
