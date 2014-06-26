input = """
3 6 2 3 4 5 6 7 0 0
1 8 1 0 2
1 9 2 0 6 7
1 10 2 0 6 7
1 11 1 0 5
1 12 1 0 4
1 13 1 0 3
1 8 1 0 13
1 13 1 0 12
1 12 1 0 11
1 11 1 0 10
1 10 1 0 9
1 9 1 0 8
0
2 ex1
10 e
5 ex4
11 d
6 ex5
8 a
7 ex6
12 c
3 ex2
13 b
4 ex3
9 f
0
B+
0
B-
1
0
1
"""
output = """
{ex5}
{ex6}
{}
{ex1, ex2, ex3, ex4, a, f, e, d, c, b}
{ex1, ex2, ex3, a, f, e, d, c, b}
{ex1, ex2, a, f, e, d, c, b}
{ex1, ex2, ex4, a, f, e, d, c, b}
{ex1, a, f, e, d, c, b}
{ex1, ex3, a, f, e, d, c, b}
{ex1, ex4, a, f, e, d, c, b}
{ex1, ex3, ex4, a, f, e, d, c, b}
{ex2, a, f, e, d, c, b}
{ex4, a, f, e, d, c, b}
{ex2, ex4, a, f, e, d, c, b}
{ex3, a, f, e, d, c, b}
{ex3, ex4, a, f, e, d, c, b}
{ex2, ex3, a, f, e, d, c, b}
{ex2, ex3, ex4, a, f, e, d, c, b}
{ex1, ex6, a, f, e, d, c, b}
{ex1, ex3, ex6, a, f, e, d, c, b}
{ex3, ex6, a, f, e, d, c, b}
{ex1, ex4, ex6, a, f, e, d, c, b}
{ex1, ex3, ex4, ex6, a, f, e, d, c, b}
{ex4, ex6, a, f, e, d, c, b}
{ex3, ex4, ex6, a, f, e, d, c, b}
{ex1, ex2, ex6, a, f, e, d, c, b}
{ex1, ex2, ex4, ex6, a, f, e, d, c, b}
{ex1, ex2, ex3, ex6, a, f, e, d, c, b}
{ex1, ex2, ex3, ex4, ex6, a, f, e, d, c, b}
{ex2, ex6, a, f, e, d, c, b}
{ex2, ex3, ex6, a, f, e, d, c, b}
{ex2, ex4, ex6, a, f, e, d, c, b}
{ex2, ex3, ex4, ex6, a, f, e, d, c, b}
{ex1, ex5, a, f, e, d, c, b}
{ex1, ex4, ex5, a, f, e, d, c, b}
{ex1, ex5, ex6, a, f, e, d, c, b}
{ex1, ex4, ex5, ex6, a, f, e, d, c, b}
{ex1, ex2, ex5, a, f, e, d, c, b}
{ex1, ex2, ex4, ex5, a, f, e, d, c, b}
{ex1, ex2, ex5, ex6, a, f, e, d, c, b}
{ex1, ex2, ex4, ex5, ex6, a, f, e, d, c, b}
{ex4, ex5, a, f, e, d, c, b}
{ex2, ex5, a, f, e, d, c, b}
{ex2, ex4, ex5, a, f, e, d, c, b}
{ex5, ex6, a, f, e, d, c, b}
{ex2, ex5, ex6, a, f, e, d, c, b}
{ex4, ex5, ex6, a, f, e, d, c, b}
{ex2, ex4, ex5, ex6, a, f, e, d, c, b}
{ex1, ex3, ex5, a, f, e, d, c, b}
{ex1, ex3, ex4, ex5, a, f, e, d, c, b}
{ex3, ex5, a, f, e, d, c, b}
{ex3, ex4, ex5, a, f, e, d, c, b}
{ex1, ex3, ex5, ex6, a, f, e, d, c, b}
{ex1, ex3, ex4, ex5, ex6, a, f, e, d, c, b}
{ex3, ex5, ex6, a, f, e, d, c, b}
{ex3, ex4, ex5, ex6, a, f, e, d, c, b}
{ex1, ex2, ex3, ex5, a, f, e, d, c, b}
{ex1, ex2, ex3, ex4, ex5, a, f, e, d, c, b}
{ex1, ex2, ex3, ex5, ex6, a, f, e, d, c, b}
{ex1, ex2, ex3, ex4, ex5, ex6, a, f, e, d, c, b}
{ex2, ex3, ex5, a, f, e, d, c, b}
{ex2, ex3, ex4, ex5, a, f, e, d, c, b}
{ex2, ex3, ex5, ex6, a, f, e, d, c, b}
{ex2, ex3, ex4, ex5, ex6, a, f, e, d, c, b}
"""
