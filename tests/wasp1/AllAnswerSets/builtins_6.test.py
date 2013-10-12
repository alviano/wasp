input = """
ouch1 :- "2" < "1".
ouch2 :- "2" <= "1".
ouch3 :- "1" > "2".
ouch4 :- "1" >= "2".

okay1 :- "1" < "2".
okay2 :- "1" <= "2".
okay3 :- "2" > "1".
okay4 :- "2" >= "1".

Ouch1 :- beta < alpha.
Ouch2 :- beta <= alpha.
Ouch3 :- alpha > beta.
Ouch4 :- alpha >= beta.

Okay1 :- alpha < beta.
Okay2 :- alpha <= beta.
Okay3 :- beta > alpha.
Okay4 :- beta >= alpha.
"""

output = """
{Okay1, Okay2, Okay3, Okay4, okay1, okay2, okay3, okay4}
"""
