input = """
% An example for which a wrong version of the quick model check caused
% answer sets to be missed, resulting in no answer set.
% Slightly modified version of checker.16.

:- not p14.
:- not p24.
:- p11, p15.
p14 | p2 :- not p5, p24. 
p1 | p15 | not_p18 :- not p5, p24. 
p24 | p8 :- p24, p2, not p25. 
p21 | p5 | p11 | p25 :- p14. 
p15 | p14 | p9. 
p11 | p24. 
p2 | p15. 
p2 | p24. 
"""

output = """
{not_p18, p14, p2, p21, p24}
{not_p18, p14, p2, p24, p25}
{p1, p14, p2, p21, p24}
{p1, p14, p2, p24, p25}
{p14, p15, p21, p24}
{p14, p15, p24, p25}
{p14, p2, p24, p5}
"""
