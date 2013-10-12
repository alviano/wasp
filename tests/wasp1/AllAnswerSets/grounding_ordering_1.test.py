input = """
#maxint=5.

maxEvol(2).

evolution(0,n,1,n,5,n,2,n,3,n,4,
            n,1,n,4,p,2,p,3,n,5,
            p,1,n,3,p,5,p,2,p,4).

evolNumber(I) :- #int(I), I>0,  maxEvol(Max), I<=Max.

evolution(II,Z1,1,ZQ,Q,ZT,T,ZU,U,ZV,V,
            T1,1,TF,F,TG,G,TH,H,TI,I,
            U1,1,UL,L,UM,M,UO,O,UP,P) :-
        evolNumber(II),
        #succ(JJ,II),
        evolution(JJ,S1,1,SB,B,SC,C,SD,D,SE,E,
                    T1,1,TF,F,TG,G,TH,H,TI,I,
                    U1,1,UL,L,UM,M,UO,O,UP,P),
        inversion(S1,1,SB,B,SC,C,SD,D,SE,E,
Z1,1,ZQ,Q,ZT,T,ZU,U,ZV,V).

inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,2,p,3,n,5,p,4).
inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,3,n,2,n,5,p,4).
inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,4,p,5,n,3,n,2).

"""

output = """
{evolNumber(1), evolNumber(2), evolution(0,n,1,n,5,n,2,n,3,n,4,n,1,n,4,p,2,p,3,n,5,p,1,n,3,p,5,p,2,p,4), inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,2,p,3,n,5,p,4), inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,3,n,2,n,5,p,4), inversion(p,1,p,2,p,3,n,5,p,4,p,1,n,4,p,5,n,3,n,2), maxEvol(2)}
"""
