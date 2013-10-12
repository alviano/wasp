input = """
i(0). i(1).
a(X) v -a(X) :- i(X).
ok :- #count{X:a(X)} > 0, not #count{T:a(T)} >= 2.
:- not ok.
"""

output = """
{-a(0), a(1), i(0), i(1), ok}
{-a(1), a(0), i(0), i(1), ok}
"""
