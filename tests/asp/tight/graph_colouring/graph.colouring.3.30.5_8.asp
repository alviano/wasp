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
link(0,9).
link(0,7).
link(0,7).
link(0,17).
link(0,17).
link(0,12).
link(1,6).
link(1,21).
link(1,23).
link(1,6).
link(1,0).
link(1,23).
link(2,9).
link(2,9).
link(2,21).
link(2,9).
link(2,25).
link(2,11).
link(3,5).
link(3,10).
link(3,26).
link(3,4).
link(3,18).
link(3,26).
link(4,21).
link(4,29).
link(4,3).
link(4,24).
link(4,2).
link(4,5).
link(5,18).
link(5,2).
link(5,4).
link(5,10).
link(5,20).
link(5,27).
link(6,18).
link(6,8).
link(6,29).
link(6,27).
link(6,14).
link(6,11).
link(7,5).
link(7,21).
link(7,11).
link(7,9).
link(7,23).
link(7,18).
link(8,27).
link(8,27).
link(8,3).
link(8,17).
link(8,6).
link(8,20).
link(9,17).
link(9,15).
link(9,13).
link(9,23).
link(9,21).
link(9,1).
link(10,9).
link(10,26).
link(10,1).
link(10,1).
link(10,1).
link(10,13).
link(11,20).
link(11,9).
link(11,17).
link(11,12).
link(11,12).
link(11,17).
link(12,8).
link(12,27).
link(12,27).
link(12,25).
link(12,11).
link(12,23).
link(13,22).
link(13,0).
link(13,0).
link(13,29).
link(13,17).
link(13,16).
link(14,16).
link(14,0).
link(14,1).
link(14,21).
link(14,10).
link(14,1).
link(15,18).
link(15,12).
link(15,16).
link(15,8).
link(15,1).
link(15,16).
link(16,27).
link(16,20).
link(16,5).
link(16,23).
link(16,11).
link(16,4).
link(17,9).
link(17,20).
link(17,18).
link(17,18).
link(17,24).
link(17,24).
link(18,2).
link(18,27).
link(18,7).
link(18,13).
link(18,25).
link(18,29).
link(19,10).
link(19,2).
link(19,9).
link(19,16).
link(19,29).
link(19,15).
link(20,21).
link(20,7).
link(20,26).
link(20,24).
link(20,15).
link(20,27).
link(21,17).
link(21,12).
link(21,23).
link(21,9).
link(21,3).
link(21,7).
link(22,26).
link(22,13).
link(22,23).
link(22,26).
link(22,4).
link(22,27).
link(23,2).
link(23,8).
link(23,11).
link(23,24).
link(23,19).
link(23,20).
link(24,15).
link(24,6).
link(24,2).
link(24,29).
link(24,17).
link(24,16).
link(25,21).
link(25,2).
link(25,9).
link(25,3).
link(25,21).
link(25,27).
link(26,1).
link(26,1).
link(26,1).
link(26,9).
link(26,27).
link(26,27).
link(27,2).
link(27,23).
link(27,10).
link(27,1).
link(27,24).
link(27,19).
link(28,9).
link(28,20).
link(28,21).
link(28,16).
link(28,5).
link(28,29).
link(29,20).
link(29,1).
link(29,20).
link(29,18).
link(29,26).
link(29,13).

#hide. #show col/2.
