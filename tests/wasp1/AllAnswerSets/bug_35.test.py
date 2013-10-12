input = """
% Date: Tue, 4 Aug 1998 15:32:14 +0200 (CEST)
% From: Christoph Koch (pfeifer) <koch@dbai.tuwien.ac.at>
% To: pfeifer@caph.dbai.tuwien.ac.at

% Assertion in rule.h

nop(huhu).

a(X) v b(X) :- nop(X).

a(X) :- b(X).
b(X) :- c(X).
c(X) :- a(X).

true.
c(huhu) :- true.
"""

output = """
{a(huhu), b(huhu), c(huhu), nop(huhu), true}
"""
