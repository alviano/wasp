input = """
h2 :- not h.
h2 :- not h2.
h :- not v2n3,not v2n2,not v2n1.
h :- not v3n3,not v3n2,not v3n1.
h :- not v4n3,not v4n2,not v4n1.
h :- not v5n3,not v5n2,not v5n1.
h :- not v6n3,not v6n2,not v6n1.
v2n1 :- not v4n1,not v3n1,not v2n3,not v2n2.
v2n2 :- not v4n2,not v3n2,not v2n3,not v2n1.
v2n3 :- not v4n3,not v3n3,not v2n2,not v2n1.
v3n1 :- not v5n1,not v2n1,not v3n3,not v3n2.
v3n2 :- not v5n2,not v2n2,not v3n3,not v3n1.
v3n3 :- not v5n3,not v2n3,not v3n2,not v3n1.
v4n1 :- not v6n1,not v5n1,not v2n1,not v4n3,not v4n2.
v4n2 :- not v6n2,not v5n2,not v2n2,not v4n3,not v4n1.
v4n3 :- not v6n3,not v5n3,not v2n3,not v4n2,not v4n1.
s :- h2.
s :- v2n1.
s :- v2n2.
s :- v2n3.
s :- v3n1.
s :- v3n2.
s :- v3n3.
s :- v4n1.
s :- v4n2.
s :- v4n3.
s :- v5n1.
s :- v5n2.
s :- v5n3.
s :- v6n1.
s :- v6n2.
s :- v6n3.
"""

output = """
"""
