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
{}
{ex5}
{ex5, b, a, f, e, d, c, ex1}
{ex1, a, f, e, d, c, b, ex5, ex2}
{ex2, b, a, f, e, d, c, ex1, ex5, ex3}
{ex2, b, a, f, e, d, c, ex1, ex5, ex4, ex3}
{ex3, c, b, a, f, e, d, ex2, ex1, ex4}
{ex4, d, c, b, a, f, e, ex3, ex2}
{ex4, d, c, b, a, f, e, ex3}
{ex4, d, c, b, a, f, e, ex3, ex6}
{ex4, d, c, b, a, f, e, ex6}
{ex6, d, c, e, b, f, a, ex3}
{ex3, c, b, a, f, e, d, ex6, ex5}
{d, c, b, a, f, e, ex3, ex6, ex5, ex4}
{ex5, d, c, b, a, f, e, ex3, ex6, ex1, ex4}
{c, b, a, f, e, d, ex5, ex3, ex6, ex1}
{ex1, a, f, e, d, c, b, ex5, ex3, ex6, ex2}
{b, a, f, e, d, c, ex1, ex5, ex3, ex6, ex2, ex4}
{ex5, ex6, f, e, d, c, b, a, ex1, ex2, ex4}
{a, f, e, d, c, b, ex5, ex6, ex1, ex2}
{a, f, e, d, c, b, ex5, ex6, ex1}
{a, f, e, d, c, b, ex5, ex6}
{f, e, d, c, b, a, ex5, ex6, ex2}
{ex5, f, e, d, c, b, a, ex2}
{ex5, f, e, a, d, c, b, ex4, ex2}
{e, d, c, b, a, f, ex5, ex4}
{e, d, c, b, a, f, ex5, ex4, ex1}
{ex4, d, c, b, a, f, e, ex5, ex3, ex1}
{ex1, a, f, e, d, c, b, ex4, ex3}
{ex1, a, f, e, d, c, b, ex4}
{ex1, a, f, e, d, c, b, ex4, ex6}
{ex1, a, f, e, d, c, b, ex4, ex2, ex6}
{ex6, ex1, a, f, e, d, c, b, ex2}
{ex2, b, a, f, e, d, c, ex6}
{f, e, d, c, b, a, ex2, ex6, ex4}
{f, e, d, c, b, a, ex2, ex6, ex3, ex4}
{e, d, c, b, a, f, ex2, ex6, ex3}
{e, d, c, b, a, f, ex2, ex3}
{b, a, f, e, d, c, ex2}
{b, a, f, e, d, c, ex2, ex4}
{b, a, f, e, d, c, ex2, ex1, ex4}
{b, a, f, e, d, c, ex1, ex2}
{b, a, f, e, d, c, ex1, ex2, ex3}
{b, a, f, e, d, c, ex1, ex3}
{b, a, f, e, d, c, ex1}
{b, a, c, f, e, d, ex3}
{b, ex4, a, d, c, f, e}
{ex5, e, d, f, ex3, c, a, b, ex4}
{ex5, e, d, f, ex3, c, a, b}
{ex5, b, a, f, e, d, c, ex4, ex2, ex1}
{b, ex5, a, f, e, d, c, ex2, ex3, ex4}
{b, ex5, a, f, e, d, c, ex2, ex3}
{b, ex5, ex1, a, f, ex3, c, e, d}
{ex6, ex1, a, f, e, d, c, b, ex5, ex4}
{ex6, ex1, a, f, e, d, c, b, ex4, ex3}
{ex6, ex1, a, f, e, d, c, b, ex4, ex3, ex2}
{ex6, ex1, a, f, e, d, c, b, ex2, ex3}
{ex6, ex1, a, f, e, d, c, b, ex3}
{ex6, ex1, a, f, e, d, c, b}
{ex6}
{ex6, ex5, f, e, d, c, b, a, ex4}
{ex6, ex5, f, e, d, c, b, a, ex2, ex4}
{ex6, ex5, f, e, d, c, b, a, ex2, ex4, ex3}
{ex6, ex5, f, e, d, c, b, a, ex2, ex3}
"""
