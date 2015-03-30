input = """
p17|p12|p8|not_p8. 
p24|p9|p24|not_p22:-p12,p23. 
p11|not_p20|p8|not_p15:-p24,p2. 
p21|p5|p12|not_p20:-p14. 
p6|p4|p7|p18:-p9,p1. 
p15|not_p22|p24|not_p12. 
not_p21|p20|p18:-p1,not p22. 
not_p23|p7|p21|p5:-p13,not p3. 
p15|p6:-p19,p5,not p3,not p7. 
not_p5:-not p5. 
p12|not_p15|p16|p17:-p6,p1. 
p24|p9|p24|p10:-p12,p23. 
not_p22|p24|p15:-not p23. 
p24|p8|not_p15:-p7,p2,not p25. 
p2|p24|p15:-not p23. 
p24|p9|p24|not_p22:-p19,p23. 
not_p4:-not p4. 
p15|p4|p3|p1:-p12,p5. 
p1|p15|p15|not_p18:-not p5,p24. 
p3|p20|p18|p3:-p1,not p22. 
:-p19,not_p19. 
p6|p7:-p19,not p23,not p3,not p11. 
p3|p20|p18:-p1,not p22. 
p19|p1|p12:-p19,not p23,not p11. 
p17|p6|not_p19|not_p8. 
not_p25|p14|p9|not_p3. 
p21|p5|p12|not_p20:-p17. 
p12|not_p15|p16|p18:-p9,p1. 
not_p21|p20|p18|p3:-p13,not p22. 
not_p20|not_p1|not_p4:-p7,p2,not p25. 
p3|p21|not_p20:-p6,p23,not p4. 
p15|p11|p24|p3. 
p3|not_p20|p18:-p13,not p22,not p14. 
p1|p14|p2|p11:-not p5,p24. 
p21|p22|p12|p25|not_p20:-p14. 
p24|not_p8|p6|not_p22:-p12,p23. 
p1|p15|p15|p11:-not p5,p24. 
not_p23|p7|not_p17|p5:-p13,not p3. 
p11|not_p20|not_p1|not_p4:-p24,p2. 
p6|not_p15|p7|p17:-p6,p1. 
"""
output = """
{not_p8, p15, not_p5, not_p4, not_p3}
{not_p8, p15, not_p5, not_p4, not_p25}
{not_p8, p9, p15, not_p5, not_p4}
{not_p8, p21, p14, p15, not_p5, not_p4}
{not_p8, not_p20, p14, p15, not_p5, not_p4}
{p12, not_p8, p14, p15, not_p5, not_p4}
{p17, p21, p15, not_p5, not_p4, not_p3}
{p17, not_p20, p15, not_p5, not_p4, not_p3}
{p17, p12, p15, not_p5, not_p4, not_p3}
{p17, p21, p15, not_p5, not_p4, not_p25}
{p17, not_p20, p15, not_p5, not_p4, not_p25}
{p17, p12, p15, not_p5, not_p4, not_p25}
{p17, p21, p9, p15, not_p5, not_p4}
{p17, not_p20, p9, p15, not_p5, not_p4}
{p17, p12, p9, p15, not_p5, not_p4}
{p17, p21, p14, p15, not_p5, not_p4}
{p17, not_p20, p14, p15, not_p5, not_p4}
{p17, p12, p14, p15, not_p5, not_p4}
{p8, p15, not_p5, not_p4, not_p19, not_p3}
{p12, p15, not_p5, not_p4, not_p19, not_p3}
{p8, p6, p15, not_p5, not_p4, not_p3}
{p12, p6, p15, not_p5, not_p4, not_p3}
{p8, p15, not_p5, not_p4, not_p19, not_p25}
{p12, p15, not_p5, not_p4, not_p19, not_p25}
{p8, p6, p15, not_p5, not_p4, not_p25}
{p12, p6, p15, not_p5, not_p4, not_p25}
{p8, p9, p15, not_p5, not_p4, not_p19}
{p12, p9, p15, not_p5, not_p4, not_p19}
{p8, p21, p14, p15, not_p5, not_p4, not_p19}
{p8, not_p20, p14, p15, not_p5, not_p4, not_p19}
{p12, p14, p15, not_p5, not_p4, not_p19}
{p8, p6, p9, p15, not_p5, not_p4}
{p12, p6, p9, p15, not_p5, not_p4}
{p8, p21, p14, p6, p15, not_p5, not_p4}
{p8, not_p20, p14, p6, p15, not_p5, not_p4}
{p12, p14, p6, p15, not_p5, not_p4}
{p8, p5, p14, p15, p25, not_p4, not_p19}
{p8, p5, p14, p15, p22, not_p4, not_p19}
{not_p8, p5, p14, p15, p25, not_p4}
{p17, p5, p14, p15, p25, not_p4}
{p8, p5, p14, p6, p15, p25, not_p4}
{not_p8, p5, p14, p15, p22, not_p4}
{p17, p5, p14, p15, p22, not_p4}
{p8, p5, p14, p6, p15, p22, not_p4}
{p17, p5, p15, not_p4, not_p3}
{p17, p5, p15, not_p4, not_p25}
{p17, p5, p9, p15, not_p4}
{not_p8, p11, p24, not_p5, not_p4, not_p18, not_p3}
{not_p8, p11, p24, not_p5, not_p4, not_p18, not_p25}
{not_p8, p11, p24, p9, not_p5, not_p4, not_p18}
{not_p8, p11, p24, p21, p14, not_p5, not_p4, not_p18}
{not_p8, p11, not_p20, p24, p14, not_p5, not_p4, not_p18}
{p12, not_p8, p11, p24, p14, not_p5, not_p4, not_p18}
{p17, p11, p24, p21, not_p5, not_p4, not_p18, not_p3}
{p17, p11, not_p20, p24, not_p5, not_p4, not_p18, not_p3}
{p17, p12, p11, p24, not_p5, not_p4, not_p18, not_p3}
{p17, p11, p24, p21, not_p5, not_p4, not_p18, not_p25}
{p17, p11, not_p20, p24, not_p5, not_p4, not_p18, not_p25}
{p17, p12, p11, p24, not_p5, not_p4, not_p18, not_p25}
{p17, p11, p24, p21, p9, not_p5, not_p4, not_p18}
{p17, p11, not_p20, p24, p9, not_p5, not_p4, not_p18}
{p17, p12, p11, p24, p9, not_p5, not_p4, not_p18}
{p17, p11, p24, p21, p14, not_p5, not_p4, not_p18}
{p17, p11, not_p20, p24, p14, not_p5, not_p4, not_p18}
{p17, p12, p11, p24, p14, not_p5, not_p4, not_p18}
{p8, p11, p24, not_p5, not_p4, not_p18, not_p19, not_p3}
{p12, p11, p24, not_p5, not_p4, not_p18, not_p19, not_p3}
{p8, p11, p24, p6, not_p5, not_p4, not_p18, not_p3}
{p12, p11, p24, p6, not_p5, not_p4, not_p18, not_p3}
{p8, p11, p24, not_p5, not_p4, not_p18, not_p19, not_p25}
{p12, p11, p24, not_p5, not_p4, not_p18, not_p19, not_p25}
{p8, p11, p24, p6, not_p5, not_p4, not_p18, not_p25}
{p12, p11, p24, p6, not_p5, not_p4, not_p18, not_p25}
{p8, p11, p24, p9, not_p5, not_p4, not_p18, not_p19}
{p12, p11, p24, p9, not_p5, not_p4, not_p18, not_p19}
{p8, p11, p24, p21, p14, not_p5, not_p4, not_p18, not_p19}
{p8, p11, not_p20, p24, p14, not_p5, not_p4, not_p18, not_p19}
{p12, p11, p24, p14, not_p5, not_p4, not_p18, not_p19}
{p8, p11, p24, p6, p9, not_p5, not_p4, not_p18}
{p12, p11, p24, p6, p9, not_p5, not_p4, not_p18}
{p8, p11, p24, p21, p14, p6, not_p5, not_p4, not_p18}
{p8, p11, not_p20, p24, p14, p6, not_p5, not_p4, not_p18}
{p12, p11, p24, p14, p6, not_p5, not_p4, not_p18}
{not_p8, p24, p1, not_p21, p3, not_p5, not_p4, not_p3}
{not_p8, p24, p1, not_p21, p3, not_p5, not_p4, not_p25}
{not_p8, p24, p6, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{not_p8, not_p15, p24, p6, p9, p1, not_p21, p3, not_p5, not_p4}
{not_p8, p24, p7, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{not_p8, not_p15, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4}
{p12, not_p8, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4}
{not_p8, p24, p21, p14, p1, not_p21, p3, not_p5, not_p4}
{not_p8, not_p20, p24, p14, p1, not_p21, p3, not_p5, not_p4}
{p12, not_p8, p24, p14, p1, not_p21, p3, not_p5, not_p4}
{not_p8, p24, p1, p20, not_p5, not_p4, not_p3}
{not_p8, p24, p18, p1, not_p5, not_p4, not_p3}
{not_p8, p24, p1, p20, not_p5, not_p4, not_p25}
{not_p8, p24, p18, p1, not_p5, not_p4, not_p25}
{not_p8, p24, p18, p9, p1, not_p5, not_p4}
{not_p8, p24, p6, p9, p1, p20, not_p5, p16, not_p4}
{not_p8, p24, p7, p9, p1, p20, not_p5, p16, not_p4}
{not_p8, not_p15, p24, p6, p9, p1, p20, not_p5, not_p4}
{not_p8, not_p15, p24, p7, p9, p1, p20, not_p5, not_p4}
{p12, not_p8, p24, p7, p9, p1, p20, not_p5, not_p4}
{not_p8, p24, p21, p14, p1, p20, not_p5, not_p4}
{not_p8, not_p20, p24, p14, p1, p20, not_p5, not_p4}
{p12, not_p8, p24, p14, p1, p20, not_p5, not_p4}
{not_p8, p24, p21, p14, p18, p1, not_p5, not_p4}
{not_p8, not_p20, p24, p14, p18, p1, not_p5, not_p4}
{p12, not_p8, p24, p14, p18, p1, not_p5, not_p4}
{p12, p24, p14, p6, p1, p20, not_p5, not_p4}
{p12, p24, p14, p1, p20, not_p5, not_p4, not_p19}
{p17, p12, p24, p14, p1, p20, not_p5, not_p4}
{p12, p24, p14, p6, p1, not_p21, p3, not_p5, not_p4}
{p12, p24, p14, p1, not_p21, p3, not_p5, not_p4, not_p19}
{p17, p12, p24, p14, p1, not_p21, p3, not_p5, not_p4}
{p12, p24, p14, p6, p18, p1, not_p5, not_p4}
{p12, p24, p14, p18, p1, not_p5, not_p4, not_p19}
{p17, p12, p24, p14, p18, p1, not_p5, not_p4}
{p17, p24, p21, p14, p1, p20, not_p5, not_p4}
{p17, p24, p21, p14, p1, not_p21, p3, not_p5, not_p4}
{p17, p24, p21, p14, p18, p1, not_p5, not_p4}
{p8, p24, p21, p14, p1, p20, not_p5, not_p4, not_p19}
{p8, p24, p21, p14, p6, p1, p20, not_p5, p16, not_p4}
{p8, not_p15, p24, p21, p14, p6, p1, p20, not_p5, not_p4}
{p8, p24, p21, p14, p1, not_p21, p3, not_p5, not_p4, not_p19}
{p8, p24, p21, p14, p6, p1, not_p21, p3, not_p5, p16, not_p4}
{p8, not_p15, p24, p21, p14, p6, p1, not_p21, p3, not_p5, not_p4}
{p8, p24, p21, p14, p18, p1, not_p5, not_p4, not_p19}
{p8, p24, p21, p14, p6, p18, p1, not_p5, p16, not_p4}
{p8, not_p15, p24, p21, p14, p6, p18, p1, not_p5, not_p4}
{p17, not_p20, p24, p14, p1, p20, not_p5, not_p4}
{p17, not_p20, p24, p14, p1, not_p21, p3, not_p5, not_p4}
{p17, not_p20, p24, p14, p18, p1, not_p5, not_p4}
{p8, not_p20, p24, p14, p1, not_p21, p3, not_p5, not_p4, not_p19}
{p8, not_p20, p24, p14, p6, p1, not_p21, p3, not_p5, p16, not_p4}
{p8, not_p20, not_p15, p24, p14, p6, p1, not_p21, p3, not_p5, not_p4}
{p8, not_p20, p24, p14, p1, p20, not_p5, not_p4, not_p19}
{p8, not_p20, p24, p14, p6, p1, p20, not_p5, p16, not_p4}
{p8, not_p20, not_p15, p24, p14, p6, p1, p20, not_p5, not_p4}
{p8, not_p20, p24, p14, p18, p1, not_p5, not_p4, not_p19}
{p8, not_p20, p24, p14, p6, p18, p1, not_p5, p16, not_p4}
{p8, not_p20, not_p15, p24, p14, p6, p18, p1, not_p5, not_p4}
{p17, p24, p21, p1, p20, not_p5, not_p4, not_p3}
{p17, not_p20, p24, p1, p20, not_p5, not_p4, not_p3}
{p17, p12, p24, p1, p20, not_p5, not_p4, not_p3}
{p17, p24, p21, p1, p20, not_p5, not_p4, not_p25}
{p17, not_p20, p24, p1, p20, not_p5, not_p4, not_p25}
{p17, p12, p24, p1, p20, not_p5, not_p4, not_p25}
{p17, p24, p21, p6, p9, p1, p20, not_p5, p16, not_p4}
{p17, p24, p21, p7, p9, p1, p20, not_p5, p16, not_p4}
{p17, not_p15, p24, p21, p6, p9, p1, p20, not_p5, not_p4}
{p17, not_p15, p24, p21, p7, p9, p1, p20, not_p5, not_p4}
{p17, not_p20, p24, p6, p9, p1, p20, not_p5, p16, not_p4}
{p17, not_p20, p24, p7, p9, p1, p20, not_p5, p16, not_p4}
{p17, not_p20, not_p15, p24, p6, p9, p1, p20, not_p5, not_p4}
{p17, not_p20, not_p15, p24, p7, p9, p1, p20, not_p5, not_p4}
{p17, p12, p24, p7, p9, p1, p20, not_p5, not_p4}
{p8, p24, p1, p20, not_p5, not_p4, not_p19, not_p3}
{p12, p24, p1, p20, not_p5, not_p4, not_p19, not_p3}
{p8, p24, p6, p1, p20, not_p5, p16, not_p4, not_p3}
{p8, not_p15, p24, p6, p1, p20, not_p5, not_p4, not_p3}
{p12, p24, p6, p1, p20, not_p5, not_p4, not_p3}
{p8, p24, p1, p20, not_p5, not_p4, not_p19, not_p25}
{p12, p24, p1, p20, not_p5, not_p4, not_p19, not_p25}
{p8, p24, p6, p1, p20, not_p5, p16, not_p4, not_p25}
{p8, not_p15, p24, p6, p1, p20, not_p5, not_p4, not_p25}
{p12, p24, p6, p1, p20, not_p5, not_p4, not_p25}
{p12, p24, p7, p9, p1, p20, not_p5, not_p4, not_p19}
{p8, not_p15, p24, p7, p9, p1, p20, not_p5, not_p4, not_p19}
{p8, p24, p7, p9, p1, p20, not_p5, p16, not_p4, not_p19}
{p12, p24, p6, p9, p1, p20, not_p5, not_p4}
{p8, p24, p6, p9, p1, p20, not_p5, p16, not_p4}
{p8, not_p15, p24, p6, p9, p1, p20, not_p5, not_p4}
{p17, p24, p21, p1, not_p21, p3, not_p5, not_p4, not_p3}
{p17, not_p20, p24, p1, not_p21, p3, not_p5, not_p4, not_p3}
{p17, p12, p24, p1, not_p21, p3, not_p5, not_p4, not_p3}
{p17, p24, p21, p1, not_p21, p3, not_p5, not_p4, not_p25}
{p17, not_p20, p24, p1, not_p21, p3, not_p5, not_p4, not_p25}
{p17, p12, p24, p1, not_p21, p3, not_p5, not_p4, not_p25}
{p17, p24, p21, p6, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{p17, not_p15, p24, p21, p6, p9, p1, not_p21, p3, not_p5, not_p4}
{p17, p24, p21, p7, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{p17, not_p15, p24, p21, p7, p9, p1, not_p21, p3, not_p5, not_p4}
{p17, not_p20, p24, p6, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{p17, not_p20, not_p15, p24, p6, p9, p1, not_p21, p3, not_p5, not_p4}
{p17, not_p20, p24, p7, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{p17, not_p20, not_p15, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4}
{p17, p12, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4}
{p8, p24, p1, not_p21, p3, not_p5, not_p4, not_p19, not_p3}
{p12, p24, p1, not_p21, p3, not_p5, not_p4, not_p19, not_p3}
{p8, p24, p6, p1, not_p21, p3, not_p5, p16, not_p4, not_p3}
{p8, not_p15, p24, p6, p1, not_p21, p3, not_p5, not_p4, not_p3}
{p12, p24, p6, p1, not_p21, p3, not_p5, not_p4, not_p3}
{p8, p24, p1, not_p21, p3, not_p5, not_p4, not_p19, not_p25}
{p12, p24, p1, not_p21, p3, not_p5, not_p4, not_p19, not_p25}
{p8, p24, p6, p1, not_p21, p3, not_p5, p16, not_p4, not_p25}
{p8, not_p15, p24, p6, p1, not_p21, p3, not_p5, not_p4, not_p25}
{p12, p24, p6, p1, not_p21, p3, not_p5, not_p4, not_p25}
{p12, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4, not_p19}
{p8, p24, p7, p9, p1, not_p21, p3, not_p5, p16, not_p4, not_p19}
{p8, not_p15, p24, p7, p9, p1, not_p21, p3, not_p5, not_p4, not_p19}
{p12, p24, p6, p9, p1, not_p21, p3, not_p5, not_p4}
{p8, p24, p6, p9, p1, not_p21, p3, not_p5, p16, not_p4}
{p8, not_p15, p24, p6, p9, p1, not_p21, p3, not_p5, not_p4}
{p17, p24, p21, p18, p9, p1, not_p5, not_p4}
{p17, not_p20, p24, p18, p9, p1, not_p5, not_p4}
{p17, p12, p24, p18, p9, p1, not_p5, not_p4}
{p17, p24, p21, p18, p1, not_p5, not_p4, not_p3}
{p17, not_p20, p24, p18, p1, not_p5, not_p4, not_p3}
{p17, p12, p24, p18, p1, not_p5, not_p4, not_p3}
{p17, p24, p21, p18, p1, not_p5, not_p4, not_p25}
{p17, not_p20, p24, p18, p1, not_p5, not_p4, not_p25}
{p17, p12, p24, p18, p1, not_p5, not_p4, not_p25}
{p8, p24, p18, p9, p1, not_p5, not_p4, not_p19}
{p8, p24, p18, p1, not_p5, not_p4, not_p19, not_p3}
{p8, p24, p18, p1, not_p5, not_p4, not_p19, not_p25}
{p12, p24, p18, p9, p1, not_p5, not_p4, not_p19}
{p12, p24, p18, p1, not_p5, not_p4, not_p19, not_p3}
{p12, p24, p18, p1, not_p5, not_p4, not_p19, not_p25}
{p8, p24, p6, p18, p9, p1, not_p5, p16, not_p4}
{p8, p24, p6, p18, p1, not_p5, p16, not_p4, not_p3}
{p8, p24, p6, p18, p1, not_p5, p16, not_p4, not_p25}
{p8, not_p15, p24, p6, p18, p9, p1, not_p5, not_p4}
{p8, not_p15, p24, p6, p18, p1, not_p5, not_p4, not_p3}
{p8, not_p15, p24, p6, p18, p1, not_p5, not_p4, not_p25}
{p12, p24, p6, p18, p9, p1, not_p5, not_p4}
{p12, p24, p6, p18, p1, not_p5, not_p4, not_p3}
{p12, p24, p6, p18, p1, not_p5, not_p4, not_p25}
{p8, p24, p5, p14, p25, not_p4, not_p19}
{p8, p24, p5, p14, p22, not_p4, not_p19}
{not_p8, p24, p5, p14, p25, not_p4}
{p17, p24, p5, p14, p25, not_p4}
{p8, p24, p5, p14, p6, p25, not_p4}
{not_p8, p24, p5, p14, p22, not_p4}
{p17, p24, p5, p14, p22, not_p4}
{p8, p24, p5, p14, p6, p22, not_p4}
{p17, p24, p5, not_p4, not_p3}
{p17, p24, p5, not_p4, not_p25}
{p17, p24, p5, p9, not_p4}
{not_p8, p24, p4, p9, p1, p20, not_p5, p16}
{not_p8, not_p15, p24, p4, p9, p1, p20, not_p5}
{p12, not_p8, p24, p4, p9, p1, p20, not_p5}
{p8, p24, p4, p9, p1, p20, not_p5, p16, not_p19}
{p8, not_p15, p24, p4, p9, p1, p20, not_p5, not_p19}
{p12, p24, p4, p9, p1, p20, not_p5, not_p19}
{p17, p12, p24, p4, p9, p1, p20, not_p5}
{p17, p24, p21, p4, p9, p1, p20, not_p5, p16}
{p17, not_p20, p24, p4, p9, p1, p20, not_p5, p16}
{p17, not_p15, p24, p21, p4, p9, p1, p20, not_p5}
{p17, not_p20, not_p15, p24, p4, p9, p1, p20, not_p5}
{not_p8, p24, p4, p9, p1, not_p21, p3, not_p5, p16}
{not_p8, not_p15, p24, p4, p9, p1, not_p21, p3, not_p5}
{p12, not_p8, p24, p4, p9, p1, not_p21, p3, not_p5}
{p8, p24, p4, p9, p1, not_p21, p3, not_p5, p16, not_p19}
{p8, not_p15, p24, p4, p9, p1, not_p21, p3, not_p5, not_p19}
{p12, p24, p4, p9, p1, not_p21, p3, not_p5, not_p19}
{p17, p12, p24, p4, p9, p1, not_p21, p3, not_p5}
{p17, p24, p21, p4, p9, p1, not_p21, p3, not_p5, p16}
{p17, not_p20, p24, p4, p9, p1, not_p21, p3, not_p5, p16}
{p17, not_p15, p24, p21, p4, p9, p1, not_p21, p3, not_p5}
{p17, not_p20, not_p15, p24, p4, p9, p1, not_p21, p3, not_p5}
{not_p8, p2, not_p22, p3, not_p5, not_p4, not_p3}
{not_p8, p2, not_p22, p3, not_p5, not_p4, not_p25}
{not_p8, p2, p9, not_p22, p3, not_p5, not_p4}
{not_p8, p2, p21, p14, not_p22, p3, not_p5, not_p4}
{not_p8, not_p20, p2, p14, not_p22, p3, not_p5, not_p4}
{p12, not_p8, p2, p14, not_p22, p3, not_p5, not_p4}
{not_p8, p11, p2, not_p22, not_p5, not_p4, not_p3}
{not_p8, p11, p2, not_p22, not_p5, not_p4, not_p25}
{not_p8, p11, p2, p9, not_p22, not_p5, not_p4}
{not_p8, p11, p2, p21, p14, not_p22, not_p5, not_p4}
{not_p8, p11, not_p20, p2, p14, not_p22, not_p5, not_p4}
{p12, not_p8, p11, p2, p14, not_p22, not_p5, not_p4}
{not_p8, p2, p5, p14, not_p22, p3, p25, not_p4}
{not_p8, p2, p5, p14, not_p22, p22, p3, not_p4}
{not_p8, p11, p2, p5, p14, not_p22, p25, not_p4}
{not_p8, p11, p2, p5, p14, not_p22, p22, not_p4}
{p17, p2, p21, not_p22, p3, not_p5, not_p4, not_p3}
{p17, not_p20, p2, not_p22, p3, not_p5, not_p4, not_p3}
{p17, p12, p2, not_p22, p3, not_p5, not_p4, not_p3}
{p17, p2, p21, not_p22, p3, not_p5, not_p4, not_p25}
{p17, not_p20, p2, not_p22, p3, not_p5, not_p4, not_p25}
{p17, p12, p2, not_p22, p3, not_p5, not_p4, not_p25}
{p17, p2, p21, p9, not_p22, p3, not_p5, not_p4}
{p17, not_p20, p2, p9, not_p22, p3, not_p5, not_p4}
{p17, p12, p2, p9, not_p22, p3, not_p5, not_p4}
{p17, p2, p21, p14, not_p22, p3, not_p5, not_p4}
{p17, not_p20, p2, p14, not_p22, p3, not_p5, not_p4}
{p17, p12, p2, p14, not_p22, p3, not_p5, not_p4}
{p17, p11, p2, p21, not_p22, not_p5, not_p4, not_p3}
{p17, p11, not_p20, p2, not_p22, not_p5, not_p4, not_p3}
{p17, p12, p11, p2, not_p22, not_p5, not_p4, not_p3}
{p17, p11, p2, p21, not_p22, not_p5, not_p4, not_p25}
{p17, p11, not_p20, p2, not_p22, not_p5, not_p4, not_p25}
{p17, p12, p11, p2, not_p22, not_p5, not_p4, not_p25}
{p17, p11, p2, p21, p9, not_p22, not_p5, not_p4}
{p17, p11, not_p20, p2, p9, not_p22, not_p5, not_p4}
{p17, p12, p11, p2, p9, not_p22, not_p5, not_p4}
{p17, p11, p2, p21, p14, not_p22, not_p5, not_p4}
{p17, p11, not_p20, p2, p14, not_p22, not_p5, not_p4}
{p17, p12, p11, p2, p14, not_p22, not_p5, not_p4}
{p17, p2, p5, not_p22, p3, not_p4, not_p3}
{p17, p2, p5, not_p22, p3, not_p4, not_p25}
{p17, p2, p5, p9, not_p22, p3, not_p4}
{p17, p2, p5, p14, not_p22, p3, p25, not_p4}
{p17, p2, p5, p14, not_p22, p22, p3, not_p4}
{p17, p11, p2, p5, not_p22, not_p4, not_p3}
{p17, p11, p2, p5, not_p22, not_p4, not_p25}
{p17, p11, p2, p5, p9, not_p22, not_p4}
{p17, p11, p2, p5, p14, not_p22, p25, not_p4}
{p17, p11, p2, p5, p14, not_p22, p22, not_p4}
{p8, p2, p5, p14, not_p22, p3, p25, not_p4, not_p19}
{p8, p2, p5, p14, p6, not_p22, p3, p25, not_p4}
{p8, p11, p2, p5, p14, not_p22, p25, not_p4, not_p19}
{p8, p11, p2, p5, p14, p6, not_p22, p25, not_p4}
{p8, p2, p5, p14, not_p22, p22, p3, not_p4, not_p19}
{p8, p2, p5, p14, p6, not_p22, p22, p3, not_p4}
{p8, p11, p2, p5, p14, not_p22, p22, not_p4, not_p19}
{p8, p11, p2, p5, p14, p6, not_p22, p22, not_p4}
{p12, p2, p14, not_p22, p3, not_p5, not_p4, not_p19}
{p12, p2, not_p22, p3, not_p5, not_p4, not_p19, not_p3}
{p12, p2, not_p22, p3, not_p5, not_p4, not_p19, not_p25}
{p12, p2, p9, not_p22, p3, not_p5, not_p4, not_p19}
{p12, p2, p14, p6, not_p22, p3, not_p5, not_p4}
{p12, p2, p6, not_p22, p3, not_p5, not_p4, not_p3}
{p12, p2, p6, not_p22, p3, not_p5, not_p4, not_p25}
{p12, p2, p6, p9, not_p22, p3, not_p5, not_p4}
{p12, p11, p2, p14, not_p22, not_p5, not_p4, not_p19}
{p12, p11, p2, not_p22, not_p5, not_p4, not_p19, not_p3}
{p12, p11, p2, not_p22, not_p5, not_p4, not_p19, not_p25}
{p12, p11, p2, p9, not_p22, not_p5, not_p4, not_p19}
{p12, p11, p2, p14, p6, not_p22, not_p5, not_p4}
{p12, p11, p2, p6, not_p22, not_p5, not_p4, not_p3}
{p12, p11, p2, p6, not_p22, not_p5, not_p4, not_p25}
{p12, p11, p2, p6, p9, not_p22, not_p5, not_p4}
{p8, p2, not_p22, p3, not_p5, not_p4, not_p19, not_p3}
{p8, p2, not_p22, p3, not_p5, not_p4, not_p19, not_p25}
{p8, p2, p9, not_p22, p3, not_p5, not_p4, not_p19}
{p8, p2, p21, p14, not_p22, p3, not_p5, not_p4, not_p19}
{p8, not_p20, p2, p14, not_p22, p3, not_p5, not_p4, not_p19}
{p8, p2, p6, not_p22, p3, not_p5, not_p4, not_p3}
{p8, p2, p6, not_p22, p3, not_p5, not_p4, not_p25}
{p8, p2, p6, p9, not_p22, p3, not_p5, not_p4}
{p8, p2, p21, p14, p6, not_p22, p3, not_p5, not_p4}
{p8, not_p20, p2, p14, p6, not_p22, p3, not_p5, not_p4}
{p8, p11, p2, not_p22, not_p5, not_p4, not_p19, not_p3}
{p8, p11, p2, not_p22, not_p5, not_p4, not_p19, not_p25}
{p8, p11, p2, p9, not_p22, not_p5, not_p4, not_p19}
{p8, p11, p2, p21, p14, not_p22, not_p5, not_p4, not_p19}
{p8, p11, not_p20, p2, p14, not_p22, not_p5, not_p4, not_p19}
{p8, p11, p2, p6, not_p22, not_p5, not_p4, not_p3}
{p8, p11, p2, p6, not_p22, not_p5, not_p4, not_p25}
{p8, p11, p2, p6, p9, not_p22, not_p5, not_p4}
{p8, p11, p2, p21, p14, p6, not_p22, not_p5, not_p4}
{p8, p11, not_p20, p2, p14, p6, not_p22, not_p5, not_p4}
"""
