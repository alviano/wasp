input = """
married(X,Y) :- husband_of(X,Y).
married(Y,X) :- husband_of(X,Y).
married(X,Y) :- wife_of(X,Y).
married(Y,X) :- wife_of(X,Y).

parent(X,Y) :- father_of(X,Y).
parent(X,Y) :- mother_of(X,Y).
parent(X,Y) :- married(X,Z), father_of(Z,Y).
parent(X,Y) :- married(X,Z), mother_of(Z,Y).

ancestor(X,Y) :- parent(X,Y).
ancestor(X,Y) :- ancestor(X,U), ancestor(U,Y).
husband_of(greco,pugliese).
husband_of(pietro,famularo).
husband_of(luigi,nik).
husband_of(franze,salvo).
husband_of(faber,tiano).
husband_of(girsch,ursino).
husband_of(salerno,perri).

wife_of(sergio,sacca).
wife_of(palopoli,nigro).
wife_of(rullo,frank).
wife_of(bucca,pupo).
wife_of(stadler,dorn).
wife_of(reith,milos).

father_of(greco,sergio).
father_of(pietro,frank).
father_of(luigi,rullo).
father_of(luigi,palopoli).
father_of(sacca,franze).
father_of(nigro,girsch).
father_of(nigro,faber).
father_of(frank,salvo).
father_of(frank,tiano).
father_of(pupo,ursino).
father_of(franze,dorn).
father_of(dorn,havelka).
father_of(dorn,stary).

mother_of(pugliese,nigro).
mother_of(nik,pupo).
mother_of(rullo,salerno).
mother_of(bucca,perri).
mother_of(tiano,milos).
mother_of(ursino,stadler).
mother_of(perri,reith).
mother_of(reith,eiter).
"""

