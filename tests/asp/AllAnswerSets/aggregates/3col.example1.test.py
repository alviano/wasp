input = """
1 2 0 0
1 3 0 0
1 4 0 0
1 5 0 0
1 6 0 0
1 7 0 0
1 8 0 0
1 9 0 0
1 10 0 0
1 11 0 0
1 12 0 0
1 13 0 0
1 14 0 0
1 15 0 0
1 16 0 0
1 17 0 0
1 18 0 0
1 19 0 0
1 20 2 1 21 22
1 21 2 1 20 22
1 22 0 0
1 23 2 1 24 25
1 24 2 1 23 25
1 25 0 0
1 26 2 1 27 28
1 27 2 1 26 28
1 28 0 0
1 29 2 1 30 31
1 30 2 1 29 31
1 31 0 0
1 32 2 1 33 34
1 33 2 1 32 34
1 34 0 0
1 35 2 1 36 37
1 36 2 1 35 37
1 37 0 0
1 38 2 1 39 40
1 39 2 1 38 40
1 40 0 0
1 41 2 1 42 43
1 42 2 1 41 43
1 43 0 0
1 44 2 1 45 46
1 45 2 1 44 46
1 46 0 0
1 47 2 1 48 49
1 48 2 1 47 49
1 49 0 0
1 50 2 1 51 52
1 51 2 1 50 52
1 52 0 0
1 53 2 1 54 55
1 54 2 1 53 55
1 55 0 0
1 56 2 1 57 58
1 57 2 1 56 58
1 58 0 0
1 59 2 1 60 61
1 60 2 1 59 61
1 61 0 0
1 62 2 1 63 64
1 63 2 1 62 64
1 64 0 0
2 65 3 0 1 50 35 20
2 66 3 0 2 50 35 20
1 67 2 1 66 65
1 1 1 1 67
2 68 3 0 1 53 38 23
2 69 3 0 2 53 38 23
1 70 2 1 69 68
1 1 1 1 70
2 71 3 0 1 56 41 26
2 72 3 0 2 56 41 26
1 73 2 1 72 71
1 1 1 1 73
2 74 3 0 1 59 44 29
2 75 3 0 2 59 44 29
1 76 2 1 75 74
1 1 1 1 76
2 77 3 0 1 62 47 32
2 78 3 0 2 62 47 32
1 79 2 1 78 77
1 1 1 1 79
1 1 2 0 53 50
1 1 2 0 38 35
1 1 2 0 23 20
1 1 2 0 56 50
1 1 2 0 41 35
1 1 2 0 26 20
1 1 2 0 59 56
1 1 2 0 44 41
1 1 2 0 29 26
1 1 2 0 62 56
1 1 2 0 47 41
1 1 2 0 32 26
1 1 2 0 62 59
1 1 2 0 47 44
1 1 2 0 32 29
0
7 link(1,2)
8 link(2,1)
9 link(1,3)
10 link(3,1)
11 link(2,3)
12 link(3,2)
13 link(3,5)
14 link(5,3)
15 link(4,5)
16 link(5,4)
17 colour(red0)
18 colour(green0)
19 colour(blue0)
20 chosenColour(5,blue0)
23 chosenColour(4,blue0)
26 chosenColour(3,blue0)
29 chosenColour(2,blue0)
32 chosenColour(1,blue0)
35 chosenColour(5,green0)
38 chosenColour(4,green0)
41 chosenColour(3,green0)
44 chosenColour(2,green0)
47 chosenColour(1,green0)
50 chosenColour(5,red0)
53 chosenColour(4,red0)
56 chosenColour(3,red0)
59 chosenColour(2,red0)
62 chosenColour(1,red0)
2 node(1)
3 node(2)
4 node(3)
5 node(4)
6 node(5)
21 notChosenColour(5,blue0)
24 notChosenColour(4,blue0)
27 notChosenColour(3,blue0)
30 notChosenColour(2,blue0)
33 notChosenColour(1,blue0)
36 notChosenColour(5,green0)
39 notChosenColour(4,green0)
42 notChosenColour(3,green0)
45 notChosenColour(2,green0)
48 notChosenColour(1,green0)
51 notChosenColour(5,red0)
54 notChosenColour(4,red0)
57 notChosenColour(3,red0)
60 notChosenColour(2,red0)
63 notChosenColour(1,red0)
0
B+
0
B-
1
0
1
"""
output = """
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), chosenColour(3,blue0), notChosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), chosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), chosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), notChosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), notChosenColour(5,green0), chosenColour(4,green0), chosenColour(3,green0), notChosenColour(2,green0), notChosenColour(1,green0), chosenColour(5,red0), notChosenColour(4,red0), notChosenColour(3,red0), notChosenColour(2,red0), chosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), notChosenColour(2,blue0), chosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), chosenColour(2,green0), notChosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), chosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), notChosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
{node(1), node(2), node(3), node(4), node(5), link(1,2), link(2,1), link(1,3), link(3,1), link(2,3), link(3,2), link(3,5), link(5,3), link(4,5), link(5,4), colour(red0), colour(green0), colour(blue0), notChosenColour(5,blue0), notChosenColour(4,blue0), notChosenColour(3,blue0), chosenColour(2,blue0), notChosenColour(1,blue0), chosenColour(5,green0), notChosenColour(4,green0), notChosenColour(3,green0), notChosenColour(2,green0), chosenColour(1,green0), notChosenColour(5,red0), chosenColour(4,red0), chosenColour(3,red0), notChosenColour(2,red0), notChosenColour(1,red0)}
"""
