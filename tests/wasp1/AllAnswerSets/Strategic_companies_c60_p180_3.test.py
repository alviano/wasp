input = """
% Strategic Companies
%
% As we want to produce X, Y or Z must be strategic.

 strategic(Y) v strategic(Z) :- produced_by(X,Y,Z).

% W is strategic, if it is controlled by strategic 
% companies X, Y, and Z  

 strategic(W) :- controlled_by(W,X,Y,Z), 
                 Strategic(X), Strategic(Y), Strategic(Z). 

% Handle special 0 symbol 

 :- strategic(0). 
 Strategic(X) :- strategic(X).
 Strategic(0) :- true. 
 true. 
controlled_by(1,10,13,13).
controlled_by(2,30,30,30).
controlled_by(3,9,46,46).
controlled_by(4,55,3,3).
controlled_by(4,55,38,57).
controlled_by(5,18,49,49).
controlled_by(5,18,51,44).
controlled_by(6,1,23,54).
controlled_by(6,1,31,33).
controlled_by(7,16,44,8).
controlled_by(8,14,56,56).
controlled_by(9,21,21,21).
controlled_by(10,5,48,48).
controlled_by(10,5,56,56).
controlled_by(11,28,49,55).
controlled_by(11,28,47,57).
controlled_by(12,35,10,10).
controlled_by(13,4,36,36).
controlled_by(14,16,2,22).
controlled_by(14,16,10,55).
controlled_by(15,19,19,19).
controlled_by(16,33,33,33).
controlled_by(17,51,49,49).
controlled_by(17,51,16,16).
controlled_by(18,9,9,9).
controlled_by(19,9,34,34).
controlled_by(20,52,4,4).
controlled_by(21,40,4,4).
controlled_by(22,26,39,24).
controlled_by(22,26,60,36).
controlled_by(23,10,39,39).
controlled_by(23,10,49,49).
controlled_by(24,3,3,3).
controlled_by(25,7,1,1).
controlled_by(25,7,29,52).
controlled_by(26,33,53,21).
controlled_by(27,29,33,33).
controlled_by(28,59,52,52).
controlled_by(29,3,48,2).
controlled_by(29,3,48,39).
controlled_by(30,25,50,9).
controlled_by(30,25,20,3).
controlled_by(31,20,3,3).
controlled_by(31,20,45,45).
controlled_by(32,51,11,20).
controlled_by(32,51,28,49).
controlled_by(33,4,60,60).
controlled_by(33,4,42,29).
controlled_by(34,46,42,42).
controlled_by(34,46,35,35).
controlled_by(35,60,55,20).
controlled_by(35,60,13,54).
controlled_by(36,3,3,3).
controlled_by(37,4,50,50).
controlled_by(37,4,17,23).
controlled_by(38,56,11,11).
controlled_by(39,44,57,57).
controlled_by(39,44,14,43).
controlled_by(40,14,24,24).
controlled_by(40,14,35,35).
controlled_by(41,54,7,31).
controlled_by(41,54,7,55).
controlled_by(42,56,31,31).
controlled_by(43,17,33,33).
controlled_by(43,17,56,56).
controlled_by(44,33,21,21).
controlled_by(44,33,48,48).
controlled_by(45,56,12,12).
controlled_by(46,30,2,2).
controlled_by(47,3,48,48).
controlled_by(47,3,60,60).
controlled_by(48,46,8,8).
controlled_by(49,15,37,35).
controlled_by(50,41,21,21).
controlled_by(50,41,58,5).
controlled_by(51,5,27,27).
controlled_by(51,5,13,13).
controlled_by(52,57,57,57).
controlled_by(53,51,2,2).
controlled_by(54,42,5,50).
controlled_by(55,14,59,59).
controlled_by(55,14,17,17).
controlled_by(56,20,48,25).
controlled_by(56,20,39,38).
controlled_by(57,26,13,2).
controlled_by(57,26,13,38).
controlled_by(58,31,45,45).
controlled_by(59,25,52,13).
controlled_by(59,25,29,21).
controlled_by(60,31,25,33).
produced_by(p1, 3,4).
produced_by(p2, 19,29).
produced_by(p3, 44,36).
produced_by(p4, 19,2).
produced_by(p5, 6,21).
produced_by(p6, 17,9).
produced_by(p7, 2,1).
produced_by(p8, 33,33).
produced_by(p9, 15,18).
produced_by(p10, 39,22).
produced_by(p11, 57,18).
produced_by(p12, 30,31).
produced_by(p13, 39,32).
produced_by(p14, 37,42).
produced_by(p15, 29,26).
produced_by(p16, 21,54).
produced_by(p17, 36,53).
produced_by(p18, 20,29).
produced_by(p19, 41,16).
produced_by(p20, 47,57).
produced_by(p21, 30,36).
produced_by(p22, 4,40).
produced_by(p23, 56,2).
produced_by(p24, 50,23).
produced_by(p25, 44,22).
produced_by(p26, 58,56).
produced_by(p27, 25,46).
produced_by(p28, 51,7).
produced_by(p29, 11,53).
produced_by(p30, 18,6).
produced_by(p31, 37,44).
produced_by(p32, 13,40).
produced_by(p33, 48,19).
produced_by(p34, 44,19).
produced_by(p35, 50,41).
produced_by(p36, 5,5).
produced_by(p37, 8,29).
produced_by(p38, 29,11).
produced_by(p39, 47,16).
produced_by(p40, 4,20).
produced_by(p41, 2,22).
produced_by(p42, 13,44).
produced_by(p43, 25,57).
produced_by(p44, 49,21).
produced_by(p45, 9,28).
produced_by(p46, 57,26).
produced_by(p47, 22,20).
produced_by(p48, 35,22).
produced_by(p49, 28,44).
produced_by(p50, 12,34).
produced_by(p51, 17,52).
produced_by(p52, 14,54).
produced_by(p53, 17,1).
produced_by(p54, 44,50).
produced_by(p55, 43,23).
produced_by(p56, 3,60).
produced_by(p57, 3,59).
produced_by(p58, 20,45).
produced_by(p59, 39,31).
produced_by(p60, 55,55).
produced_by(p61, 46,6).
produced_by(p62, 10,34).
produced_by(p63, 23,1).
produced_by(p64, 45,54).
produced_by(p65, 49,6).
produced_by(p66, 40,12).
produced_by(p67, 8,40).
produced_by(p68, 38,24).
produced_by(p69, 35,27).
produced_by(p70, 36,37).
produced_by(p71, 49,60).
produced_by(p72, 22,11).
produced_by(p73, 17,41).
produced_by(p74, 51,25).
produced_by(p75, 48,9).
produced_by(p76, 43,22).
produced_by(p77, 52,38).
produced_by(p78, 56,38).
produced_by(p79, 18,35).
produced_by(p80, 19,41).
produced_by(p81, 18,53).
produced_by(p82, 24,23).
produced_by(p83, 46,10).
produced_by(p84, 37,54).
produced_by(p85, 28,35).
produced_by(p86, 39,23).
produced_by(p87, 27,34).
produced_by(p88, 54,20).
produced_by(p89, 3,47).
produced_by(p90, 45,42).
produced_by(p91, 53,42).
produced_by(p92, 54,18).
produced_by(p93, 5,57).
produced_by(p94, 20,13).
produced_by(p95, 11,47).
produced_by(p96, 51,31).
produced_by(p97, 31,56).
produced_by(p98, 15,29).
produced_by(p99, 40,8).
produced_by(p100, 2,10).
produced_by(p101, 7,46).
produced_by(p102, 50,2).
produced_by(p103, 23,38).
produced_by(p104, 51,9).
produced_by(p105, 13,25).
produced_by(p106, 3,54).
produced_by(p107, 57,23).
produced_by(p108, 48,24).
produced_by(p109, 32,18).
produced_by(p110, 33,29).
produced_by(p111, 24,21).
produced_by(p112, 31,40).
produced_by(p113, 20,40).
produced_by(p114, 58,24).
produced_by(p115, 36,22).
produced_by(p116, 34,22).
produced_by(p117, 28,19).
produced_by(p118, 20,17).
produced_by(p119, 16,19).
produced_by(p120, 25,57).
produced_by(p121, 32,15).
produced_by(p122, 21,51).
produced_by(p123, 24,59).
produced_by(p124, 16,58).
produced_by(p125, 13,46).
produced_by(p126, 17,52).
produced_by(p127, 3,47).
produced_by(p128, 12,56).
produced_by(p129, 22,28).
produced_by(p130, 8,6).
produced_by(p131, 36,39).
produced_by(p132, 25,24).
produced_by(p133, 43,39).
produced_by(p134, 35,59).
produced_by(p135, 59,21).
produced_by(p136, 59,15).
produced_by(p137, 6,7).
produced_by(p138, 42,34).
produced_by(p139, 42,47).
produced_by(p140, 59,17).
produced_by(p141, 33,40).
produced_by(p142, 54,16).
produced_by(p143, 55,40).
produced_by(p144, 25,3).
produced_by(p145, 8,48).
produced_by(p146, 42,29).
produced_by(p147, 35,35).
produced_by(p148, 26,10).
produced_by(p149, 21,1).
produced_by(p150, 48,38).
produced_by(p151, 58,33).
produced_by(p152, 21,16).
produced_by(p153, 13,26).
produced_by(p154, 11,4).
produced_by(p155, 40,22).
produced_by(p156, 25,48).
produced_by(p157, 2,36).
produced_by(p158, 33,7).
produced_by(p159, 36,33).
produced_by(p160, 31,5).
produced_by(p161, 45,9).
produced_by(p162, 20,30).
produced_by(p163, 57,31).
produced_by(p164, 34,18).
produced_by(p165, 4,38).
produced_by(p166, 55,48).
produced_by(p167, 15,35).
produced_by(p168, 17,17).
produced_by(p169, 8,58).
produced_by(p170, 20,55).
produced_by(p171, 1,41).
produced_by(p172, 17,49).
produced_by(p173, 30,43).
produced_by(p174, 5,11).
produced_by(p175, 2,48).
produced_by(p176, 22,38).
produced_by(p177, 47,38).
produced_by(p178, 42,57).
produced_by(p179, 10,30).
produced_by(p180, 1,34).
"""

