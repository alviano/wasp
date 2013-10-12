input = """
a1:- not b1.
b1:- not a1.

a2:- not b2.
b2:- not a2.

a3:- not b3.
b3:- not a3.

"""

output = """
{a1, a2, a3}
{a1, a2, b3}
{a1, a3, b2}
{a1, b2, b3}
{a2, a3, b1}
{a2, b1, b3}
{a3, b1, b2}
{b1, b2, b3}
"""
