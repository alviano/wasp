input = """
%%%%%%%%%%%%%%%%%%%%%%%
% The inference from constraints derives:
% MustBeTrue(#count)
% False(a).
% 
% False(a) -> True(b(1)).
%
% At this point, since MustBeTrue(#count), 
% performing a new propagation 
% False(b(2)) is derived,
% hence True(c).
%%%%%%%%%%%%%%%%%%%%%%%


:- #count {X : b(X)} > 1, not #count{ Y:b(Y)} > 0.
b(2) v c.
 
b(1) :- not a.
a    :- not b(1).
 
:- a.
"""

output = """
{b(1), b(2)}
{b(1), c}
"""
