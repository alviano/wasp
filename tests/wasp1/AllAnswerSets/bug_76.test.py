input = """
%% start file count.dl %%
#maxint = 5.
n(1).
n(2).
n(3).
a(X) :- #int(X), X > 0, X < #count{Z : n(Z) }.
b(X) :- #int(X), X < #count{Z : n(Z) }.
c(X1) :- #int(X), X1 < #count{Z : n(Z) }, +(X, 1, X1).
%% end   %%

"""

output = """
{a(1), a(2), b(0), b(1), b(2), c(1), c(2), n(1), n(2), n(3)}
"""
