input = """
% Similar to choice.12.
% Triggers a bug if we have positive possibly true atoms and no final
% totalised satisfied check for rules and constraints (nor anything
% equivalent).

% Using positive PTs, a, b and c are never PTs.

c :- a,b.
a :- c, not b.
b :- c, not a.

% Both d and e are positive PTs.
% If we take both (order doesn't matter), we get the correct model {d,e}.
% However, once we assume one of d and e false and keep the other one true,
% the rules are satisfied for the partial interpretations <{d},{a,b,c},{e}>
% and <{e},{a,b,c},{d}>, but not for the totalised ones!

d v e.
e :- not a, not b.
d :- not a, not b.

"""

output = """
{d, e}
"""
