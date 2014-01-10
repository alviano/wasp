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
link(0,15).
link(0,17).
link(0,6).
link(0,12).
link(0,3).
link(0,14).
link(1,11).
link(1,14).
link(1,12).
link(1,6).
link(1,17).
link(1,11).
link(2,28).
link(2,0).
link(2,8).
link(2,3).
link(2,16).
link(2,28).
link(3,11).
link(3,28).
link(3,4).
link(3,27).
link(3,16).
link(3,2).
link(4,7).
link(4,24).
link(4,8).
link(4,3).
link(4,21).
link(4,1).
link(5,19).
link(5,25).
link(5,4).
link(5,19).
link(5,11).
link(5,1).
link(6,14).
link(6,1).
link(6,7).
link(6,19).
link(6,21).
link(6,27).
link(7,6).
link(7,22).
link(7,27).
link(7,0).
link(7,13).
link(7,15).
link(8,23).
link(8,1).
link(8,9).
link(8,27).
link(8,25).
link(8,4).
link(9,15).
link(9,4).
link(9,10).
link(9,13).
link(9,18).
link(9,20).
link(10,23).
link(10,19).
link(10,18).
link(10,12).
link(10,9).
link(10,20).
link(11,6).
link(11,13).
link(11,16).
link(11,17).
link(11,22).
link(11,17).
link(12,23).
link(12,23).
link(12,13).
link(12,2).
link(12,19).
link(12,21).
link(13,29).
link(13,5).
link(13,20).
link(13,12).
link(13,22).
link(13,19).
link(14,5).
link(14,4).
link(14,3).
link(14,24).
link(14,27).
link(14,24).
link(15,24).
link(15,14).
link(15,22).
link(15,14).
link(15,8).
link(15,23).
link(16,1).
link(16,14).
link(16,12).
link(16,4).
link(16,6).
link(16,26).
link(17,0).
link(17,7).
link(17,19).
link(17,1).
link(17,16).
link(17,3).
link(18,0).
link(18,7).
link(18,10).
link(18,14).
link(18,19).
link(18,8).
link(19,13).
link(19,11).
link(19,8).
link(19,10).
link(19,13).
link(19,15).
link(20,12).
link(20,27).
link(20,25).
link(20,12).
link(20,15).
link(20,18).
link(21,9).
link(21,22).
link(21,14).
link(21,5).
link(21,5).
link(21,10).
link(22,5).
link(22,13).
link(22,24).
link(22,10).
link(22,16).
link(22,28).
link(23,25).
link(23,11).
link(23,2).
link(23,24).
link(23,7).
link(23,15).
link(24,5).
link(24,28).
link(24,12).
link(24,25).
link(24,5).
link(24,7).
link(25,4).
link(25,19).
link(25,19).
link(25,9).
link(25,6).
link(25,26).
link(26,27).
link(26,2).
link(26,3).
link(26,15).
link(26,24).
link(26,12).
link(27,21).
link(27,28).
link(27,14).
link(27,23).
link(27,10).
link(27,21).
link(28,11).
link(28,2).
link(28,23).
link(28,23).
link(28,20).
link(28,9).
link(29,5).
link(29,16).
link(29,13).
link(29,12).
link(29,21).
link(29,1).

#hide. #show col/2.
