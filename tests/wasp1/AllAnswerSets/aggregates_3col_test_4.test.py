input = """
node(1..15).
col(blue).
col(green).
col(red).

color(N,C) v ncolor(N,C) :- node(N), col(C).
:- color(X,C), color(Y,C), link(X,Y).

:- color(N,C), color(N,C1), C!=C1.
:- not #count{ N: color(N,C) } > 14.

link(1,2).
link(1,3).
link(1,6).
link(1,14).
link(2,3).
link(2,6).
link(2,8).
link(2,13).
link(2,15).
link(3,4).
link(3,10).
link(3,11).
link(3,14).
link(4,5).
link(4,6).
link(4,8).
link(4,14).
link(5,7).
link(5,8).
link(5,9).
link(5,13).
link(5,14).
link(6,10).
link(6,12).
link(6,13).
link(7,8).
link(7,9).
link(7,15).
link(8,10).
link(8,12).
link(8,14).
link(9,10).
link(9,11).
link(9,12).
link(10,11).
link(11,12).
link(11,14).
link(12,14).
link(13,14).
link(14,15).
"""

output = """
"""
