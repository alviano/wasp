input = """
meets_conditions(S,R,P):- component(P,R),
                    var_k(S,X), var_k(R,Y), not var_k(S,Y), not var_k(R,X). 

component(P,R):- var1(P,X), k_vertex1(R,_), not var_k(R,X).
component(query,root).

var_k(S,X):- k_vertex1(S,P), var1(P,X).

k_vertex1("a",a).
k_vertex1("c",c).
k_vertex1("d",d).
k_vertex1("e",e).
k_vertex1("a_c",a).
k_vertex1("a_c",c).
k_vertex1("a_d",a).
k_vertex1("a_d",d).
k_vertex1("a_e",a).
k_vertex1("a_e",e).
k_vertex1("c_d",c).
k_vertex1("c_d",d).
k_vertex1("c_e",c).
k_vertex1("c_e",e).
k_vertex1("d_e",d).
k_vertex1("d_e",e).
k_vertex1("a_c_d",a).
k_vertex1("a_c_d",c).
k_vertex1("a_c_d",d).
k_vertex1("a_c_e",a).
k_vertex1("a_c_e",c).
k_vertex1("a_c_e",e).
k_vertex1("a_d_e",a).
k_vertex1("a_d_e",d).
k_vertex1("a_d_e",e).
k_vertex1("c_d_e",c).
k_vertex1("c_d_e",d).
k_vertex1("c_d_e",e).
var1(a,x1).
var1(a,x2).
var1(a,x4).
var1(a,x5).
var1(a,xp1).
var1(a,xp2).
var1(a,xp4).
var1(a,xp5).
var1(c,x4).
var1(c,x5).
var1(c,xp4).
var1(c,xp5).
var1(c,y).
var1(c,x6).
var1(c,xp6).
var1(d,x1).
var1(d,x4).
var1(d,z).
var1(e,x2).
var1(e,x5).
var1(e,z).

"""

