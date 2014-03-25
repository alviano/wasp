input = """
1 2 0 0
1 3 0 0
1 4 0 0
1 5 0 0
1 6 0 0
1 7 0 0
1 8 0 0
1 9 0 0
1 10 1 1 11
1 12 1 1 13
1 11 1 1 10
1 13 1 1 12
1 1 1 1 10
1 1 1 1 12
1 1 1 1 10
1 1 1 1 12
0
2 xsucc(1,2)
10 filled(2,2)
12 filled(1,1)
3 ysucc(1,2)
11 unfilled(2,2)
13 unfilled(1,1)
6 xvalue(1,1)
7 xvalue(2,1)
4 bottle(1,1,1)
5 bottle(2,2,2)
8 yvalue(1,1)
9 yvalue(2,1)
0
B+
0
B-
1
0
1
"""
output = """
{xsucc(1,2), ysucc(1,2), bottle(1,1,1), bottle(2,2,2), xvalue(1,1), xvalue(2,1), yvalue(1,1), yvalue(2,1), filled(2,2), filled(1,1)}
"""
