input = """
strategic(1343) :- strategic(118), strategic(166).
strategic(118) | strategic(253) | strategic(1834) | strategic(301).
strategic(1399) | strategic(1168) | strategic(1834) | strategic(726).
strategic(1291) | strategic(1361) | strategic(68).
strategic(999) :- strategic(412), strategic(826).
strategic(118) | strategic(129) | strategic(45) | strategic(504).
strategic(1240) :- strategic(947).
strategic(729) :- strategic(999), strategic(412).
strategic(118) | strategic(754) | strategic(826) | strategic(266).
strategic(1044) :- strategic(1240), strategic(504).
strategic(1168) :- strategic(1648), strategic(559).
strategic(663) :- strategic(1560), strategic(266).
strategic(592) :- strategic(1343), strategic(129).
strategic(1648) :- strategic(754), strategic(947).
strategic(1868) :- strategic(1893), strategic(783).
strategic(1834) :- strategic(1893), strategic(1399).
strategic(992) :- strategic(592), strategic(1367).
strategic(129) :- strategic(1893), strategic(1834).
strategic(1367) :- strategic(68), strategic(451).
strategic(1736) | strategic(129) | strategic(451) | strategic(559).
strategic(947) :- strategic(1856), strategic(592).
strategic(783) :- strategic(663), strategic(504).
strategic(968) :- strategic(1399), strategic(1868).
strategic(451) | strategic(412) | strategic(1560).
strategic(947) :- strategic(1560), strategic(1240).
:- not strategic(1044).
strategic(1648) | strategic(1291) | strategic(266) | strategic(1856).
strategic(1560) :- strategic(992), strategic(266).
strategic(504) | strategic(1811) | strategic(968) | strategic(559).
strategic(1893) :- strategic(968), strategic(729).
strategic(166) :- strategic(266), strategic(1168).
"""
output = """
"""
