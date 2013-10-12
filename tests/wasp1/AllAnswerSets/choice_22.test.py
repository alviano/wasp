input = """
% This will fail, if for some clever model generating strategy we
% rely on a/the wrong dependency graph.

c :- not a.
a v c :- not b.
c v b :- not a.
"""

output = """
{a}
{c}
"""
