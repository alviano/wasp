input = """
true.
pos(0,1,b1):- true.
pos(0,2,b2):- true.
pos(0,3,x):- true.
pos(0,4,x):- true.
pos(0,5,x):- true.
pos(0,6,x):- true.
pos(0,7,y):- true.
pos(0,8,y):- true.
pos(0,9,y):- true.
pos(0,10,y):- true.
goal(1,x).
goal(2,y).
goal(3,x).
goal(4,y).
goal(5,x).
goal(6,y).
goal(7,x).
goal(8,y).
goal(9,b1).
goal(10,b2).  


mv(N,1) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,9).

mv(N,2) :- move_number(N), not forced_move(N), not mv(N,1), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,9).
 
mv(N,3) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,1), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,9).

mv(N,4) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,1), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,9).

mv(N,5) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,1), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,9).

mv(N,6) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,1), not mv(N,7), not mv(N,8), not mv(N,9).

mv(N,7) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,1), not mv(N,8), not mv(N,9).

mv(N,8) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,1), not mv(N,9).

mv(N,9) :- move_number(N), not forced_move(N), not mv(N,2), not mv(N,3), not mv(N,4), not mv(N,5), not mv(N,6), not mv(N,7), not mv(N,8), not mv(N,1).

forced_move(4).
mv(4,9) :- true.


pos(N,P,b1):- mv(N,P).
pos(N,P,b2):- mv(N,Q), succ(Q,P).
pos(N,P,X) :- succ(M,N), pos(M,P,b1), mv(N,Q), pos(M,Q,X).
pos(N,P,X) :- succ(M,N), pos(M,P,b2), mv(N,R), succ(R,Q), pos(M,Q,X).
pos(N,P,X) :- move_number(N), succ(M,N), pos(M,P,X), not changed(N,P).

changed(N,P) :- succ(M,N), pos(M,P,b1).
changed(N,P) :- succ(M,N), pos(M,P,b2).
changed(N,P) :- mv(N,P).
changed(N,P) :- mv(N,Q), succ(Q,P).

bad :- pos(4,P,X), goal(P,Y), not equals(X,Y).

no_stable :- bad, not no_stable.



move_number(1).
move_number(2).
move_number(3).
move_number(4).

equals(4,4).
equals(x,x).
equals(y,y).
equals(b1,b1).
equals(b2,b2).

succ(0,1).
succ(1,2).
succ(2,3).
succ(3,4).
succ(4,5).
succ(5,6).
succ(6,7).
succ(7,8).
succ(8,9).
succ(9,10).
"""

output = """
"""
