input = """

pred0(1,1) v pred1(1,1).
ntables(2).    % number of tables
nchairs(1).    % number of chairs
npersons(2).  % number of people

#maxint = 2.  % number of people


person(1).
person(2).
table(1).
table(2).

at(P,T) v not_at(P,T) :- person(P), table(T).

%so many people as many chairs per each table.
:- not #count { P: at(P,T) } > X, not #count{D: at(D,T)} > X1, X1=X-1, table(T), nchairs(X).

%each person only at 1 table per time!
%:- #count{T : at(P,T)} < 1, person(P).
:- #count{T : at(P,T)} > 1, person(P).

:- not at(1,1).
"""

output = """
{at(1,1), at(2,2), nchairs(1), not_at(1,2), not_at(2,1), npersons(2), ntables(2), person(1), person(2), pred0(1,1), table(1), table(2)}
{at(1,1), at(2,2), nchairs(1), not_at(1,2), not_at(2,1), npersons(2), ntables(2), person(1), person(2), pred1(1,1), table(1), table(2)}
"""
