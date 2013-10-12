input = """
% Different kind of aggregates with ground guards
% defined in the body of rules and strong constraints. 
% Atoms named "okay" have to be true in the computed stable models,
% atoms named "ouch" have to be false.

d(1).
d(2).
d(3).
p.

%---- #count{...} op integer ----(only)

okay1 :- #sum{V : d(V)} = 6.              
 
ouch1 :- #sum{V : d(V)} = 5.              
ouch1 :- #sum{V : d(V)} = 7. 


okay2 :- #sum{V : d(V)} < 7.              

ouch2 :- #sum{V : d(V)} < 6.              

 
okay3 :- #sum{V : d(V)} <= 6.             

ouch3 :- #sum{V : d(V)} <= 5.             


okay4 :- #sum{V : d(V)} > 5.              

ouch4 :- #sum{V : d(V)} > 6.              


okay5 :- #sum{V : d(V)} >= 6.             

ouch5 :- #sum{V : d(V)} >= 7.             

%----  #sum{...} op integer ----(at the end)

okay6 :- p, #sum{V : d(V)} = 6.           

ouch6 :- p, #sum{V : d(V)} = 5.
ouch6 :- p, #sum{V : d(V)} = 7.           


okay7 :- p, #sum{V : d(V)} < 7.           

ouch7 :- p, #sum{V : d(V)} < 6.           


okay8 :- p, #sum{V : d(V)} <= 6.          

ouch8 :- p, #sum{V : d(V)} <= 5.          


okay9 :- p, #sum{V : d(V)} > 5.           

ouch9 :- p, #sum{V : d(V)} > 6.           


okay10 :- p, #sum{V : d(V)} >= 6.          

ouch10 :- p, #sum{V : d(V)} >= 7.          

%----  #sum{...} op integer ----(at the beginning)

okay11 :- #sum{V : d(V)} = 6, p.           

ouch11 :- #sum{V : d(V)} = 5, p.           
ouch11 :- #sum{V : d(V)} = 7, p.


okay12 :- #sum{V : d(V)} < 7, p.           

ouch12 :- #sum{V : d(V)} < 6, p.           


okay13 :- #sum{V : d(V)} <= 6, p.          

ouch13 :- #sum{V : d(V)} <= 5, p.          


okay14 :- #sum{V : d(V)} > 5, p.           

ouch14 :- #sum{V : d(V)} > 6, p.           


okay15 :- #sum{V : d(V)} >= 6, p.          

ouch15 :- #sum{V : d(V)} >= 7, p.          

%---- integer op #sum{...} ----(only)

okay16 :- 6 = #sum{V : d(V)}.              

ouch16 :- 5 = #sum{V : d(V)}.              
ouch16 :- 7 = #sum{V : d(V)}. 


okay17 :- 5 < #sum{V : d(V)}.              

ouch17 :- 6 < #sum{V : d(V)}.              


okay18 :- 6 <= #sum{V : d(V)}.             

ouch18 :- 7 <= #sum{V : d(V)}.             


okay19 :- 7 > #sum{V : d(V)}.              

ouch19 :- 6 > #sum{V : d(V)}.              


okay20 :- 6 >= #sum{V : d(V)}.             

ouch20 :- 5 >= #sum{V : d(V)}.             

%----  integer op #sum{...}----(at the end) 

okay21 :- p, 6 = #sum{V : d(V)}.           

ouch21 :- p, 5 = #sum{V : d(V)}.           
ouch21 :- p, 7 = #sum{V : d(V)}.


okay22 :- p, 5 < #sum{V : d(V)}.           

ouch22 :- p, 6 < #sum{V : d(V)}.           


okay23 :- p, 6 <= #sum{V : d(V)}.           

ouch23 :- p, 7 <= #sum{V : d(V)}.          


okay24 :- p, 7 > #sum{V : d(V)}.           

ouch24 :- p, 6 > #sum{V : d(V)}.           


okay25 :- p, 6 >= #sum{V : d(V)}.          

ouch25 :- p, 5 >= #sum{V : d(V)}.          

%----  integer op #sum{...}---- (at the beginning)

okay26 :- 6 = #sum{V : d(V)}, p.          

ouch27 :- 5 = #sum{V : d(V)}, p.          
ouch27 :- 7 = #sum{V : d(V)}, p. 


okay28 :- 5 < #sum{V : d(V)}, p.         

ouch28 :- 6 < #sum{V : d(V)}, p.          


okay29 :- 6 <= #sum{V : d(V)}, p.         

ouch29 :- 7 <= #sum{V : d(V)}, p.         


okay30 :- 7 > #sum{V : d(V)}, p.          

ouch30 :- 6 > #sum{V : d(V)}, p.          


okay31 :- 6 >= #sum{V : d(V)}, p.         

ouch31 :- 5 >= #sum{V : d(V)}, p.         

%---- integer < #sum{...} < integer ----(only)

okay32 :- 5 < #sum{V : d(V)} < 7.         

ouch32 :- 5 < #sum{V : d(V)} < 6.          
ouch32 :- 6 < #sum{V : d(V)} < 7.  

%---- integer < #sum{(...} < integer ----(at the end)

okay33 :- p, 5 < #sum{V : d(V)} < 7.      

ouch33 :- p, 5 < #sum{V : d(V)} < 6.      
ouch33 :- p, 6 < #sum{V : d(V)} < 7.      

%---- integer < #sum{(...} < integer ----(at the beginning)

okay34 :- 5 < #sum{V : d(V)} < 7, p.     

ouch34 :- 5 < #sum{V : d(V)} < 6, p.      
ouch34 :- 6 < #sum{V : d(V)} < 7, p.

%---- integer < #sum{...} <= integer ----(only)

okay35 :- 5 < #sum{V : d(V)} <= 6.        

ouch35 :- 6 < #sum{V : d(V)} <= 7.        

%---- integer < #sum{...} <= integer ----(at the end)

okay36 :- p, 5 < #sum{V : d(V)} <= 6.     

ouch36 :- p, 6 < #sum{V : d(V)} <= 7.     

%---- integer < #sum{...} <= integer ----(at the beginning)

okay37 :- 5 < #sum{V : d(V)} <= 6, p.     

ouch37 :- 6 < #sum{V : d(V)} <= 7, p.     

%---- integer <= #sum{...} < integer ----(at the end)

okay38 :- p, 6 <= #sum{V : d(V)} < 7.     

ouch38 :- p, 5 <= #sum{V : d(V)} < 6.     
ouch38 :- p, 7 <= #sum{V : d(V)} < 8.    

%---- integer <= #sum{...} < integer ----(at the beginning)

okay39 :- 6 <= #sum{V : d(V)} < 7, p.     

ouch39 :- 5 <= #sum{V : d(V)} < 6, p.  
ouch39 :- 7 <= #sum{V : d(V)} < 8, p.     

%---- integer <= #sum{...} <= integer ----(only)

okay40 :- 5 <= #sum{V : d(V)} <= 6.      
okay41 :- 6 <= #sum{V : d(V)} <= 7.  

ouch40 :- 4 <= #sum{V : d(V)} <= 5.     
ouch41 :- 7 <= #sum{V : d(V)} <= 8.       


okay42 :- 6 <= #sum{V : d(V)} <= 6.       

ouch42 :- 5 <= #sum{V : d(V)} <= 5.   
ouch42 :- 7 <= #sum{V : d(V)} <= 7.       

%---- integer <= #sum{...} <= integer ----(at the end)

okay43 :- p, 5 <= #sum{V : d(V)} <= 6.    
okay44 :- p, 6 <= #sum{V : d(V)} <= 7.  

ouch43 :- p, 4 <= #sum{V : d(V)} <= 5.    
ouch44 :- p, 7 <= #sum{V : d(V)} <= 8.    


okay45 :- p, 6 <= #sum{V : d(V)} <= 6.    

ouch45 :- p, 5 <= #sum{V : d(V)} <= 5.   
ouch45 :- p, 7 <= #sum{V : d(V)} <= 7.   

%---- integer <= #sum{...} <= integer ----(at the beginning)

okay46 :- 5 <= #sum{V : d(V)} <= 6, p.    
okay47 :- 6 <= #sum{V : d(V)} <= 7, p.    

ouch46 :- 4 <= #sum{V : d(V)} <= 5, p.
ouch47 :- 7 <= #sum{V : d(V)} <= 8, p.    


okay48 :- 6 <= #sum{V : d(V)} <= 6, p.    

ouch48 :- 5 <= #sum{V : d(V)} <= 5, p.    
ouch48 :- 8 <= #sum{V : d(V)} <= 8, p.    

%---various constraints----------------------------

:- #sum{V : d(V)} = 5.
:- #sum{V : d(V)} < 6.
:- 7 <= #sum{V : d(V)}.
:- #sum{V : d(V)} > 6.
:- #sum{V : d(V)} >= 7.

:- p, #sum{V : d(V)} = 5.
:- p, #sum{V : d(V)} < 6.
:- p, 7 <= #sum{V : d(V)}.
:- p, #sum{V : d(V)} > 6.
:- p, #sum{V : d(V)} >= 7.

:- #sum{V : d(V)} = 5, p.
:- #sum{V : d(V)} < 6, p.
:- 7 <= #sum{V : d(V)}, p.
:- #sum{V : d(V)} > 6, p.
:- #sum{V : d(V)} >= 7, p.

:- 5 = #sum{V : d(V)}.
:- 6 < #sum{V : d(V)}.
:- 7 <= #sum{V : d(V)}.
:- 6 > #sum{V : d(V)}. 
:- 5 >= #sum{V : d(V)}.

:- p, 5 = #sum{V : d(V)}.
:- p, 6 < #sum{V : d(V)}.
:- p, 7 <= #sum{V : d(V)}.
:- p, 6 > #sum{V : d(V)}. 
:- p, 5 >= #sum{V : d(V)}.

:- 5 = #sum{V : d(V)}, p.
:- 6 < #sum{V : d(V)}, p.
:- 7 <= #sum{V : d(V)}, p.
:- 6 > #sum{V : d(V)}, p. 
:- 5 >= #sum{V : d(V)}, p.


:- 4 <= #sum{V : d(V)} <= 5, p.
:- p, 5 <= #sum{V : d(V)} <= 5.
:- 5 <= #sum{V : d(V)} <= 5.

:- 4 < #sum{V : d(V)} < 5, p.
:- p, 4 < #sum{V : d(V)} < 5.
:- 4 < #sum{V : d(V)} < 5.

:- 4 < #sum{V : d(V)} <= 5, p.
:- p, 4 < #sum{V : d(V)} <= 5.
:- 4 < #sum{V : d(V)} <= 5.

:- 4 <= #sum{V : d(V)} < 5, p.
:- p, 4 <= #sum{V : d(V)} < 5.
:- 4 <= #sum{V : d(V)} < 5.
"""

output = """
{d(1), d(2), d(3), okay1, okay10, okay11, okay12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, okay2, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay28, okay29, okay3, okay30, okay31, okay32, okay33, okay34, okay35, okay36, okay37, okay38, okay39, okay4, okay40, okay41, okay42, okay43, okay44, okay45, okay46, okay47, okay48, okay5, okay6, okay7, okay8, okay9, p}
"""
