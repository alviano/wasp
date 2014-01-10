% The meaning of the time predicate is self-evident. As for the disk
% predicate, there are k disks 1,2,...,k. Disks 1, 2, 3, 4 denote pegs. 
% Disks 5, ... are "movable". The larger the number of the disk, 
% the "smaller" it is.
%
% The program uses additional predicates:
% on(T,N,M), which is true iff at time T, disk M is on disk N
% move(t,N), which is true iff at time T, it is disk N that will be
% moved
% where(T,N), which is true iff at time T, the disk to be moved is moved
% on top of the disk N.
% goal, which is true iff the goal state is reached at time t
% steps(T), which is the number of time steps T, required to reach the goal (provided part of Input data)

% Read in data 
 	on(0,N1,N) :- on0(N,N1).
    onG(K,N1,N) :- ongoal(N,N1), steps(K).
		   
% Specify valid arrangements of disks
 	% Basic condition. Smaller disks are on larger ones
 	:- time(T), on(T,N1,N), N1>=N.
 	
% Specify a valid move (only for T<t)
 	% pick a disk to move
    move(T,N) | noMove(T,N) :- disk(N), time(T), steps(K), T<K.
    :- move(T,N1), move(T,N2), N1 != N2.
    :- time(T), steps(K), T<K, not diskMoved(T).
    diskMoved(T) :- move(T,Fv1).

 	% pick a disk onto which to move
    where(T,N) | noWhere(T,N) :- disk(N), time(T), steps(K), T<K.
    :- where(T,N1), where(T,N2), N1 != N2.
    :- time(T), steps(K), T<K, not diskWhere(T).
    diskWhere(T) :- where(T,Fv1).

 	% pegs cannot be moved
 	:- move(T,N), N<5.

 	% only top disk can be moved
 	:- on(T,N,N1), move(T,N).

 	% a disk can be placed on top only.
 	:- on(T,N,N1), where(T,N).

 	% no disk is moved in two consecutive moves
 	:- move(T,N), move(TM1,N), TM1=T-1.

% Specify effects of a move
 	on(TP1,N1,N) :- move(T,N), where(T,N1), TP1=T+1.

 	on(TP1,N,N1) :- time(T), steps(K), T<K,
 	                on(T,N,N1), not move(T,N1), TP1=T+1.

% Goal description
	 :- not on(K,N,N1), onG(K,N,N1), steps(K).
	 :- on(K,N,N1), not onG(K,N,N1),steps(K).

% Solution
	 put(T,M,N) :- move(T,N), where(T,M), steps(K), T<K.

