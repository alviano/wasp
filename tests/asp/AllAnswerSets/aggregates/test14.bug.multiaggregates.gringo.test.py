input = """
8 2 2 3 0 0
8 2 4 5 0 0
2 6 4 0 3 5 4 3 2
2 7 4 0 2 5 4 3 2
1 8 2 1 6 7
0
8 ok
2 a(1)
3 a(2)
4 a(3)
5 a(4)
6 aggrGT3
7 aggrGT2
0
B+
0
B-
1
0
1
"""
output = """
{ok, aggrGT2, a(4), a(2)}
{ok, aggrGT2, a(4), a(1)}
{ok, aggrGT2, a(3), a(1)}
{ok, aggrGT2, a(3), a(2)}
"""
