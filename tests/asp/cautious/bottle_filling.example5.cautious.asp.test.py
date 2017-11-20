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
2 14 2 0 2 12 10
1 1 1 1 14
1 1 2 0 12 11
0
2 xsucc(1,2)
10 filled(1,2)
12 filled(1,1)
3 ysucc(1,2)
11 unfilled(1,2)
13 unfilled(1,1)
6 xvalue(1,1)
7 xvalue(2,1)
4 bottle(1,1,1)
5 bottle(1,1,2)
8 yvalue(1,2)
9 yvalue(2,0)
0
B+
0
B-
1
0
1
"""
output = """
{xsucc(1,2), ysucc(1,2), bottle(1,1,1), bottle(1,1,2), xvalue(1,1), xvalue(2,1), yvalue(1,2), yvalue(2,0), filled(1,2), filled(1,1)}
"""
