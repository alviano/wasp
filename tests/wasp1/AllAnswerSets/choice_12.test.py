input = """
% Most of this example is due to Francesco Scarcello.
% It shows the necessity to check whether a partial interpretation, which is
% about to be totalised, still contains "not false" (or "must be true")
% atoms. If it does, this represents an inconsistency.
 
a :- not g, not h.
b :- not g, not h.
c :- not g.
c :- a,b.
h :- a,b.
g :- a,b.
a :- c, not b.
b :- c, not a.
d v e.
f :- d.
h v g.
e :- not a, not b,d.
c v h :- g, not f.
"""

output = """
{a, c, d, f, h}
{a, c, e, g}
{a, c, e, h}
{b, c, d, f, h}
{b, c, e, g}
{b, c, e, h}
"""
