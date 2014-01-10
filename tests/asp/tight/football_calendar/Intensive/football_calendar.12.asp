teams(12).

team(1).
team(V) :- team(V1), V=V1+1, teams(N), V1 < N.

schedule(1).
schedule(G) :- schedule(G1), G=G1+1, teams(N), N1=N-1, G1 < N1.

match(S1,S2,G) | n_match(S1,S2,G) :- team(S1), team(S2), schedule(G), S1 < S2.

:- match(S1,S2,G), match(S1,S3,G), S2 != S3.
:- match(S1,S2,G), match(S3,S2,G), S1 != S3.

:- match(S1,S2,G), match(S1,S2,G1), G != G1.

play(S,G) :- match(S,_,G).
play(S,G) :- match(_,S,G).

:- schedule(G), team(S), not play(S,G).
