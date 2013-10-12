input = """
instance(perGrant,s_Actor).
att_val(perGrant,name,nameCG).
att_val(perGrant,name,nameGrant).
att_val(perGrant,birthDate,1904).
att_val(perGrant,dateOfDeath,1986).

instance(nameCG,name).
att_val(nameCG,firstName,"Cary").
att_val(nameCG,lastName,"Grant").

instance(nameGrant,name).
att_val(nameGrant,firstName,"Archibald").
att_val(nameGrant,firstName,"Alec").
att_val(nameGrant,lastName,"Leach").

instance(m12,s_Movie).
att_val(m12,title,"Arsenic and Old Lace").
att_val(m12,releaseDate,1944).
acted(perGrant,m12).

involved(P,M) :- acted(P,M).

matchingMovie(q1, m12).


inferred_topic(X5, X1) :- matchingMovie(X5, X4), involved(X3, X4), att_val(X3, name, X2), att_val(X2, lastName, X1).

inferred_topic(X5, X1) :- matchingMovie(X5, X4), involved(X3, X4), att_val(X3, name, X2), att_val(X2, lastName, X1).
"""

output = """
{acted(perGrant,m12), att_val(m12,releaseDate,1944), att_val(m12,title,"Arsenic and Old Lace"), att_val(nameCG,firstName,"Cary"), att_val(nameCG,lastName,"Grant"), att_val(nameGrant,firstName,"Alec"), att_val(nameGrant,firstName,"Archibald"), att_val(nameGrant,lastName,"Leach"), att_val(perGrant,birthDate,1904), att_val(perGrant,dateOfDeath,1986), att_val(perGrant,name,nameCG), att_val(perGrant,name,nameGrant), inferred_topic(q1,"Grant"), inferred_topic(q1,"Leach"), instance(m12,s_Movie), instance(nameCG,name), instance(nameGrant,name), instance(perGrant,s_Actor), involved(perGrant,m12), matchingMovie(q1,m12)}
"""
