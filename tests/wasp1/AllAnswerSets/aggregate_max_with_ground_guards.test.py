input = """
% Different kind of aggregates with ground guards
% defined in the body of rules and strong constraints. 
% Atoms named "okay" have to be true in the computed stable models,
% atoms named "ouch" have to be false.

d(1).
d(2).
d(3).
p.

%---- #max{...} op integer ----(only)

okay1 :- #max{V : d(V)} = 3.              
 
ouch1 :- #max{V : d(V)} = 2.              
ouch1 :- #max{V : d(V)} = 4. 


okay2 :- #max{V : d(V)} < 4.              

ouch2 :- #max{V : d(V)} < 3.              

 
okay3 :- #max{V : d(V)} <= 3.             

ouch3 :- #max{V : d(V)} <= 2.             


okay4 :- #max{V : d(V)} > 2.              

ouch4 :- #max{V : d(V)} > 3.              


okay5 :- #max{V : d(V)} >= 3.             

ouch5 :- #max{V : d(V)} >= 4.             

%----  #max{...} op integer ----(at the end)

okay6 :- p, #max{V : d(V)} = 3.           

ouch6 :- p, #max{V : d(V)} = 2.
ouch6 :- p, #max{V : d(V)} = 4.           


okay7 :- p, #max{V : d(V)} < 4.           

ouch7 :- p, #max{V : d(V)} < 3.           


okay8 :- p, #max{V : d(V)} <= 3.          

ouch8 :- p, #max{V : d(V)} <= 2.          


okay9 :- p, #max{V : d(V)} > 2.           

ouch9 :- p, #max{V : d(V)} > 3.           


okay10 :- p, #max{V : d(V)} >= 3.          

ouch10 :- p, #max{V : d(V)} >= 4.          

%----  #max{...} op integer ----(at the beginning)

okay11 :- #max{V : d(V)} = 3, p.           

ouch11 :- #max{V : d(V)} = 2, p.           
ouch11 :- #max{V : d(V)} = 4, p.


okay12 :- #max{V : d(V)} < 4, p.           

ouch12 :- #max{V : d(V)} < 3, p.           


okay13 :- #max{V : d(V)} <= 3, p.          

ouch13 :- #max{V : d(V)} <= 2, p.          


okay14 :- #max{V : d(V)} > 2, p.           

ouch14 :- #max{V : d(V)} > 3, p.           


okay15 :- #max{V : d(V)} >= 3, p.          

ouch15 :- #max{V : d(V)} >= 4, p.          

%---- integer op #max{...} ----(only)

okay16 :- 3 = #max{V : d(V)}.              

ouch16 :- 2 = #max{V : d(V)}.              
ouch16 :- 4 = #max{V : d(V)}. 


okay17 :- 2 < #max{V : d(V)}.              

ouch17 :- 3 < #max{V : d(V)}.              


okay18 :- 3 <= #max{V : d(V)}.             

ouch18 :- 4 <= #max{V : d(V)}.             


okay19 :- 4 > #max{V : d(V)}.              

ouch19 :- 3 > #max{V : d(V)}.              


okay20 :- 3 >= #max{V : d(V)}.             

ouch20 :- 2 >= #max{V : d(V)}.             

%----  integer op #max{...}----(at the end) 

okay21 :- p, 3 = #max{V : d(V)}.           

ouch21 :- p, 2 = #max{V : d(V)}.           
ouch21 :- p, 4 = #max{V : d(V)}.


okay22 :- p, 2 < #max{V : d(V)}.           

ouch22 :- p, 3 < #max{V : d(V)}.           


okay23 :- p, 3 <= #max{V : d(V)}.           

ouch23 :- p, 4 <= #max{V : d(V)}.          


okay24 :- p, 4 > #max{V : d(V)}.           

ouch24 :- p, 3 > #max{V : d(V)}.           


okay25 :- p, 3 >= #max{V : d(V)}.          

ouch25 :- p, 2 >= #max{V : d(V)}.          

%----  integer op #max{...}---- (at the beginning)

okay26 :- 3 = #max{V : d(V)}, p.          

ouch27 :- 2 = #max{V : d(V)}, p.          
ouch27 :- 4 = #max{V : d(V)}, p. 


okay28 :- 2 < #max{V : d(V)}, p.         

ouch28 :- 3 < #max{V : d(V)}, p.          


okay29 :- 3 <= #max{V : d(V)}, p.         

ouch29 :- 4 <= #max{V : d(V)}, p.         


okay30 :- 4 > #max{V : d(V)}, p.          

ouch30 :- 3 > #max{V : d(V)}, p.          


okay31 :- 3 >= #max{V : d(V)}, p.         

ouch31 :- 2 >= #max{V : d(V)}, p.         

%---- integer < #max{...} < integer ----(only)

okay32 :- 2 < #max{V : d(V)} < 4.         

ouch32 :- 2 < #max{V : d(V)} < 3.          
ouch32 :- 3 < #max{V : d(V)} < 4.  

%---- integer < #max{(...} < integer ----(at the end)

okay33 :- p, 2 < #max{V : d(V)} < 4.      

ouch33 :- p, 2 < #max{V : d(V)} < 3.      
ouch33 :- p, 3 < #max{V : d(V)} < 4.      

%---- integer < #max{(...} < integer ----(at the beginning)

okay34 :- 2 < #max{V : d(V)} < 4, p.     

ouch34 :- 2 < #max{V : d(V)} < 3, p.      
ouch34 :- 3 < #max{V : d(V)} < 4, p.

%---- integer < #max{...} <= integer ----(only)

okay35 :- 2 < #max{V : d(V)} <= 3.        

ouch35 :- 3 < #max{V : d(V)} <= 4.        

%---- integer < #max{...} <= integer ----(at the end)

okay36 :- p, 2 < #max{V : d(V)} <= 3.     

ouch36 :- p, 3 < #max{V : d(V)} <= 4.     

%---- integer < #max{...} <= integer ----(at the beginning)

okay37 :- 2 < #max{V : d(V)} <= 3, p.     

ouch37 :- 3 < #max{V : d(V)} <= 4, p.     

%---- integer <= #max{...} < integer ----(at the end)

okay38 :- p, 3 <= #max{V : d(V)} < 4.     

ouch38 :- p, 2 <= #max{V : d(V)} < 3.     
ouch38 :- p, 4 <= #max{V : d(V)} < 5.    

%---- integer <= #max{...} < integer ----(at the beginning)

okay39 :- 3 <= #max{V : d(V)} < 4, p.     

ouch39 :- 2 <= #max{V : d(V)} < 3, p.  
ouch39 :- 4 <= #max{V : d(V)} < 5, p.     

%---- integer <= #max{...} <= integer ----(only)

okay40 :- 2 <= #max{V : d(V)} <= 3.      
okay41 :- 3 <= #max{V : d(V)} <= 4.  

ouch40 :- 1 <= #max{V : d(V)} <= 2.     
ouch41 :- 4 <= #max{V : d(V)} <= 5.       


okay42 :- 3 <= #max{V : d(V)} <= 3.       

ouch42 :- 2 <= #max{V : d(V)} <= 2.   
ouch42 :- 4 <= #max{V : d(V)} <= 4.       

%---- integer <= #max{...} <= integer ----(at the end)

okay43 :- p, 2 <= #max{V : d(V)} <= 3.    
okay44 :- p, 3 <= #max{V : d(V)} <= 4.  

ouch43 :- p, 1 <= #max{V : d(V)} <= 2.    
ouch44 :- p, 4 <= #max{V : d(V)} <= 5.    


okay45 :- p, 3 <= #max{V : d(V)} <= 3.    

ouch45 :- p, 2 <= #max{V : d(V)} <= 2.   
ouch45 :- p, 4 <= #max{V : d(V)} <= 4.   

%---- integer <= #max{...} <= integer ----(at the beginning)

okay46 :- 2 <= #max{V : d(V)} <= 3, p.    
okay47 :- 3 <= #max{V : d(V)} <= 4, p.    

ouch46 :- 1 <= #max{V : d(V)} <= 2, p.
ouch47 :- 4 <= #max{V : d(V)} <= 5, p.    


okay48 :- 3 <= #max{V : d(V)} <= 3, p.    

ouch48 :- 2 <= #max{V : d(V)} <= 2, p.    
ouch48 :- 4 <= #max{V : d(V)} <= 4, p.    
"""

output = """
{d(1), d(2), d(3), okay1, okay10, okay11, okay12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, okay2, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay28, okay29, okay3, okay30, okay31, okay32, okay33, okay34, okay35, okay36, okay37, okay38, okay39, okay4, okay40, okay41, okay42, okay43, okay44, okay45, okay46, okay47, okay48, okay5, okay6, okay7, okay8, okay9, p}
"""
