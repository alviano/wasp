input = """
a12 :- a2, a4.
a7 v a2 :- a9.
a10 v a6.
a6 v a2 :- a14.
a14 v a9 :- a9.
a7 v a12 :- a6.
a10 v a8 :- a5.
a2 v a4 :- a7.
a6 v a13 :- a12.
a4 v a9 :- a6.
a7 :- a9, a12.
a5 :- a13, a6.
a8 :- a10, a7.
a6 v a10 :- a7.
a12 v a13 :- a10.
a5 v a1 :- a5.
a3 v a7 :- a0.
a14 v a1 :- a10.
a11 v a0 v a9.
a9 v a3 :- a11.
a8 :- a5, a10.
a6 v a7 :- a0.
a12 v a13 :- a10.
a3 v a13 :- a2.
a0 :- a13.
a2 :- a10, a12.
a14 v a1 :- a11.
a9 v a7 :- a4.
a6 v a9 :- a11.
a6 v a10 :- a11.
 :- a6, a3, a0.
 :- a1, a3, a7.
a6 v a14 :- a2.
a6 v a3 v a12.
a3 v a11 :- a5.
a10 :- a11, a3.
a10 v a4 v a8.
 :- a13, a7, a6.
a0 v a7 :- a14.
 :- a13, a3, a1.
a5 v a7 v a3.
a1 v a2 :- a3.
a2 :- a0, a8.
a4 v a5 :- a12.
a0 :- a10, a5.
a13 :- a14, a3.
a0 v a14 v a2.
a6 v a9 :- a9.
a8 :- a1.
a9 v a10 :- a4.
a0 :- a4, a9.
a11 :- a3, a1.
a10 v a11.
a2 v a11 :- a13.
a5 v a7 :- a10.
a13 :- a3, a1.
 :- a11, a5.
a14 v a4 :- a9.
a0 v a2 :- a2.
a6 :- a13.

:- not a1.
:- not a2.

"""

output = """
"""
