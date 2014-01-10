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
link(0,14).
link(0,9).
link(0,21).
link(0,6).
link(0,29).
link(0,26).
link(1,9).
link(1,18).
link(1,26).
link(1,25).
link(1,27).
link(1,12).
link(2,29).
link(2,6).
link(2,21).
link(2,20).
link(2,20).
link(2,14).
link(3,28).
link(3,28).
link(3,23).
link(3,23).
link(3,20).
link(3,22).
link(4,29).
link(4,3).
link(4,8).
link(4,1).
link(4,9).
link(4,29).
link(5,4).
link(5,11).
link(5,23).
link(5,9).
link(5,23).
link(5,2).
link(6,13).
link(6,21).
link(6,0).
link(6,25).
link(6,19).
link(6,19).
link(7,21).
link(7,9).
link(7,21).
link(7,17).
link(7,23).
link(7,19).
link(8,21).
link(8,1).
link(8,10).
link(8,13).
link(8,1).
link(8,26).
link(9,2).
link(9,1).
link(9,18).
link(9,11).
link(9,3).
link(9,15).
link(10,9).
link(10,11).
link(10,3).
link(10,4).
link(10,28).
link(10,14).
link(11,16).
link(11,5).
link(11,0).
link(11,1).
link(11,10).
link(11,1).
link(12,13).
link(12,6).
link(12,5).
link(12,13).
link(12,17).
link(12,8).
link(13,9).
link(13,0).
link(13,3).
link(13,14).
link(13,14).
link(13,20).
link(14,17).
link(14,7).
link(14,6).
link(14,0).
link(14,24).
link(14,19).
link(15,19).
link(15,27).
link(15,18).
link(15,4).
link(15,25).
link(15,2).
link(16,6).
link(16,15).
link(16,22).
link(16,17).
link(16,28).
link(16,8).
link(17,24).
link(17,3).
link(17,9).
link(17,12).
link(17,23).
link(17,2).
link(18,12).
link(18,11).
link(18,27).
link(18,17).
link(18,9).
link(18,8).
link(19,10).
link(19,11).
link(19,27).
link(19,14).
link(19,26).
link(19,2).
link(20,27).
link(20,4).
link(20,6).
link(20,23).
link(20,25).
link(20,28).
link(21,13).
link(21,10).
link(21,16).
link(21,16).
link(21,5).
link(21,28).
link(22,9).
link(22,23).
link(22,6).
link(22,18).
link(22,9).
link(22,7).
link(23,14).
link(23,10).
link(23,24).
link(23,2).
link(23,28).
link(23,29).
link(24,14).
link(24,12).
link(24,28).
link(24,28).
link(24,0).
link(24,11).
link(25,4).
link(25,22).
link(25,10).
link(25,23).
link(25,13).
link(25,6).
link(26,7).
link(26,10).
link(26,8).
link(26,27).
link(26,18).
link(26,10).
link(27,6).
link(27,29).
link(27,24).
link(27,24).
link(27,14).
link(27,18).
link(28,21).
link(28,7).
link(28,6).
link(28,25).
link(28,26).
link(28,17).
link(29,7).
link(29,2).
link(29,3).
link(29,17).
link(29,27).
link(29,5).

#hide. #show col/2.
