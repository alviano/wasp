input = """
strategic(1399) | strategic(1834).
strategic(1834) | strategic(1811) | strategic(1516).
strategic(1751) | strategic(45).
strategic(1240) | strategic(1577).
strategic(301) | strategic(1868).
strategic(504) | strategic(559).
strategic(1811) :- strategic(1751), strategic(45).
strategic(1736) :- strategic(559), strategic(1577).
strategic(1044) :- strategic(1736), strategic(1516).
 :- not strategic(1044).
"""
output = """
{strategic(1399), strategic(1516), strategic(45), strategic(1577), strategic(1868), strategic(559), strategic(1736), strategic(1044)}
{strategic(1399), strategic(1516), strategic(45), strategic(1577), strategic(301), strategic(559), strategic(1736), strategic(1044)}
{strategic(1399), strategic(1516), strategic(1751), strategic(1577), strategic(1868), strategic(559), strategic(1736), strategic(1044)}
{strategic(1399), strategic(1516), strategic(1751), strategic(1577), strategic(301), strategic(559), strategic(1736), strategic(1044)}
"""