output = """
{ancestor(bucca,eiter), ancestor(bucca,havelka), ancestor(bucca,perri), ancestor(bucca,reith), ancestor(bucca,stadler), ancestor(bucca,stary), ancestor(bucca,ursino), ancestor(dorn,havelka), ancestor(dorn,stary), ancestor(faber,eiter), ancestor(faber,milos), ancestor(famularo,dorn), ancestor(famularo,eiter), ancestor(famularo,frank), ancestor(famularo,havelka), ancestor(famularo,milos), ancestor(famularo,reith), ancestor(famularo,salerno), ancestor(famularo,salvo), ancestor(famularo,stary), ancestor(famularo,tiano), ancestor(frank,dorn), ancestor(frank,eiter), ancestor(frank,havelka), ancestor(frank,milos), ancestor(frank,reith), ancestor(frank,salerno), ancestor(frank,salvo), ancestor(frank,stary), ancestor(frank,tiano), ancestor(franze,dorn), ancestor(franze,havelka), ancestor(franze,stary), ancestor(girsch,havelka), ancestor(girsch,stadler), ancestor(girsch,stary), ancestor(greco,dorn), ancestor(greco,eiter), ancestor(greco,faber), ancestor(greco,franze), ancestor(greco,girsch), ancestor(greco,havelka), ancestor(greco,milos), ancestor(greco,nigro), ancestor(greco,sergio), ancestor(greco,stadler), ancestor(greco,stary), ancestor(luigi,dorn), ancestor(luigi,eiter), ancestor(luigi,faber), ancestor(luigi,girsch), ancestor(luigi,havelka), ancestor(luigi,milos), ancestor(luigi,palopoli), ancestor(luigi,perri), ancestor(luigi,pupo), ancestor(luigi,reith), ancestor(luigi,rullo), ancestor(luigi,salerno), ancestor(luigi,salvo), ancestor(luigi,stadler), ancestor(luigi,stary), ancestor(luigi,tiano), ancestor(luigi,ursino), ancestor(milos,eiter), ancestor(nigro,eiter), ancestor(nigro,faber), ancestor(nigro,girsch), ancestor(nigro,havelka), ancestor(nigro,milos), ancestor(nigro,stadler), ancestor(nigro,stary), ancestor(nik,dorn), ancestor(nik,eiter), ancestor(nik,faber), ancestor(nik,girsch), ancestor(nik,havelka), ancestor(nik,milos), ancestor(nik,palopoli), ancestor(nik,perri), ancestor(nik,pupo), ancestor(nik,reith), ancestor(nik,rullo), ancestor(nik,salerno), ancestor(nik,salvo), ancestor(nik,stadler), ancestor(nik,stary), ancestor(nik,tiano), ancestor(nik,ursino), ancestor(palopoli,eiter), ancestor(palopoli,faber), ancestor(palopoli,girsch), ancestor(palopoli,havelka), ancestor(palopoli,milos), ancestor(palopoli,stadler), ancestor(palopoli,stary), ancestor(perri,eiter), ancestor(perri,reith), ancestor(pietro,dorn), ancestor(pietro,eiter), ancestor(pietro,frank), ancestor(pietro,havelka), ancestor(pietro,milos), ancestor(pietro,reith), ancestor(pietro,salerno), ancestor(pietro,salvo), ancestor(pietro,stary), ancestor(pietro,tiano), ancestor(pugliese,dorn), ancestor(pugliese,eiter), ancestor(pugliese,faber), ancestor(pugliese,franze), ancestor(pugliese,girsch), ancestor(pugliese,havelka), ancestor(pugliese,milos), ancestor(pugliese,nigro), ancestor(pugliese,sergio), ancestor(pugliese,stadler), ancestor(pugliese,stary), ancestor(pupo,eiter), ancestor(pupo,havelka), ancestor(pupo,perri), ancestor(pupo,reith), ancestor(pupo,stadler), ancestor(pupo,stary), ancestor(pupo,ursino), ancestor(reith,eiter), ancestor(rullo,dorn), ancestor(rullo,eiter), ancestor(rullo,havelka), ancestor(rullo,milos), ancestor(rullo,reith), ancestor(rullo,salerno), ancestor(rullo,salvo), ancestor(rullo,stary), ancestor(rullo,tiano), ancestor(sacca,dorn), ancestor(sacca,franze), ancestor(sacca,havelka), ancestor(sacca,stary), ancestor(salerno,eiter), ancestor(salerno,reith), ancestor(salvo,dorn), ancestor(salvo,havelka), ancestor(salvo,stary), ancestor(sergio,dorn), ancestor(sergio,franze), ancestor(sergio,havelka), ancestor(sergio,stary), ancestor(stadler,havelka), ancestor(stadler,stary), ancestor(tiano,eiter), ancestor(tiano,milos), ancestor(ursino,havelka), ancestor(ursino,stadler), ancestor(ursino,stary), father_of(dorn,havelka), father_of(dorn,stary), father_of(frank,salvo), father_of(frank,tiano), father_of(franze,dorn), father_of(greco,sergio), father_of(luigi,palopoli), father_of(luigi,rullo), father_of(nigro,faber), father_of(nigro,girsch), father_of(pietro,frank), father_of(pupo,ursino), father_of(sacca,franze), husband_of(faber,tiano), husband_of(franze,salvo), husband_of(girsch,ursino), husband_of(greco,pugliese), husband_of(luigi,nik), husband_of(pietro,famularo), husband_of(salerno,perri), married(bucca,pupo), married(dorn,stadler), married(faber,tiano), married(famularo,pietro), married(frank,rullo), married(franze,salvo), married(girsch,ursino), married(greco,pugliese), married(luigi,nik), married(milos,reith), married(nigro,palopoli), married(nik,luigi), married(palopoli,nigro), married(perri,salerno), married(pietro,famularo), married(pugliese,greco), married(pupo,bucca), married(reith,milos), married(rullo,frank), married(sacca,sergio), married(salerno,perri), married(salvo,franze), married(sergio,sacca), married(stadler,dorn), married(tiano,faber), married(ursino,girsch), mother_of(bucca,perri), mother_of(nik,pupo), mother_of(perri,reith), mother_of(pugliese,nigro), mother_of(reith,eiter), mother_of(rullo,salerno), mother_of(tiano,milos), mother_of(ursino,stadler), parent(bucca,perri), parent(bucca,ursino), parent(dorn,havelka), parent(dorn,stary), parent(faber,milos), parent(famularo,frank), parent(frank,salerno), parent(frank,salvo), parent(frank,tiano), parent(franze,dorn), parent(girsch,stadler), parent(greco,nigro), parent(greco,sergio), parent(luigi,palopoli), parent(luigi,pupo), parent(luigi,rullo), parent(milos,eiter), parent(nigro,faber), parent(nigro,girsch), parent(nik,palopoli), parent(nik,pupo), parent(nik,rullo), parent(palopoli,faber), parent(palopoli,girsch), parent(perri,reith), parent(pietro,frank), parent(pugliese,nigro), parent(pugliese,sergio), parent(pupo,perri), parent(pupo,ursino), parent(reith,eiter), parent(rullo,salerno), parent(rullo,salvo), parent(rullo,tiano), parent(sacca,franze), parent(salerno,reith), parent(salvo,dorn), parent(sergio,franze), parent(stadler,havelka), parent(stadler,stary), parent(tiano,milos), parent(ursino,stadler), wife_of(bucca,pupo), wife_of(palopoli,nigro), wife_of(reith,milos), wife_of(rullo,frank), wife_of(sergio,sacca), wife_of(stadler,dorn)}
"""
