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
link(0,20).
link(0,13).
link(0,17).
link(0,21).
link(0,24).
link(0,20).
link(1,9).
link(1,29).
link(1,23).
link(1,24).
link(1,25).
link(1,8).
link(2,22).
link(2,3).
link(2,15).
link(2,5).
link(2,20).
link(2,19).
link(3,5).
link(3,12).
link(3,23).
link(3,11).
link(3,5).
link(3,21).
link(4,12).
link(4,26).
link(4,23).
link(4,10).
link(4,26).
link(4,7).
link(5,19).
link(5,23).
link(5,1).
link(5,1).
link(5,22).
link(5,3).
link(6,18).
link(6,11).
link(6,15).
link(6,2).
link(6,10).
link(6,26).
link(7,27).
link(7,26).
link(7,17).
link(7,28).
link(7,9).
link(7,24).
link(8,23).
link(8,16).
link(8,16).
link(8,6).
link(8,9).
link(8,7).
link(9,20).
link(9,29).
link(9,27).
link(9,26).
link(9,23).
link(9,10).
link(10,21).
link(10,28).
link(10,6).
link(10,17).
link(10,16).
link(10,27).
link(11,9).
link(11,19).
link(11,26).
link(11,14).
link(11,16).
link(11,22).
link(12,20).
link(12,2).
link(12,25).
link(12,0).
link(12,19).
link(12,22).
link(13,26).
link(13,17).
link(13,2).
link(13,1).
link(13,11).
link(13,24).
link(14,2).
link(14,23).
link(14,15).
link(14,15).
link(14,24).
link(14,4).
link(15,5).
link(15,6).
link(15,14).
link(15,12).
link(15,2).
link(15,16).
link(16,6).
link(16,8).
link(16,21).
link(16,9).
link(16,23).
link(16,14).
link(17,0).
link(17,3).
link(17,5).
link(17,21).
link(17,3).
link(17,11).
link(18,25).
link(18,11).
link(18,0).
link(18,20).
link(18,5).
link(18,21).
link(19,14).
link(19,6).
link(19,15).
link(19,22).
link(19,6).
link(19,22).
link(20,1).
link(20,24).
link(20,12).
link(20,6).
link(20,22).
link(20,28).
link(21,1).
link(21,28).
link(21,16).
link(21,23).
link(21,15).
link(21,12).
link(22,25).
link(22,21).
link(22,2).
link(22,16).
link(22,23).
link(22,10).
link(23,25).
link(23,1).
link(23,15).
link(23,24).
link(23,12).
link(23,17).
link(24,4).
link(24,11).
link(24,25).
link(24,19).
link(24,27).
link(24,25).
link(25,23).
link(25,17).
link(25,7).
link(25,22).
link(25,7).
link(25,14).
link(26,10).
link(26,27).
link(26,16).
link(26,10).
link(26,13).
link(26,15).
link(27,8).
link(27,16).
link(27,2).
link(27,16).
link(27,6).
link(27,9).
link(28,9).
link(28,23).
link(28,8).
link(28,25).
link(28,2).
link(28,27).
link(29,16).
link(29,14).
link(29,30).
link(29,18).
link(29,3).
link(29,30).

#hide. #show col/2.
