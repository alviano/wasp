input = """
% The last line in the example used to cause problems with the old scanner,
% where " #int" was taken for a meta command "[ ]+#i".

#maxint=1.

allowed(Emps) :-
 #int(Emps).
"""

output = """
{allowed(0), allowed(1)}
"""
