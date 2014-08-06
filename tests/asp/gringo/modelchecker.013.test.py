input = """
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Boolean 3-colorability
%    ======================  
%
%   A generalization of graph 3-uncolorability (due to 
%   I. Stewart) which has higher complexity than graph
%   3-uncolorability:  \Sigma^p_2 complete
%   
%   Instance: Graph G, whose edges are labeled with nonempty 
%             sets of propositional literals on variables 
%             v1,...,vn
%   Question: Does there exist a truth assignment to v1,...,vn
%             such that the subgraph of G, whose edges are those
%             edges e from G such that at least on literal in
%             the label of e is satisfied, is not 3-colorable. 
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Database for representing labeled graph G: 
%
% Facts pos(X,Y,V): edge between X and Y exists and has V 
%                   in its label set.
% Facts neg(X,Y,V): edge between X and Y exists and has neg V 
%                   in its label set.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Example database: clique on 4 vertices 0,...,3 each of which
%                   has label set { v1 }. This is a Yes-Instance.


 pos(0,1,v1).
 pos(0,1,v2).
 pos(1,2,v1).
 pos(2,0,v1).
 pos(0,3,v1).
 pos(1,3,v1).
 pos(2,3,v1). 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%
% This is the program ....
%
 
%  v(X) ... set of vertices, extracted from ``pos'' and ``neg''. 
  
 v(X) :- pos(X,Y,V). 
 v(Y) :- pos(X,Y,V). 
 v(X) :- neg(X,Y,V). 
 v(Y) :- neg(X,Y,V). 

% bool(V) ... set of propositional variables. 

 bool(V) :- pos(X,Y,V). 
 bool(V) :- neg(X,Y,V). 

% Assign each variable V a truth value  
 
 t(V) | f(V) :- bool(V).

% Assign each node X a color from red, green, blue.  
 
 c(X,red) | c(X,green) | c(X,blue) :- v(X). 

% if w is true, then every node gets all colors (this is illegal)
 
   c(X,red) :- w, v(X). 
 c(X,green) :- w, v(X). 
  c(X,blue) :- w, v(X). 

% if the endpoints of a selected edge are colored the same, then 
% w is true 
  
 w :- pos(X,Y,V), t(V), c(X,A), c(Y,A). 
 w :- neg(X,Y,V), f(V), c(X,A), c(Y,A). 

% select stable models containing w; these correspond to proper
% assignments to v1,...,vn showing that G is a Yes-Instance. 
 
 :- not w.
"""
output = """
{pos(0,1,v1), pos(0,1,v2), pos(0,3,v1), pos(1,2,v1), pos(1,3,v1), pos(2,0,v1), pos(2,3,v1), v(0), v(1), v(2), v(3), bool(v1), bool(v2), t(v1), t(v2), c(0,red), c(0,green), c(0,blue), c(1,red), c(1,green), c(1,blue), c(2,red), c(2,green), c(2,blue), c(3,red), c(3,green), c(3,blue), w}
{pos(0,1,v1), pos(0,1,v2), pos(0,3,v1), pos(1,2,v1), pos(1,3,v1), pos(2,0,v1), pos(2,3,v1), v(0), v(1), v(2), v(3), bool(v1), bool(v2), t(v1), f(v2), c(0,red), c(0,green), c(0,blue), c(1,red), c(1,green), c(1,blue), c(2,red), c(2,green), c(2,blue), c(3,red), c(3,green), c(3,blue), w}
"""
