input = """
#maxint = 10.

total_score(S,X) :- t, S==9, X = #max{ Y :score(Y) }.
total_score1(S,X) :- t, S=9, X = #max{ Y :score(Y) }.
	
"""

output = """
{}
"""
