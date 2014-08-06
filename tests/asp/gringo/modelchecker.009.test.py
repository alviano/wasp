input = """
% In this example, if a or c are not derived as true after d has been derived,
% both of them remain mbt. This is a problem since mbt atoms are no PTs.
% The effect is that no PTs seem to be left, which is an error. No model is
% output since the mbt atoms (which are actually PTs) still exist.

% Works solely for the positive-only PT notion.

:- not a.
:- not c.
:- not d.
a :- not b.
c :- not b.
d | e.
a | c :- not e.
a | b :- c.
c | b :- a.
"""
output = """
{a, c, d}
"""
