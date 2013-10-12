input = """
att_val(perGrant,name,nameCG).
att_val(perGrant,name,nameGrant).

att_val(nameCG,lastName,"Grant").

att_val(nameGrant,lastName,"Leach").

acted(perGrant,m12).

involved(P,M) :- acted(P,M).

matchingMovie(q1, m12).


inferred_topic(X5, X1) :- matchingMovie(X5, X4), involved(X3, X4), att_val(X3, name, X2), att_val(X2, lastName, X1).

"""

output = """
{acted(perGrant,m12), att_val(nameCG,lastName,"Grant"), att_val(nameGrant,lastName,"Leach"), att_val(perGrant,name,nameCG), att_val(perGrant,name,nameGrant), inferred_topic(q1,"Grant"), inferred_topic(q1,"Leach"), involved(perGrant,m12), matchingMovie(q1,m12)}
"""
