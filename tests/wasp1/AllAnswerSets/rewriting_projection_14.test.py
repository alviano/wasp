input = """
#maxint = 10.
turn(S) :- stage(S).
total_score(S1,X) :- turn(S), S1 == 9, X = #max{ Y :score(Y) }.
total_score1(S1,X) :- turn(S), S1 = 9, X = #max{ Y :score(Y) }.
	
"""

output = """
{}
"""
