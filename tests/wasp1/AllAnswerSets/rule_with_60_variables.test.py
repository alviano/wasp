input = """
bar(a).
foo(X1,X2,X3,X4,X5,X6,X7,X8,X9,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,X24,X25,X26,X27,X28,X29,X30,X31,X32,X33,X34,X35,X36,X37,X38,X39,X40,X41,X42,X43,X44,X45,X46,X47,X48,X49,X50,X51,X52,X53,X54,X55,X56,X57,X58,X59,X60) :- bar(X1),bar(X2),bar(X3),bar(X4),bar(X5),bar(X6),bar(X7),bar(X8),bar(X9),bar(X10),bar(X11),bar(X12),bar(X13),bar(X14),bar(X15),bar(X16),bar(X17),bar(X18),bar(X19),bar(X20),bar(X21),bar(X22),bar(X23),bar(X24),bar(X25),bar(X26),bar(X27),bar(X28),bar(X29),bar(X30),bar(X31),bar(X32),bar(X33),bar(X34),bar(X35),bar(X36),bar(X37),bar(X38),bar(X39),bar(X40),bar(X41),bar(X42),bar(X43),bar(X44),bar(X45),bar(X46),bar(X47),bar(X48),bar(X49),bar(X50),bar(X51),bar(X52),bar(X53),bar(X54),bar(X55),bar(X56),bar(X57),bar(X58),bar(X59),bar(X60).
"""

output = """
{bar(a), foo(a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a)}
"""
