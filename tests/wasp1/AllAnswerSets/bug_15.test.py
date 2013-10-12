input = """
% There must not be a grounded rule with "strat(0)" in the head!

 true. 

 prod(pasta,barilla,saiwa). 
 prod(tomatoes,frutto,barilla). 
 prod(wine,barilla,0). 
 prod(bread,saiwa,panino). 

 cont(frutto,barilla,saiwa,0).

% to produce X, Y or Z must be strategic 

 strat(Y) v strat(Z) :- prod(X,Y,Z).

% W is strategic, if it is controlled by strategic 
% companies X, Y, and Z  

 strat(W) :- cont(W,X,Y,Z), Strat(X), Strat(Y), Strat(Z). 

% handle special 0 symbol 

 Strat(X) :- strat(X).
 strat(0) :- true. 


 
"""

output = """
{Strat(0), Strat(barilla), Strat(panino), cont(frutto,barilla,saiwa,0), prod(bread,saiwa,panino), prod(pasta,barilla,saiwa), prod(tomatoes,frutto,barilla), prod(wine,barilla,0), strat(0), strat(barilla), strat(panino), true}
{Strat(0), Strat(frutto), Strat(saiwa), cont(frutto,barilla,saiwa,0), prod(bread,saiwa,panino), prod(pasta,barilla,saiwa), prod(tomatoes,frutto,barilla), prod(wine,barilla,0), strat(0), strat(frutto), strat(saiwa), true}
"""
