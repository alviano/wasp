input = """
1 2 2 1 3 4
1 3 2 1 2 4
1 4 0 0
1 5 1 0 3
1 6 1 0 2
2 8 2 0 1 6 5
1 7 1 0 8
0
7 ok
5 b(2)
6 b(1)
2 a(1)
3 a(2)
0
B+
0
B-
1
0
1
"""
output = """
{a(1), b(1), ok}
{a(2), b(2), ok}
"""