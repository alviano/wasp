input = """
node(X) :- edge(X,Y).
node(Y) :- edge(X,Y).

colored(X,r) v colored(X,g) v colored(X,b) :- node(X).

:- edge(X,Y), colored(X,C), colored(Y,C).
edge( n2, n4). edge( n2, n3). edge( n3, n5).
edge( n4, n6). edge( n4, n5). edge( n5, n7).
               edge( n6, n7).
"""

output = """
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,g), colored(n4,r), colored(n5,b), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,g), colored(n5,b), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,b), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,r), colored(n5,g), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,b), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,b), colored(n5,g), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,b), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,g), colored(n3,r), colored(n4,r), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,g), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,g), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,g), colored(n5,r), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,b), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,b), colored(n5,r), colored(n6,g), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,b), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,b), colored(n7,r), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,b), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,b), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,b), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
{colored(n2,r), colored(n3,g), colored(n4,g), colored(n5,r), colored(n6,r), colored(n7,g), edge(n2,n3), edge(n2,n4), edge(n3,n5), edge(n4,n5), edge(n4,n6), edge(n5,n7), edge(n6,n7), node(n2), node(n3), node(n4), node(n5), node(n6), node(n7)}
"""
