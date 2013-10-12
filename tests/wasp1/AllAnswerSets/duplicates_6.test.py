input = """
h2 :- not h.
h2 :- not h2.
h :- not v9n2,not v9n1.
h :- not v10n2,not v10n1.
h :- not v11n2,not v11n1.
v7n1 :- not v9n1,not v6n1,not v5n1,not v7n2.
v7n2 :- not v9n2,not v6n2,not v5n2,not v7n1.
v8n1 :- not v10n1,not v9n1,not v6n1,not v8n2.
v8n2 :- not v10n2,not v9n2,not v6n2,not v8n1.
v9n1 :- not v11n1,not v8n1,not v7n1,not v9n2.
v9n2 :- not v11n2,not v8n2,not v7n2,not v9n1.
v10n1 :- not v11n1,not v8n1,not v10n2.
v10n2 :- not v11n2,not v8n2,not v10n1.
v11n1 :- not v10n1,not v9n1,not v11n2.
v11n2 :- not v10n2,not v9n2,not v11n1.
s :- h2.
s :- v7n1.
s :- v7n2.
s :- v8n1.
s :- v8n2.
s :- v9n1.
s :- v9n2.
s :- v10n1.
s :- v10n2.
s :- v11n1.
s :- v11n2.
"""

output = """
{h2, s, v10n1, v11n2, v7n2, v8n2, v9n1}
{h2, s, v10n2, v11n1, v7n1, v8n1, v9n2}
"""
