input = """
% strat Companies
%
% As we want to produce X, Y or Z must be strategic.

strategic(Y) | strategic(Z) :- producedBy(X,Y,Z).

% W is strategic, if it is controlled by strategic 
% companies X, Y, and Z  

strategic(W) :- controlledBy(W,X,Y,Z), strat(X), strat(Y), strat(Z). 

% Handle special 0 symbol 
:- strategic(0). 
strat(X) :- strategic(X).
strat(0) :- true. 
true. 

controlledBy(1,10,13,13).
controlledBy(2,30,30,30).
controlledBy(3,9,46,46).
controlledBy(4,55,3,3).
controlledBy(4,55,38,57).
controlledBy(5,18,49,49).
controlledBy(5,18,51,44).
controlledBy(6,1,23,54).
controlledBy(6,1,31,33).
controlledBy(7,16,44,8).
controlledBy(8,14,56,56).
controlledBy(9,21,21,21).
controlledBy(10,5,48,48).
controlledBy(10,5,56,56).
controlledBy(11,28,49,55).
controlledBy(11,28,47,57).
controlledBy(12,35,10,10).
controlledBy(13,4,36,36).
controlledBy(14,16,2,22).
controlledBy(14,16,10,55).
controlledBy(15,19,19,19).
controlledBy(16,33,33,33).
controlledBy(17,51,49,49).
controlledBy(17,51,16,16).
controlledBy(18,9,9,9).
controlledBy(19,9,34,34).
controlledBy(20,52,4,4).
controlledBy(21,40,4,4).
controlledBy(22,26,39,24).
controlledBy(22,26,60,36).
controlledBy(23,10,39,39).
controlledBy(23,10,49,49).
controlledBy(24,3,3,3).
controlledBy(25,7,1,1).
controlledBy(25,7,29,52).
controlledBy(26,33,53,21).
controlledBy(27,29,33,33).
controlledBy(28,59,52,52).
controlledBy(29,3,48,2).
controlledBy(29,3,48,39).
controlledBy(30,25,50,9).
controlledBy(30,25,20,3).
controlledBy(31,20,3,3).
controlledBy(31,20,45,45).
controlledBy(32,51,11,20).
controlledBy(32,51,28,49).
controlledBy(33,4,60,60).
controlledBy(33,4,42,29).
controlledBy(34,46,42,42).
controlledBy(34,46,35,35).
controlledBy(35,60,55,20).
controlledBy(35,60,13,54).
controlledBy(36,3,3,3).
controlledBy(37,4,50,50).
controlledBy(37,4,17,23).
controlledBy(38,56,11,11).
controlledBy(39,44,57,57).
controlledBy(39,44,14,43).
controlledBy(40,14,24,24).
controlledBy(40,14,35,35).
controlledBy(41,54,7,31).
controlledBy(41,54,7,55).
controlledBy(42,56,31,31).
controlledBy(43,17,33,33).
controlledBy(43,17,56,56).
controlledBy(44,33,21,21).
controlledBy(44,33,48,48).
controlledBy(45,56,12,12).
controlledBy(46,30,2,2).
controlledBy(47,3,48,48).
controlledBy(47,3,60,60).
controlledBy(48,46,8,8).
controlledBy(49,15,37,35).
controlledBy(50,41,21,21).
controlledBy(50,41,58,5).
controlledBy(51,5,27,27).
controlledBy(51,5,13,13).
controlledBy(52,57,57,57).
controlledBy(53,51,2,2).
controlledBy(54,42,5,50).
controlledBy(55,14,59,59).
controlledBy(55,14,17,17).
controlledBy(56,20,48,25).
controlledBy(56,20,39,38).
controlledBy(57,26,13,2).
controlledBy(57,26,13,38).
controlledBy(58,31,45,45).
controlledBy(59,25,52,13).
controlledBy(59,25,29,21).
controlledBy(60,31,25,33).
producedBy(p1, 3,4).
producedBy(p2, 19,29).
producedBy(p3, 44,36).
producedBy(p4, 19,2).
producedBy(p5, 6,21).
producedBy(p6, 17,9).
producedBy(p7, 2,1).
producedBy(p8, 33,33).
producedBy(p9, 15,18).
producedBy(p10, 39,22).
producedBy(p11, 57,18).
producedBy(p12, 30,31).
producedBy(p13, 39,32).
producedBy(p14, 37,42).
producedBy(p15, 29,26).
producedBy(p16, 21,54).
producedBy(p17, 36,53).
producedBy(p18, 20,29).
producedBy(p19, 41,16).
producedBy(p20, 47,57).
producedBy(p21, 30,36).
producedBy(p22, 4,40).
producedBy(p23, 56,2).
producedBy(p24, 50,23).
producedBy(p25, 44,22).
producedBy(p26, 58,56).
producedBy(p27, 25,46).
producedBy(p28, 51,7).
producedBy(p29, 11,53).
producedBy(p30, 18,6).
producedBy(p31, 37,44).
producedBy(p32, 13,40).
producedBy(p33, 48,19).
producedBy(p34, 44,19).
producedBy(p35, 50,41).
producedBy(p36, 5,5).
producedBy(p37, 8,29).
producedBy(p38, 29,11).
producedBy(p39, 47,16).
producedBy(p40, 4,20).
producedBy(p41, 2,22).
producedBy(p42, 13,44).
producedBy(p43, 25,57).
producedBy(p44, 49,21).
producedBy(p45, 9,28).
producedBy(p46, 57,26).
producedBy(p47, 22,20).
producedBy(p48, 35,22).
producedBy(p49, 28,44).
producedBy(p50, 12,34).
producedBy(p51, 17,52).
producedBy(p52, 14,54).
producedBy(p53, 17,1).
producedBy(p54, 44,50).
producedBy(p55, 43,23).
producedBy(p56, 3,60).
producedBy(p57, 3,59).
producedBy(p58, 20,45).
producedBy(p59, 39,31).
producedBy(p60, 55,55).
producedBy(p61, 46,6).
producedBy(p62, 10,34).
producedBy(p63, 23,1).
producedBy(p64, 45,54).
producedBy(p65, 49,6).
producedBy(p66, 40,12).
producedBy(p67, 8,40).
producedBy(p68, 38,24).
producedBy(p69, 35,27).
producedBy(p70, 36,37).
producedBy(p71, 49,60).
producedBy(p72, 22,11).
producedBy(p73, 17,41).
producedBy(p74, 51,25).
producedBy(p75, 48,9).
producedBy(p76, 43,22).
producedBy(p77, 52,38).
producedBy(p78, 56,38).
producedBy(p79, 18,35).
producedBy(p80, 19,41).
producedBy(p81, 18,53).
producedBy(p82, 24,23).
producedBy(p83, 46,10).
producedBy(p84, 37,54).
producedBy(p85, 28,35).
producedBy(p86, 39,23).
producedBy(p87, 27,34).
producedBy(p88, 54,20).
producedBy(p89, 3,47).
producedBy(p90, 45,42).
producedBy(p91, 53,42).
producedBy(p92, 54,18).
producedBy(p93, 5,57).
producedBy(p94, 20,13).
producedBy(p95, 11,47).
producedBy(p96, 51,31).
producedBy(p97, 31,56).
producedBy(p98, 15,29).
producedBy(p99, 40,8).
producedBy(p100, 2,10).
producedBy(p101, 7,46).
producedBy(p102, 50,2).
producedBy(p103, 23,38).
producedBy(p104, 51,9).
producedBy(p105, 13,25).
producedBy(p106, 3,54).
producedBy(p107, 57,23).
producedBy(p108, 48,24).
producedBy(p109, 32,18).
producedBy(p110, 33,29).
producedBy(p111, 24,21).
producedBy(p112, 31,40).
producedBy(p113, 20,40).
producedBy(p114, 58,24).
producedBy(p115, 36,22).
producedBy(p116, 34,22).
producedBy(p117, 28,19).
producedBy(p118, 20,17).
producedBy(p119, 16,19).
producedBy(p120, 25,57).
producedBy(p121, 32,15).
producedBy(p122, 21,51).
producedBy(p123, 24,59).
producedBy(p124, 16,58).
producedBy(p125, 13,46).
producedBy(p126, 17,52).
producedBy(p127, 3,47).
producedBy(p128, 12,56).
producedBy(p129, 22,28).
producedBy(p130, 8,6).
producedBy(p131, 36,39).
producedBy(p132, 25,24).
producedBy(p133, 43,39).
producedBy(p134, 35,59).
producedBy(p135, 59,21).
producedBy(p136, 59,15).
producedBy(p137, 6,7).
producedBy(p138, 42,34).
producedBy(p139, 42,47).
producedBy(p140, 59,17).
producedBy(p141, 33,40).
producedBy(p142, 54,16).
producedBy(p143, 55,40).
producedBy(p144, 25,3).
producedBy(p145, 8,48).
producedBy(p146, 42,29).
producedBy(p147, 35,35).
producedBy(p148, 26,10).
producedBy(p149, 21,1).
producedBy(p150, 48,38).
producedBy(p151, 58,33).
producedBy(p152, 21,16).
producedBy(p153, 13,26).
producedBy(p154, 11,4).
producedBy(p155, 40,22).
producedBy(p156, 25,48).
producedBy(p157, 2,36).
producedBy(p158, 33,7).
producedBy(p159, 36,33).
producedBy(p160, 31,5).
producedBy(p161, 45,9).
producedBy(p162, 20,30).
producedBy(p163, 57,31).
producedBy(p164, 34,18).
producedBy(p165, 4,38).
producedBy(p166, 55,48).
producedBy(p167, 15,35).
producedBy(p168, 17,17).
producedBy(p169, 8,58).
producedBy(p170, 20,55).
producedBy(p171, 1,41).
producedBy(p172, 17,49).
producedBy(p173, 30,43).
producedBy(p174, 5,11).
producedBy(p175, 2,48).
producedBy(p176, 22,38).
producedBy(p177, 47,38).
producedBy(p178, 42,57).
producedBy(p179, 10,30).
producedBy(p180, 1,34).
"""
output = """
{producedBy(p1,3,4), producedBy(p2,19,29), producedBy(p3,44,36), producedBy(p4,19,2), producedBy(p5,6,21), producedBy(p6,17,9), producedBy(p7,2,1), producedBy(p8,33,33), producedBy(p9,15,18), producedBy(p10,39,22), producedBy(p11,57,18), producedBy(p12,30,31), producedBy(p13,39,32), producedBy(p14,37,42), producedBy(p15,29,26), producedBy(p16,21,54), producedBy(p17,36,53), producedBy(p18,20,29), producedBy(p19,41,16), producedBy(p20,47,57), producedBy(p21,30,36), producedBy(p22,4,40), producedBy(p23,56,2), producedBy(p24,50,23), producedBy(p25,44,22), producedBy(p26,58,56), producedBy(p27,25,46), producedBy(p28,51,7), producedBy(p29,11,53), producedBy(p30,18,6), producedBy(p31,37,44), producedBy(p32,13,40), producedBy(p33,48,19), producedBy(p34,44,19), producedBy(p35,50,41), producedBy(p36,5,5), producedBy(p37,8,29), producedBy(p38,29,11), producedBy(p39,47,16), producedBy(p40,4,20), producedBy(p41,2,22), producedBy(p42,13,44), producedBy(p43,25,57), producedBy(p44,49,21), producedBy(p45,9,28), producedBy(p46,57,26), producedBy(p47,22,20), producedBy(p48,35,22), producedBy(p49,28,44), producedBy(p50,12,34), producedBy(p51,17,52), producedBy(p52,14,54), producedBy(p53,17,1), producedBy(p54,44,50), producedBy(p55,43,23), producedBy(p56,3,60), producedBy(p57,3,59), producedBy(p58,20,45), producedBy(p59,39,31), producedBy(p60,55,55), producedBy(p61,46,6), producedBy(p62,10,34), producedBy(p63,23,1), producedBy(p64,45,54), producedBy(p65,49,6), producedBy(p66,40,12), producedBy(p67,8,40), producedBy(p68,38,24), producedBy(p69,35,27), producedBy(p70,36,37), producedBy(p71,49,60), producedBy(p72,22,11), producedBy(p73,17,41), producedBy(p74,51,25), producedBy(p75,48,9), producedBy(p76,43,22), producedBy(p77,52,38), producedBy(p78,56,38), producedBy(p79,18,35), producedBy(p80,19,41), producedBy(p81,18,53), producedBy(p82,24,23), producedBy(p83,46,10), producedBy(p84,37,54), producedBy(p85,28,35), producedBy(p86,39,23), producedBy(p87,27,34), producedBy(p88,54,20), producedBy(p89,3,47), producedBy(p90,45,42), producedBy(p91,53,42), producedBy(p92,54,18), producedBy(p93,5,57), producedBy(p94,20,13), producedBy(p95,11,47), producedBy(p96,51,31), producedBy(p97,31,56), producedBy(p98,15,29), producedBy(p99,40,8), producedBy(p100,2,10), producedBy(p101,7,46), producedBy(p102,50,2), producedBy(p103,23,38), producedBy(p104,51,9), producedBy(p105,13,25), producedBy(p106,3,54), producedBy(p107,57,23), producedBy(p108,48,24), producedBy(p109,32,18), producedBy(p110,33,29), producedBy(p111,24,21), producedBy(p112,31,40), producedBy(p113,20,40), producedBy(p114,58,24), producedBy(p115,36,22), producedBy(p116,34,22), producedBy(p117,28,19), producedBy(p118,20,17), producedBy(p119,16,19), producedBy(p120,25,57), producedBy(p121,32,15), producedBy(p122,21,51), producedBy(p123,24,59), producedBy(p124,16,58), producedBy(p125,13,46), producedBy(p126,17,52), producedBy(p127,3,47), producedBy(p128,12,56), producedBy(p129,22,28), producedBy(p130,8,6), producedBy(p131,36,39), producedBy(p132,25,24), producedBy(p133,43,39), producedBy(p134,35,59), producedBy(p135,59,21), producedBy(p136,59,15), producedBy(p137,6,7), producedBy(p138,42,34), producedBy(p139,42,47), producedBy(p140,59,17), producedBy(p141,33,40), producedBy(p142,54,16), producedBy(p143,55,40), producedBy(p144,25,3), producedBy(p145,8,48), producedBy(p146,42,29), producedBy(p147,35,35), producedBy(p148,26,10), producedBy(p149,21,1), producedBy(p150,48,38), producedBy(p151,58,33), producedBy(p152,21,16), producedBy(p153,13,26), producedBy(p154,11,4), producedBy(p155,40,22), producedBy(p156,25,48), producedBy(p157,2,36), producedBy(p158,33,7), producedBy(p159,36,33), producedBy(p160,31,5), producedBy(p161,45,9), producedBy(p162,20,30), producedBy(p163,57,31), producedBy(p164,34,18), producedBy(p165,4,38), producedBy(p166,55,48), producedBy(p167,15,35), producedBy(p168,17,17), producedBy(p169,8,58), producedBy(p170,20,55), producedBy(p171,1,41), producedBy(p172,17,49), producedBy(p173,30,43), producedBy(p174,5,11), producedBy(p175,2,48), producedBy(p176,22,38), producedBy(p177,47,38), producedBy(p178,42,57), producedBy(p179,10,30), producedBy(p180,1,34), controlledBy(1,10,13,13), controlledBy(2,30,30,30), controlledBy(3,9,46,46), controlledBy(4,55,3,3), controlledBy(4,55,38,57), controlledBy(5,18,49,49), controlledBy(5,18,51,44), controlledBy(6,1,23,54), controlledBy(6,1,31,33), controlledBy(7,16,44,8), controlledBy(8,14,56,56), controlledBy(9,21,21,21), controlledBy(10,5,48,48), controlledBy(10,5,56,56), controlledBy(11,28,47,57), controlledBy(11,28,49,55), controlledBy(12,35,10,10), controlledBy(13,4,36,36), controlledBy(14,16,2,22), controlledBy(14,16,10,55), controlledBy(15,19,19,19), controlledBy(16,33,33,33), controlledBy(17,51,16,16), controlledBy(17,51,49,49), controlledBy(18,9,9,9), controlledBy(19,9,34,34), controlledBy(20,52,4,4), controlledBy(21,40,4,4), controlledBy(22,26,39,24), controlledBy(22,26,60,36), controlledBy(23,10,39,39), controlledBy(23,10,49,49), controlledBy(24,3,3,3), controlledBy(25,7,1,1), controlledBy(25,7,29,52), controlledBy(26,33,53,21), controlledBy(27,29,33,33), controlledBy(28,59,52,52), controlledBy(29,3,48,2), controlledBy(29,3,48,39), controlledBy(30,25,20,3), controlledBy(30,25,50,9), controlledBy(31,20,3,3), controlledBy(31,20,45,45), controlledBy(32,51,11,20), controlledBy(32,51,28,49), controlledBy(33,4,42,29), controlledBy(33,4,60,60), controlledBy(34,46,35,35), controlledBy(34,46,42,42), controlledBy(35,60,13,54), controlledBy(35,60,55,20), controlledBy(36,3,3,3), controlledBy(37,4,17,23), controlledBy(37,4,50,50), controlledBy(38,56,11,11), controlledBy(39,44,14,43), controlledBy(39,44,57,57), controlledBy(40,14,24,24), controlledBy(40,14,35,35), controlledBy(41,54,7,31), controlledBy(41,54,7,55), controlledBy(42,56,31,31), controlledBy(43,17,33,33), controlledBy(43,17,56,56), controlledBy(44,33,21,21), controlledBy(44,33,48,48), controlledBy(45,56,12,12), controlledBy(46,30,2,2), controlledBy(47,3,48,48), controlledBy(47,3,60,60), controlledBy(48,46,8,8), controlledBy(49,15,37,35), controlledBy(50,41,21,21), controlledBy(50,41,58,5), controlledBy(51,5,13,13), controlledBy(51,5,27,27), controlledBy(52,57,57,57), controlledBy(53,51,2,2), controlledBy(54,42,5,50), controlledBy(55,14,17,17), controlledBy(55,14,59,59), controlledBy(56,20,39,38), controlledBy(56,20,48,25), controlledBy(57,26,13,2), controlledBy(57,26,13,38), controlledBy(58,31,45,45), controlledBy(59,25,29,21), controlledBy(59,25,52,13), controlledBy(60,31,25,33), true, strategic(5), strategic(16), strategic(17), strategic(33), strategic(35), strategic(43), strategic(55), strat(0), strat(5), strat(16), strat(17), strat(33), strat(35), strat(43), strat(55), strategic(3), strategic(4), strategic(19), strategic(29), strategic(44), strategic(36), strategic(2), strategic(6), strategic(21), strategic(9), strategic(1), strategic(15), strategic(18), strategic(39), strategic(22), strategic(57), strategic(30), strategic(31), strategic(32), strategic(37), strategic(42), strategic(26), strategic(54), strategic(53), strategic(20), strategic(41), strategic(47), strategic(40), strategic(56), strategic(50), strategic(23), strategic(58), strategic(25), strategic(46), strategic(51), strategic(7), strategic(11), strategic(13), strategic(48), strategic(8), strategic(49), strategic(28), strategic(12), strategic(34), strategic(52), strategic(14), strategic(60), strategic(59), strategic(45), strategic(10), strategic(38), strategic(24), strategic(27), strat(1), strat(2), strat(3), strat(4), strat(6), strat(7), strat(8), strat(9), strat(10), strat(11), strat(12), strat(13), strat(14), strat(15), strat(18), strat(19), strat(20), strat(21), strat(22), strat(23), strat(24), strat(25), strat(26), strat(27), strat(28), strat(29), strat(30), strat(31), strat(32), strat(34), strat(36), strat(37), strat(38), strat(39), strat(40), strat(41), strat(42), strat(44), strat(45), strat(46), strat(47), strat(48), strat(49), strat(50), strat(51), strat(52), strat(53), strat(54), strat(56), strat(57), strat(58), strat(59), strat(60)}
"""
