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
link(0,1).
link(0,24).
link(0,20).
link(0,27).
link(0,10).
link(1,2).
link(1,6).
link(1,17).
link(1,15).
link(1,5).
link(1,26).
link(2,7).
link(2,13).
link(2,5).
link(2,10).
link(2,5).
link(2,29).
link(3,4).
link(3,19).
link(3,23).
link(3,15).
link(3,20).
link(3,24).
link(4,29).
link(4,13).
link(4,13).
link(4,5).
link(4,16).
link(4,7).
link(5,8).
link(5,17).
link(5,27).
link(5,27).
link(5,18).
link(5,3).
link(6,26).
link(6,19).
link(6,29).
link(6,3).
link(6,19).
link(6,0).
link(7,8).
link(7,22).
link(7,0).
link(7,8).
link(7,20).
link(7,5).
link(8,23).
link(8,27).
link(8,28).
link(8,9).
link(8,10).
link(8,0).
link(9,10).
link(9,15).
link(9,20).
link(9,7).
link(9,18).
link(9,14).
link(10,8).
link(10,25).
link(10,3).
link(10,2).
link(10,1).
link(10,14).
link(11,26).
link(11,16).
link(11,21).
link(11,18).
link(11,5).
link(11,12).
link(12,0).
link(12,17).
link(12,4).
link(12,13).
link(12,22).
link(12,20).
link(13,28).
link(13,16).
link(13,21).
link(13,1).
link(13,20).
link(13,16).
link(14,9).
link(14,21).
link(14,18).
link(14,15).
link(14,4).
link(14,9).
link(15,2).
link(15,25).
link(15,23).
link(15,18).
link(15,26).
link(15,1).
link(16,25).
link(16,14).
link(16,10).
link(16,2).
link(16,3).
link(16,17).
link(17,25).
link(17,3).
link(17,4).
link(17,10).
link(17,7).
link(17,1).
link(18,3).
link(18,28).
link(18,10).
link(18,10).
link(18,6).
link(18,17).
link(19,13).
link(19,26).
link(19,20).
link(19,22).
link(19,8).
link(19,3).
link(20,3).
link(20,23).
link(20,10).
link(20,11).
link(20,19).
link(20,27).
link(21,16).
link(21,11).
link(21,8).
link(21,6).
link(21,27).
link(21,25).
link(22,16).
link(22,24).
link(22,23).
link(22,14).
link(22,17).
link(22,26).
link(23,25).
link(23,1).
link(23,24).
link(23,21).
link(23,9).
link(23,16).
link(24,18).
link(24,7).
link(24,18).
link(24,21).
link(24,2).
link(24,18).
link(25,13).
link(25,27).
link(25,20).
link(25,22).
link(25,10).
link(25,28).
link(26,10).
link(26,11).
link(26,22).
link(26,27).
link(26,8).
link(26,25).
link(27,15).
link(27,1).
link(27,16).
link(27,7).
link(27,5).
link(27,8).
link(28,11).
link(28,13).
link(28,11).
link(28,11).
link(28,7).
link(28,24).
link(29,30).
link(29,16).
link(29,22).
link(29,24).
link(29,8).
link(29,0).

#hide. #show col/2.
