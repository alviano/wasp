input = """
colored(2,g) :- not diff_col(2,g).
colored(2,y) :- not diff_col(2,y).
colored(3,g) :- not diff_col(3,g).
colored(3,y) :- not diff_col(3,y).

diff_col(2,g) :- colored(2,y).
diff_col(3,g) :- colored(3,y).
diff_col(2,y) :- colored(2,g).
diff_col(3,y) :- colored(3,g).

no_stable :- colored(2,2), colored(3,2), not no_stable.
no_stable :- colored(2,3), colored(3,3), not no_stable.
no_stable :- colored(2,g), colored(3,g), not no_stable.
no_stable :- colored(2,y), colored(3,y), not no_stable.
"""

output = """
{colored(2,g), colored(3,y), diff_col(2,y), diff_col(3,g)}
{colored(2,y), colored(3,g), diff_col(2,g), diff_col(3,y)}
"""
