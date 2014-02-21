input = """
1 2 1 1 3
1 3 1 1 2
1 4 1 1 5
1 6 1 1 7
1 5 1 1 4
1 7 1 1 6
1 8 1 0 5
1 9 2 0 8 2
1 8 2 0 9 7
1 1 2 0 5 7
1 1 1 1 8
1 1 1 1 9
0
8 reached(2)
9 reached(4)
3 out_hm(2,4)
4 out_hm(3,2)
6 out_hm(4,2)
2 in_hm(2,4)
5 in_hm(3,2)
7 in_hm(4,2)
0
B+
0
B-
1
0
1
"""
output = """
{in_hm(2,4), in_hm(3,2), out_hm(4,2), reached(2), reached(4)}
"""
