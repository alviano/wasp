input = """
"kevin alone".
"a,b" v "ich liebe dich" :- "kevin alone", not ab.

"\" :- "a,b".
"\," :- "a,b".
",\" :- "a,b".

"I am completely"("crazy for you") :- "ich liebe dich".
"""

output = """
{",\", "\", "\,", "a,b", "kevin alone"}
{"I am completely"("crazy for you"), "ich liebe dich", "kevin alone"}
"""
