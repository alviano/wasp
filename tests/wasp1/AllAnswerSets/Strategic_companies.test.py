input = """
% Strategic Companies
%
% As we want to produce X, Y or Z must be strategic.

 strategic(Y) v strategic(Z) :- produced_by(X,Y,Z).

% W is strategic, if it is controlled by strategic 
% companies X, Y, and Z  

 strategic(W) :- controlled_by(W,X,Y,Z), 
                 Strategic(X), Strategic(Y), Strategic(Z). 

% Handle special 0 symbol 

 :- strategic(0). 
 Strategic(X) :- strategic(X).
 Strategic(0) :- true. 
 true. 
 produced_by(pasta   , barilla ,saiwa). 
 produced_by(tomatoes, frutto  ,barilla). 
 produced_by(wine    , barilla ,0). 
 produced_by(bread   , saiwa   ,panino). 
 produced_by(beer    , heineken,budweiser). 

 controlled_by(frutto   , barilla  ,saiwa,0).
 controlled_by(budweiser, heineken ,0    ,0). 
 controlled_by(heineken , saiwa    ,0    ,0).
%controlled_by(saiwa    , budweiser,0    ,0). 
"""

output = """
{Strategic(0), Strategic(barilla), Strategic(budweiser), Strategic(frutto), Strategic(heineken), Strategic(saiwa), controlled_by(budweiser,heineken,0,0), controlled_by(frutto,barilla,saiwa,0), controlled_by(heineken,saiwa,0,0), produced_by(beer,heineken,budweiser), produced_by(bread,saiwa,panino), produced_by(pasta,barilla,saiwa), produced_by(tomatoes,frutto,barilla), produced_by(wine,barilla,0), strategic(barilla), strategic(budweiser), strategic(frutto), strategic(heineken), strategic(saiwa), true}
{Strategic(0), Strategic(barilla), Strategic(budweiser), Strategic(panino), controlled_by(budweiser,heineken,0,0), controlled_by(frutto,barilla,saiwa,0), controlled_by(heineken,saiwa,0,0), produced_by(beer,heineken,budweiser), produced_by(bread,saiwa,panino), produced_by(pasta,barilla,saiwa), produced_by(tomatoes,frutto,barilla), produced_by(wine,barilla,0), strategic(barilla), strategic(budweiser), strategic(panino), true}
"""
