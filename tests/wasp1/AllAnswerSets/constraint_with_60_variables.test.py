input = """
bar(a) v xbar(a).
% let variables occur twice each to avoid projection rewriting
:- bar(X1),bar(X2),bar(X3),bar(X4),bar(X5),bar(X6),bar(X7),bar(X8),bar(X9),bar(X10),bar(X11),bar(X12),bar(X13),bar(X14),bar(X15),bar(X16),bar(X17),bar(X18),bar(X19),bar(X20),bar(X21),bar(X22),bar(X23),bar(X24),bar(X25),bar(X26),bar(X27),bar(X28),bar(X29),bar(X30),bar(X31),bar(X32),bar(X33),bar(X34),bar(X35),bar(X36),bar(X37),bar(X38),bar(X39),bar(X40),bar(X41),bar(X42),bar(X43),bar(X44),bar(X45),bar(X46),bar(X47),bar(X48),bar(X49),bar(X50),bar(X51),bar(X52),bar(X53),bar(X54),bar(X55),bar(X56),bar(X57),bar(X58),bar(X59),bar(X60),
xbar(X1),xbar(X2),xbar(X3),xbar(X4),xbar(X5),xbar(X6),xbar(X7),xbar(X8),xbar(X9),xbar(X10),xbar(X11),xbar(X12),xbar(X13),xbar(X14),xbar(X15),xbar(X16),xbar(X17),xbar(X18),xbar(X19),xbar(X20),xbar(X21),xbar(X22),xbar(X23),xbar(X24),xbar(X25),xbar(X26),xbar(X27),xbar(X28),xbar(X29),xbar(X30),xbar(X31),xbar(X32),xbar(X33),xbar(X34),xbar(X35),xbar(X36),xbar(X37),xbar(X38),xbar(X39),xbar(X40),xbar(X41),xbar(X42),xbar(X43),xbar(X44),xbar(X45),xbar(X46),xbar(X47),xbar(X48),xbar(X49),xbar(X50),xbar(X51),xbar(X52),xbar(X53),xbar(X54),xbar(X55),xbar(X56),xbar(X57),xbar(X58),xbar(X59),xbar(X60).
:- xbar(a).

"""

output = """
{bar(a)}
"""
