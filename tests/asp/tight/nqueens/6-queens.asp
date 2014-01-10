n(6).

value(0).
value(X) :- value(Y), X=Y+1, X<N, n(N).

% guess horizontal position for each row
q(X,1) | q(X,2) | q(X,3) | q(X,4) | q(X,5) | q(X,6) :- value(X).

% check

% assert that each column may only contain (at most) one q
:- q(X1,Y), q(X2,Y), X1 != X2.

% assert that no two qs are in a diagonal from top left to bottom right
:- q(X1,Y1), q(X2,Y2), N=X2-X1, N=Y2-Y1, N > 0.

% assert that no two qs are in a diagonal from top right to bottom left
:- q(X1,Y1), q(X2,Y2), N=X2-X1, N=Y1-Y2, N > 0.

#hide.
#show q/2.
