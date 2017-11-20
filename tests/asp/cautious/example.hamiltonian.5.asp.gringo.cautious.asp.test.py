input = """
1 2 1 1 3
1 3 1 1 2
1 4 1 1 5
1 6 1 1 7
1 8 1 1 9
1 5 1 1 4
1 7 1 1 6
1 9 1 1 8
1 10 2 0 2 7
1 11 2 0 10 5
1 10 2 0 11 9
1 1 2 0 7 9
1 1 1 1 10
1 1 1 1 11
0
10 reached(2)
11 reached(4)
3 out_hm(1,3)
4 out_hm(2,4)
6 out_hm(3,2)
8 out_hm(4,2)
2 in_hm(1,3)
5 in_hm(2,4)
7 in_hm(3,2)
9 in_hm(4,2)
0
B+
0
B-
1
0
1
"""
output = """
{in_hm(2,4), reached(2), reached(4), out_hm(4,2), in_hm(3,2), in_hm(1,3)}
"""
