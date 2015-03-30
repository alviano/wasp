input = """
p6|p9|p6|not_p22:-p19,p23. 
p2|p24|p15:-not p23. 
p1|p12|p2|not_p16:-not p23,not p3. 
p3|p20|p18:-p1,not p22. 
p3|not_p20|p18:-p13,not p22,not p14. 
p15|p4|p3|p1:-p12,p5. 
p3|p20|p18|p3:-p1,not p22. 
not_p2|not_p24|p9:-p1,p12,not p11. 
p15|p11|p24|p3. 
not_p4:-not p4. 
not_p23|p7|p21|p5:-p13,not p3. 
p1|p15|p15|not_p18:-not p5,p24. 
p12|not_p15|p16|p17:-p6,p1. 
p15|p6:-p19,p5,not p3,not p7. 
p24|p9|p24|not_p22:-p12,p23. 
p21|p22|p11|p25|not_p20:-p17. 
p17|p6|not_p19|not_p8. 
p5|p11|p25|not_p20:-p17,not p7. 
p12|not_p15|p16|p18:-p9,p1. 
:-p16,not_p16. 
"""
output = """
{p24, p1, p3, not_p4, not_p8}
{p2, p3, not_p4, not_p8}
{p24, p1, p3, not_p4, not_p19}
{p2, p3, not_p4, not_p19}
{p24, p1, p3, not_p4, p16, p6}
{p24, p1, p3, not_p4, not_p15, p6}
{p24, p1, p12, p3, p9, not_p4, p6}
{p24, p1, p12, p3, not_p24, not_p4, p6}
{p24, p1, p12, p3, not_p2, not_p4, p6}
{p2, p3, not_p4, p6}
{p24, p1, p3, not_p4, p17, not_p20}
{p24, p1, p3, not_p4, p17, p25}
{p24, p1, p3, p11, not_p4, p17}
{p2, p3, not_p4, p17, not_p20}
{p2, p3, not_p4, p17, p25}
{p2, p3, p5, not_p4, p17, p21}
{p2, p3, p22, p5, not_p4, p17}
{p15, not_p16, not_p4, not_p8}
{p15, not_p16, not_p4, not_p19}
{p15, not_p16, not_p4, p6}
{p15, not_p16, not_p4, p17, not_p20}
{p15, not_p16, not_p4, p17, p25}
{p15, not_p16, p11, not_p4, p17}
{p15, not_p16, p5, not_p4, p17, p21}
{p15, not_p16, p22, p5, not_p4, p17}
{p2, p15, not_p4, not_p8}
{p15, p12, not_p4, not_p8}
{p15, p1, p18, not_p4, not_p8}
{p15, p1, p20, not_p4, not_p8}
{p2, p15, not_p4, not_p19}
{p15, p12, not_p4, not_p19}
{p15, p1, p18, not_p4, not_p19}
{p15, p1, p20, not_p4, not_p19}
{p2, p15, not_p4, p6}
{p15, p12, not_p4, p6}
{p15, p1, p18, not_p4, p16, p6}
{p15, p1, p18, not_p4, not_p15, p6}
{p15, p1, p20, not_p4, p16, p6}
{p15, p1, p20, not_p4, not_p15, p6}
{p2, p15, not_p4, p17, not_p20}
{p2, p15, not_p4, p17, p25}
{p2, p15, p5, not_p4, p17, p21}
{p2, p15, p22, p5, not_p4, p17}
{p15, p12, not_p4, p17, not_p20}
{p15, p1, p18, not_p4, p17, not_p20}
{p15, p1, p20, not_p4, p17, not_p20}
{p15, p12, not_p4, p17, p25}
{p15, p1, p18, not_p4, p17, p25}
{p15, p1, p20, not_p4, p17, p25}
{p15, p12, p11, not_p4, p17}
{p15, p1, p18, p11, not_p4, p17}
{p15, p1, p20, p11, not_p4, p17}
{p15, p12, p5, not_p4, p17, p21}
{p15, p12, p22, p5, not_p4, p17}
{p15, p1, p22, p5, not_p4, p17}
{p15, p1, p18, p5, not_p4, p17, p21}
{p15, p1, p20, p5, not_p4, p17, p21}
{p24, not_p16, not_p4, not_p18, not_p8}
{p2, p11, not_p4, not_p8}
{p2, p24, not_p4, not_p18, not_p8}
{p24, p12, not_p4, not_p18, not_p8}
{p24, p1, p18, not_p4, not_p8}
{p24, p1, p20, not_p4, not_p8}
{p24, not_p16, not_p4, not_p18, not_p19}
{p2, p11, not_p4, not_p19}
{p2, p24, not_p4, not_p18, not_p19}
{p24, p12, not_p4, not_p18, not_p19}
{p24, p1, p18, not_p4, not_p19}
{p24, p1, p20, not_p4, not_p19}
{p2, p11, not_p4, p6}
{p2, p11, not_p4, p17}
{p24, not_p16, p5, not_p4, p17, p21}
{p24, not_p16, p22, p5, not_p4, p17}
{p24, not_p16, not_p4, not_p18, p6}
{p24, not_p16, not_p4, not_p18, p17, not_p20}
{p24, not_p16, not_p4, not_p18, p17, p25}
{p24, not_p16, p11, not_p4, not_p18, p17}
{p2, p24, p5, not_p4, p17, p21}
{p2, p24, p22, p5, not_p4, p17}
{p2, p24, not_p4, not_p18, p6}
{p2, p24, not_p4, not_p18, p17, not_p20}
{p2, p24, not_p4, not_p18, p17, p25}
{p24, p1, p22, p5, not_p4, p17}
{p24, p1, p18, not_p4, p16, p6}
{p24, p1, p18, not_p4, not_p15, p6}
{p24, p1, p12, p18, p9, not_p4, p6}
{p24, p1, p12, p18, not_p24, not_p4, p6}
{p24, p1, p12, p18, not_p2, not_p4, p6}
{p24, p1, p20, not_p4, p16, p6}
{p24, p1, p20, not_p4, not_p15, p6}
{p24, p1, p12, p20, p9, not_p4, p6}
{p24, p1, p12, p20, not_p24, not_p4, p6}
{p24, p1, p12, p20, not_p2, not_p4, p6}
{p24, p1, p18, not_p4, p17, not_p20}
{p24, p1, p18, not_p4, p17, p25}
{p24, p1, p18, p11, not_p4, p17}
{p24, p1, p18, p5, not_p4, p17, p21}
{p24, p1, p20, not_p4, p17, not_p20}
{p24, p1, p20, not_p4, p17, p25}
{p24, p1, p20, p11, not_p4, p17}
{p24, p1, p20, p5, not_p4, p17, p21}
{p24, p12, p4, p5, p17, p21}
{p24, p12, p22, p4, p5, p17}
{p24, p12, not_p4, not_p18, p6}
{p24, p12, not_p4, not_p18, p17, not_p20}
{p24, p12, not_p4, not_p18, p17, p25}
{p24, p12, p11, not_p4, not_p18, p17}
"""
