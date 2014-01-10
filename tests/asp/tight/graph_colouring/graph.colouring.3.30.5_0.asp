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
link(0,5).
link(0,20).
link(0,22).
link(0,11).
link(0,13).
link(0,14).
link(1,6).
link(1,25).
link(1,9).
link(1,23).
link(1,25).
link(1,5).
link(2,25).
link(2,16).
link(2,9).
link(2,13).
link(2,5).
link(2,26).
link(3,11).
link(3,21).
link(3,16).
link(3,9).
link(3,21).
link(3,13).
link(4,13).
link(4,27).
link(4,10).
link(4,0).
link(4,12).
link(4,28).
link(5,19).
link(5,3).
link(5,21).
link(5,14).
link(5,12).
link(5,21).
link(6,17).
link(6,8).
link(6,23).
link(6,19).
link(6,22).
link(6,7).
link(7,12).
link(7,9).
link(7,8).
link(7,29).
link(7,15).
link(7,21).
link(8,12).
link(8,13).
link(8,15).
link(8,7).
link(8,11).
link(8,6).
link(9,3).
link(9,19).
link(9,22).
link(9,29).
link(9,13).
link(9,4).
link(10,9).
link(10,14).
link(10,9).
link(10,3).
link(10,24).
link(10,4).
link(11,18).
link(11,8).
link(11,6).
link(11,28).
link(11,15).
link(11,2).
link(12,9).
link(12,23).
link(12,10).
link(12,22).
link(12,27).
link(12,23).
link(13,22).
link(13,5).
link(13,2).
link(13,7).
link(13,18).
link(13,10).
link(14,7).
link(14,26).
link(14,17).
link(14,25).
link(14,6).
link(14,21).
link(15,24).
link(15,28).
link(15,6).
link(15,28).
link(15,21).
link(15,1).
link(16,23).
link(16,12).
link(16,28).
link(16,15).
link(16,3).
link(16,0).
link(17,21).
link(17,7).
link(17,27).
link(17,1).
link(17,8).
link(17,5).
link(18,29).
link(18,5).
link(18,1).
link(18,2).
link(18,16).
link(18,0).
link(19,18).
link(19,17).
link(19,14).
link(19,20).
link(19,10).
link(19,15).
link(20,22).
link(20,9).
link(20,27).
link(20,23).
link(20,11).
link(20,21).
link(21,24).
link(21,10).
link(21,12).
link(21,19).
link(21,4).
link(21,13).
link(22,6).
link(22,4).
link(22,10).
link(22,18).
link(22,28).
link(22,6).
link(23,12).
link(23,0).
link(23,26).
link(23,25).
link(23,10).
link(23,2).
link(24,13).
link(24,3).
link(24,15).
link(24,2).
link(24,7).
link(24,13).
link(25,2).
link(25,27).
link(25,26).
link(25,28).
link(25,19).
link(25,9).
link(26,3).
link(26,13).
link(26,4).
link(26,28).
link(26,16).
link(26,5).
link(27,18).
link(27,23).
link(27,25).
link(27,14).
link(27,10).
link(27,0).
link(28,15).
link(28,6).
link(28,27).
link(28,27).
link(28,4).
link(28,23).
link(29,1).
link(29,0).
link(29,10).
link(29,0).
link(29,28).
link(29,3).

#hide. #show col/2.
