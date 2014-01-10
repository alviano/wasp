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
link(0,8).
link(0,12).
link(0,5).
link(0,20).
link(0,19).
link(0,12).
link(1,9).
link(1,5).
link(1,0).
link(1,11).
link(1,5).
link(1,28).
link(2,21).
link(2,1).
link(2,5).
link(2,15).
link(2,29).
link(2,1).
link(3,12).
link(3,1).
link(3,17).
link(3,22).
link(3,0).
link(3,9).
link(4,27).
link(4,6).
link(4,10).
link(4,23).
link(4,13).
link(4,26).
link(5,18).
link(5,4).
link(5,6).
link(5,25).
link(5,15).
link(5,12).
link(6,1).
link(6,26).
link(6,7).
link(6,2).
link(6,13).
link(6,7).
link(7,17).
link(7,14).
link(7,29).
link(7,24).
link(7,19).
link(7,29).
link(8,26).
link(8,21).
link(8,27).
link(8,19).
link(8,26).
link(8,18).
link(9,17).
link(9,2).
link(9,27).
link(9,13).
link(9,13).
link(9,1).
link(10,18).
link(10,11).
link(10,28).
link(10,6).
link(10,17).
link(10,21).
link(11,2).
link(11,16).
link(11,2).
link(11,9).
link(11,15).
link(11,6).
link(12,17).
link(12,5).
link(12,20).
link(12,8).
link(12,23).
link(12,13).
link(13,10).
link(13,24).
link(13,9).
link(13,27).
link(13,3).
link(13,3).
link(14,10).
link(14,23).
link(14,4).
link(14,8).
link(14,12).
link(14,20).
link(15,19).
link(15,13).
link(15,14).
link(15,7).
link(15,16).
link(15,21).
link(16,4).
link(16,4).
link(16,17).
link(16,22).
link(16,12).
link(16,5).
link(17,1).
link(17,10).
link(17,22).
link(17,11).
link(17,6).
link(17,4).
link(18,1).
link(18,0).
link(18,27).
link(18,26).
link(18,28).
link(18,24).
link(19,2).
link(19,7).
link(19,2).
link(19,20).
link(19,17).
link(19,2).
link(20,17).
link(20,16).
link(20,19).
link(20,18).
link(20,2).
link(20,23).
link(21,26).
link(21,23).
link(21,26).
link(21,2).
link(21,23).
link(21,9).
link(22,25).
link(22,4).
link(22,10).
link(22,21).
link(22,18).
link(22,14).
link(23,14).
link(23,24).
link(23,3).
link(23,9).
link(23,25).
link(23,19).
link(24,25).
link(24,8).
link(24,26).
link(24,13).
link(24,3).
link(24,0).
link(25,5).
link(25,15).
link(25,10).
link(25,19).
link(25,9).
link(25,12).
link(26,20).
link(26,21).
link(26,5).
link(26,2).
link(26,3).
link(26,9).
link(27,8).
link(27,5).
link(27,13).
link(27,9).
link(27,1).
link(27,25).
link(28,20).
link(28,25).
link(28,22).
link(28,29).
link(28,9).
link(28,6).
link(29,6).
link(29,19).
link(29,22).
link(29,7).
link(29,6).
link(29,3).

#hide. #show col/2.
