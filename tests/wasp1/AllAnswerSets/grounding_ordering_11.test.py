input = """
male(john).    republican(john).
male(matt).    republican(matt).
female(joana). republican(joana).
female(luise). democrat(luise).

moreMaleRepublicans :- 
   #count{X:republican(X), female(X)} < N, 
   #count{Y: republican(Y), male(Y)}  = N. 
  
"""

output = """
{democrat(luise), female(joana), female(luise), male(john), male(matt), moreMaleRepublicans, republican(joana), republican(john), republican(matt)}
"""
