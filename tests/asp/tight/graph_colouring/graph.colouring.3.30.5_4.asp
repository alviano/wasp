% Guess colours.

col(N,C) | n_col(N,C) :- node(N), colour(C).



% At least one color per node.

:- node(X), not colored(X).

colored(X) :- col(X,Fv1).



% Only one color per node.

:- col(N,C1), col(N,C2), C1!=C2.



% No two adjacent nodes have the same colour. 

:- link(X,Y),  X<Y, col(X,C), col(Y,C).
colour(1..3).
node(0).
node(1).
node(2).
node(3).
node(4).
node(5).
node(6).
node(7).
node(8).
node(9).
node(10).
node(11).
node(12).
node(13).
node(14).
node(15).
node(16).
node(17).
node(18).
node(19).
node(20).
node(21).
node(22).
node(23).
node(24).
node(25).
node(26).
node(27).
node(28).
node(29).
link(0,16).
link(0,22).
link(0,19).
link(0,1).
link(0,18).
link(0,13).
link(1,11).
link(1,13).
link(1,3).
link(1,24).
link(1,20).
link(1,23).
link(2,28).
link(2,27).
link(2,4).
link(2,16).
link(2,4).
link(2,26).
link(3,1).
link(3,4).
link(3,16).
link(3,23).
link(3,20).
link(3,2).
link(4,10).
link(4,13).
link(4,5).
link(4,9).
link(4,1).
link(4,18).
link(5,3).
link(5,0).
link(5,24).
link(5,21).
link(5,15).
link(5,15).
link(6,26).
link(6,17).
link(6,2).
link(6,12).
link(6,26).
link(6,15).
link(7,14).
link(7,10).
link(7,28).
link(7,20).
link(7,14).
link(7,6).
link(8,22).
link(8,20).
link(8,14).
link(8,25).
link(8,12).
link(8,10).
link(9,0).
link(9,24).
link(9,16).
link(9,2).
link(9,22).
link(9,26).
link(10,27).
link(10,1).
link(10,26).
link(10,1).
link(10,15).
link(10,27).
link(11,3).
link(11,2).
link(11,12).
link(11,25).
link(11,21).
link(11,14).
link(12,7).
link(12,1).
link(12,10).
link(12,26).
link(12,5).
link(12,6).
link(13,10).
link(13,14).
link(13,19).
link(13,3).
link(13,23).
link(13,8).
link(14,1).
link(14,13).
link(14,19).
link(14,0).
link(14,15).
link(14,17).
link(15,0).
link(15,17).
link(15,4).
link(15,16).
link(15,21).
link(15,25).
link(16,29).
link(16,19).
link(16,10).
link(16,19).
link(16,15).
link(16,0).
link(17,3).
link(17,13).
link(17,22).
link(17,27).
link(17,20).
link(17,2).
link(18,23).
link(18,1).
link(18,14).
link(18,17).
link(18,1).
link(18,14).
link(19,26).
link(19,5).
link(19,20).
link(19,6).
link(19,11).
link(19,7).
link(20,14).
link(20,27).
link(20,29).
link(20,15).
link(20,10).
link(20,7).
link(21,14).
link(21,9).
link(21,14).
link(21,26).
link(21,14).
link(21,3).
link(22,15).
link(22,0).
link(22,23).
link(22,3).
link(22,29).
link(22,1).
link(23,26).
link(23,25).
link(23,26).
link(23,0).
link(23,24).
link(23,28).
link(24,1).
link(24,20).
link(24,27).
link(24,10).
link(24,2).
link(24,4).
link(25,15).
link(25,7).
link(25,6).
link(25,1).
link(25,4).
link(25,18).
link(26,27).
link(26,16).
link(26,19).
link(26,4).
link(26,9).
link(26,11).
link(27,21).
link(27,16).
link(27,16).
link(27,2).
link(27,14).
link(27,0).
link(28,5).
link(28,2).
link(28,9).
link(28,22).
link(28,12).
link(28,7).
link(29,24).
link(29,13).
link(29,10).
link(29,18).
link(29,22).
link(29,30).

#hide. #show col/2.