output = """
{Strategic(0), Strategic(1), Strategic(10), Strategic(11), Strategic(12), Strategic(13), Strategic(14), Strategic(15), Strategic(16), Strategic(17), Strategic(18), Strategic(19), Strategic(2), Strategic(20), Strategic(21), Strategic(22), Strategic(23), Strategic(24), Strategic(25), Strategic(26), Strategic(27), Strategic(28), Strategic(29), Strategic(3), Strategic(30), Strategic(31), Strategic(32), Strategic(33), Strategic(34), Strategic(35), Strategic(36), Strategic(37), Strategic(38), Strategic(39), Strategic(4), Strategic(40), Strategic(41), Strategic(42), Strategic(43), Strategic(44), Strategic(45), Strategic(46), Strategic(47), Strategic(48), Strategic(49), Strategic(5), Strategic(50), Strategic(51), Strategic(52), Strategic(53), Strategic(54), Strategic(55), Strategic(56), Strategic(57), Strategic(58), Strategic(59), Strategic(6), Strategic(60), Strategic(7), Strategic(8), Strategic(9), controlled_by(1,10,13,13), controlled_by(10,5,48,48), controlled_by(10,5,56,56), controlled_by(11,28,47,57), controlled_by(11,28,49,55), controlled_by(12,35,10,10), controlled_by(13,4,36,36), controlled_by(14,16,10,55), controlled_by(14,16,2,22), controlled_by(15,19,19,19), controlled_by(16,33,33,33), controlled_by(17,51,16,16), controlled_by(17,51,49,49), controlled_by(18,9,9,9), controlled_by(19,9,34,34), controlled_by(2,30,30,30), controlled_by(20,52,4,4), controlled_by(21,40,4,4), controlled_by(22,26,39,24), controlled_by(22,26,60,36), controlled_by(23,10,39,39), controlled_by(23,10,49,49), controlled_by(24,3,3,3), controlled_by(25,7,1,1), controlled_by(25,7,29,52), controlled_by(26,33,53,21), controlled_by(27,29,33,33), controlled_by(28,59,52,52), controlled_by(29,3,48,2), controlled_by(29,3,48,39), controlled_by(3,9,46,46), controlled_by(30,25,20,3), controlled_by(30,25,50,9), controlled_by(31,20,3,3), controlled_by(31,20,45,45), controlled_by(32,51,11,20), controlled_by(32,51,28,49), controlled_by(33,4,42,29), controlled_by(33,4,60,60), controlled_by(34,46,35,35), controlled_by(34,46,42,42), controlled_by(35,60,13,54), controlled_by(35,60,55,20), controlled_by(36,3,3,3), controlled_by(37,4,17,23), controlled_by(37,4,50,50), controlled_by(38,56,11,11), controlled_by(39,44,14,43), controlled_by(39,44,57,57), controlled_by(4,55,3,3), controlled_by(4,55,38,57), controlled_by(40,14,24,24), controlled_by(40,14,35,35), controlled_by(41,54,7,31), controlled_by(41,54,7,55), controlled_by(42,56,31,31), controlled_by(43,17,33,33), controlled_by(43,17,56,56), controlled_by(44,33,21,21), controlled_by(44,33,48,48), controlled_by(45,56,12,12), controlled_by(46,30,2,2), controlled_by(47,3,48,48), controlled_by(47,3,60,60), controlled_by(48,46,8,8), controlled_by(49,15,37,35), controlled_by(5,18,49,49), controlled_by(5,18,51,44), controlled_by(50,41,21,21), controlled_by(50,41,58,5), controlled_by(51,5,13,13), controlled_by(51,5,27,27), controlled_by(52,57,57,57), controlled_by(53,51,2,2), controlled_by(54,42,5,50), controlled_by(55,14,17,17), controlled_by(55,14,59,59), controlled_by(56,20,39,38), controlled_by(56,20,48,25), controlled_by(57,26,13,2), controlled_by(57,26,13,38), controlled_by(58,31,45,45), controlled_by(59,25,29,21), controlled_by(59,25,52,13), controlled_by(6,1,23,54), controlled_by(6,1,31,33), controlled_by(60,31,25,33), controlled_by(7,16,44,8), controlled_by(8,14,56,56), controlled_by(9,21,21,21), produced_by(p1,3,4), produced_by(p10,39,22), produced_by(p100,2,10), produced_by(p101,7,46), produced_by(p102,50,2), produced_by(p103,23,38), produced_by(p104,51,9), produced_by(p105,13,25), produced_by(p106,3,54), produced_by(p107,57,23), produced_by(p108,48,24), produced_by(p109,32,18), produced_by(p11,57,18), produced_by(p110,33,29), produced_by(p111,24,21), produced_by(p112,31,40), produced_by(p113,20,40), produced_by(p114,58,24), produced_by(p115,36,22), produced_by(p116,34,22), produced_by(p117,28,19), produced_by(p118,20,17), produced_by(p119,16,19), produced_by(p12,30,31), produced_by(p120,25,57), produced_by(p121,32,15), produced_by(p122,21,51), produced_by(p123,24,59), produced_by(p124,16,58), produced_by(p125,13,46), produced_by(p126,17,52), produced_by(p127,3,47), produced_by(p128,12,56), produced_by(p129,22,28), produced_by(p13,39,32), produced_by(p130,8,6), produced_by(p131,36,39), produced_by(p132,25,24), produced_by(p133,43,39), produced_by(p134,35,59), produced_by(p135,59,21), produced_by(p136,59,15), produced_by(p137,6,7), produced_by(p138,42,34), produced_by(p139,42,47), produced_by(p14,37,42), produced_by(p140,59,17), produced_by(p141,33,40), produced_by(p142,54,16), produced_by(p143,55,40), produced_by(p144,25,3), produced_by(p145,8,48), produced_by(p146,42,29), produced_by(p147,35,35), produced_by(p148,26,10), produced_by(p149,21,1), produced_by(p15,29,26), produced_by(p150,48,38), produced_by(p151,58,33), produced_by(p152,21,16), produced_by(p153,13,26), produced_by(p154,11,4), produced_by(p155,40,22), produced_by(p156,25,48), produced_by(p157,2,36), produced_by(p158,33,7), produced_by(p159,36,33), produced_by(p16,21,54), produced_by(p160,31,5), produced_by(p161,45,9), produced_by(p162,20,30), produced_by(p163,57,31), produced_by(p164,34,18), produced_by(p165,4,38), produced_by(p166,55,48), produced_by(p167,15,35), produced_by(p168,17,17), produced_by(p169,8,58), produced_by(p17,36,53), produced_by(p170,20,55), produced_by(p171,1,41), produced_by(p172,17,49), produced_by(p173,30,43), produced_by(p174,5,11), produced_by(p175,2,48), produced_by(p176,22,38), produced_by(p177,47,38), produced_by(p178,42,57), produced_by(p179,10,30), produced_by(p18,20,29), produced_by(p180,1,34), produced_by(p19,41,16), produced_by(p2,19,29), produced_by(p20,47,57), produced_by(p21,30,36), produced_by(p22,4,40), produced_by(p23,56,2), produced_by(p24,50,23), produced_by(p25,44,22), produced_by(p26,58,56), produced_by(p27,25,46), produced_by(p28,51,7), produced_by(p29,11,53), produced_by(p3,44,36), produced_by(p30,18,6), produced_by(p31,37,44), produced_by(p32,13,40), produced_by(p33,48,19), produced_by(p34,44,19), produced_by(p35,50,41), produced_by(p36,5,5), produced_by(p37,8,29), produced_by(p38,29,11), produced_by(p39,47,16), produced_by(p4,19,2), produced_by(p40,4,20), produced_by(p41,2,22), produced_by(p42,13,44), produced_by(p43,25,57), produced_by(p44,49,21), produced_by(p45,9,28), produced_by(p46,57,26), produced_by(p47,22,20), produced_by(p48,35,22), produced_by(p49,28,44), produced_by(p5,6,21), produced_by(p50,12,34), produced_by(p51,17,52), produced_by(p52,14,54), produced_by(p53,17,1), produced_by(p54,44,50), produced_by(p55,43,23), produced_by(p56,3,60), produced_by(p57,3,59), produced_by(p58,20,45), produced_by(p59,39,31), produced_by(p6,17,9), produced_by(p60,55,55), produced_by(p61,46,6), produced_by(p62,10,34), produced_by(p63,23,1), produced_by(p64,45,54), produced_by(p65,49,6), produced_by(p66,40,12), produced_by(p67,8,40), produced_by(p68,38,24), produced_by(p69,35,27), produced_by(p7,2,1), produced_by(p70,36,37), produced_by(p71,49,60), produced_by(p72,22,11), produced_by(p73,17,41), produced_by(p74,51,25), produced_by(p75,48,9), produced_by(p76,43,22), produced_by(p77,52,38), produced_by(p78,56,38), produced_by(p79,18,35), produced_by(p8,33,33), produced_by(p80,19,41), produced_by(p81,18,53), produced_by(p82,24,23), produced_by(p83,46,10), produced_by(p84,37,54), produced_by(p85,28,35), produced_by(p86,39,23), produced_by(p87,27,34), produced_by(p88,54,20), produced_by(p89,3,47), produced_by(p9,15,18), produced_by(p90,45,42), produced_by(p91,53,42), produced_by(p92,54,18), produced_by(p93,5,57), produced_by(p94,20,13), produced_by(p95,11,47), produced_by(p96,51,31), produced_by(p97,31,56), produced_by(p98,15,29), produced_by(p99,40,8), strategic(1), strategic(10), strategic(11), strategic(12), strategic(13), strategic(14), strategic(15), strategic(16), strategic(17), strategic(18), strategic(19), strategic(2), strategic(20), strategic(21), strategic(22), strategic(23), strategic(24), strategic(25), strategic(26), strategic(27), strategic(28), strategic(29), strategic(3), strategic(30), strategic(31), strategic(32), strategic(33), strategic(34), strategic(35), strategic(36), strategic(37), strategic(38), strategic(39), strategic(4), strategic(40), strategic(41), strategic(42), strategic(43), strategic(44), strategic(45), strategic(46), strategic(47), strategic(48), strategic(49), strategic(5), strategic(50), strategic(51), strategic(52), strategic(53), strategic(54), strategic(55), strategic(56), strategic(57), strategic(58), strategic(59), strategic(6), strategic(60), strategic(7), strategic(8), strategic(9), true}
"""
