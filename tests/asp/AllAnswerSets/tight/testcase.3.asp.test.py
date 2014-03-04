input = """
1 2 2 1 3 4
1 3 2 1 2 4
1 4 0 0
1 5 2 1 6 7
1 6 2 1 5 7
1 7 0 0
1 8 2 0 2 5
1 9 2 1 10 11
1 10 2 1 9 11
1 11 0 0
1 12 2 1 13 14
1 13 2 1 12 14
1 14 0 0
1 15 2 0 9 12
1 16 2 1 17 18
1 17 2 1 16 18
1 18 0 0
1 19 2 1 20 21
1 20 2 1 19 21
1 21 0 0
1 22 2 0 16 19
0
22 c
9 b1
3 n_a1
15 b
6 n_a2
8 a
10 n_b1
5 a2
16 c1
17 n_c1
2 a1
19 c2
13 n_b2
20 n_c2
12 b2
0
B+
0
B-
1
0
1
"""
output = """
{n_a1, n_a2, n_b1, n_b2, n_c1, n_c2}
{n_a1, n_a2, n_b1, n_b2, n_c1, c2}
{n_a1, n_a2, n_b1, n_b2, c1, c2, c}
{n_a1, n_a2, n_b1, n_b2, c1, n_c2}
{n_a1, n_a2, n_b1, b2, c1, c2, c}
{n_a1, n_a2, n_b1, b2, n_c1, c2}
{n_a1, n_a2, n_b1, b2, c1, n_c2}
{n_a1, n_a2, n_b1, b2, n_c1, n_c2}
{n_a1, n_a2, b1, b2, b, c1, c2, c}
{n_a1, n_a2, b1, n_b2, c1, c2, c}
{n_a1, n_a2, b1, b2, b, c1, n_c2}
{n_a1, n_a2, b1, n_b2, c1, n_c2}
{n_a1, n_a2, b1, b2, b, n_c1, c2}
{n_a1, n_a2, b1, b2, b, n_c1, n_c2}
{n_a1, n_a2, b1, n_b2, n_c1, c2}
{n_a1, n_a2, b1, n_b2, n_c1, n_c2}
{n_a1, a2, b1, b2, b, c1, c2, c}
{n_a1, a2, n_b1, b2, c1, c2, c}
{n_a1, a2, b1, b2, b, n_c1, c2}
{n_a1, a2, n_b1, b2, n_c1, c2}
{n_a1, a2, b1, n_b2, c1, c2, c}
{n_a1, a2, b1, n_b2, n_c1, c2}
{n_a1, a2, n_b1, n_b2, c1, c2, c}
{n_a1, a2, n_b1, n_b2, n_c1, c2}
{n_a1, a2, b1, b2, b, c1, n_c2}
{n_a1, a2, b1, n_b2, c1, n_c2}
{n_a1, a2, n_b1, b2, c1, n_c2}
{n_a1, a2, n_b1, n_b2, c1, n_c2}
{n_a1, a2, b1, b2, b, n_c1, n_c2}
{n_a1, a2, n_b1, b2, n_c1, n_c2}
{n_a1, a2, b1, n_b2, n_c1, n_c2}
{n_a1, a2, n_b1, n_b2, n_c1, n_c2}
{a1, a2, a, b1, b2, b, c1, c2, c}
{a1, n_a2, b1, b2, b, c1, c2, c}
{a1, a2, a, b1, b2, b, c1, n_c2}
{a1, n_a2, b1, b2, b, c1, n_c2}
{a1, a2, a, b1, b2, b, n_c1, c2}
{a1, a2, a, b1, b2, b, n_c1, n_c2}
{a1, n_a2, b1, b2, b, n_c1, c2}
{a1, n_a2, b1, b2, b, n_c1, n_c2}
{a1, a2, a, b1, n_b2, c1, c2, c}
{a1, a2, a, b1, n_b2, n_c1, c2}
{a1, n_a2, b1, n_b2, c1, c2, c}
{a1, n_a2, b1, n_b2, n_c1, c2}
{a1, a2, a, b1, n_b2, c1, n_c2}
{a1, n_a2, b1, n_b2, c1, n_c2}
{a1, a2, a, b1, n_b2, n_c1, n_c2}
{a1, n_a2, b1, n_b2, n_c1, n_c2}
{a1, a2, a, n_b1, b2, c1, c2, c}
{a1, a2, a, n_b1, n_b2, c1, c2, c}
{a1, a2, a, n_b1, b2, c1, n_c2}
{a1, a2, a, n_b1, n_b2, c1, n_c2}
{a1, a2, a, n_b1, b2, n_c1, c2}
{a1, a2, a, n_b1, b2, n_c1, n_c2}
{a1, a2, a, n_b1, n_b2, n_c1, c2}
{a1, a2, a, n_b1, n_b2, n_c1, n_c2}
{a1, n_a2, n_b1, b2, c1, c2, c}
{a1, n_a2, n_b1, b2, n_c1, c2}
{a1, n_a2, n_b1, n_b2, c1, c2, c}
{a1, n_a2, n_b1, n_b2, n_c1, c2}
{a1, n_a2, n_b1, b2, c1, n_c2}
{a1, n_a2, n_b1, n_b2, c1, n_c2}
{a1, n_a2, n_b1, b2, n_c1, n_c2}
{a1, n_a2, n_b1, n_b2, n_c1, n_c2}
"""
