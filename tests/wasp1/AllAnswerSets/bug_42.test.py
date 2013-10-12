input = """
true.
a(1) :- true.
b(1) :- true.

gen1 :- not a(1).
gen2 :- not b(1).

%%%%%%%%%%%%%%%%%%%%%%

unrel v a(2).
gen3 :- unrel.

gen3 :- gen1.
a(2) :- not gen3.

gen4 :- gen2.
b(2) :- not gen4.

"""

output = """
{a(1), a(2), b(1), b(2), true}
{a(1), b(1), b(2), gen3, true, unrel}
"""
