{a;b;c;d;e;f}.

:- a, not b, c, d.

:- not c, d, e, f.
:- not c, d, e, not f.
:- not c, d, not e, f.
:- not c, d, not e, not f.

:- c, not d, e, f.
:- c, not d, e, not f.
:- c, not d, not e, f.
:- c, not d, not e, not f.

:- not c, not d, e, f.
:- not c, not d, e, not f.
:- not c, not d, not e, f.
:- not c, not d, not e, not f.
