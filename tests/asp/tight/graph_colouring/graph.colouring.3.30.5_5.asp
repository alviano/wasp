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
link(0,1).
link(0,17).
link(0,15).
link(0,10).
link(0,19).
link(0,7).
link(1,7).
link(1,2).
link(1,6).
link(1,13).
link(1,27).
link(1,2).
link(2,5).
link(2,22).
link(2,25).
link(2,28).
link(2,3).
link(2,9).
link(3,23).
link(3,10).
link(3,12).
link(3,12).
link(3,17).
link(3,23).
link(4,14).
link(4,28).
link(4,23).
link(4,25).
link(4,5).
link(4,2).
link(5,13).
link(5,9).
link(5,18).
link(5,10).
link(5,10).
link(5,6).
link(6,26).
link(6,29).
link(6,0).
link(6,18).
link(6,27).
link(6,19).
link(7,11).
link(7,10).
link(7,10).
link(7,28).
link(7,8).
link(7,14).
link(8,20).
link(8,7).
link(8,4).
link(8,20).
link(8,25).
link(8,3).
link(9,10).
link(9,4).
link(9,0).
link(9,21).
link(9,7).
link(9,24).
link(10,9).
link(10,14).
link(10,8).
link(10,3).
link(10,3).
link(10,20).
link(11,23).
link(11,21).
link(11,24).
link(11,23).
link(11,2).
link(11,12).
link(12,10).
link(12,4).
link(12,0).
link(12,16).
link(12,11).
link(12,25).
link(13,14).
link(13,9).
link(13,14).
link(13,11).
link(13,16).
link(13,3).
link(14,26).
link(14,19).
link(14,6).
link(14,18).
link(14,19).
link(14,21).
link(15,0).
link(15,10).
link(15,16).
link(15,21).
link(15,26).
link(15,6).
link(16,23).
link(16,5).
link(16,7).
link(16,0).
link(16,7).
link(16,14).
link(17,18).
link(17,5).
link(17,20).
link(17,3).
link(17,9).
link(17,25).
link(18,2).
link(18,26).
link(18,17).
link(18,9).
link(18,26).
link(18,0).
link(19,10).
link(19,28).
link(19,16).
link(19,26).
link(19,7).
link(19,26).
link(20,23).
link(20,7).
link(20,0).
link(20,3).
link(20,11).
link(20,14).
link(21,9).
link(21,4).
link(21,22).
link(21,14).
link(21,10).
link(21,23).
link(22,23).
link(22,12).
link(22,7).
link(22,6).
link(22,3).
link(22,14).
link(23,9).
link(23,28).
link(23,5).
link(23,7).
link(23,18).
link(23,25).
link(24,11).
link(24,10).
link(24,27).
link(24,23).
link(24,14).
link(24,13).
link(25,27).
link(25,6).
link(25,3).
link(25,28).
link(25,14).
link(25,2).
link(26,9).
link(26,4).
link(26,5).
link(26,27).
link(26,28).
link(26,11).
link(27,13).
link(27,4).
link(27,19).
link(27,21).
link(27,28).
link(27,21).
link(28,7).
link(28,6).
link(28,6).
link(28,17).
link(28,29).
link(28,20).
link(29,2).
link(29,5).
link(29,24).
link(29,1).
link(29,9).
link(29,6).

#hide. #show col/2.
