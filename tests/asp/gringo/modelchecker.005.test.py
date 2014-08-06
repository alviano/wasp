input = """
% This is a meta-interpreter by Axel Polleres, originally split over several
% files, which detected a problem with our extraction of unfounded sets from
% model candidates.  Fixed by revision 1.20 of satz.C.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2qbf_5_5_20c.check1

firstRule(1).
lit(h,"y6",1) :- .
lit(h,"-y6",1) :- .
atom("y6","y6").
atom("-y6","y6").
notok :- ninS("y6"),ninS("-y6").
rule(1). nextRule(1,2).
lit(h,"y7",2) :- .
lit(h,"-y7",2) :- .
atom("y7","y7").
atom("-y7","y7").
notok :- ninS("y7"),ninS("-y7").
rule(2). nextRule(2,3).
lit(h,"y8",3) :- .
lit(h,"-y8",3) :- .
atom("y8","y8").
atom("-y8","y8").
notok :- ninS("y8"),ninS("-y8").
rule(3). nextRule(3,4).
lit(h,"y9",4) :- .
lit(h,"-y9",4) :- .
atom("y9","y9").
atom("-y9","y9").
notok :- ninS("y9"),ninS("-y9").
rule(4). nextRule(4,5).
lit(h,"y10",5) :- .
lit(h,"-y10",5) :- .
atom("y10","y10").
atom("-y10","y10").
notok :- ninS("y10"),ninS("-y10").
rule(5). nextRule(5,6).
lit(h,"w",6) :- x2.
atom("w","w").
lit(p,"-y7",6) :- x2.
lit(p,"-y6",6) :- x2.
inS("w") :- inS("-y7"),inS("-y6"),x2.
rule(6). nextRule(6,7).
lit(h,"w",7) :- x2.
atom("w","w").
lit(p,"-y10",7) :- x2.
lit(p,"-y6",7) :- x2.
inS("w") :- inS("-y10"),inS("-y6"),x2.
rule(7). nextRule(7,8).
lit(h,"w",8) :- x3,x2.
atom("w","w").
lit(p,"-y10",8) :- x3,x2.
inS("w") :- inS("-y10"),x3,x2.
rule(8). nextRule(8,9).
lit(h,"w",9) :- x2.
atom("w","w").
lit(p,"-y7",9) :- x2.
lit(p,"-y8",9) :- x2.
inS("w") :- inS("-y7"),inS("-y8"),x2.
rule(9). nextRule(9,10).
lit(h,"w",10) :- -x5,x4.
atom("w","w").
lit(p,"-y6",10) :- -x5,x4.
inS("w") :- inS("-y6"),-x5,x4.
rule(10). nextRule(10,11).
lit(h,"w",11) :- -x5.
atom("w","w").
lit(p,"y8",11) :- -x5.
lit(p,"y9",11) :- -x5.
inS("w") :- inS("y8"),inS("y9"),-x5.
rule(11). nextRule(11,12).
lit(h,"w",12) :- -x3.
atom("w","w").
lit(p,"-y9",12) :- -x3.
lit(p,"-y6",12) :- -x3.
inS("w") :- inS("-y9"),inS("-y6"),-x3.
rule(12). nextRule(12,13).
lit(h,"w",13) :- x1,-x4.
atom("w","w").
lit(p,"y6",13) :- x1,-x4.
inS("w") :- inS("y6"),x1,-x4.
rule(13). nextRule(13,14).
lit(h,"w",14) :- -x1.
atom("w","w").
lit(p,"y8",14) :- -x1.
lit(p,"y6",14) :- -x1.
inS("w") :- inS("y8"),inS("y6"),-x1.
rule(14). nextRule(14,15).
lit(h,"w",15) :- -x5,x4.
atom("w","w").
lit(p,"-y10",15) :- -x5,x4.
inS("w") :- inS("-y10"),-x5,x4.
rule(15). nextRule(15,16).
lit(h,"w",16) :- -x4.
atom("w","w").
lit(p,"y10",16) :- -x4.
lit(p,"y8",16) :- -x4.
inS("w") :- inS("y10"),inS("y8"),-x4.
rule(16). nextRule(16,17).
lit(h,"w",17) :- x4,-x2.
atom("w","w").
lit(p,"-y9",17) :- x4,-x2.
inS("w") :- inS("-y9"),x4,-x2.
rule(17). nextRule(17,18).
lit(h,"w",18) :- .
atom("w","w").
lit(p,"y8",18) :- .
lit(p,"y10",18) :- .
lit(p,"y7",18) :- .
inS("w") :- inS("y8"),inS("y10"),inS("y7").
rule(18). nextRule(18,19).
lit(h,"w",19) :- -x3,-x1.
atom("w","w").
lit(p,"y7",19) :- -x3,-x1.
inS("w") :- inS("y7"),-x3,-x1.
rule(19). nextRule(19,20).
lit(h,"w",20) :- -x2,-x5.
atom("w","w").
lit(p,"-y9",20) :- -x2,-x5.
inS("w") :- inS("-y9"),-x2,-x5.
rule(20). nextRule(20,21).
lit(h,"w",21) :- x4.
atom("w","w").
lit(p,"y10",21) :- x4.
lit(p,"-y7",21) :- x4.
inS("w") :- inS("y10"),inS("-y7"),x4.
rule(21). nextRule(21,22).
lit(h,"w",22) :- .
atom("w","w").
lit(p,"y8",22) :- .
lit(p,"y10",22) :- .
lit(p,"y6",22) :- .
inS("w") :- inS("y8"),inS("y10"),inS("y6").
rule(22). nextRule(22,23).
lit(h,"w",23) :- x4.
atom("w","w").
lit(p,"-y7",23) :- x4.
lit(p,"y8",23) :- x4.
inS("w") :- inS("-y7"),inS("y8"),x4.
rule(23). nextRule(23,24).
lit(h,"w",24) :- -x5.
atom("w","w").
lit(p,"-y6",24) :- -x5.
lit(p,"-y9",24) :- -x5.
inS("w") :- inS("-y6"),inS("-y9"),-x5.
rule(24). nextRule(24,25).
lit(h,"w",25) :- -x3.
atom("w","w").
lit(p,"-y8",25) :- -x3.
lit(p,"-y7",25) :- -x3.
inS("w") :- inS("-y8"),inS("-y7"),-x3.
rule(25). nextRule(25,26).
lit(n,"w",26) :- .
notok :- ninS("w").

rule(26). lastRule(26).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2qbf_5_5_20c.guess

x1 | -x1.
x2 | -x2.
x3 | -x3.
x4 | -x4.
x5 | -x5.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Pi_meta_opt

hlit(L) :- lit(h,L,R).
inS(L) | ninS(L) :- hlit(L).
ninS(L) :- lit(HPN,L,R), not hlit(L).
notok :- inS(L), inS(NL), L != NL, atom(L,A), atom(NL,A).
dep(L,L1) :- lit(h,L,R),lit(p,L1,R),inS(L),inS(L1).
dep(L,L2) :- lit(h,L,R),lit(p,L1,R),dep(L1,L2),inS(L).

phi(L,L1) | phi(L1,L) :- dep(L,L1), dep(L1,L), L < L1.
phi(L,L2) :- phi(L,L1),phi(L1,L2).
failsToProve(L,R) :- lit(h,L,R), lit(p,L1,R), ninS(L1).
failsToProve(L,R) :- lit(h,L,R), lit(n,L1,R), inS(L1).
failsToProve(L,R) :-  not lit(h,L,R), rule(R), lit(HPN,L,R1).
failsToProve(L,R) :- lit(h,L1,R), inS(L1), L1 != L, inS(L).
failsToProve(L,R) :- lit(p,L1,R), phi(L1,L).
failsToProve(L,R) :- lit(p,L,R).
allFailUpto(L,R) :- failsToProve(L,R), firstRule(R).
allFailUpto(L,R1) :- failsToProve(L,R1), allFailUpto(L,R), nextRule(R,R1).
notok :-  allFailUpto(L,R), lastRule(R), inS(L).
phi(L,L1) :- notok, inS(L), inS(L1).
inS(L)     :- notok, lit(HPN,L,R).
ninS(L)    :- notok, lit(HPN,L,R).

:- not notok.
"""
output = """
"""
