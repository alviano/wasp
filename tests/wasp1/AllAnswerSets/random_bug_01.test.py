input = """
p35 :-  p31,p12,p48.
p37 :-  p28,p25,not p81.
p11 :-  p88,p22,p27.
p42 :-  p3,p73,not p6.
p90 :-  p53,p75,p65.
p26 :-  p96,p7,not p66.
p19 :-  not p30,not p5,not p24.
p93 :-  p9,not p63,p86.
p1 :-  p56,p60,not p26.
p13 :-  p34,not p22,p44.
p18 :-  not p91,p53,p35.
p23 :-  not p62,p32,p25.
p86 :-  not p63,not p52,p66.
p20 :-  p38,not p67,p7.
p70 :-  p87,p20,not p37.
p33 :-  not p33,not p55,p68.
p36 :-  not p93,p48,p78.
p39 :-  p62,not p38,not p81.
p10 :-  p78,p39,not p94.
p94 :-  p1,not p63,not p58.
p94 :-  not p12,p77,p25.
p60 :-  p28,p86,not p26.
p21 :-  p48,p60,p62.
p69 :-  not p35,p2,p74.
p1 :-  not p78,not p61,p23.
p48 :-  not p0,not p44,not p21.
p17 :-  not p7,not p70,not p34.
p53 :-  not p92,not p79,not p62.
p87 :-  p77,p30,p78.
p99 :-  not p86,p36,not p47.
p37 :-  p4,not p86,p50.
p50 :-  not p38,p4,p47.
p20 :-  not p74,p73,p92.
p63 :-  not p0,p83,not p96.
p41 :-  p85,p99,not p1.
p80 :-  p93,p69,not p27.
p18 :-  not p56,not p84,p0.
p7 :-  not p58,not p72,p92.
p60 :-  p5,p75,not p15.
p25 :-  p49,p7,p16.
p20 :-  p70,p33,not p58.
p37 :-  not p72,not p3,not p52.
p2 :-  not p36,p24,not p9.
p19 :-  not p91,not p40,p41.
p51 :-  not p8,p97,p3.
p98 :-  p34,p46,p91.
p69 :-  p50,not p52,not p12.
p25 :-  not p50,p61,p7.
p55 :-  p19,not p74,not p39.
p96 :-  not p10,not p50,not p21.
p4 :-  p83,not p37,p65.
p37 :-  p65,not p86,not p78.
p18 :-  not p80,not p69,not p22.
p74 :-  p16,not p70,p31.
p11 :-  p56,not p22,not p14.
p88 :-  p95,not p21,not p48.
p71 :-  not p28,p5,p54.
p74 :-  not p20,not p77,p60.
p45 :-  not p77,p73,not p65.
p70 :-  not p47,p2,p15.
p67 :-  p43,not p28,not p88.
p52 :-  p99,p97,not p2.
p97 :-  not p0,not p88,p0.
p39 :-  not p20,not p80,not p3.
p38 :-  not p80,p48,not p7.
p18 :-  not p35,not p88,p29.
p39 :-  p19,p0,p89.
p4 :-  not p13,p96,p50.
p35 :-  p62,p56,not p99.
p20 :-  p93,p5,not p64.
p42 :-  not p94,p12,p18.
p77 :-  p52,p69,p28.
p95 :-  not p78,not p13,not p12.
p96 :-  not p4,p63,not p15.
p69 :-  p20,not p17,p98.
p86 :-  not p10,p69,not p46.
p45 :-  p77,p35,p38.
p56 :-  not p67,p86,not p51.
p42 :-  not p25,p73,p85.
p84 :-  p0,not p25,not p38.
p15 :-  p7,not p18,p84.
p55 :-  not p60,p93,p38.
p95 :-  p99,p3,not p62.
p11 :-  not p17,p48,p59.
p21 :-  p73,not p69,p11.
p71 :-  p74,p68,p39.
p44 :-  p42,p75,not p9.
p58 :-  not p32,not p26,p29.
p32 :-  p97,not p27,not p42.
p18 :-  not p42,not p14,not p78.
p73 :-  p77,p4,not p31.
p48 :-  p32,p35,p22.
p79 :-  not p10,p80,p95.
p8 :-  p4,not p83,not p48.
p70 :-  p22,not p13,not p0.
p6 :-  p11,not p77,p38.
p56 :-  not p66,p21,not p37.
p88 :-  not p34,not p76,not p4.
p9 :-  p30,not p25,not p15.
p33 :-  not p36,not p47,p40.
p26 :-  p75,p5,not p4.
p21 :-  p51,not p31,not p15.
p77 :-  p68,p50,p42.
p73 :-  not p11,not p97,not p6.
p63 :-  not p76,p61,not p99.
p97 :-  p73,p82,p70.
p62 :-  not p80,not p28,not p52.
p60 :-  not p14,not p4,not p64.
p83 :-  p89,not p61,p2.
p4 :-  p79,not p4,not p52.
p64 :-  not p84,p98,not p27.
p38 :-  not p4,not p67,not p70.
p64 :-  not p88,not p52,p50.
p10 :-  not p7,not p2,not p4.
p81 :-  not p51,p56,p21.
p82 :-  p64,not p92,p0.
p20 :-  not p71,p94,not p59.
p46 :-  not p76,p22,not p0.
p77 :-  p28,p52,not p44.
p41 :-  p84,not p30,p76.
p89 :-  not p98,p57,not p64.
p42 :-  p16,p95,p44.
p98 :-  p25,p31,not p34.
p78 :-  not p22,p6,p5.
p64 :-  p49,p90,p19.
p6 :-  not p69,p35,p23.
p62 :-  not p27,p82,p62.
p93 :-  p0,p77,not p65.
p63 :-  not p66,p62,p25.
p61 :-  not p5,p71,not p55.
p69 :-  p94,not p5,p48.
p26 :-  p47,p4,p13.
p71 :-  not p88,p25,p50.
p30 :-  p18,p81,p64.
p24 :-  not p2,not p52,not p91.
p90 :-  p64,not p38,p6.
p73 :-  p85,not p14,p47.
p97 :-  not p45,not p85,not p81.
p99 :-  not p54,not p1,not p28.
p82 :-  p89,not p59,p88.
p1 :-  not p54,p10,not p10.
p71 :-  not p16,not p80,p27.
p97 :-  not p12,p65,not p41.
p38 :-  not p16,not p47,not p60.
p78 :-  p42,not p83,p2.
p64 :-  not p73,not p82,not p8.
p79 :-  not p19,p0,not p80.
p31 :-  not p29,p84,not p63.
p72 :-  p83,not p65,not p78.
p65 :-  not p14,not p18,p89.
p66 :-  p21,p18,p48.
p57 :-  p42,not p26,p86.
p15 :-  not p6,p4,not p18.
p33 :-  not p28,p7,p72.
p55 :-  p36,p30,not p47.
p55 :-  not p1,p82,not p45.
p57 :-  not p36,not p89,p51.
p64 :-  p60,not p26,p4.
p33 :-  p1,p81,not p95.
p83 :-  not p4,not p67,p93.
p28 :-  p16,p81,not p92.
p48 :-  not p41,p68,p78.
p6 :-  not p25,p54,not p76.
p12 :-  p7,not p39,p82.
p29 :-  p29,not p42,p75.
p93 :-  p74,not p86,p78.
p28 :-  p94,p11,not p0.
p92 :-  p91,p34,not p49.
p28 :-  not p52,p52,not p52.
p91 :-  not p86,p5,not p91.
p29 :-  p0,not p43,not p38.
p92 :-  p48,p43,p12.
p1 :-  p31,p90,not p47.
p6 :-  p92,p63,p30.
p13 :-  p79,not p76,not p70.
p53 :-  p12,p12,not p40.
p60 :-  not p3,not p28,p69.
p0 :-  p53,p35,not p15.
p21 :-  p36,p68,p34.
p77 :-  not p37,p91,p39.
p71 :-  not p32,p69,p69.
p57 :-  p33,not p16,p19.
p91 :-  p13,not p10,not p42.
p91 :-  p40,not p51,not p46.
p88 :-  p60,p60,not p65.
p70 :-  p43,p40,p6.
p36 :-  p35,p72,p60.
p79 :-  not p46,p14,p5.
p99 :-  not p14,not p89,p70.
p73 :-  not p49,p1,p88.
p47 :-  not p85,not p5,not p37.
p51 :-  p36,p2,not p5.
p61 :-  not p18,p18,not p30.
p26 :-  p32,p80,not p60.
p87 :-  not p81,not p16,p87.
p38 :-  not p58,p1,p49.
p85 :-  not p34,p0,p60.
p23 :-  p66,p56,not p9.
p74 :-  not p81,not p20,p97.
p25 :-  not p22,not p65,p0.
p81 :-  not p21,p59,not p81.
p69 :-  not p99,not p72,not p57.
p24 :-  not p37,p87,p41.
p72 :-  not p64,not p7,not p18.
p13 :-  p61,p16,p36.
p35 :-  not p67,p60,not p32.
p8 :-  not p52,p51,p83.
p69 :-  not p92,not p89,p87.
p73 :-  not p45,not p8,p28.
p96 :-  p60,not p19,not p14.
p60 :-  not p66,not p73,not p8.
p86 :-  p44,not p2,not p11.
p79 :-  p77,p75,not p66.
p13 :-  p75,not p20,p59.
p33 :-  p8,p8,not p23.
p90 :-  p16,not p55,p28.
p93 :-  not p52,p57,p53.
p20 :-  not p59,p55,not p31.
p60 :-  not p15,not p21,p51.
p51 :-  p73,p12,p43.
p78 :-  p55,not p46,not p50.
p9 :-  not p18,not p6,p58.
p92 :-  not p97,p26,p84.
p87 :-  not p92,not p9,p11.
p61 :-  p7,p27,p61.
p95 :-  p86,not p38,not p16.
p97 :-  p12,not p55,not p13.
p21 :-  not p20,p13,p85.
p40 :-  p31,not p78,not p6.
p27 :-  not p18,p99,p59.
p46 :-  not p23,p30,not p41.
p60 :-  p21,p71,not p95.
p89 :-  not p67,p71,not p10.
p2 :-  p71,p60,not p81.
p17 :-  p26,not p3,p96.
p78 :-  not p93,not p78,not p90.
p18 :-  not p85,p33,not p34.
p42 :-  p52,p84,not p55.
p6 :-  p61,not p72,not p74.
p28 :-  not p79,p57,not p42.
p55 :-  not p34,p84,not p71.
p29 :-  not p58,not p60,not p87.
p35 :-  p57,p53,p88.
p32 :-  not p60,p27,p55.
p16 :-  p57,not p31,p90.
p43 :-  p46,not p49,not p38.
p65 :-  p22,p70,p91.
p33 :-  not p93,p56,p44.
p2 :-  not p43,not p48,not p97.
p84 :-  not p76,not p56,not p1.
p24 :-  not p0,p76,not p1.
p57 :-  not p41,p43,p58.
p57 :-  not p45,p73,p93.
p45 :-  not p17,p67,not p89.
p27 :-  not p90,not p78,p5.
p24 :-  not p68,not p82,not p36.
p42 :-  not p65,p5,not p84.
p69 :-  not p61,not p76,p90.
p27 :-  not p92,p82,not p27.
p30 :-  not p52,p2,not p73.
p70 :-  not p8,p9,not p88.
p46 :-  not p94,not p60,not p95.
p5 :-  not p42,not p81,not p72.
p81 :-  not p12,p88,p21.
p72 :-  p67,not p31,p35.
p56 :-  p8,p62,p46.
p81 :-  p2,not p79,p88.
p87 :-  not p91,not p81,not p36.
p84 :-  p12,not p41,p4.
p44 :-  not p94,not p23,p23.
p87 :-  p37,not p22,not p97.
p94 :-  p14,p33,p80.
p58 :-  p88,not p83,p59.
p91 :-  p58,not p54,p93.
p69 :-  p28,p99,p33.
p12 :-  not p43,p50,not p38.
p67 :-  not p89,not p99,p93.
p55 :-  p91,p12,p16.
p85 :-  not p3,not p85,p61.
p78 :-  not p2,p24,not p6.
p66 :-  p85,not p59,p95.
p3 :-  p41,not p70,not p47.
p7 :-  p36,not p66,p2.
p22 :-  p82,p12,not p53.
p79 :-  not p18,p53,p2.
p43 :-  p24,p54,p34.
p41 :-  not p69,not p64,not p25.
p53 :-  p32,not p58,not p31.
p58 :-  p98,not p65,not p52.
p66 :-  not p47,not p98,p97.
p72 :-  not p65,p88,p1.
p4 :-  not p54,p47,p64.
p90 :-  p54,not p48,p77.
p77 :-  p22,p89,not p0.
p30 :-  p1,not p59,not p95.
p8 :-  p48,not p70,p56.
p64 :-  p80,p4,p69.
p82 :-  p24,p43,p6.
p18 :-  not p64,not p25,p61.
p63 :-  p34,p28,not p9.
"""

output = """
"""
