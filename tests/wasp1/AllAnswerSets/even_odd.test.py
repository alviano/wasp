input = """
n(eins).
n(zwei).
n(drei).
n(vier).
n(fuenf).
n(sechs).
n(sieben).
n(acht).
n(neun).

even(zwei).
even(vier).
even(sechs).
even(acht).

odd(I) :- n(I), not even(I).

even2(I) :- n(I), not odd(I).
"""

output = """
{even(acht), even(sechs), even(vier), even(zwei), even2(acht), even2(sechs), even2(vier), even2(zwei), n(acht), n(drei), n(eins), n(fuenf), n(neun), n(sechs), n(sieben), n(vier), n(zwei), odd(drei), odd(eins), odd(fuenf), odd(neun), odd(sieben)}
"""