output = """
{component(a,"c"), component(a,"c_d"), component(a,"c_d_e"), component(a,"c_e"), component(a,"d"), component(a,"d_e"), component(a,"e"), component(c,"a"), component(c,"a_d"), component(c,"a_d_e"), component(c,"a_e"), component(c,"d"), component(c,"d_e"), component(c,"e"), component(d,"a"), component(d,"a_c"), component(d,"c"), component(d,"c_e"), component(d,"e"), component(e,"a"), component(e,"a_c"), component(e,"c"), component(e,"c_d"), component(e,"d"), component(query,root), k_vertex1("a",a), k_vertex1("a_c",a), k_vertex1("a_c",c), k_vertex1("a_c_d",a), k_vertex1("a_c_d",c), k_vertex1("a_c_d",d), k_vertex1("a_c_e",a), k_vertex1("a_c_e",c), k_vertex1("a_c_e",e), k_vertex1("a_d",a), k_vertex1("a_d",d), k_vertex1("a_d_e",a), k_vertex1("a_d_e",d), k_vertex1("a_d_e",e), k_vertex1("a_e",a), k_vertex1("a_e",e), k_vertex1("c",c), k_vertex1("c_d",c), k_vertex1("c_d",d), k_vertex1("c_d_e",c), k_vertex1("c_d_e",d), k_vertex1("c_d_e",e), k_vertex1("c_e",c), k_vertex1("c_e",e), k_vertex1("d",d), k_vertex1("d_e",d), k_vertex1("d_e",e), k_vertex1("e",e), meets_conditions("a","c",a), meets_conditions("a","c",d), meets_conditions("a","c",e), meets_conditions("a","c_d",a), meets_conditions("a","c_d",e), meets_conditions("a","c_d_e",a), meets_conditions("a","c_e",a), meets_conditions("a","c_e",d), meets_conditions("a","d",a), meets_conditions("a","d",c), meets_conditions("a","d",e), meets_conditions("a","d_e",a), meets_conditions("a","d_e",c), meets_conditions("a","e",a), meets_conditions("a","e",c), meets_conditions("a","e",d), meets_conditions("a_c","a_d",c), meets_conditions("a_c","a_d_e",c), meets_conditions("a_c","a_e",c), meets_conditions("a_c","c_d",a), meets_conditions("a_c","c_d",e), meets_conditions("a_c","c_d_e",a), meets_conditions("a_c","c_e",a), meets_conditions("a_c","c_e",d), meets_conditions("a_c","d",a), meets_conditions("a_c","d",c), meets_conditions("a_c","d",e), meets_conditions("a_c","d_e",a), meets_conditions("a_c","d_e",c), meets_conditions("a_c","e",a), meets_conditions("a_c","e",c), meets_conditions("a_c","e",d), meets_conditions("a_d","a_c",d), meets_conditions("a_d","a_c",e), meets_conditions("a_d","c",a), meets_conditions("a_d","c",d), meets_conditions("a_d","c",e), meets_conditions("a_d","c_d",a), meets_conditions("a_d","c_d",e), meets_conditions("a_d","c_d_e",a), meets_conditions("a_d","c_e",a), meets_conditions("a_d","c_e",d), meets_conditions("a_d_e","a_c",d), meets_conditions("a_d_e","a_c",e), meets_conditions("a_d_e","c",a), meets_conditions("a_d_e","c",d), meets_conditions("a_d_e","c",e), meets_conditions("a_d_e","c_d",a), meets_conditions("a_d_e","c_d",e), meets_conditions("a_d_e","c_d_e",a), meets_conditions("a_d_e","c_e",a), meets_conditions("a_d_e","c_e",d), meets_conditions("a_e","a_c",d), meets_conditions("a_e","a_c",e), meets_conditions("a_e","c",a), meets_conditions("a_e","c",d), meets_conditions("a_e","c",e), meets_conditions("a_e","c_d",a), meets_conditions("a_e","c_d",e), meets_conditions("a_e","c_d_e",a), meets_conditions("a_e","c_e",a), meets_conditions("a_e","c_e",d), meets_conditions("c","a",c), meets_conditions("c","a",d), meets_conditions("c","a",e), meets_conditions("c","a_d",c), meets_conditions("c","a_d_e",c), meets_conditions("c","a_e",c), meets_conditions("c","d",a), meets_conditions("c","d",c), meets_conditions("c","d",e), meets_conditions("c","d_e",a), meets_conditions("c","d_e",c), meets_conditions("c","e",a), meets_conditions("c","e",c), meets_conditions("c","e",d), meets_conditions("c_d","a",c), meets_conditions("c_d","a",d), meets_conditions("c_d","a",e), meets_conditions("c_d","a_c",d), meets_conditions("c_d","a_c",e), meets_conditions("c_d","a_d",c), meets_conditions("c_d","a_d_e",c), meets_conditions("c_d","a_e",c), meets_conditions("c_d","c_e",a), meets_conditions("c_d","c_e",d), meets_conditions("c_d","d_e",a), meets_conditions("c_d","d_e",c), meets_conditions("c_d","e",a), meets_conditions("c_d","e",c), meets_conditions("c_d","e",d), meets_conditions("c_d_e","a",c), meets_conditions("c_d_e","a",d), meets_conditions("c_d_e","a",e), meets_conditions("c_d_e","a_c",d), meets_conditions("c_d_e","a_c",e), meets_conditions("c_d_e","a_d",c), meets_conditions("c_d_e","a_d_e",c), meets_conditions("c_d_e","a_e",c), meets_conditions("c_e","a",c), meets_conditions("c_e","a",d), meets_conditions("c_e","a",e), meets_conditions("c_e","a_c",d), meets_conditions("c_e","a_c",e), meets_conditions("c_e","a_d",c), meets_conditions("c_e","a_d_e",c), meets_conditions("c_e","a_e",c), meets_conditions("c_e","c_d",a), meets_conditions("c_e","c_d",e), meets_conditions("c_e","d",a), meets_conditions("c_e","d",c), meets_conditions("c_e","d",e), meets_conditions("c_e","d_e",a), meets_conditions("c_e","d_e",c), meets_conditions("d","a",c), meets_conditions("d","a",d), meets_conditions("d","a",e), meets_conditions("d","a_c",d), meets_conditions("d","a_c",e), meets_conditions("d","c",a), meets_conditions("d","c",d), meets_conditions("d","c",e), meets_conditions("d","c_e",a), meets_conditions("d","c_e",d), meets_conditions("d","e",a), meets_conditions("d","e",c), meets_conditions("d","e",d), meets_conditions("d_e","a",c), meets_conditions("d_e","a",d), meets_conditions("d_e","a",e), meets_conditions("d_e","a_c",d), meets_conditions("d_e","a_c",e), meets_conditions("d_e","c",a), meets_conditions("d_e","c",d), meets_conditions("d_e","c",e), meets_conditions("d_e","c_d",a), meets_conditions("d_e","c_d",e), meets_conditions("d_e","c_e",a), meets_conditions("d_e","c_e",d), meets_conditions("e","a",c), meets_conditions("e","a",d), meets_conditions("e","a",e), meets_conditions("e","a_c",d), meets_conditions("e","a_c",e), meets_conditions("e","c",a), meets_conditions("e","c",d), meets_conditions("e","c",e), meets_conditions("e","c_d",a), meets_conditions("e","c_d",e), meets_conditions("e","d",a), meets_conditions("e","d",c), meets_conditions("e","d",e), var1(a,x1), var1(a,x2), var1(a,x4), var1(a,x5), var1(a,xp1), var1(a,xp2), var1(a,xp4), var1(a,xp5), var1(c,x4), var1(c,x5), var1(c,x6), var1(c,xp4), var1(c,xp5), var1(c,xp6), var1(c,y), var1(d,x1), var1(d,x4), var1(d,z), var1(e,x2), var1(e,x5), var1(e,z), var_k("a",x1), var_k("a",x2), var_k("a",x4), var_k("a",x5), var_k("a",xp1), var_k("a",xp2), var_k("a",xp4), var_k("a",xp5), var_k("a_c",x1), var_k("a_c",x2), var_k("a_c",x4), var_k("a_c",x5), var_k("a_c",x6), var_k("a_c",xp1), var_k("a_c",xp2), var_k("a_c",xp4), var_k("a_c",xp5), var_k("a_c",xp6), var_k("a_c",y), var_k("a_c_d",x1), var_k("a_c_d",x2), var_k("a_c_d",x4), var_k("a_c_d",x5), var_k("a_c_d",x6), var_k("a_c_d",xp1), var_k("a_c_d",xp2), var_k("a_c_d",xp4), var_k("a_c_d",xp5), var_k("a_c_d",xp6), var_k("a_c_d",y), var_k("a_c_d",z), var_k("a_c_e",x1), var_k("a_c_e",x2), var_k("a_c_e",x4), var_k("a_c_e",x5), var_k("a_c_e",x6), var_k("a_c_e",xp1), var_k("a_c_e",xp2), var_k("a_c_e",xp4), var_k("a_c_e",xp5), var_k("a_c_e",xp6), var_k("a_c_e",y), var_k("a_c_e",z), var_k("a_d",x1), var_k("a_d",x2), var_k("a_d",x4), var_k("a_d",x5), var_k("a_d",xp1), var_k("a_d",xp2), var_k("a_d",xp4), var_k("a_d",xp5), var_k("a_d",z), var_k("a_d_e",x1), var_k("a_d_e",x2), var_k("a_d_e",x4), var_k("a_d_e",x5), var_k("a_d_e",xp1), var_k("a_d_e",xp2), var_k("a_d_e",xp4), var_k("a_d_e",xp5), var_k("a_d_e",z), var_k("a_e",x1), var_k("a_e",x2), var_k("a_e",x4), var_k("a_e",x5), var_k("a_e",xp1), var_k("a_e",xp2), var_k("a_e",xp4), var_k("a_e",xp5), var_k("a_e",z), var_k("c",x4), var_k("c",x5), var_k("c",x6), var_k("c",xp4), var_k("c",xp5), var_k("c",xp6), var_k("c",y), var_k("c_d",x1), var_k("c_d",x4), var_k("c_d",x5), var_k("c_d",x6), var_k("c_d",xp4), var_k("c_d",xp5), var_k("c_d",xp6), var_k("c_d",y), var_k("c_d",z), var_k("c_d_e",x1), var_k("c_d_e",x2), var_k("c_d_e",x4), var_k("c_d_e",x5), var_k("c_d_e",x6), var_k("c_d_e",xp4), var_k("c_d_e",xp5), var_k("c_d_e",xp6), var_k("c_d_e",y), var_k("c_d_e",z), var_k("c_e",x2), var_k("c_e",x4), var_k("c_e",x5), var_k("c_e",x6), var_k("c_e",xp4), var_k("c_e",xp5), var_k("c_e",xp6), var_k("c_e",y), var_k("c_e",z), var_k("d",x1), var_k("d",x4), var_k("d",z), var_k("d_e",x1), var_k("d_e",x2), var_k("d_e",x4), var_k("d_e",x5), var_k("d_e",z), var_k("e",x2), var_k("e",x5), var_k("e",z)}
"""
