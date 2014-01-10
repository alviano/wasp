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
link(0,13).
link(0,17).
link(0,20).
link(0,2).
link(0,23).
link(0,10).
link(1,22).
link(1,28).
link(1,13).
link(1,14).
link(1,2).
link(1,0).
link(2,6).
link(2,20).
link(2,13).
link(2,26).
link(2,10).
link(2,8).
link(3,21).
link(3,4).
link(3,22).
link(3,10).
link(3,14).
link(3,9).
link(4,22).
link(4,16).
link(4,3).
link(4,26).
link(4,7).
link(4,28).
link(5,27).
link(5,12).
link(5,8).
link(5,19).
link(5,20).
link(5,6).
link(6,3).
link(6,12).
link(6,16).
link(6,24).
link(6,12).
link(6,26).
link(7,26).
link(7,25).
link(7,5).
link(7,9).
link(7,13).
link(7,2).
link(8,11).
link(8,29).
link(8,0).
link(8,3).
link(8,11).
link(8,17).
link(9,24).
link(9,24).
link(9,24).
link(9,28).
link(9,6).
link(9,29).
link(10,17).
link(10,19).
link(10,18).
link(10,11).
link(10,19).
link(10,24).
link(11,5).
link(11,28).
link(11,22).
link(11,16).
link(11,15).
link(11,24).
link(12,9).
link(12,8).
link(12,4).
link(12,8).
link(12,10).
link(12,22).
link(13,5).
link(13,6).
link(13,7).
link(13,3).
link(13,3).
link(13,16).
link(14,6).
link(14,20).
link(14,15).
link(14,26).
link(14,15).
link(14,21).
link(15,17).
link(15,18).
link(15,1).
link(15,3).
link(15,21).
link(15,27).
link(16,17).
link(16,20).
link(16,5).
link(16,12).
link(16,12).
link(16,17).
link(17,18).
link(17,10).
link(17,4).
link(17,24).
link(17,29).
link(17,28).
link(18,19).
link(18,16).
link(18,23).
link(18,16).
link(18,19).
link(18,8).
link(19,20).
link(19,1).
link(19,3).
link(19,8).
link(19,17).
link(19,22).
link(20,8).
link(20,10).
link(20,15).
link(20,13).
link(20,9).
link(20,27).
link(21,19).
link(21,0).
link(21,23).
link(21,15).
link(21,7).
link(21,5).
link(22,11).
link(22,11).
link(22,23).
link(22,4).
link(22,15).
link(22,26).
link(23,16).
link(23,27).
link(23,17).
link(23,15).
link(23,25).
link(23,24).
link(24,26).
link(24,18).
link(24,16).
link(24,25).
link(24,7).
link(24,9).
link(25,26).
link(25,11).
link(25,22).
link(25,9).
link(25,26).
link(25,11).
link(26,19).
link(26,28).
link(26,20).
link(26,1).
link(26,15).
link(26,11).
link(27,28).
link(27,10).
link(27,15).
link(27,2).
link(27,9).
link(27,7).
link(28,10).
link(28,18).
link(28,8).
link(28,14).
link(28,7).
link(28,19).
link(29,21).
link(29,20).
link(29,16).
link(29,6).
link(29,24).
link(29,24).

#hide. #show col/2.
