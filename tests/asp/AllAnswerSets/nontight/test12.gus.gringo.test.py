input = """
1 5 1 1 3
1 5 2 1 2 9
1 5 1 0 8
1 8 1 0 5
1 6 1 0 7
1 7 1 0 6
1 7 2 2 3 2
1 9 2 1 2 5
3 3 2 3 4 0 0
0
2 a
3 b
5 d
6 e
7 f
8 g
9 h
0
B+
0
B-
1
0
1
"""
output = """
{b}
{b}
{a, b}
{g, d, a}
{d, g, e, f, h}
{e, f, d, g, h}
{a, d, g}
{a, b}
"""