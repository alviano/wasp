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
link(0,7).
link(0,12).
link(0,4).
link(0,14).
link(0,1).
link(0,14).
link(1,21).
link(1,14).
link(1,15).
link(1,23).
link(1,10).
link(1,19).
link(2,26).
link(2,13).
link(2,10).
link(2,20).
link(2,7).
link(2,29).
link(3,17).
link(3,20).
link(3,28).
link(3,23).
link(3,15).
link(3,6).
link(4,27).
link(4,2).
link(4,25).
link(4,1).
link(4,5).
link(4,14).
link(5,27).
link(5,22).
link(5,1).
link(5,10).
link(5,6).
link(5,14).
link(6,13).
link(6,14).
link(6,14).
link(6,0).
link(6,11).
link(6,9).
link(7,9).
link(7,25).
link(7,14).
link(7,25).
link(7,5).
link(7,11).
link(8,28).
link(8,18).
link(8,2).
link(8,18).
link(8,28).
link(8,2).
link(9,28).
link(9,18).
link(9,17).
link(9,6).
link(9,10).
link(9,25).
link(10,3).
link(10,15).
link(10,8).
link(10,23).
link(10,13).
link(10,0).
link(11,1).
link(11,23).
link(11,8).
link(11,22).
link(11,12).
link(11,20).
link(12,19).
link(12,8).
link(12,11).
link(12,5).
link(12,25).
link(12,3).
link(13,6).
link(13,25).
link(13,23).
link(13,12).
link(13,1).
link(13,25).
link(14,11).
link(14,28).
link(14,13).
link(14,15).
link(14,17).
link(14,27).
link(15,16).
link(15,20).
link(15,8).
link(15,3).
link(15,22).
link(15,16).
link(16,9).
link(16,24).
link(16,12).
link(16,18).
link(16,8).
link(16,28).
link(17,3).
link(17,22).
link(17,22).
link(17,11).
link(17,18).
link(17,10).
link(18,1).
link(18,20).
link(18,24).
link(18,11).
link(18,2).
link(18,26).
link(19,21).
link(19,21).
link(19,20).
link(19,13).
link(19,8).
link(19,28).
link(20,23).
link(20,12).
link(20,17).
link(20,22).
link(20,21).
link(20,14).
link(21,15).
link(21,28).
link(21,25).
link(21,0).
link(21,14).
link(21,19).
link(22,14).
link(22,1).
link(22,9).
link(22,21).
link(22,15).
link(22,28).
link(23,4).
link(23,14).
link(23,5).
link(23,17).
link(23,26).
link(23,19).
link(24,14).
link(24,10).
link(24,25).
link(24,21).
link(24,26).
link(24,12).
link(25,17).
link(25,15).
link(25,19).
link(25,4).
link(25,10).
link(25,9).
link(26,14).
link(26,9).
link(26,20).
link(26,5).
link(26,10).
link(26,1).
link(27,19).
link(27,23).
link(27,1).
link(27,20).
link(27,5).
link(27,0).
link(28,13).
link(28,6).
link(28,11).
link(28,18).
link(28,16).
link(28,9).
link(29,2).
link(29,28).
link(29,9).
link(29,13).
link(29,3).
link(29,5).

#hide. #show col/2.
