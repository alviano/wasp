input = """
% This constraint is detected to be violated after it is realized
% that the ground idb is empty.

% Handled in dl.C, unlike bug.34, where this is done by depgraph.h.

d :- e.

:- not d.

"""

output = """
"""
