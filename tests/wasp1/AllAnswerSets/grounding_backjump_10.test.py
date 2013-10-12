input = """
aa:- link(X5,X0,X6), pp(X7, in_state,X6,open), h(X7,X3), time(X3), 
     tank_of(X0,X4), h(X8,X3), qq(X8, press, X5,X1), ss(X2,press,X0,X1), 
     tank_of(X1,X4).

link(l,l,l). 
pp(p,in_state,l,open). 
h(p,1). 
time(1). 
time(2). 
h(p,2). 
tank_of(l,t). 
h(press,1). 
h(pre,2).  
qq(pre,press,l,1). 
ss(2, press,l,1). 
tank_of(1,t). 
"""

output = """
{aa, h(p,1), h(p,2), h(pre,2), h(press,1), link(l,l,l), pp(p,in_state,l,open), qq(pre,press,l,1), ss(2,press,l,1), tank_of(1,t), tank_of(l,t), time(1), time(2)}
"""
