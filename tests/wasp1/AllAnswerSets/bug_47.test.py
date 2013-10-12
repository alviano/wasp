input = """
p(a,b,b).
f(q,q,q,q).
q(Anno,V) :- p(Id,"VIBO VALENZIA",_), f(Id,_,Anno,V).

"""

output = """
{f(q,q,q,q), p(a,b,b)}
"""
