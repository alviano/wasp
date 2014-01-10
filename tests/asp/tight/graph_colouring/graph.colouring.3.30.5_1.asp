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
link(0,6).
link(0,12).
link(0,20).
link(0,28).
link(0,13).
link(0,6).
link(1,21).
link(1,6).
link(1,16).
link(1,3).
link(1,27).
link(1,2).
link(2,14).
link(2,24).
link(2,6).
link(2,7).
link(2,5).
link(2,23).
link(3,5).
link(3,28).
link(3,15).
link(3,26).
link(3,26).
link(3,26).
link(4,6).
link(4,29).
link(4,9).
link(4,5).
link(4,6).
link(4,19).
link(5,9).
link(5,16).
link(5,26).
link(5,1).
link(5,28).
link(5,9).
link(6,5).
link(6,2).
link(6,27).
link(6,14).
link(6,28).
link(6,24).
link(7,2).
link(7,28).
link(7,4).
link(7,8).
link(7,13).
link(7,20).
link(8,7).
link(8,9).
link(8,0).
link(8,4).
link(8,21).
link(8,1).
link(9,19).
link(9,13).
link(9,14).
link(9,15).
link(9,5).
link(9,14).
link(10,11).
link(10,19).
link(10,9).
link(10,19).
link(10,11).
link(10,5).
link(11,10).
link(11,19).
link(11,6).
link(11,14).
link(11,13).
link(11,20).
link(12,13).
link(12,19).
link(12,3).
link(12,24).
link(12,15).
link(12,22).
link(13,17).
link(13,3).
link(13,7).
link(13,17).
link(13,5).
link(13,7).
link(14,4).
link(14,2).
link(14,16).
link(14,0).
link(14,2).
link(14,22).
link(15,25).
link(15,10).
link(15,19).
link(15,14).
link(15,12).
link(15,19).
link(16,1).
link(16,1).
link(16,12).
link(16,19).
link(16,9).
link(16,6).
link(17,18).
link(17,3).
link(17,10).
link(17,7).
link(17,22).
link(17,10).
link(18,25).
link(18,25).
link(18,9).
link(18,11).
link(18,20).
link(18,7).
link(19,12).
link(19,5).
link(19,18).
link(19,10).
link(19,0).
link(19,23).
link(20,7).
link(20,29).
link(20,27).
link(20,5).
link(20,26).
link(20,18).
link(21,22).
link(21,6).
link(21,25).
link(21,2).
link(21,17).
link(21,18).
link(22,16).
link(22,19).
link(22,25).
link(22,13).
link(22,14).
link(22,2).
link(23,1).
link(23,27).
link(23,21).
link(23,2).
link(23,4).
link(23,26).
link(24,14).
link(24,23).
link(24,7).
link(24,7).
link(24,1).
link(24,4).
link(25,19).
link(25,18).
link(25,6).
link(25,26).
link(25,23).
link(25,15).
link(26,7).
link(26,8).
link(26,18).
link(26,10).
link(26,2).
link(26,0).
link(27,17).
link(27,13).
link(27,16).
link(27,1).
link(27,9).
link(27,16).
link(28,20).
link(28,25).
link(28,19).
link(28,2).
link(28,2).
link(28,21).
link(29,18).
link(29,1).
link(29,19).
link(29,9).
link(29,11).
link(29,25).

#hide. #show col/2.
