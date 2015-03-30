input = """
p15|p6:-p19,p5,not p3,not p7. 
p3|p20|p18:-p1,not p22. 
p12|not_p15|p16|p18:-p9,p1. 
p1|p14|p2|p11:-not p5,p24. 
not_p21|p20|p18:-p1,not p22. 
p6|p7:-p19,not p23,not p3,not p11. 
p15|p11|p24|p3. 
p3|p20|p18|p3:-p1,not p22. 
not_p21|p20|p18|p3:-p13,not p22. 
p21|p5|p12|not_p20:-p17. 
not_p4:-not p4. 
not_p20|not_p1|not_p4:-p7,p2,not p25. 
p6|p4|p7|p18:-p9,p1. 
p24|p9|p24|p10:-p12,p23. 
p12|not_p15|p16|p17:-p6,p1. 
not_p21|p15|p22|not_p12. 
not_p23|p7|not_p17|p5:-p13,not p3. 
p3|not_p20|p18:-p13,not p22,not p14. 
p17|p12|p8|p25. 
p24|p9|p24|not_p22:-p19,p23. 
p5|p11|p25|not_p20:-p17,not p7. 
:-p16,not_p16. 
p3|p21|not_p20:-p6,p23,not p4. 
p11|p8|p4|p16. 
p19|p1|p12:-p19,not p23,not p11. 
p21|p5|p12|not_p20:-p14. 
not_p25|p14|p9|not_p3. 
p17|p6|not_p19|not_p8. 
p11|not_p20|not_p1|not_p4:-p24,p2. 
p11|not_p20|p8|not_p15:-p24,p2. 
not_p22|p24|p15:-not p23. 
p15|p4|p3|p1:-p12,p5. 
:-p19,not_p19. 
p1|p15|p15|not_p18:-not p5,p24. 
p24|not_p8|p6|not_p22:-p12,p23. 
p24|p9|p24|not_p22:-p12,p23. 
p6|p9|p6|not_p22:-p19,p23. 
not_p2|not_p24|p9:-p1,p12,not p11. 
p24|p8|not_p15:-p7,p2,not p25. 
p1|p15|p15|p11:-not p5,p24. 
p6|not_p15|p7|p17:-p6,p1. 
not_p23|p7|p21|p5:-p13,not p3. 
p21|p22|p11|p25|not_p20:-p17. 
p21|p22|p12|p25|not_p20:-p14. 
p2|p24|p15:-not p23. 
p1|p12|p2|not_p16:-not p23,not p3. 
p15|not_p22|p24|not_p12. 
p17|p12|p8|not_p8. 
not_p5:-not p5. 
not_p5|not_p5|not_p19|p22. 
"""
output = """
{p15, p16, p2, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p12, p16, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p18, p1, p16, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p20, p1, p16, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p16, p2, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p12, p16, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p18, p1, p16, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p20, p1, p16, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p12, p16, p14, not_p20, p17, not_p4, not_p5}
{p15, p18, p1, p16, p14, not_p20, p17, not_p4, not_p5}
{p15, p20, p1, p16, p14, not_p20, p17, not_p4, not_p5}
{p15, p16, p14, p2, not_p20, p17, not_p4, not_p5}
{p15, p18, p1, p16, p9, not_p20, p17, not_p4, not_p5}
{p15, p6, p20, p1, p16, p9, not_p20, p17, not_p4, not_p5}
{p15, p7, p20, p1, p16, p9, not_p20, p17, not_p4, not_p5}
{p15, p16, p9, p2, not_p20, p17, not_p4, not_p5}
{p15, p12, p16, p9, not_p20, p17, not_p4, not_p5}
{p15, p16, p14, p2, p21, p17, not_p4, p25, not_p5}
{p15, p18, p1, p16, p14, p21, p17, not_p4, p25, not_p5}
{p15, p20, p1, p16, p14, p21, p17, not_p4, p25, not_p5}
{p15, p16, p2, p21, p17, not_p4, p25, not_p3, not_p5}
{p15, p18, p1, p16, p21, p17, not_p4, p25, not_p3, not_p5}
{p15, p20, p1, p16, p21, p17, not_p4, p25, not_p3, not_p5}
{p15, p16, p2, p21, p17, not_p4, p25, not_p25, not_p5}
{p15, p18, p1, p16, p21, p17, not_p4, p25, not_p25, not_p5}
{p15, p20, p1, p16, p21, p17, not_p4, p25, not_p25, not_p5}
{p15, p7, p20, p1, p16, p9, p21, p17, not_p4, not_p5}
{p15, p6, p20, p1, p16, p9, p21, p17, not_p4, p25, not_p5}
{p15, p6, p20, p1, p16, p9, p11, p21, p17, not_p4, not_p5}
{p15, p18, p1, p16, p9, p21, p17, not_p4, p25, not_p5}
{p15, p16, p9, p2, p21, p17, not_p4, p25, not_p5}
{p15, p12, p16, p17, not_p4, p25, not_p3, not_p5}
{p15, p12, p16, p17, not_p4, p25, not_p25, not_p5}
{p15, p12, p16, p14, p17, not_p4, p25, not_p5}
{p15, p12, p16, p9, p17, not_p4, p25, not_p5}
{p15, p16, p2, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p18, p1, p16, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p20, p1, p16, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p12, p16, not_p4, not_p3, not_p8, not_p5}
{p15, p16, p2, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p18, p1, p16, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p20, p1, p16, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p12, p16, not_p4, not_p25, not_p8, not_p5}
{p15, p7, p20, p1, p16, p9, not_p4, p8, not_p8, not_p5}
{p15, p16, p14, p2, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p16, p14, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p20, p1, p16, p14, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p16, p14, p2, p21, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p16, p14, p21, not_p4, p25, not_p8, not_p5}
{p15, p20, p1, p16, p14, p21, not_p4, p25, not_p8, not_p5}
{p15, p16, p9, p2, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p16, p9, not_p4, p25, not_p8, not_p5}
{p15, p6, p20, p1, p16, p9, not_p4, p25, not_p8, not_p5}
{p15, p7, p20, p1, p16, p9, not_p4, p25, not_p8, not_p5}
{p15, p12, p16, p14, not_p4, not_p8, not_p5}
{p15, p12, p16, p9, not_p4, not_p8, not_p5}
{p15, p6, p18, p1, p16, p9, not_p4, p8, not_p5}
{p15, p6, p18, p1, p16, not_p4, p8, not_p3, not_p5}
{p15, p6, p18, p1, p16, not_p4, p8, not_p25, not_p5}
{p15, p6, p18, p1, p16, p14, not_p20, not_p4, p8, not_p5}
{p15, p6, p18, p1, p16, p14, p21, not_p4, p8, not_p5}
{p15, p6, p20, p1, p16, p9, not_p4, p8, not_p5}
{p15, p6, p20, p1, p16, not_p4, p8, not_p3, not_p5}
{p15, p6, p20, p1, p16, not_p4, p8, not_p25, not_p5}
{p15, p6, p20, p1, p16, p14, not_p20, not_p4, p8, not_p5}
{p15, p6, p20, p1, p16, p14, p21, not_p4, p8, not_p5}
{p15, p7, p20, p1, p16, p9, not_p4, p8, not_p19, not_p5}
{p15, p12, p16, not_p4, not_p3, not_p19, not_p5}
{p15, p12, p16, not_p4, not_p25, not_p19, not_p5}
{p15, p12, p16, p14, not_p4, not_p19, not_p5}
{p15, p12, p16, p9, not_p4, not_p19, not_p5}
{p15, p6, p12, p16, p14, not_p4, not_p5}
{p15, p6, p12, p16, not_p4, not_p3, not_p5}
{p15, p6, p12, p16, not_p4, not_p25, not_p5}
{p15, p6, p12, p16, p9, not_p4, not_p5}
{p15, p5, p22, p12, p16, p17, not_p4, not_p3}
{p15, p5, p22, p16, p2, p17, not_p4, not_p3}
{p15, p5, p1, p22, p16, p17, not_p4, not_p3}
{p15, p5, p22, p12, p16, p17, not_p4, not_p25}
{p15, p5, p22, p16, p2, p17, not_p4, not_p25}
{p15, p5, p1, p22, p16, p17, not_p4, not_p25}
{p15, p5, p22, p12, p16, p14, p17, not_p4}
{p15, p5, p22, p16, p14, p2, p17, not_p4}
{p15, p5, p1, p22, p16, p14, p17, not_p4}
{p15, p5, p22, p12, p16, p9, p17, not_p4}
{p15, p5, p22, p16, p9, p2, p17, not_p4}
{p15, p5, p18, p1, p22, p16, p9, p17, not_p4}
{p15, p6, p5, p1, p22, p16, p9, p17, not_p4}
{p15, p5, p7, p1, p22, p16, p9, p17, not_p4}
{p15, p5, p16, p2, p17, not_p4, p25, not_p3, not_p5}
{p15, p5, p18, p1, p16, p17, not_p4, p25, not_p3, not_p5}
{p15, p5, p20, p1, p16, p17, not_p4, p25, not_p3, not_p5}
{p15, p5, p12, p16, p21, p17, not_p4, not_p3, not_p5}
{p15, p5, p16, p2, p21, p17, not_p4, not_p3, not_p5}
{p15, p5, p18, p1, p16, p21, p17, not_p4, not_p3, not_p5}
{p15, p5, p20, p1, p16, p21, p17, not_p4, not_p3, not_p5}
{p15, p5, p16, p2, p17, not_p4, p25, not_p3, not_p19}
{p15, p5, p18, p1, p16, p17, not_p4, p25, not_p3, not_p19}
{p15, p5, p20, p1, p16, p17, not_p4, p25, not_p3, not_p19}
{p15, p5, p16, p2, p21, p17, not_p4, not_p3, not_p19}
{p15, p5, p18, p1, p16, p21, p17, not_p4, not_p3, not_p19}
{p15, p5, p20, p1, p16, p21, p17, not_p4, not_p3, not_p19}
{p15, p5, p16, p2, p17, not_p4, p25, not_p25, not_p5}
{p15, p5, p18, p1, p16, p17, not_p4, p25, not_p25, not_p5}
{p15, p5, p20, p1, p16, p17, not_p4, p25, not_p25, not_p5}
{p15, p5, p12, p16, p21, p17, not_p4, not_p25, not_p5}
{p15, p5, p16, p2, p21, p17, not_p4, not_p25, not_p5}
{p15, p5, p18, p1, p16, p21, p17, not_p4, not_p25, not_p5}
{p15, p5, p20, p1, p16, p21, p17, not_p4, not_p25, not_p5}
{p15, p5, p16, p2, p17, not_p4, p25, not_p25, not_p19}
{p15, p5, p18, p1, p16, p17, not_p4, p25, not_p25, not_p19}
{p15, p5, p20, p1, p16, p17, not_p4, p25, not_p25, not_p19}
{p15, p5, p16, p2, p21, p17, not_p4, not_p25, not_p19}
{p15, p5, p18, p1, p16, p21, p17, not_p4, not_p25, not_p19}
{p15, p5, p20, p1, p16, p21, p17, not_p4, not_p25, not_p19}
{p15, p5, p16, p14, p2, p17, not_p4, p25, not_p5}
{p15, p5, p16, p14, p2, p17, not_p4, p25, not_p19}
{p15, p5, p18, p1, p16, p14, p17, not_p4, p25, not_p5}
{p15, p5, p20, p1, p16, p14, p17, not_p4, p25, not_p5}
{p15, p5, p18, p1, p16, p14, p17, not_p4, p25, not_p19}
{p15, p5, p20, p1, p16, p14, p17, not_p4, p25, not_p19}
{p15, p5, p12, p16, p14, p21, p17, not_p4, not_p5}
{p15, p5, p16, p14, p2, p21, p17, not_p4, not_p5}
{p15, p5, p18, p1, p16, p14, p21, p17, not_p4, not_p5}
{p15, p5, p20, p1, p16, p14, p21, p17, not_p4, not_p5}
{p15, p5, p16, p14, p2, p21, p17, not_p4, not_p19}
{p15, p5, p18, p1, p16, p14, p21, p17, not_p4, not_p19}
{p15, p5, p20, p1, p16, p14, p21, p17, not_p4, not_p19}
{p15, p5, p18, p1, p16, p9, p17, not_p4, p25, not_p5}
{p15, p5, p18, p1, p16, p9, p21, p17, not_p4, not_p5}
{p15, p6, p5, p20, p1, p16, p9, p17, not_p4, p25, not_p5}
{p15, p6, p5, p20, p1, p16, p9, p21, p17, not_p4, not_p5}
{p15, p6, p5, p20, p1, p16, p9, p11, p17, not_p4, not_p5}
{p15, p5, p7, p20, p1, p16, p9, p17, not_p4, p25, not_p5}
{p15, p5, p7, p20, p1, p16, p9, p11, p17, not_p4, not_p5}
{p15, p5, p12, p16, p9, p21, p17, not_p4, not_p5}
{p15, p5, p16, p9, p2, p17, not_p4, p25, not_p5}
{p15, p5, p16, p9, p2, p21, p17, not_p4, not_p5}
{p15, p5, p18, p1, p16, p9, p17, not_p4, p25, not_p19}
{p15, p5, p18, p1, p16, p9, p21, p17, not_p4, not_p19}
{p15, p6, p5, p20, p1, p16, p9, p17, not_p4, p25, not_p19}
{p15, p6, p5, p20, p1, p16, p9, p21, p17, not_p4, not_p19}
{p15, p6, p5, p20, p1, p16, p9, p11, p17, not_p4, not_p19}
{p15, p5, p7, p20, p1, p16, p9, p17, not_p4, p25, not_p19}
{p15, p5, p7, p20, p1, p16, p9, p11, p17, not_p4, not_p19}
{p15, p5, p16, p9, p2, p17, not_p4, p25, not_p19}
{p15, p5, p16, p9, p2, p21, p17, not_p4, not_p19}
{p15, p5, p16, p14, p2, not_p4, p25, not_p8, not_p5}
{p15, p5, p16, p14, p2, not_p4, p25, not_p19, not_p8}
{p15, p5, p18, p1, p16, p14, not_p4, p25, not_p8, not_p5}
{p15, p5, p20, p1, p16, p14, not_p4, p25, not_p8, not_p5}
{p15, p5, p18, p1, p16, p14, not_p4, p25, not_p19, not_p8}
{p15, p5, p20, p1, p16, p14, not_p4, p25, not_p19, not_p8}
{p15, p5, p22, p16, p14, p2, not_p4, p25, not_p8}
{p15, p5, p1, p22, p16, p14, not_p4, p25, not_p8}
{p15, p6, p5, p1, p22, p16, p14, not_p4, p8}
{p15, p6, p5, p18, p1, p16, p14, not_p4, p25, p8, not_p5}
{p15, p6, p5, p20, p1, p16, p14, not_p4, p25, p8, not_p5}
{p15, p20, p1, p16, p9, p4, p8, not_p19, not_p5}
{p15, p20, p1, p16, p9, not_p20, p17, p4, not_p5}
{p15, p20, p1, p16, p9, p21, p17, p4, p25, not_p5}
{p15, p20, p1, p16, p9, p11, p21, p17, p4, not_p5}
{p15, p20, p1, p16, p9, p4, p8, not_p8, not_p5}
{p15, p20, p1, p16, p9, p4, p25, not_p8, not_p5}
{p15, p5, p20, p1, p16, p9, p17, p4, p25, not_p5}
{p15, p5, p20, p1, p16, p9, p21, p17, p4, not_p5}
{p15, p5, p20, p1, p16, p9, p11, p17, p4, not_p5}
{p15, p5, p20, p1, p16, p9, p17, p4, p25, not_p19}
{p15, p5, p20, p1, p16, p9, p21, p17, p4, not_p19}
{p15, p5, p20, p1, p16, p9, p11, p17, p4, not_p19}
{p15, p5, p1, p22, p16, p9, p17, p4}
{p15, p14, not_p20, p17, p4, not_p16, not_p5}
{p15, p14, p2, not_p20, p17, p4, not_p5}
{p15, p12, p14, not_p20, p17, p4, not_p5}
{p15, p18, p1, p14, not_p20, p17, p4, not_p5}
{p15, p20, p1, p14, not_p20, p17, p4, not_p5}
{p15, not_p20, p17, p4, not_p3, not_p16, not_p5}
{p15, not_p20, p17, p4, not_p25, not_p16, not_p5}
{p15, p9, not_p20, p17, p4, not_p16, not_p5}
{p15, p2, not_p20, p17, p4, not_p3, not_p5}
{p15, p12, not_p20, p17, p4, not_p3, not_p5}
{p15, p18, p1, not_p20, p17, p4, not_p3, not_p5}
{p15, p20, p1, not_p20, p17, p4, not_p3, not_p5}
{p15, p2, not_p20, p17, p4, not_p25, not_p5}
{p15, p12, not_p20, p17, p4, not_p25, not_p5}
{p15, p18, p1, not_p20, p17, p4, not_p25, not_p5}
{p15, p20, p1, not_p20, p17, p4, not_p25, not_p5}
{p15, p9, p2, not_p20, p17, p4, not_p5}
{p15, p20, p1, not_p15, p9, not_p20, p17, p4, not_p5}
{p15, p18, p1, p9, not_p20, p17, p4, not_p5}
{p15, p12, p9, not_p20, p17, p4, not_p5}
{p15, p12, p14, p17, p4, p25, not_p5}
{p15, p12, p17, p4, p25, not_p3, not_p5}
{p15, p12, p17, p4, p25, not_p25, not_p5}
{p15, p12, p9, p17, p4, p25, not_p5}
{p15, p14, p21, p17, p4, p25, not_p16, not_p5}
{p15, p21, p17, p4, p25, not_p3, not_p16, not_p5}
{p15, p21, p17, p4, p25, not_p25, not_p16, not_p5}
{p15, p9, p21, p17, p4, p25, not_p16, not_p5}
{p15, p14, p2, p21, p17, p4, p25, not_p5}
{p15, p2, p21, p17, p4, p25, not_p3, not_p5}
{p15, p2, p21, p17, p4, p25, not_p25, not_p5}
{p15, p9, p2, p21, p17, p4, p25, not_p5}
{p15, p18, p1, p14, p21, p17, p4, p25, not_p5}
{p15, p18, p1, p9, p21, p17, p4, p25, not_p5}
{p15, p18, p1, p21, p17, p4, p25, not_p3, not_p5}
{p15, p18, p1, p21, p17, p4, p25, not_p25, not_p5}
{p15, p20, p1, p14, p21, p17, p4, p25, not_p5}
{p15, p20, p1, p21, p17, p4, p25, not_p3, not_p5}
{p15, p20, p1, p21, p17, p4, p25, not_p25, not_p5}
{p15, p20, p1, not_p15, p9, p21, p17, p4, p25, not_p5}
{p15, p20, p1, not_p15, p9, p11, p21, p17, p4, not_p5}
{p15, p4, p25, not_p3, not_p8, not_p16, not_p5}
{p15, p2, p4, p25, not_p3, not_p8, not_p5}
{p15, p18, p1, p4, p25, not_p3, not_p8, not_p5}
{p15, p20, p1, p4, p25, not_p3, not_p8, not_p5}
{p15, p12, p4, not_p3, not_p8, not_p5}
{p15, p4, p25, not_p25, not_p8, not_p16, not_p5}
{p15, p2, p4, p25, not_p25, not_p8, not_p5}
{p15, p18, p1, p4, p25, not_p25, not_p8, not_p5}
{p15, p20, p1, p4, p25, not_p25, not_p8, not_p5}
{p15, p12, p4, not_p25, not_p8, not_p5}
{p15, p20, p1, not_p15, p9, p4, p8, not_p8, not_p5}
{p15, p14, not_p20, p4, p25, not_p8, not_p16, not_p5}
{p15, p14, p2, not_p20, p4, p25, not_p8, not_p5}
{p15, p18, p1, p14, not_p20, p4, p25, not_p8, not_p5}
{p15, p20, p1, p14, not_p20, p4, p25, not_p8, not_p5}
{p15, p14, p21, p4, p25, not_p8, not_p16, not_p5}
{p15, p14, p2, p21, p4, p25, not_p8, not_p5}
{p15, p18, p1, p14, p21, p4, p25, not_p8, not_p5}
{p15, p20, p1, p14, p21, p4, p25, not_p8, not_p5}
{p15, p9, p4, p25, not_p8, not_p16, not_p5}
{p15, p9, p2, p4, p25, not_p8, not_p5}
{p15, p18, p1, p9, p4, p25, not_p8, not_p5}
{p15, p20, p1, not_p15, p9, p4, p25, not_p8, not_p5}
{p15, p12, p14, p4, not_p8, not_p5}
{p15, p12, p9, p4, not_p8, not_p5}
{p15, p20, p1, not_p15, p9, p4, p8, not_p19, not_p5}
{p15, p12, p14, p4, not_p19, not_p5}
{p15, p12, p4, not_p3, not_p19, not_p5}
{p15, p12, p4, not_p25, not_p19, not_p5}
{p15, p12, p9, p4, not_p19, not_p5}
{p15, p6, p12, p14, p4, not_p5}
{p15, p6, p12, p4, not_p3, not_p5}
{p15, p6, p12, p4, not_p25, not_p5}
{p15, p6, p12, p9, p4, not_p5}
{p15, p5, p22, p14, p17, p4, not_p16}
{p15, p5, p22, p12, p14, p17, p4}
{p15, p5, p22, p14, p2, p17, p4}
{p15, p5, p1, p22, p14, p17, p4}
{p15, p5, p22, p17, p4, not_p3, not_p16}
{p15, p5, p22, p17, p4, not_p25, not_p16}
{p15, p5, p22, p9, p17, p4, not_p16}
{p15, p5, p22, p12, p17, p4, not_p3}
{p15, p5, p22, p2, p17, p4, not_p3}
{p15, p5, p1, p22, p17, p4, not_p3}
{p15, p5, p22, p12, p17, p4, not_p25}
{p15, p5, p22, p2, p17, p4, not_p25}
{p15, p5, p1, p22, p17, p4, not_p25}
{p15, p5, p22, p12, p9, p17, p4}
{p15, p5, p1, p22, not_p15, p9, p17, p4}
{p15, p5, p18, p1, p22, p9, p17, p4}
{p15, p5, p22, p9, p2, p17, p4}
{p15, p5, p14, p17, p4, p25, not_p16, not_p5}
{p15, p5, p17, p4, p25, not_p3, not_p16, not_p5}
{p15, p5, p17, p4, p25, not_p25, not_p16, not_p5}
{p15, p5, p9, p17, p4, p25, not_p16, not_p5}
{p15, p5, p21, p17, p4, not_p3, not_p16, not_p5}
{p15, p5, p21, p17, p4, not_p25, not_p16, not_p5}
{p15, p5, p14, p21, p17, p4, not_p16, not_p5}
{p15, p5, p9, p21, p17, p4, not_p16, not_p5}
{p15, p5, p14, p17, p4, p25, not_p19, not_p16}
{p15, p5, p17, p4, p25, not_p3, not_p19, not_p16}
{p15, p5, p17, p4, p25, not_p25, not_p19, not_p16}
{p15, p5, p9, p17, p4, p25, not_p19, not_p16}
{p15, p5, p21, p17, p4, not_p3, not_p19, not_p16}
{p15, p5, p14, p21, p17, p4, not_p19, not_p16}
{p15, p5, p21, p17, p4, not_p25, not_p19, not_p16}
{p15, p5, p9, p21, p17, p4, not_p19, not_p16}
{p15, p5, p2, p17, p4, p25, not_p3, not_p5}
{p15, p5, p18, p1, p17, p4, p25, not_p3, not_p5}
{p15, p5, p20, p1, p17, p4, p25, not_p3, not_p5}
{p15, p5, p12, p21, p17, p4, not_p3, not_p5}
{p15, p5, p2, p21, p17, p4, not_p3, not_p5}
{p15, p5, p18, p1, p21, p17, p4, not_p3, not_p5}
{p15, p5, p20, p1, p21, p17, p4, not_p3, not_p5}
{p15, p5, p2, p17, p4, p25, not_p3, not_p19}
{p15, p5, p18, p1, p17, p4, p25, not_p3, not_p19}
{p15, p5, p20, p1, p17, p4, p25, not_p3, not_p19}
{p15, p5, p2, p21, p17, p4, not_p3, not_p19}
{p15, p5, p18, p1, p21, p17, p4, not_p3, not_p19}
{p15, p5, p20, p1, p21, p17, p4, not_p3, not_p19}
{p15, p5, p2, p17, p4, p25, not_p25, not_p5}
{p15, p5, p18, p1, p17, p4, p25, not_p25, not_p5}
{p15, p5, p20, p1, p17, p4, p25, not_p25, not_p5}
{p15, p5, p12, p21, p17, p4, not_p25, not_p5}
{p15, p5, p2, p21, p17, p4, not_p25, not_p5}
{p15, p5, p18, p1, p21, p17, p4, not_p25, not_p5}
{p15, p5, p20, p1, p21, p17, p4, not_p25, not_p5}
{p15, p5, p2, p17, p4, p25, not_p25, not_p19}
{p15, p5, p18, p1, p17, p4, p25, not_p25, not_p19}
{p15, p5, p20, p1, p17, p4, p25, not_p25, not_p19}
{p15, p5, p2, p21, p17, p4, not_p25, not_p19}
{p15, p5, p18, p1, p21, p17, p4, not_p25, not_p19}
{p15, p5, p20, p1, p21, p17, p4, not_p25, not_p19}
{p15, p5, p14, p2, p17, p4, p25, not_p5}
{p15, p5, p14, p2, p17, p4, p25, not_p19}
{p15, p5, p18, p1, p14, p17, p4, p25, not_p5}
{p15, p5, p20, p1, p14, p17, p4, p25, not_p5}
{p15, p5, p18, p1, p14, p17, p4, p25, not_p19}
{p15, p5, p20, p1, p14, p17, p4, p25, not_p19}
{p15, p5, p12, p14, p21, p17, p4, not_p5}
{p15, p5, p14, p2, p21, p17, p4, not_p5}
{p15, p5, p18, p1, p14, p21, p17, p4, not_p5}
{p15, p5, p20, p1, p14, p21, p17, p4, not_p5}
{p15, p5, p14, p2, p21, p17, p4, not_p19}
{p15, p5, p18, p1, p14, p21, p17, p4, not_p19}
{p15, p5, p20, p1, p14, p21, p17, p4, not_p19}
{p15, p5, p12, p9, p21, p17, p4, not_p5}
{p15, p5, p20, p1, not_p15, p9, p17, p4, p25, not_p5}
{p15, p5, p20, p1, not_p15, p9, p21, p17, p4, not_p5}
{p15, p5, p20, p1, not_p15, p9, p11, p17, p4, not_p5}
{p15, p5, p18, p1, p9, p17, p4, p25, not_p5}
{p15, p5, p18, p1, p9, p21, p17, p4, not_p5}
{p15, p5, p9, p2, p17, p4, p25, not_p5}
{p15, p5, p9, p2, p21, p17, p4, not_p5}
{p15, p5, p20, p1, not_p15, p9, p17, p4, p25, not_p19}
{p15, p5, p20, p1, not_p15, p9, p21, p17, p4, not_p19}
{p15, p5, p20, p1, not_p15, p9, p11, p17, p4, not_p19}
{p15, p5, p18, p1, p9, p17, p4, p25, not_p19}
{p15, p5, p18, p1, p9, p21, p17, p4, not_p19}
{p15, p5, p9, p2, p17, p4, p25, not_p19}
{p15, p5, p9, p2, p21, p17, p4, not_p19}
{p15, p5, p14, p4, p25, not_p8, not_p16, not_p5}
{p15, p5, p14, p2, p4, p25, not_p8, not_p5}
{p15, p5, p18, p1, p14, p4, p25, not_p8, not_p5}
{p15, p5, p20, p1, p14, p4, p25, not_p8, not_p5}
{p15, p5, p14, p4, p25, not_p19, not_p8, not_p16}
{p15, p5, p14, p2, p4, p25, not_p19, not_p8}
{p15, p5, p18, p1, p14, p4, p25, not_p19, not_p8}
{p15, p5, p20, p1, p14, p4, p25, not_p19, not_p8}
{p15, p5, p22, p14, p4, p25, not_p8, not_p16}
{p15, p5, p22, p14, p2, p4, p25, not_p8}
{p15, p5, p1, p22, p14, p4, p25, not_p8}
{p15, not_p4, p8, not_p3, not_p8, not_p16, not_p5}
{p15, not_p4, p8, not_p25, not_p8, not_p16, not_p5}
{p15, p14, not_p20, not_p4, p8, not_p8, not_p16, not_p5}
{p15, p14, p21, not_p4, p8, not_p8, not_p16, not_p5}
{p15, p9, not_p4, p8, not_p8, not_p16, not_p5}
{p15, not_p4, p8, not_p3, not_p19, not_p16, not_p5}
{p15, not_p4, p8, not_p25, not_p19, not_p16, not_p5}
{p15, p14, not_p20, not_p4, p8, not_p19, not_p16, not_p5}
{p15, p14, p21, not_p4, p8, not_p19, not_p16, not_p5}
{p15, p9, not_p4, p8, not_p19, not_p16, not_p5}
{p15, not_p20, p17, not_p4, p8, not_p3, not_p16, not_p5}
{p15, p21, p17, not_p4, p25, p8, not_p3, not_p16, not_p5}
{p15, p6, not_p4, p8, not_p3, not_p16, not_p5}
{p15, not_p20, p17, not_p4, p8, not_p25, not_p16, not_p5}
{p15, p21, p17, not_p4, p25, p8, not_p25, not_p16, not_p5}
{p15, p6, not_p4, p8, not_p25, not_p16, not_p5}
{p15, p14, not_p20, p17, not_p4, p8, not_p16, not_p5}
{p15, p9, not_p20, p17, not_p4, p8, not_p16, not_p5}
{p15, p14, p21, p17, not_p4, p25, p8, not_p16, not_p5}
{p15, p9, p21, p17, not_p4, p25, p8, not_p16, not_p5}
{p15, p6, p14, not_p20, not_p4, p8, not_p16, not_p5}
{p15, p6, p14, p21, not_p4, p8, not_p16, not_p5}
{p15, p6, p9, not_p4, p8, not_p16, not_p5}
{p15, p2, not_p4, p8, not_p3, not_p8, not_p5}
{p15, p12, not_p4, p8, not_p3, not_p8, not_p5}
{p15, p18, p1, not_p4, p8, not_p3, not_p8, not_p5}
{p15, p20, p1, not_p4, p8, not_p3, not_p8, not_p5}
{p15, p2, not_p4, p8, not_p25, not_p8, not_p5}
{p15, p12, not_p4, p8, not_p25, not_p8, not_p5}
{p15, p18, p1, not_p4, p8, not_p25, not_p8, not_p5}
{p15, p20, p1, not_p4, p8, not_p25, not_p8, not_p5}
{p15, p14, p2, not_p20, not_p4, p8, not_p8, not_p5}
{p15, p14, p2, p21, not_p4, p8, not_p8, not_p5}
{p15, p9, p2, not_p4, p8, not_p8, not_p5}
{p15, p12, p14, not_p4, p8, not_p8, not_p5}
{p15, p18, p1, p14, not_p20, not_p4, p8, not_p8, not_p5}
{p15, p20, p1, p14, not_p20, not_p4, p8, not_p8, not_p5}
{p15, p18, p1, p14, p21, not_p4, p8, not_p8, not_p5}
{p15, p20, p1, p14, p21, not_p4, p8, not_p8, not_p5}
{p15, p12, p9, not_p4, p8, not_p8, not_p5}
{p15, p18, p1, p9, not_p4, p8, not_p8, not_p5}
{p15, p7, p20, p1, not_p15, p9, not_p4, p8, not_p8, not_p5}
{p15, p2, not_p4, p8, not_p3, not_p19, not_p5}
{p15, p12, not_p4, p8, not_p3, not_p19, not_p5}
{p15, p18, p1, not_p4, p8, not_p3, not_p19, not_p5}
{p15, p20, p1, not_p4, p8, not_p3, not_p19, not_p5}
{p15, p2, not_p4, p8, not_p25, not_p19, not_p5}
{p15, p12, not_p4, p8, not_p25, not_p19, not_p5}
{p15, p18, p1, not_p4, p8, not_p25, not_p19, not_p5}
{p15, p20, p1, not_p4, p8, not_p25, not_p19, not_p5}
{p15, p14, p2, not_p20, not_p4, p8, not_p19, not_p5}
{p15, p14, p2, p21, not_p4, p8, not_p19, not_p5}
{p15, p9, p2, not_p4, p8, not_p19, not_p5}
{p15, p12, p14, not_p4, p8, not_p19, not_p5}
{p15, p18, p1, p14, not_p20, not_p4, p8, not_p19, not_p5}
{p15, p20, p1, p14, not_p20, not_p4, p8, not_p19, not_p5}
{p15, p18, p1, p14, p21, not_p4, p8, not_p19, not_p5}
{p15, p20, p1, p14, p21, not_p4, p8, not_p19, not_p5}
{p15, p12, p9, not_p4, p8, not_p19, not_p5}
{p15, p18, p1, p9, not_p4, p8, not_p19, not_p5}
{p15, p7, p20, p1, not_p15, p9, not_p4, p8, not_p19, not_p5}
{p15, p2, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p15, p12, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p15, p18, p1, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p15, p20, p1, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p15, p2, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p18, p1, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p20, p1, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p12, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p6, p2, not_p4, p8, not_p3, not_p5}
{p15, p6, p12, not_p4, p8, not_p3, not_p5}
{p15, p6, p18, p1, not_p15, not_p4, p8, not_p3, not_p5}
{p15, p6, p20, p1, not_p15, not_p4, p8, not_p3, not_p5}
{p15, p2, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p15, p12, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p15, p18, p1, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p15, p20, p1, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p15, p2, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p18, p1, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p20, p1, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p12, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p6, p2, not_p4, p8, not_p25, not_p5}
{p15, p6, p12, not_p4, p8, not_p25, not_p5}
{p15, p6, p18, p1, not_p15, not_p4, p8, not_p25, not_p5}
{p15, p6, p20, p1, not_p15, not_p4, p8, not_p25, not_p5}
{p15, p7, p20, p1, not_p15, p9, not_p20, p17, not_p4, p8, not_p5}
{p15, p7, p20, p1, not_p15, p9, p21, p17, not_p4, p8, not_p5}
{p15, p14, p2, not_p20, p17, not_p4, p8, not_p5}
{p15, p12, p14, not_p20, p17, not_p4, p8, not_p5}
{p15, p18, p1, p14, not_p20, p17, not_p4, p8, not_p5}
{p15, p20, p1, p14, not_p20, p17, not_p4, p8, not_p5}
{p15, p9, p2, not_p20, p17, not_p4, p8, not_p5}
{p15, p12, p9, not_p20, p17, not_p4, p8, not_p5}
{p15, p18, p1, p9, not_p20, p17, not_p4, p8, not_p5}
{p15, p14, p2, p21, p17, not_p4, p25, p8, not_p5}
{p15, p18, p1, p14, p21, p17, not_p4, p25, p8, not_p5}
{p15, p20, p1, p14, p21, p17, not_p4, p25, p8, not_p5}
{p15, p9, p2, p21, p17, not_p4, p25, p8, not_p5}
{p15, p18, p1, p9, p21, p17, not_p4, p25, p8, not_p5}
{p15, p12, p14, p17, not_p4, p25, p8, not_p5}
{p15, p12, p9, p17, not_p4, p25, p8, not_p5}
{p15, p6, p12, p14, not_p4, p8, not_p5}
{p15, p6, p18, p1, not_p15, p14, not_p20, not_p4, p8, not_p5}
{p15, p6, p20, p1, not_p15, p14, not_p20, not_p4, p8, not_p5}
{p15, p6, p14, p2, not_p20, not_p4, p8, not_p5}
{p15, p6, p18, p1, not_p15, p14, p21, not_p4, p8, not_p5}
{p15, p6, p20, p1, not_p15, p14, p21, not_p4, p8, not_p5}
{p15, p6, p14, p2, p21, not_p4, p8, not_p5}
{p15, p6, p9, p2, not_p4, p8, not_p5}
{p15, p6, p18, p1, not_p15, p9, not_p4, p8, not_p5}
{p15, p6, p20, p1, not_p15, p9, not_p4, p8, not_p5}
{p15, p6, p12, p9, not_p4, p8, not_p5}
{p15, p12, p14, p11, p17, not_p4, not_p5}
{p15, p12, p11, p17, not_p4, not_p3, not_p5}
{p15, p12, p11, p17, not_p4, not_p25, not_p5}
{p15, p12, p9, p11, p17, not_p4, not_p5}
{p15, p14, p11, not_p20, p17, not_p4, not_p16, not_p5}
{p15, p11, not_p20, p17, not_p4, not_p3, not_p16, not_p5}
{p15, p11, not_p20, p17, not_p4, not_p25, not_p16, not_p5}
{p15, p9, p11, not_p20, p17, not_p4, not_p16, not_p5}
{p15, p11, p21, p17, not_p4, not_p3, not_p16, not_p5}
{p15, p11, p21, p17, not_p4, not_p25, not_p16, not_p5}
{p15, p14, p11, p21, p17, not_p4, not_p16, not_p5}
{p15, p9, p11, p21, p17, not_p4, not_p16, not_p5}
{p15, p2, p11, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p18, p1, p11, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p20, p1, p11, not_p20, p17, not_p4, not_p3, not_p5}
{p15, p2, p11, p21, p17, not_p4, not_p3, not_p5}
{p15, p18, p1, p11, p21, p17, not_p4, not_p3, not_p5}
{p15, p20, p1, p11, p21, p17, not_p4, not_p3, not_p5}
{p15, p2, p11, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p18, p1, p11, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p20, p1, p11, not_p20, p17, not_p4, not_p25, not_p5}
{p15, p2, p11, p21, p17, not_p4, not_p25, not_p5}
{p15, p18, p1, p11, p21, p17, not_p4, not_p25, not_p5}
{p15, p20, p1, p11, p21, p17, not_p4, not_p25, not_p5}
{p15, p9, p2, p11, not_p20, p17, not_p4, not_p5}
{p15, p18, p1, p9, p11, not_p20, p17, not_p4, not_p5}
{p15, p6, p20, p1, not_p15, p9, p11, not_p20, p17, not_p4, not_p5}
{p15, p7, p20, p1, not_p15, p9, p11, not_p20, p17, not_p4, not_p5}
{p15, p9, p2, p11, p21, p17, not_p4, not_p5}
{p15, p18, p1, p9, p11, p21, p17, not_p4, not_p5}
{p15, p6, p20, p1, not_p15, p9, p11, p21, p17, not_p4, not_p5}
{p15, p7, p20, p1, not_p15, p9, p11, p21, p17, not_p4, not_p5}
{p15, p14, p2, p11, not_p20, p17, not_p4, not_p5}
{p15, p18, p1, p14, p11, not_p20, p17, not_p4, not_p5}
{p15, p20, p1, p14, p11, not_p20, p17, not_p4, not_p5}
{p15, p14, p2, p11, p21, p17, not_p4, not_p5}
{p15, p18, p1, p14, p11, p21, p17, not_p4, not_p5}
{p15, p20, p1, p14, p11, p21, p17, not_p4, not_p5}
{p15, p12, p14, p11, not_p4, not_p8, not_p5}
{p15, p12, p11, not_p4, not_p3, not_p8, not_p5}
{p15, p12, p11, not_p4, not_p25, not_p8, not_p5}
{p15, p12, p9, p11, not_p4, not_p8, not_p5}
{p15, p11, not_p4, p25, not_p3, not_p8, not_p16, not_p5}
{p15, p11, not_p4, p25, not_p25, not_p8, not_p16, not_p5}
{p15, p14, p11, not_p20, not_p4, p25, not_p8, not_p16, not_p5}
{p15, p14, p11, p21, not_p4, p25, not_p8, not_p16, not_p5}
{p15, p9, p11, not_p4, p25, not_p8, not_p16, not_p5}
{p15, p2, p11, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p2, p11, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p14, p2, p11, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p14, p2, p11, p21, not_p4, p25, not_p8, not_p5}
{p15, p9, p2, p11, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p9, p11, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p11, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p18, p1, p11, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p18, p1, p14, p11, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p18, p1, p14, p11, p21, not_p4, p25, not_p8, not_p5}
{p15, p20, p1, p11, not_p4, p25, not_p3, not_p8, not_p5}
{p15, p20, p1, p11, not_p4, p25, not_p25, not_p8, not_p5}
{p15, p20, p1, p14, p11, not_p20, not_p4, p25, not_p8, not_p5}
{p15, p20, p1, p14, p11, p21, not_p4, p25, not_p8, not_p5}
{p15, p6, p20, p1, not_p15, p9, p11, not_p4, p25, not_p8, not_p5}
{p15, p7, p20, p1, not_p15, p9, p11, not_p4, p25, not_p8, not_p5}
{p15, p12, p14, p11, not_p4, not_p19, not_p5}
{p15, p12, p11, not_p4, not_p3, not_p19, not_p5}
{p15, p12, p11, not_p4, not_p25, not_p19, not_p5}
{p15, p12, p9, p11, not_p4, not_p19, not_p5}
{p15, p6, p12, p14, p11, not_p4, not_p5}
{p15, p6, p12, p11, not_p4, not_p3, not_p5}
{p15, p6, p12, p11, not_p4, not_p25, not_p5}
{p15, p6, p12, p9, p11, not_p4, not_p5}
{p15, p5, p11, p17, not_p4, not_p3, not_p16, not_p5}
{p15, p5, p11, p17, not_p4, not_p3, not_p19, not_p16}
{p15, p5, p22, p11, p17, not_p4, not_p3, not_p16}
{p15, p5, p11, p17, not_p4, not_p25, not_p16, not_p5}
{p15, p5, p11, p17, not_p4, not_p25, not_p19, not_p16}
{p15, p5, p22, p11, p17, not_p4, not_p25, not_p16}
{p15, p5, p22, p14, p11, p17, not_p4, not_p16}
{p15, p5, p14, p11, p17, not_p4, p25, not_p16, not_p5}
{p15, p5, p14, p11, p17, not_p4, p25, not_p19, not_p16}
{p15, p5, p9, p11, p17, not_p4, not_p16, not_p5}
{p15, p5, p9, p11, p17, not_p4, not_p19, not_p16}
{p15, p5, p22, p9, p11, p17, not_p4, not_p16}
{p15, p5, p2, p11, p17, not_p4, not_p3, not_p5}
{p15, p5, p2, p11, p17, not_p4, not_p3, not_p19}
{p15, p5, p22, p2, p11, p17, not_p4, not_p3}
{p15, p5, p18, p1, p11, p17, not_p4, not_p3, not_p5}
{p15, p5, p20, p1, p11, p17, not_p4, not_p3, not_p5}
{p15, p5, p18, p1, p11, p17, not_p4, not_p3, not_p19}
{p15, p5, p20, p1, p11, p17, not_p4, not_p3, not_p19}
{p15, p5, p1, p22, p11, p17, not_p4, not_p3}
{p15, p5, p2, p11, p17, not_p4, not_p25, not_p5}
{p15, p5, p2, p11, p17, not_p4, not_p25, not_p19}
{p15, p5, p22, p2, p11, p17, not_p4, not_p25}
{p15, p5, p18, p1, p11, p17, not_p4, not_p25, not_p5}
{p15, p5, p20, p1, p11, p17, not_p4, not_p25, not_p5}
{p15, p5, p18, p1, p11, p17, not_p4, not_p25, not_p19}
{p15, p5, p20, p1, p11, p17, not_p4, not_p25, not_p19}
{p15, p5, p1, p22, p11, p17, not_p4, not_p25}
{p15, p5, p22, p14, p2, p11, p17, not_p4}
{p15, p5, p1, p22, p14, p11, p17, not_p4}
{p15, p5, p14, p2, p11, p17, not_p4, p25, not_p5}
{p15, p5, p18, p1, p14, p11, p17, not_p4, p25, not_p5}
{p15, p5, p20, p1, p14, p11, p17, not_p4, p25, not_p5}
{p15, p5, p14, p2, p11, p17, not_p4, p25, not_p19}
{p15, p5, p18, p1, p14, p11, p17, not_p4, p25, not_p19}
{p15, p5, p20, p1, p14, p11, p17, not_p4, p25, not_p19}
{p15, p5, p9, p2, p11, p17, not_p4, not_p5}
{p15, p5, p9, p2, p11, p17, not_p4, not_p19}
{p15, p5, p22, p9, p2, p11, p17, not_p4}
{p15, p5, p18, p1, p9, p11, p17, not_p4, not_p5}
{p15, p6, p5, p20, p1, not_p15, p9, p11, p17, not_p4, not_p5}
{p15, p5, p7, p20, p1, not_p15, p9, p11, p17, not_p4, not_p5}
{p15, p5, p18, p1, p9, p11, p17, not_p4, not_p19}
{p15, p6, p5, p20, p1, not_p15, p9, p11, p17, not_p4, not_p19}
{p15, p5, p7, p20, p1, not_p15, p9, p11, p17, not_p4, not_p19}
{p15, p5, p18, p1, p22, p9, p11, p17, not_p4}
{p15, p6, p5, p1, p22, not_p15, p9, p11, p17, not_p4}
{p15, p5, p7, p1, p22, not_p15, p9, p11, p17, not_p4}
{p15, p5, p22, p14, p17, not_p4, p8, not_p16}
{p15, p5, p22, p17, not_p4, p8, not_p3, not_p16}
{p15, p5, p22, p17, not_p4, p8, not_p25, not_p16}
{p15, p5, p22, p9, p17, not_p4, p8, not_p16}
{p15, p5, p22, p12, p17, not_p4, p8, not_p3}
{p15, p5, p22, p2, p17, not_p4, p8, not_p3}
{p15, p5, p1, p22, p17, not_p4, p8, not_p3}
{p15, p5, p22, p12, p17, not_p4, p8, not_p25}
{p15, p5, p22, p2, p17, not_p4, p8, not_p25}
{p15, p5, p1, p22, p17, not_p4, p8, not_p25}
{p15, p5, p22, p12, p14, p17, not_p4, p8}
{p15, p5, p22, p14, p2, p17, not_p4, p8}
{p15, p5, p1, p22, p14, p17, not_p4, p8}
{p15, p5, p22, p12, p9, p17, not_p4, p8}
{p15, p5, p18, p1, p22, p9, p17, not_p4, p8}
{p15, p5, p7, p1, p22, not_p15, p9, p17, not_p4, p8}
{p15, p5, p22, p9, p2, p17, not_p4, p8}
{p15, p5, p14, p17, not_p4, p25, p8, not_p16, not_p5}
{p15, p5, p17, not_p4, p25, p8, not_p3, not_p16, not_p5}
{p15, p5, p17, not_p4, p25, p8, not_p25, not_p16, not_p5}
{p15, p5, p9, p17, not_p4, p25, p8, not_p16, not_p5}
{p15, p5, p21, p17, not_p4, p8, not_p3, not_p16, not_p5}
{p15, p5, p21, p17, not_p4, p8, not_p25, not_p16, not_p5}
{p15, p5, p14, p21, p17, not_p4, p8, not_p16, not_p5}
{p15, p5, p9, p21, p17, not_p4, p8, not_p16, not_p5}
{p15, p5, p2, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p5, p18, p1, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p5, p20, p1, p17, not_p4, p25, p8, not_p3, not_p5}
{p15, p5, p12, p21, p17, not_p4, p8, not_p3, not_p5}
{p15, p5, p2, p21, p17, not_p4, p8, not_p3, not_p5}
{p15, p5, p18, p1, p21, p17, not_p4, p8, not_p3, not_p5}
{p15, p5, p20, p1, p21, p17, not_p4, p8, not_p3, not_p5}
{p15, p5, p14, p2, p17, not_p4, p25, p8, not_p5}
{p15, p5, p2, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p5, p9, p2, p17, not_p4, p25, p8, not_p5}
{p15, p5, p18, p1, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p5, p20, p1, p17, not_p4, p25, p8, not_p25, not_p5}
{p15, p5, p18, p1, p14, p17, not_p4, p25, p8, not_p5}
{p15, p5, p20, p1, p14, p17, not_p4, p25, p8, not_p5}
{p15, p5, p18, p1, p9, p17, not_p4, p25, p8, not_p5}
{p15, p5, p7, p20, p1, not_p15, p9, p17, not_p4, p25, p8, not_p5}
{p15, p5, p12, p21, p17, not_p4, p8, not_p25, not_p5}
{p15, p5, p2, p21, p17, not_p4, p8, not_p25, not_p5}
{p15, p5, p18, p1, p21, p17, not_p4, p8, not_p25, not_p5}
{p15, p5, p20, p1, p21, p17, not_p4, p8, not_p25, not_p5}
{p15, p5, p12, p14, p21, p17, not_p4, p8, not_p5}
{p15, p5, p14, p2, p21, p17, not_p4, p8, not_p5}
{p15, p5, p18, p1, p14, p21, p17, not_p4, p8, not_p5}
{p15, p5, p20, p1, p14, p21, p17, not_p4, p8, not_p5}
{p15, p5, p12, p9, p21, p17, not_p4, p8, not_p5}
{p15, p5, p18, p1, p9, p21, p17, not_p4, p8, not_p5}
{p15, p5, p9, p2, p21, p17, not_p4, p8, not_p5}
{p15, p5, p22, p14, not_p4, p8, not_p8, not_p16}
{p15, p5, p22, p14, p2, not_p4, p8, not_p8}
{p15, p5, p1, p22, p14, not_p4, p8, not_p8}
{p15, p5, p14, p11, not_p4, p25, not_p8, not_p16, not_p5}
{p15, p5, p14, p11, not_p4, p25, not_p19, not_p8, not_p16}
{p15, p5, p22, p14, p11, not_p4, p25, not_p8, not_p16}
{p15, p5, p14, p2, p11, not_p4, p25, not_p8, not_p5}
{p15, p5, p14, p2, p11, not_p4, p25, not_p19, not_p8}
{p15, p5, p22, p14, p2, p11, not_p4, p25, not_p8}
{p15, p5, p18, p1, p14, p11, not_p4, p25, not_p8, not_p5}
{p15, p5, p20, p1, p14, p11, not_p4, p25, not_p8, not_p5}
{p15, p5, p18, p1, p14, p11, not_p4, p25, not_p19, not_p8}
{p15, p5, p20, p1, p14, p11, not_p4, p25, not_p19, not_p8}
{p15, p5, p1, p22, p14, p11, not_p4, p25, not_p8}
{p15, p5, p14, not_p4, p25, p8, not_p8, not_p16, not_p5}
{p15, p5, p14, p2, not_p4, p25, p8, not_p8, not_p5}
{p15, p5, p18, p1, p14, not_p4, p25, p8, not_p8, not_p5}
{p15, p5, p20, p1, p14, not_p4, p25, p8, not_p8, not_p5}
{p15, p5, p14, not_p4, p25, p8, not_p19, not_p16}
{p15, p5, p14, p2, not_p4, p25, p8, not_p19}
{p15, p5, p18, p1, p14, not_p4, p25, p8, not_p19}
{p15, p5, p20, p1, p14, not_p4, p25, p8, not_p19}
{p15, p5, p22, p14, not_p4, p8, not_p19, not_p16}
{p15, p5, p22, p14, p2, not_p4, p8, not_p19}
{p15, p5, p1, p22, p14, not_p4, p8, not_p19}
{p15, p6, p5, p1, p22, not_p15, p14, not_p4, p8}
{p15, p6, p5, p18, p1, not_p15, p14, not_p4, p25, p8, not_p5}
{p15, p6, p5, p20, p1, not_p15, p14, not_p4, p25, p8, not_p5}
{p15, p6, p5, p14, not_p4, p25, p8, not_p16, not_p5}
{p15, p6, p5, p14, p2, not_p4, p25, p8, not_p5}
{p15, p6, p5, p22, p14, not_p4, p8, not_p16}
{p15, p6, p5, p22, p14, p2, not_p4, p8}
{p20, p1, p16, p9, p24, p4, not_p12, p8, not_p19, not_p5}
{p20, p1, p16, p9, p24, not_p20, p17, p4, not_p12, not_p5}
{p20, p1, p16, p9, p24, p21, p17, p4, p25, not_p12, not_p5}
{p20, p1, p16, p9, p11, p24, p21, p17, p4, not_p12, not_p5}
{p20, p1, p16, p9, p24, p4, not_p12, p8, not_p8, not_p5}
{p20, p1, p16, p9, p24, p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p16, p9, p24, not_p21, p4, p8, not_p19, not_p5}
{p3, p1, p16, p9, p24, not_p21, p4, p8, not_p19, not_p5}
{p1, p22, p16, p9, p24, p4, p8, not_p19, not_p5}
{p1, p22, p16, p9, p24, not_p20, p17, p4, not_p5}
{p1, p22, p16, p9, p24, p21, p17, p4, p25, not_p5}
{p1, p22, p16, p9, p11, p24, p21, p17, p4, not_p5}
{p20, p1, p16, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p3, p1, p16, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p20, p1, p16, p9, p24, not_p21, p21, p17, p4, p25, not_p5}
{p20, p1, p16, p9, p11, p24, not_p21, p21, p17, p4, not_p5}
{p3, p1, p16, p9, p24, not_p21, p21, p17, p4, p25, not_p5}
{p3, p1, p16, p9, p11, p24, not_p21, p21, p17, p4, not_p5}
{p20, p1, p16, p9, p24, not_p21, p4, p8, not_p8, not_p5}
{p3, p1, p16, p9, p24, not_p21, p4, p8, not_p8, not_p5}
{p1, p22, p16, p9, p24, p4, p8, not_p8, not_p5}
{p20, p1, p16, p9, p24, not_p21, p4, p25, not_p8, not_p5}
{p3, p1, p16, p9, p24, not_p21, p4, p25, not_p8, not_p5}
{p1, p22, p16, p9, p24, p4, p25, not_p8, not_p5}
{p7, p20, p1, p12, p16, p9, p24, p17, not_p4, p25, not_p12, not_p5}
{p7, p20, p1, p16, p9, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p7, p20, p1, p16, p9, p24, p21, p17, not_p4, not_p12, not_p5}
{p7, p1, p22, p16, p9, p24, not_p20, p17, not_p4, not_p5}
{p7, p1, p22, p16, p9, p24, p21, p17, not_p4, not_p5}
{p7, p1, p22, p12, p16, p9, p24, p17, not_p4, not_p5}
{p7, p20, p1, p12, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p3, p7, p1, p12, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p7, p20, p1, p16, p9, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p3, p7, p1, p16, p9, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p7, p20, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p5}
{p3, p7, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p5}
{p1, p22, p16, p14, p24, not_p20, p17, not_p4, not_p5}
{p1, p22, p16, p24, not_p20, p17, not_p4, not_p3, not_p5}
{p1, p22, p16, p24, not_p20, p17, not_p4, not_p25, not_p5}
{p6, p1, p22, p16, p9, p24, not_p20, p17, not_p4, not_p5}
{p18, p1, p22, p16, p9, p24, not_p20, p17, not_p4, not_p5}
{p1, p22, p16, p24, p21, p17, not_p4, p25, not_p3, not_p5}
{p1, p22, p12, p16, p24, p17, not_p4, p25, not_p3, not_p24, not_p5}
{p1, p22, p12, p16, p24, p17, not_p4, p25, not_p3, not_p2, not_p5}
{p1, p22, p16, p24, p21, p17, not_p4, p25, not_p25, not_p5}
{p1, p22, p12, p16, p24, p17, not_p4, p25, not_p25, not_p24, not_p5}
{p1, p22, p12, p16, p24, p17, not_p4, p25, not_p25, not_p2, not_p5}
{p1, p22, p16, p14, p24, p21, p17, not_p4, p25, not_p5}
{p1, p22, p12, p16, p14, p24, p17, not_p4, p25, not_p24, not_p5}
{p1, p22, p12, p16, p14, p24, p17, not_p4, p25, not_p2, not_p5}
{p6, p1, p22, p16, p9, p24, p21, p17, not_p4, p25, not_p5}
{p18, p1, p22, p16, p9, p24, p21, p17, not_p4, p25, not_p5}
{p18, p1, p22, p12, p16, p9, p24, p17, not_p4, p25, not_p5}
{p6, p1, p22, p16, p9, p11, p24, p21, p17, not_p4, not_p5}
{p18, p1, p16, p14, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p18, p1, p16, p9, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p18, p1, p16, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p5}
{p18, p1, p16, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p5}
{p18, p1, p16, p14, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p18, p1, p16, p9, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p18, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p18, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p6, p20, p1, p16, p9, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p6, p20, p1, p16, p9, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p20, p1, p16, p14, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p20, p1, p16, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p5}
{p20, p1, p16, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p5}
{p20, p1, p16, p14, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p20, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p20, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p6, p3, p1, p16, p9, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p3, p1, p16, p14, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p3, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p3, p1, p16, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p18, p1, p16, p14, p24, p21, p17, not_p4, p25, not_p12, not_p5}
{p18, p1, p16, p9, p24, p21, p17, not_p4, p25, not_p12, not_p5}
{p18, p1, p16, p24, p21, p17, not_p4, p25, not_p12, not_p3, not_p5}
{p18, p1, p16, p24, p21, p17, not_p4, p25, not_p12, not_p25, not_p5}
{p18, p1, p16, p14, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p18, p1, p16, p9, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p18, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p3, not_p5}
{p18, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p25, not_p5}
{p6, p20, p1, p16, p9, p24, p21, p17, not_p4, p25, not_p12, not_p5}
{p6, p20, p1, p16, p9, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p6, p20, p1, p16, p9, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p6, p20, p1, p16, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p20, p1, p16, p14, p24, p21, p17, not_p4, p25, not_p12, not_p5}
{p20, p1, p16, p24, p21, p17, not_p4, p25, not_p12, not_p3, not_p5}
{p20, p1, p16, p24, p21, p17, not_p4, p25, not_p12, not_p25, not_p5}
{p20, p1, p16, p14, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p20, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p3, not_p5}
{p20, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p25, not_p5}
{p6, p3, p1, p16, p9, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p3, p1, p16, p14, p24, not_p21, p21, p17, not_p4, p25, not_p5}
{p3, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p3, not_p5}
{p3, p1, p16, p24, not_p21, p21, p17, not_p4, p25, not_p25, not_p5}
{p6, p3, p1, p16, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p18, p1, p12, p16, p9, p24, p17, not_p4, p25, not_p12, not_p5}
{p18, p1, p12, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p18, p1, p12, p16, p14, p24, p17, not_p4, p25, not_p12, not_p24, not_p5}
{p18, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p24, not_p5}
{p18, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, p17, not_p4, p25, not_p12, not_p2, not_p5}
{p18, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p2, not_p5}
{p18, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p2, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p2, not_p5}
{p20, p1, p12, p16, p14, p24, p17, not_p4, p25, not_p12, not_p24, not_p5}
{p20, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p24, not_p5}
{p20, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, p17, not_p4, p25, not_p12, not_p2, not_p5}
{p20, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p2, not_p5}
{p20, p1, p12, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p2, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p2, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p24, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, p17, not_p4, p25, not_p2, not_p5}
{p3, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p2, not_p5}
{p3, p1, p12, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p2, not_p5}
{p1, p22, p16, p24, not_p4, p25, not_p3, not_p8, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p3, not_p8, not_p24, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p3, not_p8, not_p2, not_p5}
{p1, p22, p16, p24, not_p4, p25, not_p25, not_p8, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p25, not_p8, not_p24, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p25, not_p8, not_p2, not_p5}
{p7, p1, p22, p16, p9, p24, not_p4, p8, not_p8, not_p5}
{p1, p22, p16, p14, p24, not_p20, not_p4, p25, not_p8, not_p5}
{p1, p22, p16, p14, p24, p21, not_p4, p25, not_p8, not_p5}
{p6, p1, p22, p16, p9, p24, not_p4, p25, not_p8, not_p5}
{p18, p1, p22, p16, p9, p24, not_p4, p25, not_p8, not_p5}
{p7, p1, p22, p16, p9, p24, not_p4, p25, not_p8, not_p5}
{p1, p22, p12, p16, p14, p24, not_p4, not_p8, not_p24, not_p5}
{p1, p22, p12, p16, p14, p24, not_p4, not_p8, not_p2, not_p5}
{p18, p1, p22, p12, p16, p9, p24, not_p4, not_p8, not_p5}
{p7, p1, p22, p12, p16, p9, p24, not_p4, not_p8, not_p5}
{p18, p1, p16, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p8, not_p2, not_p5}
{p18, p1, p16, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p2, not_p5}
{p20, p1, p16, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p8, not_p2, not_p5}
{p20, p1, p16, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p2, not_p5}
{p3, p1, p16, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p8, not_p2, not_p5}
{p18, p1, p16, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p8, not_p2, not_p5}
{p18, p1, p16, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p2, not_p5}
{p20, p1, p16, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p8, not_p2, not_p5}
{p20, p1, p16, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p2, not_p5}
{p3, p1, p16, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p8, not_p2, not_p5}
{p7, p20, p1, p16, p9, p24, not_p4, not_p12, p8, not_p8, not_p5}
{p7, p20, p1, p16, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p3, p7, p1, p16, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p18, p1, p16, p9, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p16, p9, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p18, p1, p16, p14, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p16, p14, p24, p21, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p16, p14, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p18, p1, p16, p14, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p20, p1, p16, p14, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p16, p14, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p20, p1, p16, p14, p24, p21, not_p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p16, p14, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p6, p20, p1, p16, p9, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p7, p20, p1, p16, p9, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p6, p20, p1, p16, p9, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p7, p20, p1, p16, p9, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p3, p1, p16, p14, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p3, p1, p16, p14, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p6, p3, p1, p16, p9, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p3, p7, p1, p16, p9, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p18, p1, p12, p16, p9, p24, not_p4, not_p12, not_p8, not_p5}
{p18, p1, p12, p16, p9, p24, not_p21, not_p4, not_p8, not_p5}
{p7, p20, p1, p12, p16, p9, p24, not_p4, not_p12, not_p8, not_p5}
{p7, p20, p1, p12, p16, p9, p24, not_p21, not_p4, not_p8, not_p5}
{p3, p7, p1, p12, p16, p9, p24, not_p21, not_p4, not_p8, not_p5}
{p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p8, not_p2, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p2, not_p5}
{p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p8, not_p2, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p2, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p24, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p8, not_p2, not_p5}
{p6, p1, p22, p16, p9, p24, not_p4, p8, not_p5}
{p6, p18, p1, p16, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, p16, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, p16, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p20, p1, p16, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p3, p1, p16, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p1, p22, p16, p24, not_p4, p8, not_p3, not_p5}
{p6, p1, p22, p16, p24, not_p4, p8, not_p25, not_p5}
{p6, p1, p22, p16, p14, p24, not_p20, not_p4, p8, not_p5}
{p6, p1, p22, p16, p14, p24, p21, not_p4, p8, not_p5}
{p6, p18, p1, p16, p24, not_p4, not_p12, p8, not_p3, not_p5}
{p6, p18, p1, p16, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p20, p1, p16, p24, not_p4, not_p12, p8, not_p3, not_p5}
{p6, p20, p1, p16, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p3, p1, p16, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p18, p1, p16, p24, not_p4, not_p12, p8, not_p25, not_p5}
{p6, p18, p1, p16, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p20, p1, p16, p24, not_p4, not_p12, p8, not_p25, not_p5}
{p6, p20, p1, p16, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p3, p1, p16, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p18, p1, p16, p14, p24, not_p20, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, p16, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p20, p1, p16, p14, p24, not_p20, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, p16, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p3, p1, p16, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p18, p1, p16, p14, p24, p21, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, p16, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p6, p20, p1, p16, p14, p24, p21, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, p16, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p6, p3, p1, p16, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p7, p1, p22, p16, p9, p24, not_p4, p8, not_p19, not_p5}
{p7, p20, p1, p16, p9, p24, not_p4, not_p12, p8, not_p19, not_p5}
{p7, p20, p1, p16, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p3, p7, p1, p16, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p6, p1, p22, p12, p16, p9, p24, not_p4, not_p5}
{p6, p1, p22, p12, p16, p14, p24, not_p4, not_p24, not_p5}
{p6, p1, p22, p12, p16, p14, p24, not_p4, not_p2, not_p5}
{p6, p1, p22, p12, p16, p24, not_p4, not_p3, not_p24, not_p5}
{p6, p1, p22, p12, p16, p24, not_p4, not_p25, not_p24, not_p5}
{p6, p1, p22, p12, p16, p24, not_p4, not_p3, not_p2, not_p5}
{p6, p1, p22, p12, p16, p24, not_p4, not_p25, not_p2, not_p5}
{p18, p1, p22, p12, p16, p9, p24, not_p4, not_p19, not_p5}
{p7, p1, p22, p12, p16, p9, p24, not_p4, not_p19, not_p5}
{p1, p22, p12, p16, p14, p24, not_p4, not_p19, not_p24, not_p5}
{p1, p22, p12, p16, p14, p24, not_p4, not_p19, not_p2, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p3, not_p19, not_p24, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p25, not_p19, not_p24, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p3, not_p19, not_p2, not_p5}
{p1, p22, p12, p16, p24, not_p4, not_p25, not_p19, not_p2, not_p5}
{p6, p18, p1, p12, p16, p9, p24, not_p4, not_p12, not_p5}
{p6, p18, p1, p12, p16, p9, p24, not_p21, not_p4, not_p5}
{p6, p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p24, not_p5}
{p6, p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p2, not_p5}
{p6, p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p2, not_p5}
{p6, p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p2, not_p5}
{p6, p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p24, not_p5}
{p6, p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p2, not_p5}
{p6, p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p2, not_p5}
{p6, p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p2, not_p5}
{p18, p1, p12, p16, p9, p24, not_p4, not_p12, not_p19, not_p5}
{p18, p1, p12, p16, p9, p24, not_p21, not_p4, not_p19, not_p5}
{p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, not_p4, not_p12, not_p19, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p19, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p19, not_p2, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p24, not_p5}
{p18, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p2, not_p5}
{p18, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p2, not_p5}
{p6, p20, p1, p12, p16, p9, p24, not_p4, not_p12, not_p5}
{p6, p20, p1, p12, p16, p9, p24, not_p21, not_p4, not_p5}
{p6, p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p24, not_p5}
{p6, p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p2, not_p5}
{p6, p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p2, not_p5}
{p6, p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p2, not_p5}
{p6, p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p24, not_p5}
{p6, p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p2, not_p5}
{p6, p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p2, not_p5}
{p6, p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p2, not_p5}
{p7, p20, p1, p12, p16, p9, p24, not_p4, not_p12, not_p19, not_p5}
{p7, p20, p1, p12, p16, p9, p24, not_p21, not_p4, not_p19, not_p5}
{p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, not_p4, not_p12, not_p19, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p3, not_p19, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p4, not_p12, not_p25, not_p19, not_p2, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p24, not_p5}
{p20, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p2, not_p5}
{p20, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p2, not_p5}
{p6, p3, p1, p12, p16, p9, p24, not_p21, not_p4, not_p5}
{p6, p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p24, not_p5}
{p6, p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p2, not_p5}
{p6, p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p24, not_p5}
{p6, p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p24, not_p5}
{p6, p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p2, not_p5}
{p6, p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p2, not_p5}
{p3, p7, p1, p12, p16, p9, p24, not_p21, not_p4, not_p19, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p24, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p24, not_p5}
{p3, p1, p12, p16, p14, p24, not_p21, not_p4, not_p19, not_p2, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p3, not_p19, not_p2, not_p5}
{p3, p1, p12, p16, p24, not_p21, not_p4, not_p25, not_p19, not_p2, not_p5}
{p5, p22, not_p15, p16, p14, p2, p24, p17, not_p4}
{p5, p22, not_p15, p16, p2, p24, p17, not_p4, not_p3}
{p5, p22, not_p15, p16, p2, p24, p17, not_p4, not_p25}
{p5, p22, not_p15, p16, p9, p2, p24, p17, not_p4}
{p5, p1, p22, p16, p24, p17, not_p4, not_p3}
{p5, p1, p22, p16, p24, p17, not_p4, not_p25}
{p5, p1, p22, p16, p14, p24, p17, not_p4}
{p5, p18, p1, p22, p16, p9, p24, p17, not_p4}
{p6, p5, p1, p22, p16, p9, p24, p17, not_p4}
{p5, p7, p1, p22, p16, p9, p24, p17, not_p4}
{p5, not_p15, p16, p2, p24, p17, not_p4, p25, not_p12, not_p3, not_p5}
{p5, not_p15, p16, p2, p24, not_p21, p17, not_p4, p25, not_p3, not_p5}
{p5, not_p15, p16, p2, p24, p21, p17, not_p4, not_p12, not_p3, not_p5}
{p5, not_p15, p16, p2, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p5, not_p15, p16, p2, p24, p17, not_p4, p25, not_p12, not_p3, not_p19}
{p5, not_p15, p16, p2, p24, not_p21, p17, not_p4, p25, not_p3, not_p19}
{p5, not_p15, p16, p2, p24, p21, p17, not_p4, not_p12, not_p3, not_p19}
{p5, not_p15, p16, p2, p24, not_p21, p21, p17, not_p4, not_p3, not_p19}
{p5, p18, p1, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p5}
{p5, p18, p1, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p5}
{p5, p18, p1, p16, p24, p21, p17, not_p4, not_p12, not_p3, not_p5}
{p5, p18, p1, p16, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p5, p20, p1, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p5}
{p5, p20, p1, p16, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p5, p20, p1, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p5}
{p5, p20, p1, p16, p24, p21, p17, not_p4, not_p12, not_p3, not_p5}
{p5, p18, p1, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p19}
{p5, p18, p1, p16, p24, p21, p17, not_p4, not_p12, not_p3, not_p19}
{p5, p20, p1, p16, p24, p17, not_p4, p25, not_p12, not_p3, not_p19}
{p5, p20, p1, p16, p24, p21, p17, not_p4, not_p12, not_p3, not_p19}
{p5, p18, p1, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p19}
{p5, p18, p1, p16, p24, not_p21, p21, p17, not_p4, not_p3, not_p19}
{p5, p20, p1, p16, p24, not_p21, p17, not_p4, p25, not_p3, not_p19}
{p5, p20, p1, p16, p24, not_p21, p21, p17, not_p4, not_p3, not_p19}
{p5, not_p15, p16, p2, p24, p17, not_p4, p25, not_p12, not_p25, not_p5}
{p5, not_p15, p16, p2, p24, not_p21, p17, not_p4, p25, not_p25, not_p5}
{p5, not_p15, p16, p2, p24, p21, p17, not_p4, not_p12, not_p25, not_p5}
{p5, not_p15, p16, p2, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p5, not_p15, p16, p2, p24, p17, not_p4, p25, not_p12, not_p25, not_p19}
{p5, not_p15, p16, p2, p24, not_p21, p17, not_p4, p25, not_p25, not_p19}
{p5, not_p15, p16, p2, p24, p21, p17, not_p4, not_p12, not_p25, not_p19}
{p5, not_p15, p16, p2, p24, not_p21, p21, p17, not_p4, not_p25, not_p19}
{p5, p18, p1, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p5}
{p5, p18, p1, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p5}
{p5, p18, p1, p16, p24, p21, p17, not_p4, not_p12, not_p25, not_p5}
{p5, p18, p1, p16, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p5, p20, p1, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p5}
{p5, p20, p1, p16, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p5, p20, p1, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p5}
{p5, p20, p1, p16, p24, p21, p17, not_p4, not_p12, not_p25, not_p5}
{p5, p18, p1, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p19}
{p5, p18, p1, p16, p24, p21, p17, not_p4, not_p12, not_p25, not_p19}
{p5, p20, p1, p16, p24, p17, not_p4, p25, not_p12, not_p25, not_p19}
{p5, p20, p1, p16, p24, p21, p17, not_p4, not_p12, not_p25, not_p19}
{p5, p18, p1, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p19}
{p5, p18, p1, p16, p24, not_p21, p21, p17, not_p4, not_p25, not_p19}
{p5, p20, p1, p16, p24, not_p21, p17, not_p4, p25, not_p25, not_p19}
{p5, p20, p1, p16, p24, not_p21, p21, p17, not_p4, not_p25, not_p19}
{p5, not_p15, p16, p14, p2, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, not_p15, p16, p14, p2, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, not_p15, p16, p14, p2, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, not_p15, p16, p14, p2, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p18, p1, p16, p14, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p18, p1, p16, p14, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p20, p1, p16, p14, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p20, p1, p16, p14, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p18, p1, p16, p14, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p20, p1, p16, p14, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p18, p1, p16, p14, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p20, p1, p16, p14, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, not_p15, p16, p14, p2, p24, p21, p17, not_p4, not_p12, not_p5}
{p5, not_p15, p16, p14, p2, p24, not_p21, p21, p17, not_p4, not_p5}
{p5, p18, p1, p16, p14, p24, p21, p17, not_p4, not_p12, not_p5}
{p5, p20, p1, p16, p14, p24, p21, p17, not_p4, not_p12, not_p5}
{p5, p18, p1, p16, p14, p24, not_p21, p21, p17, not_p4, not_p5}
{p5, p20, p1, p16, p14, p24, not_p21, p21, p17, not_p4, not_p5}
{p5, not_p15, p16, p14, p2, p24, p21, p17, not_p4, not_p12, not_p19}
{p5, p18, p1, p16, p14, p24, p21, p17, not_p4, not_p12, not_p19}
{p5, p20, p1, p16, p14, p24, p21, p17, not_p4, not_p12, not_p19}
{p5, not_p15, p16, p14, p2, p24, not_p21, p21, p17, not_p4, not_p19}
{p5, p18, p1, p16, p14, p24, not_p21, p21, p17, not_p4, not_p19}
{p5, p20, p1, p16, p14, p24, not_p21, p21, p17, not_p4, not_p19}
{p5, not_p15, p16, p9, p2, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, not_p15, p16, p9, p2, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, not_p15, p16, p9, p2, p24, p21, p17, not_p4, not_p12, not_p5}
{p5, not_p15, p16, p9, p2, p24, not_p21, p21, p17, not_p4, not_p5}
{p5, not_p15, p16, p9, p2, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, not_p15, p16, p9, p2, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, not_p15, p16, p9, p2, p24, p21, p17, not_p4, not_p12, not_p19}
{p5, not_p15, p16, p9, p2, p24, not_p21, p21, p17, not_p4, not_p19}
{p5, p18, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p18, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p18, p1, p16, p9, p24, p21, p17, not_p4, not_p12, not_p5}
{p5, p18, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p5}
{p6, p5, p20, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p5}
{p6, p5, p20, p1, p16, p9, p24, p21, p17, not_p4, not_p12, not_p5}
{p6, p5, p20, p1, p16, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p6, p5, p20, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p6, p5, p20, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p5}
{p6, p5, p20, p1, p16, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p5, p7, p20, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p7, p20, p1, p16, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p5, p7, p20, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p7, p20, p1, p16, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p5, p18, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p18, p1, p16, p9, p24, p21, p17, not_p4, not_p12, not_p19}
{p6, p5, p20, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p19}
{p6, p5, p20, p1, p16, p9, p24, p21, p17, not_p4, not_p12, not_p19}
{p6, p5, p20, p1, p16, p9, p11, p24, p17, not_p4, not_p12, not_p19}
{p5, p7, p20, p1, p16, p9, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p7, p20, p1, p16, p9, p11, p24, p17, not_p4, not_p12, not_p19}
{p5, p18, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p18, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p19}
{p6, p5, p20, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p19}
{p6, p5, p20, p1, p16, p9, p24, not_p21, p21, p17, not_p4, not_p19}
{p6, p5, p20, p1, p16, p9, p11, p24, not_p21, p17, not_p4, not_p19}
{p5, p7, p20, p1, p16, p9, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p7, p20, p1, p16, p9, p11, p24, not_p21, p17, not_p4, not_p19}
{p5, p22, not_p15, p16, p14, p2, p24, not_p4, p25, not_p8}
{p5, p1, p22, p16, p14, p24, not_p4, p25, not_p8}
{p5, not_p15, p16, p14, p2, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, not_p15, p16, p14, p2, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, p18, p1, p16, p14, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, p20, p1, p16, p14, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, p18, p1, p16, p14, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, p20, p1, p16, p14, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, not_p15, p16, p14, p2, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, not_p15, p16, p14, p2, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p5, p18, p1, p16, p14, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, p20, p1, p16, p14, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, p18, p1, p16, p14, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p5, p20, p1, p16, p14, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p6, p5, p1, p22, p16, p14, p24, not_p4, p8}
{p6, p5, p18, p1, p16, p14, p24, not_p4, p25, not_p12, p8, not_p5}
{p6, p5, p18, p1, p16, p14, p24, not_p21, not_p4, p25, p8, not_p5}
{p6, p5, p20, p1, p16, p14, p24, not_p21, not_p4, p25, p8, not_p5}
{p6, p5, p20, p1, p16, p14, p24, not_p4, p25, not_p12, p8, not_p5}
{p5, p20, p1, p16, p9, p24, p17, p4, p25, not_p12, not_p5}
{p5, p20, p1, p16, p9, p24, p21, p17, p4, not_p12, not_p5}
{p5, p20, p1, p16, p9, p11, p24, p17, p4, not_p12, not_p5}
{p5, p20, p1, p16, p9, p24, not_p21, p17, p4, p25, not_p5}
{p5, p20, p1, p16, p9, p24, not_p21, p21, p17, p4, not_p5}
{p5, p20, p1, p16, p9, p11, p24, not_p21, p17, p4, not_p5}
{p5, p20, p1, p16, p9, p24, p17, p4, p25, not_p12, not_p19}
{p5, p20, p1, p16, p9, p24, p21, p17, p4, not_p12, not_p19}
{p5, p20, p1, p16, p9, p11, p24, p17, p4, not_p12, not_p19}
{p5, p20, p1, p16, p9, p24, not_p21, p17, p4, p25, not_p19}
{p5, p20, p1, p16, p9, p24, not_p21, p21, p17, p4, not_p19}
{p5, p20, p1, p16, p9, p11, p24, not_p21, p17, p4, not_p19}
{p5, p1, p22, p16, p9, p24, p17, p4}
{p1, p22, p14, p24, not_p20, p17, p4, not_p5}
{p18, p1, p14, p24, not_p20, p17, p4, not_p12, not_p5}
{p20, p1, p14, p24, not_p20, p17, p4, not_p12, not_p5}
{p18, p1, p14, p24, not_p21, not_p20, p17, p4, not_p5}
{p20, p1, p14, p24, not_p21, not_p20, p17, p4, not_p5}
{p3, p1, p14, p24, not_p21, not_p20, p17, p4, not_p5}
{p1, p22, p24, not_p20, p17, p4, not_p3, not_p5}
{p1, p22, p24, not_p20, p17, p4, not_p25, not_p5}
{p1, p22, not_p15, p9, p24, not_p20, p17, p4, not_p5}
{p1, p22, p12, p9, p24, not_p20, p17, p4, not_p5}
{p18, p1, p22, p9, p24, not_p20, p17, p4, not_p5}
{p18, p1, p24, not_p20, p17, p4, not_p12, not_p3, not_p5}
{p18, p1, p24, not_p21, not_p20, p17, p4, not_p3, not_p5}
{p20, p1, p24, not_p20, p17, p4, not_p12, not_p3, not_p5}
{p20, p1, p24, not_p21, not_p20, p17, p4, not_p3, not_p5}
{p3, p1, p24, not_p21, not_p20, p17, p4, not_p3, not_p5}
{p18, p1, p24, not_p20, p17, p4, not_p12, not_p25, not_p5}
{p18, p1, p24, not_p21, not_p20, p17, p4, not_p25, not_p5}
{p20, p1, p24, not_p20, p17, p4, not_p12, not_p25, not_p5}
{p20, p1, p24, not_p21, not_p20, p17, p4, not_p25, not_p5}
{p3, p1, p24, not_p21, not_p20, p17, p4, not_p25, not_p5}
{p18, p1, p9, p24, not_p20, p17, p4, not_p12, not_p5}
{p18, p1, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p20, p1, not_p15, p9, p24, not_p20, p17, p4, not_p12, not_p5}
{p20, p1, not_p15, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p20, p1, p12, p9, p24, not_p20, p17, p4, not_p12, not_p5}
{p20, p1, p12, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p3, p1, not_p15, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p3, p1, p12, p9, p24, not_p21, not_p20, p17, p4, not_p5}
{p1, p22, p14, p24, p21, p17, p4, p25, not_p5}
{p1, p22, p24, p21, p17, p4, p25, not_p3, not_p5}
{p1, p22, p24, p21, p17, p4, p25, not_p25, not_p5}
{p1, p22, not_p15, p9, p24, p21, p17, p4, p25, not_p5}
{p18, p1, p22, p9, p24, p21, p17, p4, p25, not_p5}
{p1, p22, p12, p24, p17, p4, p25, not_p3, not_p24, not_p5}
{p1, p22, p12, p24, p17, p4, p25, not_p3, not_p2, not_p5}
{p1, p22, p12, p24, p17, p4, p25, not_p25, not_p24, not_p5}
{p1, p22, p12, p24, p17, p4, p25, not_p25, not_p2, not_p5}
{p1, p22, p12, p14, p24, p17, p4, p25, not_p24, not_p5}
{p1, p22, p12, p14, p24, p17, p4, p25, not_p2, not_p5}
{p1, p22, p12, p9, p24, p17, p4, p25, not_p5}
{p1, p22, not_p15, p9, p11, p24, p21, p17, p4, not_p5}
{p1, p22, p12, p9, p11, p24, p17, p4, not_p5}
{p18, p1, p14, p24, p21, p17, p4, p25, not_p12, not_p5}
{p18, p1, p9, p24, p21, p17, p4, p25, not_p12, not_p5}
{p18, p1, p24, p21, p17, p4, p25, not_p12, not_p3, not_p5}
{p18, p1, p24, p21, p17, p4, p25, not_p12, not_p25, not_p5}
{p18, p1, p14, p24, not_p21, p21, p17, p4, p25, not_p5}
{p18, p1, p9, p24, not_p21, p21, p17, p4, p25, not_p5}
{p18, p1, p24, not_p21, p21, p17, p4, p25, not_p3, not_p5}
{p18, p1, p24, not_p21, p21, p17, p4, p25, not_p25, not_p5}
{p18, p1, p12, p9, p24, p17, p4, p25, not_p12, not_p5}
{p18, p1, p12, p14, p24, p17, p4, p25, not_p12, not_p24, not_p5}
{p18, p1, p12, p14, p24, p17, p4, p25, not_p12, not_p2, not_p5}
{p18, p1, p12, p24, p17, p4, p25, not_p12, not_p3, not_p24, not_p5}
{p18, p1, p12, p24, p17, p4, p25, not_p12, not_p25, not_p24, not_p5}
{p18, p1, p12, p24, p17, p4, p25, not_p12, not_p3, not_p2, not_p5}
{p18, p1, p12, p24, p17, p4, p25, not_p12, not_p25, not_p2, not_p5}
{p18, p1, p12, p9, p24, not_p21, p17, p4, p25, not_p5}
{p18, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p2, not_p5}
{p20, p1, p14, p24, p21, p17, p4, p25, not_p12, not_p5}
{p20, p1, p24, p21, p17, p4, p25, not_p12, not_p3, not_p5}
{p20, p1, p24, p21, p17, p4, p25, not_p12, not_p25, not_p5}
{p20, p1, not_p15, p9, p24, p21, p17, p4, p25, not_p12, not_p5}
{p20, p1, p14, p24, not_p21, p21, p17, p4, p25, not_p5}
{p20, p1, p24, not_p21, p21, p17, p4, p25, not_p3, not_p5}
{p20, p1, p24, not_p21, p21, p17, p4, p25, not_p25, not_p5}
{p20, p1, not_p15, p9, p24, not_p21, p21, p17, p4, p25, not_p5}
{p20, p1, p12, p9, p24, p17, p4, p25, not_p12, not_p5}
{p20, p1, p12, p14, p24, p17, p4, p25, not_p12, not_p24, not_p5}
{p20, p1, p12, p14, p24, p17, p4, p25, not_p12, not_p2, not_p5}
{p20, p1, p12, p24, p17, p4, p25, not_p12, not_p3, not_p24, not_p5}
{p20, p1, p12, p24, p17, p4, p25, not_p12, not_p25, not_p24, not_p5}
{p20, p1, p12, p24, p17, p4, p25, not_p12, not_p3, not_p2, not_p5}
{p20, p1, p12, p24, p17, p4, p25, not_p12, not_p25, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p2, not_p5}
{p20, p1, p12, p9, p24, not_p21, p17, p4, p25, not_p5}
{p20, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p2, not_p5}
{p3, p1, p14, p24, not_p21, p21, p17, p4, p25, not_p5}
{p3, p1, p24, not_p21, p21, p17, p4, p25, not_p3, not_p5}
{p3, p1, p24, not_p21, p21, p17, p4, p25, not_p25, not_p5}
{p3, p1, not_p15, p9, p24, not_p21, p21, p17, p4, p25, not_p5}
{p3, p1, p12, p9, p24, not_p21, p17, p4, p25, not_p5}
{p3, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, p17, p4, p25, not_p2, not_p5}
{p3, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p17, p4, p25, not_p3, not_p2, not_p5}
{p3, p1, p12, p24, not_p21, p17, p4, p25, not_p25, not_p2, not_p5}
{p20, p1, not_p15, p9, p11, p24, p21, p17, p4, not_p12, not_p5}
{p20, p1, not_p15, p9, p11, p24, not_p21, p21, p17, p4, not_p5}
{p3, p1, not_p15, p9, p11, p24, not_p21, p21, p17, p4, not_p5}
{p20, p1, p12, p9, p11, p24, p17, p4, not_p12, not_p5}
{p20, p1, p12, p9, p11, p24, not_p21, p17, p4, not_p5}
{p3, p1, p12, p9, p11, p24, not_p21, p17, p4, not_p5}
{p1, p22, p24, p4, p25, not_p3, not_p8, not_p5}
{p1, p22, p12, p24, p4, not_p3, not_p8, not_p24, not_p5}
{p1, p22, p12, p24, p4, not_p3, not_p8, not_p2, not_p5}
{p1, p22, p24, p4, p25, not_p25, not_p8, not_p5}
{p1, p22, p12, p24, p4, not_p25, not_p8, not_p24, not_p5}
{p1, p22, p12, p24, p4, not_p25, not_p8, not_p2, not_p5}
{p1, p22, not_p15, p9, p24, p4, p8, not_p8, not_p5}
{p1, p22, p14, p24, not_p20, p4, p25, not_p8, not_p5}
{p1, p22, p14, p24, p21, p4, p25, not_p8, not_p5}
{p18, p1, p22, p9, p24, p4, p25, not_p8, not_p5}
{p1, p22, not_p15, p9, p24, p4, p25, not_p8, not_p5}
{p1, p22, p12, p14, p24, p4, not_p8, not_p24, not_p5}
{p1, p22, p12, p14, p24, p4, not_p8, not_p2, not_p5}
{p1, p22, p12, p9, p24, p4, not_p8, not_p5}
{p18, p1, p24, p4, p25, not_p12, not_p3, not_p8, not_p5}
{p18, p1, p24, not_p21, p4, p25, not_p3, not_p8, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p3, not_p8, not_p24, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p3, not_p8, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p2, not_p5}
{p20, p1, p24, p4, p25, not_p12, not_p3, not_p8, not_p5}
{p20, p1, p24, not_p21, p4, p25, not_p3, not_p8, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p3, not_p8, not_p24, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p3, not_p8, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p2, not_p5}
{p3, p1, p24, not_p21, p4, p25, not_p3, not_p8, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p3, not_p8, not_p2, not_p5}
{p18, p1, p24, p4, p25, not_p12, not_p25, not_p8, not_p5}
{p18, p1, p24, not_p21, p4, p25, not_p25, not_p8, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p25, not_p8, not_p24, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p25, not_p8, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p2, not_p5}
{p20, p1, p24, p4, p25, not_p12, not_p25, not_p8, not_p5}
{p20, p1, p24, not_p21, p4, p25, not_p25, not_p8, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p25, not_p8, not_p24, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p25, not_p8, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p2, not_p5}
{p3, p1, p24, not_p21, p4, p25, not_p25, not_p8, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p25, not_p8, not_p2, not_p5}
{p20, p1, not_p15, p9, p24, p4, not_p12, p8, not_p8, not_p5}
{p20, p1, not_p15, p9, p24, not_p21, p4, p8, not_p8, not_p5}
{p3, p1, not_p15, p9, p24, not_p21, p4, p8, not_p8, not_p5}
{p18, p1, p9, p24, p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p9, p24, not_p21, p4, p25, not_p8, not_p5}
{p18, p1, p14, p24, not_p20, p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p14, p24, p21, p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p14, p24, not_p21, not_p20, p4, p25, not_p8, not_p5}
{p18, p1, p14, p24, not_p21, p21, p4, p25, not_p8, not_p5}
{p20, p1, p14, p24, not_p20, p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p14, p24, not_p21, not_p20, p4, p25, not_p8, not_p5}
{p20, p1, p14, p24, p21, p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p14, p24, not_p21, p21, p4, p25, not_p8, not_p5}
{p20, p1, not_p15, p9, p24, p4, p25, not_p12, not_p8, not_p5}
{p20, p1, not_p15, p9, p24, not_p21, p4, p25, not_p8, not_p5}
{p3, p1, p14, p24, not_p21, not_p20, p4, p25, not_p8, not_p5}
{p3, p1, p14, p24, not_p21, p21, p4, p25, not_p8, not_p5}
{p3, p1, not_p15, p9, p24, not_p21, p4, p25, not_p8, not_p5}
{p18, p1, p12, p9, p24, p4, not_p12, not_p8, not_p5}
{p18, p1, p12, p9, p24, not_p21, p4, not_p8, not_p5}
{p20, p1, p12, p9, p24, p4, not_p12, not_p8, not_p5}
{p20, p1, p12, p9, p24, not_p21, p4, not_p8, not_p5}
{p3, p1, p12, p9, p24, not_p21, p4, not_p8, not_p5}
{p18, p1, p12, p14, p24, p4, not_p12, not_p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, p4, not_p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, p4, not_p12, not_p8, not_p2, not_p5}
{p18, p1, p12, p14, p24, not_p21, p4, not_p8, not_p2, not_p5}
{p20, p1, p12, p14, p24, p4, not_p12, not_p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, p4, not_p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, p4, not_p12, not_p8, not_p2, not_p5}
{p20, p1, p12, p14, p24, not_p21, p4, not_p8, not_p2, not_p5}
{p3, p1, p12, p14, p24, not_p21, p4, not_p8, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, p4, not_p8, not_p2, not_p5}
{p1, p22, not_p15, p9, p24, p4, p8, not_p19, not_p5}
{p20, p1, not_p15, p9, p24, p4, not_p12, p8, not_p19, not_p5}
{p20, p1, not_p15, p9, p24, not_p21, p4, p8, not_p19, not_p5}
{p3, p1, not_p15, p9, p24, not_p21, p4, p8, not_p19, not_p5}
{p6, p1, p22, p12, p9, p24, p4, not_p5}
{p6, p1, p22, p12, p14, p24, p4, not_p24, not_p5}
{p6, p1, p22, p12, p14, p24, p4, not_p2, not_p5}
{p6, p1, p22, p12, p24, p4, not_p3, not_p24, not_p5}
{p6, p1, p22, p12, p24, p4, not_p25, not_p24, not_p5}
{p6, p1, p22, p12, p24, p4, not_p3, not_p2, not_p5}
{p6, p1, p22, p12, p24, p4, not_p25, not_p2, not_p5}
{p1, p22, p12, p9, p24, p4, not_p19, not_p5}
{p1, p22, p12, p14, p24, p4, not_p19, not_p24, not_p5}
{p1, p22, p12, p14, p24, p4, not_p19, not_p2, not_p5}
{p1, p22, p12, p24, p4, not_p3, not_p19, not_p24, not_p5}
{p1, p22, p12, p24, p4, not_p25, not_p19, not_p24, not_p5}
{p1, p22, p12, p24, p4, not_p3, not_p19, not_p2, not_p5}
{p1, p22, p12, p24, p4, not_p25, not_p19, not_p2, not_p5}
{p6, p18, p1, p12, p9, p24, p4, not_p12, not_p5}
{p6, p18, p1, p12, p9, p24, not_p21, p4, not_p5}
{p6, p18, p1, p12, p14, p24, p4, not_p12, not_p24, not_p5}
{p6, p18, p1, p12, p24, p4, not_p12, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p24, p4, not_p12, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p14, p24, p4, not_p12, not_p2, not_p5}
{p6, p18, p1, p12, p24, p4, not_p12, not_p3, not_p2, not_p5}
{p6, p18, p1, p12, p24, p4, not_p12, not_p25, not_p2, not_p5}
{p6, p18, p1, p12, p14, p24, not_p21, p4, not_p24, not_p5}
{p6, p18, p1, p12, p14, p24, not_p21, p4, not_p2, not_p5}
{p6, p18, p1, p12, p24, not_p21, p4, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p21, p4, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p21, p4, not_p3, not_p2, not_p5}
{p6, p18, p1, p12, p24, not_p21, p4, not_p25, not_p2, not_p5}
{p18, p1, p12, p9, p24, p4, not_p12, not_p19, not_p5}
{p18, p1, p12, p9, p24, not_p21, p4, not_p19, not_p5}
{p18, p1, p12, p14, p24, p4, not_p12, not_p19, not_p24, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p3, not_p19, not_p24, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p25, not_p19, not_p24, not_p5}
{p18, p1, p12, p14, p24, p4, not_p12, not_p19, not_p2, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p3, not_p19, not_p2, not_p5}
{p18, p1, p12, p24, p4, not_p12, not_p25, not_p19, not_p2, not_p5}
{p18, p1, p12, p14, p24, not_p21, p4, not_p19, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, p4, not_p19, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p2, not_p5}
{p18, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p2, not_p5}
{p6, p20, p1, p12, p9, p24, p4, not_p12, not_p5}
{p6, p20, p1, p12, p9, p24, not_p21, p4, not_p5}
{p6, p20, p1, p12, p14, p24, p4, not_p12, not_p24, not_p5}
{p6, p20, p1, p12, p24, p4, not_p12, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p24, p4, not_p12, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p14, p24, p4, not_p12, not_p2, not_p5}
{p6, p20, p1, p12, p24, p4, not_p12, not_p3, not_p2, not_p5}
{p6, p20, p1, p12, p24, p4, not_p12, not_p25, not_p2, not_p5}
{p6, p20, p1, p12, p14, p24, not_p21, p4, not_p24, not_p5}
{p6, p20, p1, p12, p14, p24, not_p21, p4, not_p2, not_p5}
{p6, p20, p1, p12, p24, not_p21, p4, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p21, p4, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p21, p4, not_p3, not_p2, not_p5}
{p6, p20, p1, p12, p24, not_p21, p4, not_p25, not_p2, not_p5}
{p20, p1, p12, p9, p24, p4, not_p12, not_p19, not_p5}
{p20, p1, p12, p9, p24, not_p21, p4, not_p19, not_p5}
{p20, p1, p12, p14, p24, p4, not_p12, not_p19, not_p24, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p3, not_p19, not_p24, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p25, not_p19, not_p24, not_p5}
{p20, p1, p12, p14, p24, p4, not_p12, not_p19, not_p2, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p3, not_p19, not_p2, not_p5}
{p20, p1, p12, p24, p4, not_p12, not_p25, not_p19, not_p2, not_p5}
{p20, p1, p12, p14, p24, not_p21, p4, not_p19, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, p4, not_p19, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p2, not_p5}
{p20, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p2, not_p5}
{p6, p3, p1, p12, p9, p24, not_p21, p4, not_p5}
{p6, p3, p1, p12, p14, p24, not_p21, p4, not_p24, not_p5}
{p6, p3, p1, p12, p14, p24, not_p21, p4, not_p2, not_p5}
{p6, p3, p1, p12, p24, not_p21, p4, not_p3, not_p24, not_p5}
{p6, p3, p1, p12, p24, not_p21, p4, not_p25, not_p24, not_p5}
{p6, p3, p1, p12, p24, not_p21, p4, not_p3, not_p2, not_p5}
{p6, p3, p1, p12, p24, not_p21, p4, not_p25, not_p2, not_p5}
{p3, p1, p12, p9, p24, not_p21, p4, not_p19, not_p5}
{p3, p1, p12, p14, p24, not_p21, p4, not_p19, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, p4, not_p19, not_p2, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p3, not_p19, not_p2, not_p5}
{p3, p1, p12, p24, not_p21, p4, not_p25, not_p19, not_p2, not_p5}
{p5, p22, p14, p24, p17, p4, not_p16}
{p5, p22, p12, p14, p24, p17, p4}
{p5, p22, not_p15, p14, p2, p24, p17, p4, not_p1}
{p5, p22, not_p15, p14, p2, p24, p17, not_p4, p4}
{p5, p1, p22, p14, p24, p17, p4}
{p5, p22, p24, p17, p4, not_p3, not_p16}
{p5, p22, p24, p17, p4, not_p25, not_p16}
{p5, p22, p9, p24, p17, p4, not_p16}
{p5, p22, p12, p24, p17, p4, not_p3}
{p5, p22, not_p15, p2, p24, p17, p4, not_p1, not_p3}
{p5, p22, not_p15, p2, p24, p17, not_p4, p4, not_p3}
{p5, p1, p22, p24, p17, p4, not_p3}
{p5, p22, p12, p24, p17, p4, not_p25}
{p5, p22, not_p15, p2, p24, p17, p4, not_p1, not_p25}
{p5, p22, not_p15, p2, p24, p17, not_p4, p4, not_p25}
{p5, p1, p22, p24, p17, p4, not_p25}
{p5, p22, p12, p9, p24, p17, p4}
{p5, p1, p22, not_p15, p9, p24, p17, p4}
{p5, p18, p1, p22, p9, p24, p17, p4}
{p5, p22, not_p15, p9, p2, p24, p17, p4, not_p1}
{p5, p22, not_p15, p9, p2, p24, p17, not_p4, p4}
{p5, p14, p24, p17, p4, p25, not_p12, not_p16, not_p5}
{p5, p24, p17, p4, p25, not_p12, not_p3, not_p16, not_p5}
{p5, p24, p17, p4, p25, not_p12, not_p25, not_p16, not_p5}
{p5, p9, p24, p17, p4, p25, not_p12, not_p16, not_p5}
{p5, p14, p24, not_p21, p17, p4, p25, not_p16, not_p5}
{p5, p24, not_p21, p17, p4, p25, not_p3, not_p16, not_p5}
{p5, p24, not_p21, p17, p4, p25, not_p25, not_p16, not_p5}
{p5, p9, p24, not_p21, p17, p4, p25, not_p16, not_p5}
{p5, p14, p24, p21, p17, p4, not_p12, not_p16, not_p5}
{p5, p24, p21, p17, p4, not_p12, not_p3, not_p16, not_p5}
{p5, p24, p21, p17, p4, not_p12, not_p25, not_p16, not_p5}
{p5, p9, p24, p21, p17, p4, not_p12, not_p16, not_p5}
{p5, p24, not_p21, p21, p17, p4, not_p3, not_p16, not_p5}
{p5, p14, p24, not_p21, p21, p17, p4, not_p16, not_p5}
{p5, p24, not_p21, p21, p17, p4, not_p25, not_p16, not_p5}
{p5, p9, p24, not_p21, p21, p17, p4, not_p16, not_p5}
{p5, p14, p24, p17, p4, p25, not_p12, not_p19, not_p16}
{p5, p24, p17, p4, p25, not_p12, not_p3, not_p19, not_p16}
{p5, p24, p17, p4, p25, not_p12, not_p25, not_p19, not_p16}
{p5, p9, p24, p17, p4, p25, not_p12, not_p19, not_p16}
{p5, p14, p24, not_p21, p17, p4, p25, not_p19, not_p16}
{p5, p24, not_p21, p17, p4, p25, not_p3, not_p19, not_p16}
{p5, p24, not_p21, p17, p4, p25, not_p25, not_p19, not_p16}
{p5, p9, p24, not_p21, p17, p4, p25, not_p19, not_p16}
{p5, p14, p24, p21, p17, p4, not_p12, not_p19, not_p16}
{p5, p24, p21, p17, p4, not_p12, not_p3, not_p19, not_p16}
{p5, p24, p21, p17, p4, not_p12, not_p25, not_p19, not_p16}
{p5, p9, p24, p21, p17, p4, not_p12, not_p19, not_p16}
{p5, p14, p24, not_p21, p21, p17, p4, not_p19, not_p16}
{p5, p24, not_p21, p21, p17, p4, not_p3, not_p19, not_p16}
{p5, p24, not_p21, p21, p17, p4, not_p25, not_p19, not_p16}
{p5, p9, p24, not_p21, p21, p17, p4, not_p19, not_p16}
{p5, p12, p24, p21, p17, p4, not_p12, not_p3, not_p5}
{p5, not_p15, p2, p24, p17, p4, not_p1, p25, not_p12, not_p3, not_p5}
{p5, not_p15, p2, p24, p21, p17, p4, not_p1, not_p12, not_p3, not_p5}
{p5, not_p15, p2, p24, p17, not_p4, p4, p25, not_p12, not_p3, not_p5}
{p5, not_p15, p2, p24, p21, p17, not_p4, p4, not_p12, not_p3, not_p5}
{p5, p18, p1, p24, p17, p4, p25, not_p12, not_p3, not_p5}
{p5, p20, p1, p24, p17, p4, p25, not_p12, not_p3, not_p5}
{p5, p18, p1, p24, p21, p17, p4, not_p12, not_p3, not_p5}
{p5, p20, p1, p24, p21, p17, p4, not_p12, not_p3, not_p5}
{p5, p12, p24, not_p21, p21, p17, p4, not_p3, not_p5}
{p5, not_p15, p2, p24, not_p21, p17, p4, not_p1, p25, not_p3, not_p5}
{p5, not_p15, p2, p24, not_p21, p21, p17, p4, not_p1, not_p3, not_p5}
{p5, not_p15, p2, p24, not_p21, p17, not_p4, p4, p25, not_p3, not_p5}
{p5, not_p15, p2, p24, not_p21, p21, p17, not_p4, p4, not_p3, not_p5}
{p5, p18, p1, p24, not_p21, p17, p4, p25, not_p3, not_p5}
{p5, p18, p1, p24, not_p21, p21, p17, p4, not_p3, not_p5}
{p5, p20, p1, p24, not_p21, p17, p4, p25, not_p3, not_p5}
{p5, p20, p1, p24, not_p21, p21, p17, p4, not_p3, not_p5}
{p5, not_p15, p2, p24, p17, p4, not_p1, p25, not_p12, not_p3, not_p19}
{p5, not_p15, p2, p24, p21, p17, p4, not_p1, not_p12, not_p3, not_p19}
{p5, not_p15, p2, p24, p17, not_p4, p4, p25, not_p12, not_p3, not_p19}
{p5, not_p15, p2, p24, p21, p17, not_p4, p4, not_p12, not_p3, not_p19}
{p5, p18, p1, p24, p17, p4, p25, not_p12, not_p3, not_p19}
{p5, p20, p1, p24, p17, p4, p25, not_p12, not_p3, not_p19}
{p5, p18, p1, p24, p21, p17, p4, not_p12, not_p3, not_p19}
{p5, p20, p1, p24, p21, p17, p4, not_p12, not_p3, not_p19}
{p5, not_p15, p2, p24, not_p21, p17, p4, not_p1, p25, not_p3, not_p19}
{p5, not_p15, p2, p24, not_p21, p21, p17, p4, not_p1, not_p3, not_p19}
{p5, not_p15, p2, p24, not_p21, p17, not_p4, p4, p25, not_p3, not_p19}
{p5, not_p15, p2, p24, not_p21, p21, p17, not_p4, p4, not_p3, not_p19}
{p5, p18, p1, p24, not_p21, p17, p4, p25, not_p3, not_p19}
{p5, p20, p1, p24, not_p21, p17, p4, p25, not_p3, not_p19}
{p5, p18, p1, p24, not_p21, p21, p17, p4, not_p3, not_p19}
{p5, p20, p1, p24, not_p21, p21, p17, p4, not_p3, not_p19}
{p5, p12, p24, p21, p17, p4, not_p12, not_p25, not_p5}
{p5, not_p15, p2, p24, p17, p4, not_p1, p25, not_p12, not_p25, not_p5}
{p5, not_p15, p2, p24, p21, p17, p4, not_p1, not_p12, not_p25, not_p5}
{p5, not_p15, p2, p24, p17, not_p4, p4, p25, not_p12, not_p25, not_p5}
{p5, not_p15, p2, p24, p21, p17, not_p4, p4, not_p12, not_p25, not_p5}
{p5, p18, p1, p24, p17, p4, p25, not_p12, not_p25, not_p5}
{p5, p20, p1, p24, p17, p4, p25, not_p12, not_p25, not_p5}
{p5, p18, p1, p24, p21, p17, p4, not_p12, not_p25, not_p5}
{p5, p20, p1, p24, p21, p17, p4, not_p12, not_p25, not_p5}
{p5, p12, p24, not_p21, p21, p17, p4, not_p25, not_p5}
{p5, not_p15, p2, p24, not_p21, p17, p4, not_p1, p25, not_p25, not_p5}
{p5, not_p15, p2, p24, not_p21, p21, p17, p4, not_p1, not_p25, not_p5}
{p5, not_p15, p2, p24, not_p21, p17, not_p4, p4, p25, not_p25, not_p5}
{p5, not_p15, p2, p24, not_p21, p21, p17, not_p4, p4, not_p25, not_p5}
{p5, p18, p1, p24, not_p21, p17, p4, p25, not_p25, not_p5}
{p5, p18, p1, p24, not_p21, p21, p17, p4, not_p25, not_p5}
{p5, p20, p1, p24, not_p21, p17, p4, p25, not_p25, not_p5}
{p5, p20, p1, p24, not_p21, p21, p17, p4, not_p25, not_p5}
{p5, not_p15, p2, p24, p17, p4, not_p1, p25, not_p12, not_p25, not_p19}
{p5, not_p15, p2, p24, p21, p17, p4, not_p1, not_p12, not_p25, not_p19}
{p5, not_p15, p2, p24, p17, not_p4, p4, p25, not_p12, not_p25, not_p19}
{p5, not_p15, p2, p24, p21, p17, not_p4, p4, not_p12, not_p25, not_p19}
{p5, p18, p1, p24, p17, p4, p25, not_p12, not_p25, not_p19}
{p5, p20, p1, p24, p17, p4, p25, not_p12, not_p25, not_p19}
{p5, p18, p1, p24, p21, p17, p4, not_p12, not_p25, not_p19}
{p5, p20, p1, p24, p21, p17, p4, not_p12, not_p25, not_p19}
{p5, not_p15, p2, p24, not_p21, p17, p4, not_p1, p25, not_p25, not_p19}
{p5, not_p15, p2, p24, not_p21, p21, p17, p4, not_p1, not_p25, not_p19}
{p5, not_p15, p2, p24, not_p21, p17, not_p4, p4, p25, not_p25, not_p19}
{p5, not_p15, p2, p24, not_p21, p21, p17, not_p4, p4, not_p25, not_p19}
{p5, p18, p1, p24, not_p21, p17, p4, p25, not_p25, not_p19}
{p5, p20, p1, p24, not_p21, p17, p4, p25, not_p25, not_p19}
{p5, p18, p1, p24, not_p21, p21, p17, p4, not_p25, not_p19}
{p5, p20, p1, p24, not_p21, p21, p17, p4, not_p25, not_p19}
{p5, p12, p14, p24, p21, p17, p4, not_p12, not_p5}
{p5, p12, p14, p24, not_p21, p21, p17, p4, not_p5}
{p5, not_p15, p14, p2, p24, p17, p4, not_p1, p25, not_p12, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, p17, p4, not_p1, p25, not_p5}
{p5, not_p15, p14, p2, p24, p17, not_p4, p4, p25, not_p12, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, p17, not_p4, p4, p25, not_p5}
{p5, not_p15, p14, p2, p24, p17, p4, not_p1, p25, not_p12, not_p19}
{p5, not_p15, p14, p2, p24, not_p21, p17, p4, not_p1, p25, not_p19}
{p5, not_p15, p14, p2, p24, p17, not_p4, p4, p25, not_p12, not_p19}
{p5, not_p15, p14, p2, p24, not_p21, p17, not_p4, p4, p25, not_p19}
{p5, p18, p1, p14, p24, p17, p4, p25, not_p12, not_p5}
{p5, p18, p1, p14, p24, not_p21, p17, p4, p25, not_p5}
{p5, p20, p1, p14, p24, not_p21, p17, p4, p25, not_p5}
{p5, p20, p1, p14, p24, p17, p4, p25, not_p12, not_p5}
{p5, p18, p1, p14, p24, p17, p4, p25, not_p12, not_p19}
{p5, p20, p1, p14, p24, p17, p4, p25, not_p12, not_p19}
{p5, p18, p1, p14, p24, not_p21, p17, p4, p25, not_p19}
{p5, p20, p1, p14, p24, not_p21, p17, p4, p25, not_p19}
{p5, not_p15, p14, p2, p24, p21, p17, p4, not_p1, not_p12, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, p21, p17, p4, not_p1, not_p5}
{p5, not_p15, p14, p2, p24, p21, p17, not_p4, p4, not_p12, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, p21, p17, not_p4, p4, not_p5}
{p5, p18, p1, p14, p24, p21, p17, p4, not_p12, not_p5}
{p5, p20, p1, p14, p24, p21, p17, p4, not_p12, not_p5}
{p5, p18, p1, p14, p24, not_p21, p21, p17, p4, not_p5}
{p5, p20, p1, p14, p24, not_p21, p21, p17, p4, not_p5}
{p5, not_p15, p14, p2, p24, p21, p17, p4, not_p1, not_p12, not_p19}
{p5, not_p15, p14, p2, p24, not_p21, p21, p17, p4, not_p1, not_p19}
{p5, not_p15, p14, p2, p24, p21, p17, not_p4, p4, not_p12, not_p19}
{p5, not_p15, p14, p2, p24, not_p21, p21, p17, not_p4, p4, not_p19}
{p5, p18, p1, p14, p24, p21, p17, p4, not_p12, not_p19}
{p5, p20, p1, p14, p24, p21, p17, p4, not_p12, not_p19}
{p5, p18, p1, p14, p24, not_p21, p21, p17, p4, not_p19}
{p5, p20, p1, p14, p24, not_p21, p21, p17, p4, not_p19}
{p5, p12, p9, p24, p21, p17, p4, not_p12, not_p5}
{p5, p20, p1, not_p15, p9, p24, p17, p4, p25, not_p12, not_p5}
{p5, p20, p1, not_p15, p9, p24, p21, p17, p4, not_p12, not_p5}
{p5, p20, p1, not_p15, p9, p11, p24, p17, p4, not_p12, not_p5}
{p5, p18, p1, p9, p24, p17, p4, p25, not_p12, not_p5}
{p5, p18, p1, p9, p24, p21, p17, p4, not_p12, not_p5}
{p5, not_p15, p9, p2, p24, p17, p4, not_p1, p25, not_p12, not_p5}
{p5, not_p15, p9, p2, p24, p17, not_p4, p4, p25, not_p12, not_p5}
{p5, not_p15, p9, p2, p24, p21, p17, p4, not_p1, not_p12, not_p5}
{p5, not_p15, p9, p2, p24, p21, p17, not_p4, p4, not_p12, not_p5}
{p5, p12, p9, p24, not_p21, p21, p17, p4, not_p5}
{p5, p20, p1, not_p15, p9, p24, not_p21, p17, p4, p25, not_p5}
{p5, p20, p1, not_p15, p9, p24, not_p21, p21, p17, p4, not_p5}
{p5, p20, p1, not_p15, p9, p11, p24, not_p21, p17, p4, not_p5}
{p5, p18, p1, p9, p24, not_p21, p17, p4, p25, not_p5}
{p5, p18, p1, p9, p24, not_p21, p21, p17, p4, not_p5}
{p5, not_p15, p9, p2, p24, not_p21, p17, p4, not_p1, p25, not_p5}
{p5, not_p15, p9, p2, p24, not_p21, p21, p17, p4, not_p1, not_p5}
{p5, not_p15, p9, p2, p24, not_p21, p17, not_p4, p4, p25, not_p5}
{p5, not_p15, p9, p2, p24, not_p21, p21, p17, not_p4, p4, not_p5}
{p5, p20, p1, not_p15, p9, p24, p17, p4, p25, not_p12, not_p19}
{p5, p20, p1, not_p15, p9, p24, p21, p17, p4, not_p12, not_p19}
{p5, p20, p1, not_p15, p9, p11, p24, p17, p4, not_p12, not_p19}
{p5, p18, p1, p9, p24, p17, p4, p25, not_p12, not_p19}
{p5, p18, p1, p9, p24, p21, p17, p4, not_p12, not_p19}
{p5, not_p15, p9, p2, p24, p17, p4, not_p1, p25, not_p12, not_p19}
{p5, not_p15, p9, p2, p24, p17, not_p4, p4, p25, not_p12, not_p19}
{p5, not_p15, p9, p2, p24, p21, p17, p4, not_p1, not_p12, not_p19}
{p5, not_p15, p9, p2, p24, p21, p17, not_p4, p4, not_p12, not_p19}
{p5, p20, p1, not_p15, p9, p24, not_p21, p17, p4, p25, not_p19}
{p5, p20, p1, not_p15, p9, p24, not_p21, p21, p17, p4, not_p19}
{p5, p20, p1, not_p15, p9, p11, p24, not_p21, p17, p4, not_p19}
{p5, p18, p1, p9, p24, not_p21, p17, p4, p25, not_p19}
{p5, p18, p1, p9, p24, not_p21, p21, p17, p4, not_p19}
{p5, not_p15, p9, p2, p24, not_p21, p17, p4, not_p1, p25, not_p19}
{p5, not_p15, p9, p2, p24, not_p21, p17, not_p4, p4, p25, not_p19}
{p5, not_p15, p9, p2, p24, not_p21, p21, p17, p4, not_p1, not_p19}
{p5, not_p15, p9, p2, p24, not_p21, p21, p17, not_p4, p4, not_p19}
{p5, p22, p14, p24, p4, p25, not_p8, not_p16}
{p5, p22, not_p15, p14, p2, p24, p4, not_p1, p25, not_p8}
{p5, p22, not_p15, p14, p2, p24, not_p4, p4, p25, not_p8}
{p5, p1, p22, p14, p24, p4, p25, not_p8}
{p5, p14, p24, p4, p25, not_p12, not_p8, not_p16, not_p5}
{p5, p14, p24, not_p21, p4, p25, not_p8, not_p16, not_p5}
{p5, p14, p24, p4, p25, not_p12, not_p19, not_p8, not_p16}
{p5, p14, p24, not_p21, p4, p25, not_p19, not_p8, not_p16}
{p5, not_p15, p14, p2, p24, p4, not_p1, p25, not_p12, not_p8, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, p4, not_p1, p25, not_p8, not_p5}
{p5, not_p15, p14, p2, p24, not_p4, p4, p25, not_p12, not_p8, not_p5}
{p5, not_p15, p14, p2, p24, not_p21, not_p4, p4, p25, not_p8, not_p5}
{p5, not_p15, p14, p2, p24, p4, not_p1, p25, not_p12, not_p19, not_p8}
{p5, not_p15, p14, p2, p24, not_p21, p4, not_p1, p25, not_p19, not_p8}
{p5, not_p15, p14, p2, p24, not_p4, p4, p25, not_p12, not_p19, not_p8}
{p5, not_p15, p14, p2, p24, not_p21, not_p4, p4, p25, not_p19, not_p8}
{p5, p18, p1, p14, p24, p4, p25, not_p12, not_p8, not_p5}
{p5, p18, p1, p14, p24, not_p21, p4, p25, not_p8, not_p5}
{p5, p20, p1, p14, p24, not_p21, p4, p25, not_p8, not_p5}
{p5, p20, p1, p14, p24, p4, p25, not_p12, not_p8, not_p5}
{p5, p18, p1, p14, p24, p4, p25, not_p12, not_p19, not_p8}
{p5, p18, p1, p14, p24, not_p21, p4, p25, not_p19, not_p8}
{p5, p20, p1, p14, p24, not_p21, p4, p25, not_p19, not_p8}
{p5, p20, p1, p14, p24, p4, p25, not_p12, not_p19, not_p8}
{p1, p22, p24, not_p4, p8, not_p3, not_p8, not_p5}
{p1, p22, p24, not_p4, p8, not_p25, not_p8, not_p5}
{p1, p22, p14, p24, not_p20, not_p4, p8, not_p8, not_p5}
{p1, p22, p14, p24, p21, not_p4, p8, not_p8, not_p5}
{p1, p22, p12, p14, p24, not_p4, p8, not_p8, not_p24, not_p5}
{p1, p22, p12, p14, p24, not_p4, p8, not_p8, not_p2, not_p5}
{p18, p1, p22, p9, p24, not_p4, p8, not_p8, not_p5}
{p7, p1, p22, not_p15, p9, p24, not_p4, p8, not_p8, not_p5}
{p7, p1, p22, p12, p9, p24, not_p4, p8, not_p8, not_p5}
{p1, p22, p24, not_p4, p8, not_p3, not_p19, not_p5}
{p1, p22, p24, not_p4, p8, not_p25, not_p19, not_p5}
{p1, p22, p14, p24, not_p20, not_p4, p8, not_p19, not_p5}
{p1, p22, p14, p24, p21, not_p4, p8, not_p19, not_p5}
{p1, p22, p12, p14, p24, not_p4, p8, not_p19, not_p24, not_p5}
{p1, p22, p12, p14, p24, not_p4, p8, not_p19, not_p2, not_p5}
{p18, p1, p22, p9, p24, not_p4, p8, not_p19, not_p5}
{p7, p1, p22, not_p15, p9, p24, not_p4, p8, not_p19, not_p5}
{p7, p1, p22, p12, p9, p24, not_p4, p8, not_p19, not_p5}
{p6, p1, p22, not_p15, p24, not_p4, p8, not_p3, not_p5}
{p6, p1, p22, p12, p24, not_p4, p8, not_p3, not_p24, not_p5}
{p6, p1, p22, p12, p24, not_p4, p8, not_p3, not_p2, not_p5}
{p1, p22, p24, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p1, p22, p24, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p1, p22, p12, p24, p17, not_p4, p25, p8, not_p3, not_p24, not_p5}
{p1, p22, p12, p24, p17, not_p4, p25, p8, not_p3, not_p2, not_p5}
{p6, p1, p22, not_p15, p24, not_p4, p8, not_p25, not_p5}
{p6, p1, p22, p12, p24, not_p4, p8, not_p25, not_p24, not_p5}
{p6, p1, p22, p12, p24, not_p4, p8, not_p25, not_p2, not_p5}
{p1, p22, p24, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p1, p22, p24, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p1, p22, p12, p24, p17, not_p4, p25, p8, not_p25, not_p24, not_p5}
{p1, p22, p12, p24, p17, not_p4, p25, p8, not_p25, not_p2, not_p5}
{p6, p1, p22, p12, p14, p24, not_p4, p8, not_p24, not_p5}
{p6, p1, p22, p12, p14, p24, not_p4, p8, not_p2, not_p5}
{p6, p1, p22, not_p15, p14, p24, not_p20, not_p4, p8, not_p5}
{p6, p1, p22, not_p15, p14, p24, p21, not_p4, p8, not_p5}
{p1, p22, p14, p24, not_p20, p17, not_p4, p8, not_p5}
{p1, p22, p14, p24, p21, p17, not_p4, p25, p8, not_p5}
{p1, p22, p12, p14, p24, p17, not_p4, p25, p8, not_p24, not_p5}
{p1, p22, p12, p14, p24, p17, not_p4, p25, p8, not_p2, not_p5}
{p6, p1, p22, not_p15, p9, p24, not_p4, p8, not_p5}
{p6, p1, p22, p12, p9, p24, not_p4, p8, not_p5}
{p18, p1, p22, p9, p24, not_p20, p17, not_p4, p8, not_p5}
{p18, p1, p22, p9, p24, p21, p17, not_p4, p25, p8, not_p5}
{p18, p1, p22, p12, p9, p24, p17, not_p4, p25, p8, not_p5}
{p7, p1, p22, not_p15, p9, p24, not_p20, p17, not_p4, p8, not_p5}
{p7, p1, p22, not_p15, p9, p24, p21, p17, not_p4, p8, not_p5}
{p7, p1, p22, p12, p9, p24, p17, not_p4, p8, not_p5}
{p18, p1, p9, p24, not_p4, not_p12, p8, not_p8, not_p5}
{p18, p1, p24, not_p4, not_p12, p8, not_p3, not_p8, not_p5}
{p18, p1, p24, not_p4, not_p12, p8, not_p25, not_p8, not_p5}
{p18, p1, p14, p24, not_p20, not_p4, not_p12, p8, not_p8, not_p5}
{p18, p1, p14, p24, p21, not_p4, not_p12, p8, not_p8, not_p5}
{p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p8, not_p2, not_p5}
{p18, p1, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p18, p1, p24, not_p21, not_p4, p8, not_p3, not_p8, not_p5}
{p18, p1, p24, not_p21, not_p4, p8, not_p25, not_p8, not_p5}
{p18, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p8, not_p5}
{p18, p1, p14, p24, not_p21, p21, not_p4, p8, not_p8, not_p5}
{p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p2, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p4, not_p12, p8, not_p8, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p4, not_p12, p8, not_p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p20, p1, p24, not_p4, not_p12, p8, not_p3, not_p8, not_p5}
{p20, p1, p24, not_p4, not_p12, p8, not_p25, not_p8, not_p5}
{p20, p1, p14, p24, not_p20, not_p4, not_p12, p8, not_p8, not_p5}
{p20, p1, p14, p24, p21, not_p4, not_p12, p8, not_p8, not_p5}
{p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p8, not_p2, not_p5}
{p20, p1, p24, not_p21, not_p4, p8, not_p3, not_p8, not_p5}
{p20, p1, p24, not_p21, not_p4, p8, not_p25, not_p8, not_p5}
{p20, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p8, not_p5}
{p20, p1, p14, p24, not_p21, p21, not_p4, p8, not_p8, not_p5}
{p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p2, not_p5}
{p3, p7, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p3, p7, p1, p12, p9, p24, not_p21, not_p4, p8, not_p8, not_p5}
{p3, p1, p24, not_p21, not_p4, p8, not_p3, not_p8, not_p5}
{p3, p1, p24, not_p21, not_p4, p8, not_p25, not_p8, not_p5}
{p3, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p8, not_p5}
{p3, p1, p14, p24, not_p21, p21, not_p4, p8, not_p8, not_p5}
{p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p8, not_p2, not_p5}
{p18, p1, p9, p24, not_p4, not_p12, p8, not_p19, not_p5}
{p18, p1, p24, not_p4, not_p12, p8, not_p3, not_p19, not_p5}
{p18, p1, p24, not_p4, not_p12, p8, not_p25, not_p19, not_p5}
{p18, p1, p14, p24, not_p20, not_p4, not_p12, p8, not_p19, not_p5}
{p18, p1, p14, p24, p21, not_p4, not_p12, p8, not_p19, not_p5}
{p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p19, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p19, not_p2, not_p5}
{p18, p1, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p18, p1, p24, not_p21, not_p4, p8, not_p3, not_p19, not_p5}
{p18, p1, p24, not_p21, not_p4, p8, not_p25, not_p19, not_p5}
{p18, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p19, not_p5}
{p18, p1, p14, p24, not_p21, p21, not_p4, p8, not_p19, not_p5}
{p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p2, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p4, not_p12, p8, not_p19, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p7, p20, p1, p12, p9, p24, not_p4, not_p12, p8, not_p19, not_p5}
{p7, p20, p1, p12, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p20, p1, p24, not_p4, not_p12, p8, not_p3, not_p19, not_p5}
{p20, p1, p24, not_p4, not_p12, p8, not_p25, not_p19, not_p5}
{p20, p1, p14, p24, not_p20, not_p4, not_p12, p8, not_p19, not_p5}
{p20, p1, p14, p24, p21, not_p4, not_p12, p8, not_p19, not_p5}
{p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p19, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p19, not_p2, not_p5}
{p20, p1, p24, not_p21, not_p4, p8, not_p3, not_p19, not_p5}
{p20, p1, p24, not_p21, not_p4, p8, not_p25, not_p19, not_p5}
{p20, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p19, not_p5}
{p20, p1, p14, p24, not_p21, p21, not_p4, p8, not_p19, not_p5}
{p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p2, not_p5}
{p3, p7, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p3, p7, p1, p12, p9, p24, not_p21, not_p4, p8, not_p19, not_p5}
{p3, p1, p24, not_p21, not_p4, p8, not_p3, not_p19, not_p5}
{p3, p1, p24, not_p21, not_p4, p8, not_p25, not_p19, not_p5}
{p3, p1, p14, p24, not_p21, not_p20, not_p4, p8, not_p19, not_p5}
{p3, p1, p14, p24, not_p21, p21, not_p4, p8, not_p19, not_p5}
{p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p19, not_p2, not_p5}
{p6, p18, p1, not_p15, p24, not_p4, not_p12, p8, not_p3, not_p5}
{p6, p18, p1, not_p15, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p18, p1, p12, p24, not_p4, not_p12, p8, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p4, not_p12, p8, not_p3, not_p2, not_p5}
{p6, p18, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p2, not_p5}
{p18, p1, p24, not_p20, p17, not_p4, not_p12, p8, not_p3, not_p5}
{p18, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p24, not_p5}
{p18, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p2, not_p5}
{p18, p1, p24, p21, p17, not_p4, p25, not_p12, p8, not_p3, not_p5}
{p18, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p18, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p2, not_p5}
{p18, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p6, p20, p1, not_p15, p24, not_p4, not_p12, p8, not_p3, not_p5}
{p6, p20, p1, not_p15, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p20, p1, p12, p24, not_p4, not_p12, p8, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p4, not_p12, p8, not_p3, not_p2, not_p5}
{p6, p20, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p2, not_p5}
{p20, p1, p24, not_p20, p17, not_p4, not_p12, p8, not_p3, not_p5}
{p20, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p24, not_p5}
{p20, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p2, not_p5}
{p20, p1, p24, p21, p17, not_p4, p25, not_p12, p8, not_p3, not_p5}
{p20, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p20, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p2, not_p5}
{p20, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p6, p3, p1, not_p15, p24, not_p21, not_p4, p8, not_p3, not_p5}
{p6, p3, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p24, not_p5}
{p6, p3, p1, p12, p24, not_p21, not_p4, p8, not_p3, not_p2, not_p5}
{p3, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p3, not_p5}
{p3, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p2, not_p5}
{p3, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p6, p18, p1, not_p15, p24, not_p4, not_p12, p8, not_p25, not_p5}
{p6, p18, p1, not_p15, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p18, p1, p12, p24, not_p4, not_p12, p8, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p4, not_p12, p8, not_p25, not_p2, not_p5}
{p6, p18, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p24, not_p5}
{p6, p18, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p2, not_p5}
{p18, p1, p24, not_p20, p17, not_p4, not_p12, p8, not_p25, not_p5}
{p18, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p24, not_p5}
{p18, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p2, not_p5}
{p18, p1, p24, p21, p17, not_p4, p25, not_p12, p8, not_p25, not_p5}
{p18, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p18, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p24, not_p5}
{p18, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p2, not_p5}
{p18, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p6, p20, p1, not_p15, p24, not_p4, not_p12, p8, not_p25, not_p5}
{p6, p20, p1, not_p15, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p20, p1, p12, p24, not_p4, not_p12, p8, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p4, not_p12, p8, not_p25, not_p2, not_p5}
{p6, p20, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p24, not_p5}
{p6, p20, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p2, not_p5}
{p20, p1, p24, not_p20, p17, not_p4, not_p12, p8, not_p25, not_p5}
{p20, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p24, not_p5}
{p20, p1, p12, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p2, not_p5}
{p20, p1, p24, p21, p17, not_p4, p25, not_p12, p8, not_p25, not_p5}
{p20, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p20, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p24, not_p5}
{p20, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p2, not_p5}
{p20, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p6, p3, p1, not_p15, p24, not_p21, not_p4, p8, not_p25, not_p5}
{p6, p3, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p24, not_p5}
{p6, p3, p1, p12, p24, not_p21, not_p4, p8, not_p25, not_p2, not_p5}
{p3, p1, p24, not_p21, not_p20, p17, not_p4, p8, not_p25, not_p5}
{p3, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p24, not_p5}
{p3, p1, p12, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p2, not_p5}
{p3, p1, p24, not_p21, p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p6, p18, p1, not_p15, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p18, p1, p12, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, p12, p9, p24, not_p21, not_p4, p8, not_p5}
{p18, p1, p9, p24, not_p20, p17, not_p4, not_p12, p8, not_p5}
{p18, p1, p12, p9, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p18, p1, p9, p24, p21, p17, not_p4, p25, not_p12, p8, not_p5}
{p18, p1, p9, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p18, p1, p12, p9, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p18, p1, p9, p24, not_p21, p21, p17, not_p4, p25, p8, not_p5}
{p6, p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p24, not_p5}
{p6, p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p24, not_p5}
{p6, p18, p1, p12, p14, p24, not_p4, not_p12, p8, not_p2, not_p5}
{p6, p18, p1, p12, p14, p24, not_p21, not_p4, p8, not_p2, not_p5}
{p6, p18, p1, not_p15, p14, p24, not_p20, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, not_p15, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p18, p1, not_p15, p14, p24, p21, not_p4, not_p12, p8, not_p5}
{p6, p18, p1, not_p15, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p18, p1, p14, p24, not_p20, p17, not_p4, not_p12, p8, not_p5}
{p18, p1, p14, p24, p21, p17, not_p4, p25, not_p12, p8, not_p5}
{p18, p1, p12, p14, p24, p17, not_p4, p25, not_p12, p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, p17, not_p4, p25, not_p12, p8, not_p2, not_p5}
{p18, p1, p14, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p18, p1, p14, p24, not_p21, p21, p17, not_p4, p25, p8, not_p5}
{p18, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p24, not_p5}
{p18, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p2, not_p5}
{p6, p20, p1, not_p15, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p20, p1, p12, p9, p24, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, p12, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p20, p1, not_p15, p14, p24, not_p20, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, not_p15, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p20, p1, not_p15, p14, p24, p21, not_p4, not_p12, p8, not_p5}
{p6, p20, p1, not_p15, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p6, p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p24, not_p5}
{p6, p20, p1, p12, p14, p24, not_p4, not_p12, p8, not_p2, not_p5}
{p6, p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p24, not_p5}
{p6, p20, p1, p12, p14, p24, not_p21, not_p4, p8, not_p2, not_p5}
{p20, p1, p14, p24, not_p20, p17, not_p4, not_p12, p8, not_p5}
{p20, p1, p14, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p20, p1, p14, p24, p21, p17, not_p4, p25, not_p12, p8, not_p5}
{p20, p1, p14, p24, not_p21, p21, p17, not_p4, p25, p8, not_p5}
{p20, p1, p12, p14, p24, p17, not_p4, p25, not_p12, p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, p17, not_p4, p25, not_p12, p8, not_p2, not_p5}
{p20, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p24, not_p5}
{p20, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p2, not_p5}
{p7, p20, p1, p12, p9, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p20, p17, not_p4, not_p12, p8, not_p5}
{p7, p20, p1, p12, p9, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p20, p17, not_p4, not_p12, p8, not_p5}
{p7, p20, p1, not_p15, p9, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p7, p20, p1, p12, p9, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p7, p20, p1, not_p15, p9, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p6, p3, p1, not_p15, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p3, p1, p12, p9, p24, not_p21, not_p4, p8, not_p5}
{p6, p3, p1, not_p15, p14, p24, not_p21, not_p20, not_p4, p8, not_p5}
{p6, p3, p1, not_p15, p14, p24, not_p21, p21, not_p4, p8, not_p5}
{p6, p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p24, not_p5}
{p6, p3, p1, p12, p14, p24, not_p21, not_p4, p8, not_p2, not_p5}
{p3, p1, p14, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p3, p7, p1, not_p15, p9, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p3, p7, p1, p12, p9, p24, not_p21, not_p20, p17, not_p4, p8, not_p5}
{p3, p7, p1, not_p15, p9, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p3, p7, p1, p12, p9, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p3, p1, p14, p24, not_p21, p21, p17, not_p4, p25, p8, not_p5}
{p3, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p24, not_p5}
{p3, p1, p12, p14, p24, not_p21, p17, not_p4, p25, p8, not_p2, not_p5}
{p3, p7, p1, p12, p9, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p12, p14, p11, p24, p17, not_p4, not_p12, not_p18, not_p5}
{p12, p14, p11, p24, not_p21, p17, not_p4, not_p18, not_p5}
{p22, p12, p14, p11, p24, p17, not_p4, not_p18, not_p5}
{p12, p11, p24, p17, not_p4, not_p12, not_p3, not_p18, not_p5}
{p12, p11, p24, not_p21, p17, not_p4, not_p3, not_p18, not_p5}
{p22, p12, p11, p24, p17, not_p4, not_p3, not_p18, not_p5}
{p12, p11, p24, p17, not_p4, not_p12, not_p25, not_p18, not_p5}
{p12, p11, p24, not_p21, p17, not_p4, not_p25, not_p18, not_p5}
{p22, p12, p11, p24, p17, not_p4, not_p25, not_p18, not_p5}
{p12, p9, p11, p24, p17, not_p4, not_p12, not_p18, not_p5}
{p12, p9, p11, p24, not_p21, p17, not_p4, not_p18, not_p5}
{p22, p12, p9, p11, p24, p17, not_p4, not_p18, not_p5}
{p14, p11, p24, not_p20, p17, not_p4, not_p12, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, not_p20, p17, not_p4, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, not_p20, p17, not_p4, not_p18, not_p16, not_p5}
{p11, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p20, p17, not_p4, not_p3, not_p18, not_p16, not_p5}
{p11, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p20, p17, not_p4, not_p25, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p20, p17, not_p4, not_p12, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p18, not_p16, not_p5}
{p22, p9, p11, p24, not_p20, p17, not_p4, not_p18, not_p16, not_p5}
{p11, p24, p21, p17, not_p4, not_p12, not_p3, not_p18, not_p16, not_p5}
{p11, p24, not_p21, p21, p17, not_p4, not_p3, not_p18, not_p16, not_p5}
{p22, p11, p24, p21, p17, not_p4, not_p3, not_p18, not_p16, not_p5}
{p11, p24, p21, p17, not_p4, not_p12, not_p25, not_p18, not_p16, not_p5}
{p11, p24, not_p21, p21, p17, not_p4, not_p25, not_p18, not_p16, not_p5}
{p22, p11, p24, p21, p17, not_p4, not_p25, not_p18, not_p16, not_p5}
{p14, p11, p24, p21, p17, not_p4, not_p12, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, p21, p17, not_p4, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, p21, p17, not_p4, not_p18, not_p16, not_p5}
{p9, p11, p24, p21, p17, not_p4, not_p12, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p21, p21, p17, not_p4, not_p18, not_p16, not_p5}
{p22, p9, p11, p24, p21, p17, not_p4, not_p18, not_p16, not_p5}
{p2, p11, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p18, not_p5}
{p22, p2, p11, p24, not_p20, p17, not_p4, not_p3, not_p18, not_p5}
{p2, p11, p24, p21, p17, not_p4, not_p12, not_p3, not_p18, not_p5}
{p2, p11, p24, not_p21, p21, p17, not_p4, not_p3, not_p18, not_p5}
{p22, p2, p11, p24, p21, p17, not_p4, not_p3, not_p18, not_p5}
{p2, p11, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p18, not_p5}
{p22, p2, p11, p24, not_p20, p17, not_p4, not_p25, not_p18, not_p5}
{p2, p11, p24, p21, p17, not_p4, not_p12, not_p25, not_p18, not_p5}
{p2, p11, p24, not_p21, p21, p17, not_p4, not_p25, not_p18, not_p5}
{p22, p2, p11, p24, p21, p17, not_p4, not_p25, not_p18, not_p5}
{p14, p2, p11, p24, not_p20, p17, not_p4, not_p12, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, not_p20, p17, not_p4, not_p18, not_p5}
{p22, p14, p2, p11, p24, not_p20, p17, not_p4, not_p18, not_p5}
{p9, p2, p11, p24, not_p20, p17, not_p4, not_p12, not_p18, not_p5}
{p9, p2, p11, p24, not_p21, not_p20, p17, not_p4, not_p18, not_p5}
{p22, p9, p2, p11, p24, not_p20, p17, not_p4, not_p18, not_p5}
{p14, p2, p11, p24, p21, p17, not_p4, not_p12, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, p21, p17, not_p4, not_p18, not_p5}
{p22, p14, p2, p11, p24, p21, p17, not_p4, not_p18, not_p5}
{p9, p2, p11, p24, p21, p17, not_p4, not_p12, not_p18, not_p5}
{p9, p2, p11, p24, not_p21, p21, p17, not_p4, not_p18, not_p5}
{p22, p9, p2, p11, p24, p21, p17, not_p4, not_p18, not_p5}
{p12, p14, p11, p24, not_p4, not_p12, not_p8, not_p18, not_p5}
{p12, p14, p11, p24, not_p21, not_p4, not_p8, not_p18, not_p5}
{p22, p12, p14, p11, p24, not_p4, not_p8, not_p18, not_p5}
{p12, p14, p11, p24, not_p4, not_p12, not_p19, not_p18, not_p5}
{p12, p14, p11, p24, not_p21, not_p4, not_p19, not_p18, not_p5}
{p22, p12, p14, p11, p24, not_p4, not_p19, not_p18, not_p5}
{p6, p12, p14, p11, p24, not_p4, not_p12, not_p18, not_p5}
{p6, p12, p14, p11, p24, not_p21, not_p4, not_p18, not_p5}
{p6, p22, p12, p14, p11, p24, not_p4, not_p18, not_p5}
{p12, p11, p24, not_p4, not_p12, not_p3, not_p8, not_p18, not_p5}
{p12, p11, p24, not_p21, not_p4, not_p3, not_p8, not_p18, not_p5}
{p22, p12, p11, p24, not_p4, not_p3, not_p8, not_p18, not_p5}
{p12, p11, p24, not_p4, not_p12, not_p25, not_p8, not_p18, not_p5}
{p12, p11, p24, not_p21, not_p4, not_p25, not_p8, not_p18, not_p5}
{p22, p12, p11, p24, not_p4, not_p25, not_p8, not_p18, not_p5}
{p12, p9, p11, p24, not_p4, not_p12, not_p8, not_p18, not_p5}
{p12, p9, p11, p24, not_p21, not_p4, not_p8, not_p18, not_p5}
{p22, p12, p9, p11, p24, not_p4, not_p8, not_p18, not_p5}
{p12, p11, p24, not_p4, not_p12, not_p3, not_p19, not_p18, not_p5}
{p12, p11, p24, not_p21, not_p4, not_p3, not_p19, not_p18, not_p5}
{p22, p12, p11, p24, not_p4, not_p3, not_p19, not_p18, not_p5}
{p12, p11, p24, not_p4, not_p12, not_p25, not_p19, not_p18, not_p5}
{p12, p11, p24, not_p21, not_p4, not_p25, not_p19, not_p18, not_p5}
{p22, p12, p11, p24, not_p4, not_p25, not_p19, not_p18, not_p5}
{p12, p9, p11, p24, not_p4, not_p12, not_p19, not_p18, not_p5}
{p12, p9, p11, p24, not_p21, not_p4, not_p19, not_p18, not_p5}
{p22, p12, p9, p11, p24, not_p4, not_p19, not_p18, not_p5}
{p6, p12, p11, p24, not_p4, not_p12, not_p3, not_p18, not_p5}
{p6, p12, p11, p24, not_p21, not_p4, not_p3, not_p18, not_p5}
{p6, p22, p12, p11, p24, not_p4, not_p3, not_p18, not_p5}
{p6, p12, p11, p24, not_p4, not_p12, not_p25, not_p18, not_p5}
{p6, p12, p11, p24, not_p21, not_p4, not_p25, not_p18, not_p5}
{p6, p22, p12, p11, p24, not_p4, not_p25, not_p18, not_p5}
{p6, p12, p9, p11, p24, not_p4, not_p12, not_p18, not_p5}
{p6, p12, p9, p11, p24, not_p21, not_p4, not_p18, not_p5}
{p6, p22, p12, p9, p11, p24, not_p4, not_p18, not_p5}
{p11, p24, not_p4, not_p12, p8, not_p3, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p8, not_p3, not_p8, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p8, not_p3, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p4, not_p12, p8, not_p25, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p8, not_p25, not_p8, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p8, not_p25, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p20, not_p4, not_p12, p8, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, not_p20, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, not_p20, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, p21, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, p21, not_p4, not_p12, p8, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, p21, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p4, not_p12, p8, not_p8, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p21, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p22, p9, p11, p24, not_p4, p8, not_p8, not_p18, not_p16, not_p5}
{p2, p11, p24, not_p4, not_p12, p8, not_p3, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p8, not_p3, not_p8, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p8, not_p3, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p4, not_p12, p8, not_p25, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p8, not_p25, not_p8, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p8, not_p25, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p20, not_p4, not_p12, p8, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, not_p20, not_p4, p8, not_p8, not_p18, not_p5}
{p22, p14, p2, p11, p24, not_p20, not_p4, p8, not_p8, not_p18, not_p5}
{p22, p14, p2, p11, p24, p21, not_p4, p8, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, p21, not_p4, not_p12, p8, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, p21, not_p4, p8, not_p8, not_p18, not_p5}
{p9, p2, p11, p24, not_p4, not_p12, p8, not_p8, not_p18, not_p5}
{p9, p2, p11, p24, not_p21, not_p4, p8, not_p8, not_p18, not_p5}
{p22, p9, p2, p11, p24, not_p4, p8, not_p8, not_p18, not_p5}
{p11, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p25, not_p3, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p25, not_p25, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, not_p20, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, p21, not_p4, p25, not_p12, not_p8, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, p21, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, p21, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p4, p25, not_p12, not_p8, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p22, p9, p11, p24, not_p4, p25, not_p8, not_p18, not_p16, not_p5}
{p2, p11, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p25, not_p3, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p25, not_p25, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p18, not_p5}
{p22, p14, p2, p11, p24, not_p20, not_p4, p25, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, p21, not_p4, p25, not_p12, not_p8, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, p21, not_p4, p25, not_p8, not_p18, not_p5}
{p22, p14, p2, p11, p24, p21, not_p4, p25, not_p8, not_p18, not_p5}
{p9, p2, p11, p24, not_p4, p25, not_p12, not_p8, not_p18, not_p5}
{p9, p2, p11, p24, not_p21, not_p4, p25, not_p8, not_p18, not_p5}
{p22, p9, p2, p11, p24, not_p4, p25, not_p8, not_p18, not_p5}
{p11, p24, not_p4, not_p12, p8, not_p3, not_p19, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p8, not_p3, not_p19, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p8, not_p3, not_p19, not_p18, not_p16, not_p5}
{p11, p24, not_p4, not_p12, p8, not_p25, not_p19, not_p18, not_p16, not_p5}
{p11, p24, not_p21, not_p4, p8, not_p25, not_p19, not_p18, not_p16, not_p5}
{p22, p11, p24, not_p4, p8, not_p25, not_p19, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p20, not_p4, not_p12, p8, not_p19, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, not_p20, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, not_p20, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p22, p14, p11, p24, p21, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p14, p11, p24, p21, not_p4, not_p12, p8, not_p19, not_p18, not_p16, not_p5}
{p14, p11, p24, not_p21, p21, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p4, not_p12, p8, not_p19, not_p18, not_p16, not_p5}
{p9, p11, p24, not_p21, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p22, p9, p11, p24, not_p4, p8, not_p19, not_p18, not_p16, not_p5}
{p6, p11, p24, not_p4, not_p12, p8, not_p3, not_p18, not_p16, not_p5}
{p6, p11, p24, not_p21, not_p4, p8, not_p3, not_p18, not_p16, not_p5}
{p6, p22, p11, p24, not_p4, p8, not_p3, not_p18, not_p16, not_p5}
{p6, p11, p24, not_p4, not_p12, p8, not_p25, not_p18, not_p16, not_p5}
{p6, p11, p24, not_p21, not_p4, p8, not_p25, not_p18, not_p16, not_p5}
{p6, p22, p11, p24, not_p4, p8, not_p25, not_p18, not_p16, not_p5}
{p6, p14, p11, p24, not_p20, not_p4, not_p12, p8, not_p18, not_p16, not_p5}
{p6, p14, p11, p24, not_p21, not_p20, not_p4, p8, not_p18, not_p16, not_p5}
{p6, p22, p14, p11, p24, not_p20, not_p4, p8, not_p18, not_p16, not_p5}
{p6, p22, p14, p11, p24, p21, not_p4, p8, not_p18, not_p16, not_p5}
{p6, p14, p11, p24, p21, not_p4, not_p12, p8, not_p18, not_p16, not_p5}
{p6, p14, p11, p24, not_p21, p21, not_p4, p8, not_p18, not_p16, not_p5}
{p6, p9, p11, p24, not_p4, not_p12, p8, not_p18, not_p16, not_p5}
{p6, p9, p11, p24, not_p21, not_p4, p8, not_p18, not_p16, not_p5}
{p6, p22, p9, p11, p24, not_p4, p8, not_p18, not_p16, not_p5}
{p2, p11, p24, not_p4, not_p12, p8, not_p3, not_p19, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p8, not_p3, not_p19, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p8, not_p3, not_p19, not_p18, not_p5}
{p2, p11, p24, not_p4, not_p12, p8, not_p25, not_p19, not_p18, not_p5}
{p2, p11, p24, not_p21, not_p4, p8, not_p25, not_p19, not_p18, not_p5}
{p22, p2, p11, p24, not_p4, p8, not_p25, not_p19, not_p18, not_p5}
{p14, p2, p11, p24, not_p20, not_p4, not_p12, p8, not_p19, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, not_p20, not_p4, p8, not_p19, not_p18, not_p5}
{p22, p14, p2, p11, p24, not_p20, not_p4, p8, not_p19, not_p18, not_p5}
{p22, p14, p2, p11, p24, p21, not_p4, p8, not_p19, not_p18, not_p5}
{p14, p2, p11, p24, p21, not_p4, not_p12, p8, not_p19, not_p18, not_p5}
{p14, p2, p11, p24, not_p21, p21, not_p4, p8, not_p19, not_p18, not_p5}
{p9, p2, p11, p24, not_p4, not_p12, p8, not_p19, not_p18, not_p5}
{p9, p2, p11, p24, not_p21, not_p4, p8, not_p19, not_p18, not_p5}
{p22, p9, p2, p11, p24, not_p4, p8, not_p19, not_p18, not_p5}
{p6, p2, p11, p24, not_p4, not_p12, p8, not_p3, not_p18, not_p5}
{p6, p2, p11, p24, not_p21, not_p4, p8, not_p3, not_p18, not_p5}
{p6, p22, p2, p11, p24, not_p4, p8, not_p3, not_p18, not_p5}
{p6, p2, p11, p24, not_p4, not_p12, p8, not_p25, not_p18, not_p5}
{p6, p2, p11, p24, not_p21, not_p4, p8, not_p25, not_p18, not_p5}
{p6, p22, p2, p11, p24, not_p4, p8, not_p25, not_p18, not_p5}
{p6, p14, p2, p11, p24, not_p20, not_p4, not_p12, p8, not_p18, not_p5}
{p6, p14, p2, p11, p24, not_p21, not_p20, not_p4, p8, not_p18, not_p5}
{p6, p22, p14, p2, p11, p24, not_p20, not_p4, p8, not_p18, not_p5}
{p6, p22, p14, p2, p11, p24, p21, not_p4, p8, not_p18, not_p5}
{p6, p14, p2, p11, p24, p21, not_p4, not_p12, p8, not_p18, not_p5}
{p6, p14, p2, p11, p24, not_p21, p21, not_p4, p8, not_p18, not_p5}
{p6, p9, p2, p11, p24, not_p4, not_p12, p8, not_p18, not_p5}
{p6, p9, p2, p11, p24, not_p21, not_p4, p8, not_p18, not_p5}
{p6, p22, p9, p2, p11, p24, not_p4, p8, not_p18, not_p5}
{p1, p22, p11, p24, not_p20, p17, not_p4, not_p3, not_p5}
{p1, p22, p11, p24, p21, p17, not_p4, not_p3, not_p5}
{p1, p22, p12, p11, p24, p17, not_p4, not_p3, not_p5}
{p18, p1, p11, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p5}
{p18, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p20, p1, p11, p24, not_p20, p17, not_p4, not_p12, not_p3, not_p5}
{p20, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p3, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p3, not_p5}
{p18, p1, p11, p24, p21, p17, not_p4, not_p12, not_p3, not_p5}
{p18, p1, p11, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p20, p1, p11, p24, p21, p17, not_p4, not_p12, not_p3, not_p5}
{p20, p1, p11, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p3, p1, p11, p24, not_p21, p21, p17, not_p4, not_p3, not_p5}
{p18, p1, p12, p11, p24, p17, not_p4, not_p12, not_p3, not_p5}
{p18, p1, p12, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p20, p1, p12, p11, p24, p17, not_p4, not_p12, not_p3, not_p5}
{p20, p1, p12, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p3, p1, p12, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p1, p22, p11, p24, not_p20, p17, not_p4, not_p25, not_p5}
{p1, p22, p11, p24, p21, p17, not_p4, not_p25, not_p5}
{p1, p22, p12, p11, p24, p17, not_p4, not_p25, not_p5}
{p18, p1, p11, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p5}
{p18, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p20, p1, p11, p24, not_p20, p17, not_p4, not_p12, not_p25, not_p5}
{p20, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p3, p1, p11, p24, not_p21, not_p20, p17, not_p4, not_p25, not_p5}
{p18, p1, p11, p24, p21, p17, not_p4, not_p12, not_p25, not_p5}
{p18, p1, p11, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p20, p1, p11, p24, p21, p17, not_p4, not_p12, not_p25, not_p5}
{p20, p1, p11, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p3, p1, p11, p24, not_p21, p21, p17, not_p4, not_p25, not_p5}
{p18, p1, p12, p11, p24, p17, not_p4, not_p12, not_p25, not_p5}
{p18, p1, p12, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p20, p1, p12, p11, p24, p17, not_p4, not_p12, not_p25, not_p5}
{p20, p1, p12, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p3, p1, p12, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p18, p1, p9, p11, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p18, p1, p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p18, p1, p22, p9, p11, p24, not_p20, p17, not_p4, not_p5}
{p18, p1, p9, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p18, p1, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p18, p1, p22, p9, p11, p24, p21, p17, not_p4, not_p5}
{p18, p1, p12, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p18, p1, p12, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p18, p1, p22, p12, p9, p11, p24, p17, not_p4, not_p5}
{p7, p20, p1, p12, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p7, p1, p22, p12, p9, p11, p24, p17, not_p4, not_p5}
{p7, p20, p1, p12, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p3, p7, p1, p12, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p6, p3, p1, not_p15, p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p6, p1, p22, not_p15, p9, p11, p24, not_p20, p17, not_p4, not_p5}
{p6, p1, p22, not_p15, p9, p11, p24, p21, p17, not_p4, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p6, p3, p1, not_p15, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p7, p1, p22, not_p15, p9, p11, p24, not_p20, p17, not_p4, not_p5}
{p7, p1, p22, not_p15, p9, p11, p24, p21, p17, not_p4, not_p5}
{p3, p7, p1, not_p15, p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p3, p7, p1, not_p15, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p1, p22, p14, p11, p24, not_p20, p17, not_p4, not_p5}
{p1, p22, p14, p11, p24, p21, p17, not_p4, not_p5}
{p1, p22, p12, p14, p11, p24, p17, not_p4, not_p5}
{p18, p1, p14, p11, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p18, p1, p14, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p20, p1, p14, p11, p24, not_p20, p17, not_p4, not_p12, not_p5}
{p20, p1, p14, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p3, p1, p14, p11, p24, not_p21, not_p20, p17, not_p4, not_p5}
{p18, p1, p14, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p18, p1, p14, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p20, p1, p14, p11, p24, p21, p17, not_p4, not_p12, not_p5}
{p20, p1, p14, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p3, p1, p14, p11, p24, not_p21, p21, p17, not_p4, not_p5}
{p18, p1, p12, p14, p11, p24, p17, not_p4, not_p12, not_p5}
{p18, p1, p12, p14, p11, p24, not_p21, p17, not_p4, not_p5}
{p20, p1, p12, p14, p11, p24, p17, not_p4, not_p12, not_p5}
{p20, p1, p12, p14, p11, p24, not_p21, p17, not_p4, not_p5}
{p3, p1, p12, p14, p11, p24, not_p21, p17, not_p4, not_p5}
{p1, p22, p11, p24, not_p4, p25, not_p3, not_p8, not_p5}
{p1, p22, p11, p24, not_p4, p25, not_p25, not_p8, not_p5}
{p1, p22, p14, p11, p24, not_p20, not_p4, p25, not_p8, not_p5}
{p1, p22, p14, p11, p24, p21, not_p4, p25, not_p8, not_p5}
{p6, p1, p22, not_p15, p9, p11, p24, not_p4, p25, not_p8, not_p5}
{p18, p1, p22, p9, p11, p24, not_p4, p25, not_p8, not_p5}
{p7, p1, p22, not_p15, p9, p11, p24, not_p4, p25, not_p8, not_p5}
{p18, p1, p11, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p5}
{p18, p1, p11, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p20, p1, p11, p24, not_p4, p25, not_p12, not_p3, not_p8, not_p5}
{p20, p1, p11, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p3, p1, p11, p24, not_p21, not_p4, p25, not_p3, not_p8, not_p5}
{p18, p1, p11, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p5}
{p18, p1, p11, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p20, p1, p11, p24, not_p4, p25, not_p12, not_p25, not_p8, not_p5}
{p20, p1, p11, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p3, p1, p11, p24, not_p21, not_p4, p25, not_p25, not_p8, not_p5}
{p18, p1, p14, p11, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p14, p11, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p18, p1, p14, p11, p24, p21, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p14, p11, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p20, p1, p14, p11, p24, not_p20, not_p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p14, p11, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p20, p1, p14, p11, p24, p21, not_p4, p25, not_p12, not_p8, not_p5}
{p20, p1, p14, p11, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p3, p1, p14, p11, p24, not_p21, not_p20, not_p4, p25, not_p8, not_p5}
{p3, p1, p14, p11, p24, not_p21, p21, not_p4, p25, not_p8, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p6, p20, p1, not_p15, p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p6, p3, p1, not_p15, p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p18, p1, p9, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p18, p1, p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p7, p20, p1, not_p15, p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p3, p7, p1, not_p15, p9, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p1, p22, p12, p14, p11, p24, not_p4, not_p8, not_p5}
{p18, p1, p12, p14, p11, p24, not_p4, not_p12, not_p8, not_p5}
{p20, p1, p12, p14, p11, p24, not_p4, not_p12, not_p8, not_p5}
{p18, p1, p12, p14, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p20, p1, p12, p14, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p3, p1, p12, p14, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p1, p22, p12, p11, p24, not_p4, not_p3, not_p8, not_p5}
{p1, p22, p12, p11, p24, not_p4, not_p25, not_p8, not_p5}
{p18, p1, p22, p12, p9, p11, p24, not_p4, not_p8, not_p5}
{p7, p1, p22, p12, p9, p11, p24, not_p4, not_p8, not_p5}
{p18, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p8, not_p5}
{p18, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p8, not_p5}
{p20, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p8, not_p5}
{p20, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p8, not_p5}
{p3, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p8, not_p5}
{p18, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p8, not_p5}
{p18, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p8, not_p5}
{p20, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p8, not_p5}
{p20, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p8, not_p5}
{p3, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p8, not_p5}
{p18, p1, p12, p9, p11, p24, not_p4, not_p12, not_p8, not_p5}
{p18, p1, p12, p9, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p7, p20, p1, p12, p9, p11, p24, not_p4, not_p12, not_p8, not_p5}
{p7, p20, p1, p12, p9, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p3, p7, p1, p12, p9, p11, p24, not_p21, not_p4, not_p8, not_p5}
{p1, p22, p12, p14, p11, p24, not_p4, not_p19, not_p5}
{p18, p1, p12, p14, p11, p24, not_p4, not_p12, not_p19, not_p5}
{p20, p1, p12, p14, p11, p24, not_p4, not_p12, not_p19, not_p5}
{p18, p1, p12, p14, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p20, p1, p12, p14, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p3, p1, p12, p14, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p1, p22, p12, p11, p24, not_p4, not_p3, not_p19, not_p5}
{p1, p22, p12, p11, p24, not_p4, not_p25, not_p19, not_p5}
{p18, p1, p22, p12, p9, p11, p24, not_p4, not_p19, not_p5}
{p7, p1, p22, p12, p9, p11, p24, not_p4, not_p19, not_p5}
{p18, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p19, not_p5}
{p18, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p19, not_p5}
{p20, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p19, not_p5}
{p20, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p19, not_p5}
{p3, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p19, not_p5}
{p18, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p19, not_p5}
{p18, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p19, not_p5}
{p20, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p19, not_p5}
{p20, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p19, not_p5}
{p3, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p19, not_p5}
{p18, p1, p12, p9, p11, p24, not_p4, not_p12, not_p19, not_p5}
{p18, p1, p12, p9, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p7, p20, p1, p12, p9, p11, p24, not_p4, not_p12, not_p19, not_p5}
{p7, p20, p1, p12, p9, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p3, p7, p1, p12, p9, p11, p24, not_p21, not_p4, not_p19, not_p5}
{p6, p1, p22, p12, p14, p11, p24, not_p4, not_p5}
{p6, p1, p22, p12, p9, p11, p24, not_p4, not_p5}
{p6, p1, p22, p12, p11, p24, not_p4, not_p3, not_p5}
{p6, p1, p22, p12, p11, p24, not_p4, not_p25, not_p5}
{p6, p18, p1, p12, p14, p11, p24, not_p4, not_p12, not_p5}
{p6, p18, p1, p12, p9, p11, p24, not_p4, not_p12, not_p5}
{p6, p18, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p5}
{p6, p18, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p5}
{p6, p18, p1, p12, p14, p11, p24, not_p21, not_p4, not_p5}
{p6, p18, p1, p12, p9, p11, p24, not_p21, not_p4, not_p5}
{p6, p18, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p5}
{p6, p18, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p5}
{p6, p20, p1, p12, p14, p11, p24, not_p4, not_p12, not_p5}
{p6, p20, p1, p12, p9, p11, p24, not_p4, not_p12, not_p5}
{p6, p20, p1, p12, p11, p24, not_p4, not_p12, not_p3, not_p5}
{p6, p20, p1, p12, p11, p24, not_p4, not_p12, not_p25, not_p5}
{p6, p20, p1, p12, p14, p11, p24, not_p21, not_p4, not_p5}
{p6, p20, p1, p12, p9, p11, p24, not_p21, not_p4, not_p5}
{p6, p20, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p5}
{p6, p20, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p5}
{p6, p3, p1, p12, p14, p11, p24, not_p21, not_p4, not_p5}
{p6, p3, p1, p12, p9, p11, p24, not_p21, not_p4, not_p5}
{p6, p3, p1, p12, p11, p24, not_p21, not_p4, not_p3, not_p5}
{p6, p3, p1, p12, p11, p24, not_p21, not_p4, not_p25, not_p5}
{p5, p22, p14, p11, p24, p17, not_p4, not_p16}
{p5, p22, p11, p24, p17, not_p4, not_p3, not_p16}
{p5, p22, p11, p24, p17, not_p4, not_p25, not_p16}
{p5, p22, p9, p11, p24, p17, not_p4, not_p16}
{p5, p22, p14, p2, p11, p24, p17, not_p4}
{p5, p22, p2, p11, p24, p17, not_p4, not_p3}
{p5, p22, p2, p11, p24, p17, not_p4, not_p25}
{p5, p22, p9, p2, p11, p24, p17, not_p4}
{p5, p1, p22, p11, p24, p17, not_p4, not_p3}
{p5, p1, p22, p11, p24, p17, not_p4, not_p25}
{p5, p1, p22, p14, p11, p24, p17, not_p4}
{p5, p18, p1, p22, p9, p11, p24, p17, not_p4}
{p6, p5, p1, p22, not_p15, p9, p11, p24, p17, not_p4}
{p5, p7, p1, p22, not_p15, p9, p11, p24, p17, not_p4}
{p5, p18, p1, p11, p24, p17, not_p4, not_p12, not_p3, not_p5}
{p5, p18, p1, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p5, p18, p1, p11, p24, p17, not_p4, not_p12, not_p3, not_p19}
{p5, p18, p1, p11, p24, not_p21, p17, not_p4, not_p3, not_p19}
{p5, p20, p1, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p5, p20, p1, p11, p24, not_p21, p17, not_p4, not_p3, not_p19}
{p5, p20, p1, p11, p24, p17, not_p4, not_p12, not_p3, not_p5}
{p5, p20, p1, p11, p24, p17, not_p4, not_p12, not_p3, not_p19}
{p5, p11, p24, p17, not_p4, not_p12, not_p3, not_p16, not_p5}
{p5, p11, p24, not_p21, p17, not_p4, not_p3, not_p16, not_p5}
{p5, p2, p11, p24, p17, not_p4, not_p12, not_p3, not_p5}
{p5, p2, p11, p24, not_p21, p17, not_p4, not_p3, not_p5}
{p5, p11, p24, p17, not_p4, not_p12, not_p3, not_p19, not_p16}
{p5, p2, p11, p24, p17, not_p4, not_p12, not_p3, not_p19}
{p5, p11, p24, not_p21, p17, not_p4, not_p3, not_p19, not_p16}
{p5, p2, p11, p24, not_p21, p17, not_p4, not_p3, not_p19}
{p5, p18, p1, p11, p24, p17, not_p4, not_p12, not_p25, not_p5}
{p5, p18, p1, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p5, p18, p1, p11, p24, p17, not_p4, not_p12, not_p25, not_p19}
{p5, p18, p1, p11, p24, not_p21, p17, not_p4, not_p25, not_p19}
{p5, p20, p1, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p5, p20, p1, p11, p24, not_p21, p17, not_p4, not_p25, not_p19}
{p5, p20, p1, p11, p24, p17, not_p4, not_p12, not_p25, not_p5}
{p5, p20, p1, p11, p24, p17, not_p4, not_p12, not_p25, not_p19}
{p5, p11, p24, p17, not_p4, not_p12, not_p25, not_p16, not_p5}
{p5, p11, p24, not_p21, p17, not_p4, not_p25, not_p16, not_p5}
{p5, p2, p11, p24, p17, not_p4, not_p12, not_p25, not_p5}
{p5, p2, p11, p24, not_p21, p17, not_p4, not_p25, not_p5}
{p5, p11, p24, p17, not_p4, not_p12, not_p25, not_p19, not_p16}
{p5, p2, p11, p24, p17, not_p4, not_p12, not_p25, not_p19}
{p5, p11, p24, not_p21, p17, not_p4, not_p25, not_p19, not_p16}
{p5, p2, p11, p24, not_p21, p17, not_p4, not_p25, not_p19}
{p5, p14, p11, p24, p17, not_p4, p25, not_p12, not_p16, not_p5}
{p5, p14, p2, p11, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p18, p1, p14, p11, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p20, p1, p14, p11, p24, p17, not_p4, p25, not_p12, not_p5}
{p5, p14, p11, p24, not_p21, p17, not_p4, p25, not_p16, not_p5}
{p5, p14, p2, p11, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p18, p1, p14, p11, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p20, p1, p14, p11, p24, not_p21, p17, not_p4, p25, not_p5}
{p5, p14, p11, p24, p17, not_p4, p25, not_p12, not_p19, not_p16}
{p5, p14, p2, p11, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p18, p1, p14, p11, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p20, p1, p14, p11, p24, p17, not_p4, p25, not_p12, not_p19}
{p5, p14, p11, p24, not_p21, p17, not_p4, p25, not_p19, not_p16}
{p5, p14, p2, p11, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p18, p1, p14, p11, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p20, p1, p14, p11, p24, not_p21, p17, not_p4, p25, not_p19}
{p5, p9, p11, p24, p17, not_p4, not_p12, not_p16, not_p5}
{p5, p18, p1, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p6, p5, p20, p1, not_p15, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p5, p7, p20, p1, not_p15, p9, p11, p24, p17, not_p4, not_p12, not_p5}
{p5, p9, p2, p11, p24, p17, not_p4, not_p12, not_p5}
{p5, p9, p11, p24, not_p21, p17, not_p4, not_p16, not_p5}
{p5, p18, p1, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p6, p5, p20, p1, not_p15, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p5, p7, p20, p1, not_p15, p9, p11, p24, not_p21, p17, not_p4, not_p5}
{p5, p9, p2, p11, p24, not_p21, p17, not_p4, not_p5}
{p5, p9, p11, p24, p17, not_p4, not_p12, not_p19, not_p16}
{p5, p18, p1, p9, p11, p24, p17, not_p4, not_p12, not_p19}
{p6, p5, p20, p1, not_p15, p9, p11, p24, p17, not_p4, not_p12, not_p19}
{p5, p7, p20, p1, not_p15, p9, p11, p24, p17, not_p4, not_p12, not_p19}
{p5, p9, p2, p11, p24, p17, not_p4, not_p12, not_p19}
{p5, p9, p11, p24, not_p21, p17, not_p4, not_p19, not_p16}
{p5, p18, p1, p9, p11, p24, not_p21, p17, not_p4, not_p19}
{p6, p5, p20, p1, not_p15, p9, p11, p24, not_p21, p17, not_p4, not_p19}
{p5, p7, p20, p1, not_p15, p9, p11, p24, not_p21, p17, not_p4, not_p19}
{p5, p9, p2, p11, p24, not_p21, p17, not_p4, not_p19}
{p5, p22, p14, p24, p17, not_p4, p8, not_p16}
{p5, p22, p24, p17, not_p4, p8, not_p3, not_p16}
{p5, p22, p24, p17, not_p4, p8, not_p25, not_p16}
{p5, p22, p9, p24, p17, not_p4, p8, not_p16}
{p5, p22, p14, p2, p24, p17, not_p4, p8}
{p5, p22, p2, p24, p17, not_p4, p8, not_p3}
{p5, p22, p2, p24, p17, not_p4, p8, not_p25}
{p5, p22, p9, p2, p24, p17, not_p4, p8}
{p5, p1, p22, p24, p17, not_p4, p8, not_p3}
{p5, p1, p22, p24, p17, not_p4, p8, not_p25}
{p5, p1, p22, p14, p24, p17, not_p4, p8}
{p5, p18, p1, p22, p9, p24, p17, not_p4, p8}
{p5, p7, p1, p22, not_p15, p9, p24, p17, not_p4, p8}
{p5, p18, p1, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p5}
{p5, p18, p1, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p5, p18, p1, p24, p21, p17, not_p4, not_p12, p8, not_p3, not_p5}
{p5, p18, p1, p24, not_p21, p21, p17, not_p4, p8, not_p3, not_p5}
{p5, p20, p1, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p5, p20, p1, p24, not_p21, p21, p17, not_p4, p8, not_p3, not_p5}
{p5, p20, p1, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p5}
{p5, p20, p1, p24, p21, p17, not_p4, not_p12, p8, not_p3, not_p5}
{p5, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p16, not_p5}
{p5, p24, p21, p17, not_p4, not_p12, p8, not_p3, not_p16, not_p5}
{p5, p2, p24, p17, not_p4, p25, not_p12, p8, not_p3, not_p5}
{p5, p2, p24, p21, p17, not_p4, not_p12, p8, not_p3, not_p5}
{p5, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p16, not_p5}
{p5, p2, p24, not_p21, p17, not_p4, p25, p8, not_p3, not_p5}
{p5, p24, not_p21, p21, p17, not_p4, p8, not_p3, not_p16, not_p5}
{p5, p2, p24, not_p21, p21, p17, not_p4, p8, not_p3, not_p5}
{p5, p18, p1, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p5}
{p5, p18, p1, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p5, p18, p1, p24, p21, p17, not_p4, not_p12, p8, not_p25, not_p5}
{p5, p18, p1, p24, not_p21, p21, p17, not_p4, p8, not_p25, not_p5}
{p5, p20, p1, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p5, p20, p1, p24, not_p21, p21, p17, not_p4, p8, not_p25, not_p5}
{p5, p20, p1, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p5}
{p5, p20, p1, p24, p21, p17, not_p4, not_p12, p8, not_p25, not_p5}
{p5, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p16, not_p5}
{p5, p24, p21, p17, not_p4, not_p12, p8, not_p25, not_p16, not_p5}
{p5, p2, p24, p17, not_p4, p25, not_p12, p8, not_p25, not_p5}
{p5, p2, p24, p21, p17, not_p4, not_p12, p8, not_p25, not_p5}
{p5, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p16, not_p5}
{p5, p2, p24, not_p21, p17, not_p4, p25, p8, not_p25, not_p5}
{p5, p24, not_p21, p21, p17, not_p4, p8, not_p25, not_p16, not_p5}
{p5, p2, p24, not_p21, p21, p17, not_p4, p8, not_p25, not_p5}
{p5, p18, p1, p14, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p18, p1, p14, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p20, p1, p14, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p20, p1, p14, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p14, p24, p17, not_p4, p25, not_p12, p8, not_p16, not_p5}
{p5, p14, p2, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p14, p24, not_p21, p17, not_p4, p25, p8, not_p16, not_p5}
{p5, p14, p2, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p18, p1, p14, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p5, p20, p1, p14, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p5, p14, p24, p21, p17, not_p4, not_p12, p8, not_p16, not_p5}
{p5, p14, p2, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p5, p18, p1, p14, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p5, p20, p1, p14, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p5, p14, p24, not_p21, p21, p17, not_p4, p8, not_p16, not_p5}
{p5, p14, p2, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p5, p9, p24, p17, not_p4, p25, not_p12, p8, not_p16, not_p5}
{p5, p18, p1, p9, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p7, p20, p1, not_p15, p9, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p9, p2, p24, p17, not_p4, p25, not_p12, p8, not_p5}
{p5, p9, p24, p21, p17, not_p4, not_p12, p8, not_p16, not_p5}
{p5, p18, p1, p9, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p5, p9, p2, p24, p21, p17, not_p4, not_p12, p8, not_p5}
{p5, p9, p24, not_p21, p17, not_p4, p25, p8, not_p16, not_p5}
{p5, p18, p1, p9, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p7, p20, p1, not_p15, p9, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p9, p2, p24, not_p21, p17, not_p4, p25, p8, not_p5}
{p5, p9, p24, not_p21, p21, p17, not_p4, p8, not_p16, not_p5}
{p5, p18, p1, p9, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p5, p9, p2, p24, not_p21, p21, p17, not_p4, p8, not_p5}
{p5, p22, p14, p24, not_p4, p8, not_p8, not_p16}
{p5, p22, p14, p2, p24, not_p4, p8, not_p8}
{p5, p1, p22, p14, p24, not_p4, p8, not_p8}
{p5, p22, p14, p11, p24, not_p4, p25, not_p8, not_p16}
{p5, p22, p14, p2, p11, p24, not_p4, p25, not_p8}
{p5, p1, p22, p14, p11, p24, not_p4, p25, not_p8}
{p5, p14, p11, p24, not_p4, p25, not_p12, not_p8, not_p16, not_p5}
{p5, p14, p11, p24, not_p21, not_p4, p25, not_p8, not_p16, not_p5}
{p5, p14, p11, p24, not_p4, p25, not_p12, not_p19, not_p8, not_p16}
{p5, p14, p11, p24, not_p21, not_p4, p25, not_p19, not_p8, not_p16}
{p5, p14, p2, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, p14, p2, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, p14, p2, p11, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, p14, p2, p11, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p5, p18, p1, p14, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, p18, p1, p14, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, p20, p1, p14, p11, p24, not_p21, not_p4, p25, not_p8, not_p5}
{p5, p20, p1, p14, p11, p24, not_p4, p25, not_p12, not_p8, not_p5}
{p5, p18, p1, p14, p11, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, p18, p1, p14, p11, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p5, p20, p1, p14, p11, p24, not_p21, not_p4, p25, not_p19, not_p8}
{p5, p20, p1, p14, p11, p24, not_p4, p25, not_p12, not_p19, not_p8}
{p5, p14, p24, not_p4, p25, not_p12, p8, not_p8, not_p16, not_p5}
{p5, p14, p2, p24, not_p4, p25, not_p12, p8, not_p8, not_p5}
{p5, p18, p1, p14, p24, not_p4, p25, not_p12, p8, not_p8, not_p5}
{p5, p20, p1, p14, p24, not_p4, p25, not_p12, p8, not_p8, not_p5}
{p5, p14, p24, not_p21, not_p4, p25, p8, not_p8, not_p16, not_p5}
{p5, p14, p2, p24, not_p21, not_p4, p25, p8, not_p8, not_p5}
{p5, p18, p1, p14, p24, not_p21, not_p4, p25, p8, not_p8, not_p5}
{p5, p20, p1, p14, p24, not_p21, not_p4, p25, p8, not_p8, not_p5}
{p5, p22, p14, p24, not_p4, p8, not_p19, not_p16}
{p5, p22, p14, p2, p24, not_p4, p8, not_p19}
{p5, p1, p22, p14, p24, not_p4, p8, not_p19}
{p5, p14, p24, not_p4, p25, not_p12, p8, not_p19, not_p16}
{p5, p14, p2, p24, not_p4, p25, not_p12, p8, not_p19}
{p5, p18, p1, p14, p24, not_p4, p25, not_p12, p8, not_p19}
{p5, p20, p1, p14, p24, not_p4, p25, not_p12, p8, not_p19}
{p5, p14, p24, not_p21, not_p4, p25, p8, not_p19, not_p16}
{p5, p14, p2, p24, not_p21, not_p4, p25, p8, not_p19}
{p5, p18, p1, p14, p24, not_p21, not_p4, p25, p8, not_p19}
{p5, p20, p1, p14, p24, not_p21, not_p4, p25, p8, not_p19}
{p6, p5, p1, p22, not_p15, p14, p24, not_p4, p8}
{p6, p5, p22, p14, p24, not_p4, p8, not_p16}
{p6, p5, p22, p14, p2, p24, not_p4, p8}
{p6, p5, p18, p1, not_p15, p14, p24, not_p4, p25, not_p12, p8, not_p5}
{p6, p5, p20, p1, not_p15, p14, p24, not_p4, p25, not_p12, p8, not_p5}
{p6, p5, p14, p24, not_p4, p25, not_p12, p8, not_p16, not_p5}
{p6, p5, p14, p2, p24, not_p4, p25, not_p12, p8, not_p5}
{p6, p5, p18, p1, not_p15, p14, p24, not_p21, not_p4, p25, p8, not_p5}
{p6, p5, p20, p1, not_p15, p14, p24, not_p21, not_p4, p25, p8, not_p5}
{p6, p5, p14, p24, not_p21, not_p4, p25, p8, not_p16, not_p5}
{p6, p5, p14, p2, p24, not_p21, not_p4, p25, p8, not_p5}
{p2, p11, not_p4, not_p12, p8, not_p3, not_p8, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p8, not_p3, not_p8, not_p22, not_p5}
{p22, p2, p11, not_p4, p8, not_p3, not_p8, not_p22, not_p5}
{p2, p11, not_p4, not_p12, p8, not_p25, not_p8, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p8, not_p25, not_p8, not_p22, not_p5}
{p22, p2, p11, not_p4, p8, not_p25, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p20, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p21, not_p20, not_p4, p8, not_p8, not_p22, not_p5}
{p22, p14, p2, p11, not_p20, not_p4, p8, not_p8, not_p22, not_p5}
{p22, p14, p2, p11, p21, not_p4, p8, not_p8, not_p22, not_p5}
{p14, p2, p11, p21, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p21, p21, not_p4, p8, not_p8, not_p22, not_p5}
{p9, p2, p11, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p9, p2, p11, not_p21, not_p4, p8, not_p8, not_p22, not_p5}
{p22, p9, p2, p11, not_p4, p8, not_p8, not_p22, not_p5}
{p2, p11, not_p4, not_p12, p8, not_p3, not_p19, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p8, not_p3, not_p19, not_p22, not_p5}
{p22, p2, p11, not_p4, p8, not_p3, not_p19, not_p22, not_p5}
{p2, p11, not_p4, not_p12, p8, not_p25, not_p19, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p8, not_p25, not_p19, not_p22, not_p5}
{p22, p2, p11, not_p4, p8, not_p25, not_p19, not_p22, not_p5}
{p14, p2, p11, not_p20, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p14, p2, p11, not_p21, not_p20, not_p4, p8, not_p19, not_p22, not_p5}
{p22, p14, p2, p11, not_p20, not_p4, p8, not_p19, not_p22, not_p5}
{p22, p14, p2, p11, p21, not_p4, p8, not_p19, not_p22, not_p5}
{p14, p2, p11, p21, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p14, p2, p11, not_p21, p21, not_p4, p8, not_p19, not_p22, not_p5}
{p9, p2, p11, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p9, p2, p11, not_p21, not_p4, p8, not_p19, not_p22, not_p5}
{p22, p9, p2, p11, not_p4, p8, not_p19, not_p22, not_p5}
{p6, p2, p11, not_p4, not_p12, p8, not_p3, not_p22, not_p5}
{p6, p2, p11, not_p21, not_p4, p8, not_p3, not_p22, not_p5}
{p6, p22, p2, p11, not_p4, p8, not_p3, not_p22, not_p5}
{p6, p2, p11, not_p4, not_p12, p8, not_p25, not_p22, not_p5}
{p6, p2, p11, not_p21, not_p4, p8, not_p25, not_p22, not_p5}
{p6, p22, p2, p11, not_p4, p8, not_p25, not_p22, not_p5}
{p6, p14, p2, p11, not_p20, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p14, p2, p11, not_p21, not_p20, not_p4, p8, not_p22, not_p5}
{p6, p22, p14, p2, p11, not_p20, not_p4, p8, not_p22, not_p5}
{p6, p22, p14, p2, p11, p21, not_p4, p8, not_p22, not_p5}
{p6, p14, p2, p11, p21, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p14, p2, p11, not_p21, p21, not_p4, p8, not_p22, not_p5}
{p6, p9, p2, p11, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p9, p2, p11, not_p21, not_p4, p8, not_p22, not_p5}
{p6, p22, p9, p2, p11, not_p4, p8, not_p22, not_p5}
{p3, p2, not_p4, not_p12, p8, not_p3, not_p8, not_p22, not_p5}
{p3, p2, not_p21, not_p4, p8, not_p3, not_p8, not_p22, not_p5}
{p3, p22, p2, not_p4, p8, not_p3, not_p8, not_p22, not_p5}
{p3, p2, not_p4, not_p12, p8, not_p25, not_p8, not_p22, not_p5}
{p3, p2, not_p21, not_p4, p8, not_p25, not_p8, not_p22, not_p5}
{p3, p22, p2, not_p4, p8, not_p25, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p20, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p21, not_p20, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p22, p14, p2, not_p20, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p14, p2, p21, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p21, p21, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p22, p14, p2, p21, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p22, p12, p14, p2, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p12, p14, p2, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p9, p2, not_p4, not_p12, p8, not_p8, not_p22, not_p5}
{p3, p9, p2, not_p21, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p22, p9, p2, not_p4, p8, not_p8, not_p22, not_p5}
{p3, p2, not_p4, not_p12, p8, not_p3, not_p19, not_p22, not_p5}
{p3, p2, not_p21, not_p4, p8, not_p3, not_p19, not_p22, not_p5}
{p3, p22, p2, not_p4, p8, not_p3, not_p19, not_p22, not_p5}
{p3, p2, not_p4, not_p12, p8, not_p25, not_p19, not_p22, not_p5}
{p3, p2, not_p21, not_p4, p8, not_p25, not_p19, not_p22, not_p5}
{p3, p22, p2, not_p4, p8, not_p25, not_p19, not_p22, not_p5}
{p3, p14, p2, not_p20, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p3, p14, p2, not_p21, not_p20, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p22, p14, p2, not_p20, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p14, p2, p21, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p3, p14, p2, not_p21, p21, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p22, p14, p2, p21, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p22, p12, p14, p2, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p12, p14, p2, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p9, p2, not_p4, not_p12, p8, not_p19, not_p22, not_p5}
{p3, p9, p2, not_p21, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p22, p9, p2, not_p4, p8, not_p19, not_p22, not_p5}
{p3, p2, not_p20, p17, not_p4, not_p12, p8, not_p3, not_p22, not_p5}
{p3, p2, not_p21, not_p20, p17, not_p4, p8, not_p3, not_p22, not_p5}
{p3, p22, p2, not_p20, p17, not_p4, p8, not_p3, not_p22, not_p5}
{p3, p12, p2, p17, not_p4, p25, not_p12, p8, not_p3, not_p22, not_p5}
{p3, p12, p2, not_p21, p17, not_p4, p25, p8, not_p3, not_p22, not_p5}
{p3, p22, p12, p2, p17, not_p4, p25, p8, not_p3, not_p22, not_p5}
{p3, p22, p2, p21, p17, not_p4, p25, p8, not_p3, not_p22, not_p5}
{p3, p2, p21, p17, not_p4, p25, not_p12, p8, not_p3, not_p22, not_p5}
{p3, p2, not_p21, p21, p17, not_p4, p25, p8, not_p3, not_p22, not_p5}
{p6, p3, p2, not_p4, not_p12, p8, not_p3, not_p22, not_p5}
{p6, p3, p2, not_p21, not_p4, p8, not_p3, not_p22, not_p5}
{p6, p3, p22, p2, not_p4, p8, not_p3, not_p22, not_p5}
{p3, p14, p2, not_p20, p17, not_p4, not_p12, p8, not_p22, not_p5}
{p3, p14, p2, not_p21, not_p20, p17, not_p4, p8, not_p22, not_p5}
{p3, p22, p14, p2, not_p20, p17, not_p4, p8, not_p22, not_p5}
{p3, p2, not_p20, p17, not_p4, not_p12, p8, not_p25, not_p22, not_p5}
{p3, p2, not_p21, not_p20, p17, not_p4, p8, not_p25, not_p22, not_p5}
{p3, p22, p2, not_p20, p17, not_p4, p8, not_p25, not_p22, not_p5}
{p3, p9, p2, not_p20, p17, not_p4, not_p12, p8, not_p22, not_p5}
{p3, p9, p2, not_p21, not_p20, p17, not_p4, p8, not_p22, not_p5}
{p3, p22, p9, p2, not_p20, p17, not_p4, p8, not_p22, not_p5}
{p3, p14, p2, p21, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p3, p14, p2, not_p21, p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p22, p14, p2, p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p2, p21, p17, not_p4, p25, not_p12, p8, not_p25, not_p22, not_p5}
{p3, p2, not_p21, p21, p17, not_p4, p25, p8, not_p25, not_p22, not_p5}
{p3, p22, p2, p21, p17, not_p4, p25, p8, not_p25, not_p22, not_p5}
{p3, p9, p2, p21, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p3, p9, p2, not_p21, p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p22, p9, p2, p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p22, p12, p14, p2, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p22, p12, p2, p17, not_p4, p25, p8, not_p25, not_p22, not_p5}
{p3, p22, p12, p9, p2, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p12, p2, p17, not_p4, p25, not_p12, p8, not_p25, not_p22, not_p5}
{p3, p12, p2, not_p21, p17, not_p4, p25, p8, not_p25, not_p22, not_p5}
{p3, p12, p14, p2, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p3, p12, p9, p2, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p3, p12, p9, p2, not_p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p6, p3, p2, not_p4, not_p12, p8, not_p25, not_p22, not_p5}
{p6, p3, p2, not_p21, not_p4, p8, not_p25, not_p22, not_p5}
{p6, p3, p22, p2, not_p4, p8, not_p25, not_p22, not_p5}
{p6, p3, p14, p2, not_p20, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p3, p14, p2, not_p21, not_p20, not_p4, p8, not_p22, not_p5}
{p6, p3, p22, p14, p2, not_p20, not_p4, p8, not_p22, not_p5}
{p6, p3, p14, p2, p21, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p3, p14, p2, not_p21, p21, not_p4, p8, not_p22, not_p5}
{p6, p3, p22, p14, p2, p21, not_p4, p8, not_p22, not_p5}
{p6, p3, p22, p12, p14, p2, not_p4, p8, not_p22, not_p5}
{p6, p3, p12, p14, p2, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p3, p12, p14, p2, not_p21, not_p4, p8, not_p22, not_p5}
{p6, p3, p9, p2, not_p4, not_p12, p8, not_p22, not_p5}
{p6, p3, p9, p2, not_p21, not_p4, p8, not_p22, not_p5}
{p6, p3, p22, p9, p2, not_p4, p8, not_p22, not_p5}
{p5, p3, p22, p14, p2, p17, not_p4, p8, not_p22}
{p5, p3, p22, p2, p17, not_p4, p8, not_p3, not_p22}
{p5, p3, p22, p2, p17, not_p4, p8, not_p25, not_p22}
{p5, p3, p22, p9, p2, p17, not_p4, p8, not_p22}
{p5, p3, p2, p17, not_p4, p25, not_p12, p8, not_p3, not_p22, not_p5}
{p5, p3, p2, not_p21, p17, not_p4, p25, p8, not_p3, not_p22, not_p5}
{p5, p3, p2, p21, p17, not_p4, not_p12, p8, not_p3, not_p22, not_p5}
{p5, p3, p2, not_p21, p21, p17, not_p4, p8, not_p3, not_p22, not_p5}
{p5, p3, p2, p17, not_p4, p25, not_p12, p8, not_p25, not_p22, not_p5}
{p5, p3, p2, not_p21, p17, not_p4, p25, p8, not_p25, not_p22, not_p5}
{p5, p3, p2, p21, p17, not_p4, not_p12, p8, not_p25, not_p22, not_p5}
{p5, p3, p2, not_p21, p21, p17, not_p4, p8, not_p25, not_p22, not_p5}
{p5, p3, p14, p2, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p5, p3, p14, p2, p21, p17, not_p4, not_p12, p8, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, p21, p17, not_p4, p8, not_p22, not_p5}
{p5, p3, p9, p2, p17, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p5, p3, p9, p2, not_p21, p17, not_p4, p25, p8, not_p22, not_p5}
{p5, p3, p9, p2, p21, p17, not_p4, not_p12, p8, not_p22, not_p5}
{p5, p3, p9, p2, not_p21, p21, p17, not_p4, p8, not_p22, not_p5}
{p5, p22, p14, p2, p11, not_p4, p8, not_p8, not_p22}
{p5, p3, p22, p14, p2, not_p4, p8, not_p8, not_p22}
{p5, p3, p14, p2, not_p4, p25, not_p12, p8, not_p8, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, not_p4, p25, p8, not_p8, not_p22, not_p5}
{p5, p22, p14, p2, p11, not_p4, p8, not_p19, not_p22}
{p5, p3, p22, p14, p2, not_p4, p8, not_p19, not_p22}
{p5, p14, p2, p11, not_p4, p25, not_p12, p8, not_p19, not_p22}
{p5, p14, p2, p11, not_p21, not_p4, p25, p8, not_p19, not_p22}
{p5, p3, p14, p2, not_p4, p25, not_p12, p8, not_p19, not_p22}
{p5, p3, p14, p2, not_p21, not_p4, p25, p8, not_p19, not_p22}
{p6, p5, p22, p14, p2, p11, not_p4, p8, not_p22}
{p6, p5, p3, p22, p14, p2, not_p4, p8, not_p22}
{p6, p5, p14, p2, p11, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p6, p5, p14, p2, p11, not_p21, not_p4, p25, p8, not_p22, not_p5}
{p6, p5, p3, p14, p2, not_p4, p25, not_p12, p8, not_p22, not_p5}
{p6, p5, p3, p14, p2, not_p21, not_p4, p25, p8, not_p22, not_p5}
{p3, p14, p2, not_p20, p17, p4, not_p12, not_p22, not_p5}
{p3, p14, p2, not_p21, not_p20, p17, p4, not_p22, not_p5}
{p3, p22, p14, p2, not_p20, p17, p4, not_p22, not_p5}
{p3, p2, not_p20, p17, p4, not_p12, not_p3, not_p22, not_p5}
{p3, p2, not_p21, not_p20, p17, p4, not_p3, not_p22, not_p5}
{p3, p22, p2, not_p20, p17, p4, not_p3, not_p22, not_p5}
{p3, p2, not_p20, p17, p4, not_p12, not_p25, not_p22, not_p5}
{p3, p2, not_p21, not_p20, p17, p4, not_p25, not_p22, not_p5}
{p3, p22, p2, not_p20, p17, p4, not_p25, not_p22, not_p5}
{p3, p9, p2, not_p20, p17, p4, not_p12, not_p22, not_p5}
{p3, p9, p2, not_p21, not_p20, p17, p4, not_p22, not_p5}
{p3, p22, p9, p2, not_p20, p17, p4, not_p22, not_p5}
{p3, p14, p2, p21, p17, p4, p25, not_p12, not_p22, not_p5}
{p3, p14, p2, not_p21, p21, p17, p4, p25, not_p22, not_p5}
{p3, p22, p14, p2, p21, p17, p4, p25, not_p22, not_p5}
{p3, p2, p21, p17, p4, p25, not_p12, not_p3, not_p22, not_p5}
{p3, p2, not_p21, p21, p17, p4, p25, not_p3, not_p22, not_p5}
{p3, p22, p2, p21, p17, p4, p25, not_p3, not_p22, not_p5}
{p3, p2, p21, p17, p4, p25, not_p12, not_p25, not_p22, not_p5}
{p3, p2, not_p21, p21, p17, p4, p25, not_p25, not_p22, not_p5}
{p3, p22, p2, p21, p17, p4, p25, not_p25, not_p22, not_p5}
{p3, p9, p2, p21, p17, p4, p25, not_p12, not_p22, not_p5}
{p3, p9, p2, not_p21, p21, p17, p4, p25, not_p22, not_p5}
{p3, p22, p9, p2, p21, p17, p4, p25, not_p22, not_p5}
{p3, p22, p12, p14, p2, p17, p4, p25, not_p22, not_p5}
{p3, p22, p12, p2, p17, p4, p25, not_p3, not_p22, not_p5}
{p3, p22, p12, p2, p17, p4, p25, not_p25, not_p22, not_p5}
{p3, p22, p12, p9, p2, p17, p4, p25, not_p22, not_p5}
{p3, p12, p2, p17, p4, p25, not_p12, not_p3, not_p22, not_p5}
{p3, p12, p2, not_p21, p17, p4, p25, not_p3, not_p22, not_p5}
{p3, p12, p14, p2, p17, p4, p25, not_p12, not_p22, not_p5}
{p3, p12, p2, p17, p4, p25, not_p12, not_p25, not_p22, not_p5}
{p3, p12, p9, p2, p17, p4, p25, not_p12, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, p17, p4, p25, not_p22, not_p5}
{p3, p12, p2, not_p21, p17, p4, p25, not_p25, not_p22, not_p5}
{p3, p12, p9, p2, not_p21, p17, p4, p25, not_p22, not_p5}
{p2, p11, not_p20, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p2, p11, not_p21, not_p20, p17, not_p4, not_p3, not_p22, not_p5}
{p22, p2, p11, not_p20, p17, not_p4, not_p3, not_p22, not_p5}
{p2, p11, p21, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p2, p11, not_p21, p21, p17, not_p4, not_p3, not_p22, not_p5}
{p22, p2, p11, p21, p17, not_p4, not_p3, not_p22, not_p5}
{p12, p2, p11, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p12, p2, p11, not_p21, p17, not_p4, not_p3, not_p22, not_p5}
{p22, p12, p2, p11, p17, not_p4, not_p3, not_p22, not_p5}
{p2, p11, not_p20, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p2, p11, not_p21, not_p20, p17, not_p4, not_p25, not_p22, not_p5}
{p22, p2, p11, not_p20, p17, not_p4, not_p25, not_p22, not_p5}
{p2, p11, p21, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p2, p11, not_p21, p21, p17, not_p4, not_p25, not_p22, not_p5}
{p22, p2, p11, p21, p17, not_p4, not_p25, not_p22, not_p5}
{p12, p2, p11, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p12, p2, p11, not_p21, p17, not_p4, not_p25, not_p22, not_p5}
{p22, p12, p2, p11, p17, not_p4, not_p25, not_p22, not_p5}
{p14, p2, p11, not_p20, p17, not_p4, not_p12, not_p22, not_p5}
{p14, p2, p11, not_p21, not_p20, p17, not_p4, not_p22, not_p5}
{p22, p14, p2, p11, not_p20, p17, not_p4, not_p22, not_p5}
{p9, p2, p11, not_p20, p17, not_p4, not_p12, not_p22, not_p5}
{p9, p2, p11, not_p21, not_p20, p17, not_p4, not_p22, not_p5}
{p22, p9, p2, p11, not_p20, p17, not_p4, not_p22, not_p5}
{p14, p2, p11, p21, p17, not_p4, not_p12, not_p22, not_p5}
{p14, p2, p11, not_p21, p21, p17, not_p4, not_p22, not_p5}
{p22, p14, p2, p11, p21, p17, not_p4, not_p22, not_p5}
{p9, p2, p11, p21, p17, not_p4, not_p12, not_p22, not_p5}
{p9, p2, p11, not_p21, p21, p17, not_p4, not_p22, not_p5}
{p22, p9, p2, p11, p21, p17, not_p4, not_p22, not_p5}
{p12, p14, p2, p11, p17, not_p4, not_p12, not_p22, not_p5}
{p12, p14, p2, p11, not_p21, p17, not_p4, not_p22, not_p5}
{p22, p12, p14, p2, p11, p17, not_p4, not_p22, not_p5}
{p12, p9, p2, p11, p17, not_p4, not_p12, not_p22, not_p5}
{p12, p9, p2, p11, not_p21, p17, not_p4, not_p22, not_p5}
{p22, p12, p9, p2, p11, p17, not_p4, not_p22, not_p5}
{p3, p16, p14, p2, not_p20, p17, not_p4, not_p12, not_p22, not_p5}
{p3, p16, p14, p2, not_p21, not_p20, p17, not_p4, not_p22, not_p5}
{p3, p22, p16, p14, p2, not_p20, p17, not_p4, not_p22, not_p5}
{p3, p16, p2, not_p20, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p3, p16, p2, not_p21, not_p20, p17, not_p4, not_p3, not_p22, not_p5}
{p3, p22, p16, p2, not_p20, p17, not_p4, not_p3, not_p22, not_p5}
{p3, p16, p2, not_p20, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p3, p16, p2, not_p21, not_p20, p17, not_p4, not_p25, not_p22, not_p5}
{p3, p22, p16, p2, not_p20, p17, not_p4, not_p25, not_p22, not_p5}
{p3, p16, p9, p2, not_p20, p17, not_p4, not_p12, not_p22, not_p5}
{p3, p16, p9, p2, not_p21, not_p20, p17, not_p4, not_p22, not_p5}
{p3, p22, p16, p9, p2, not_p20, p17, not_p4, not_p22, not_p5}
{p3, p16, p14, p2, p21, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p3, p16, p14, p2, not_p21, p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p22, p16, p14, p2, p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p16, p2, p21, p17, not_p4, p25, not_p12, not_p3, not_p22, not_p5}
{p3, p16, p2, not_p21, p21, p17, not_p4, p25, not_p3, not_p22, not_p5}
{p3, p22, p16, p2, p21, p17, not_p4, p25, not_p3, not_p22, not_p5}
{p3, p16, p2, p21, p17, not_p4, p25, not_p12, not_p25, not_p22, not_p5}
{p3, p16, p2, not_p21, p21, p17, not_p4, p25, not_p25, not_p22, not_p5}
{p3, p22, p16, p2, p21, p17, not_p4, p25, not_p25, not_p22, not_p5}
{p3, p16, p9, p2, p21, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p3, p16, p9, p2, not_p21, p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p22, p16, p9, p2, p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p12, p16, p2, p17, not_p4, p25, not_p12, not_p3, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, p17, not_p4, p25, not_p3, not_p22, not_p5}
{p3, p22, p12, p16, p2, p17, not_p4, p25, not_p3, not_p22, not_p5}
{p3, p12, p16, p2, p17, not_p4, p25, not_p12, not_p25, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, p17, not_p4, p25, not_p25, not_p22, not_p5}
{p3, p22, p12, p16, p2, p17, not_p4, p25, not_p25, not_p22, not_p5}
{p3, p12, p16, p14, p2, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p3, p12, p16, p14, p2, not_p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p22, p12, p16, p14, p2, p17, not_p4, p25, not_p22, not_p5}
{p3, p12, p16, p9, p2, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p3, p12, p16, p9, p2, not_p21, p17, not_p4, p25, not_p22, not_p5}
{p3, p22, p12, p16, p9, p2, p17, not_p4, p25, not_p22, not_p5}
{p5, p3, p22, p14, p2, p17, p4, not_p22}
{p5, p3, p22, p2, p17, p4, not_p3, not_p22}
{p5, p3, p22, p2, p17, p4, not_p25, not_p22}
{p5, p3, p22, p9, p2, p17, p4, not_p22}
{p5, p3, p2, p17, p4, p25, not_p12, not_p3, not_p22, not_p5}
{p5, p3, p2, not_p21, p17, p4, p25, not_p3, not_p22, not_p5}
{p5, p3, p2, p21, p17, p4, not_p12, not_p3, not_p22, not_p5}
{p5, p3, p2, not_p21, p21, p17, p4, not_p3, not_p22, not_p5}
{p5, p3, p2, p17, p4, p25, not_p12, not_p3, not_p19, not_p22}
{p5, p3, p2, not_p21, p17, p4, p25, not_p3, not_p19, not_p22}
{p5, p3, p2, p21, p17, p4, not_p12, not_p3, not_p19, not_p22}
{p5, p3, p2, not_p21, p21, p17, p4, not_p3, not_p19, not_p22}
{p5, p3, p2, p17, p4, p25, not_p12, not_p25, not_p22, not_p5}
{p5, p3, p2, not_p21, p17, p4, p25, not_p25, not_p22, not_p5}
{p5, p3, p2, p21, p17, p4, not_p12, not_p25, not_p22, not_p5}
{p5, p3, p2, not_p21, p21, p17, p4, not_p25, not_p22, not_p5}
{p5, p3, p2, p17, p4, p25, not_p12, not_p25, not_p19, not_p22}
{p5, p3, p2, not_p21, p17, p4, p25, not_p25, not_p19, not_p22}
{p5, p3, p2, p21, p17, p4, not_p12, not_p25, not_p19, not_p22}
{p5, p3, p2, not_p21, p21, p17, p4, not_p25, not_p19, not_p22}
{p5, p3, p14, p2, p17, p4, p25, not_p12, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, p17, p4, p25, not_p22, not_p5}
{p5, p3, p14, p2, p17, p4, p25, not_p12, not_p19, not_p22}
{p5, p3, p14, p2, not_p21, p17, p4, p25, not_p19, not_p22}
{p5, p3, p14, p2, p21, p17, p4, not_p12, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, p21, p17, p4, not_p22, not_p5}
{p5, p3, p14, p2, p21, p17, p4, not_p12, not_p19, not_p22}
{p5, p3, p14, p2, not_p21, p21, p17, p4, not_p19, not_p22}
{p5, p3, p9, p2, p17, p4, p25, not_p12, not_p22, not_p5}
{p5, p3, p9, p2, not_p21, p17, p4, p25, not_p22, not_p5}
{p5, p3, p9, p2, p21, p17, p4, not_p12, not_p22, not_p5}
{p5, p3, p9, p2, not_p21, p21, p17, p4, not_p22, not_p5}
{p5, p3, p9, p2, p17, p4, p25, not_p12, not_p19, not_p22}
{p5, p3, p9, p2, not_p21, p17, p4, p25, not_p19, not_p22}
{p5, p3, p9, p2, p21, p17, p4, not_p12, not_p19, not_p22}
{p5, p3, p9, p2, not_p21, p21, p17, p4, not_p19, not_p22}
{p5, p22, p2, p11, p17, not_p4, not_p3, not_p22}
{p5, p22, p2, p11, p17, not_p4, not_p25, not_p22}
{p5, p22, p14, p2, p11, p17, not_p4, not_p22}
{p5, p22, p9, p2, p11, p17, not_p4, not_p22}
{p5, p2, p11, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p5, p2, p11, not_p21, p17, not_p4, not_p3, not_p22, not_p5}
{p5, p2, p11, p17, not_p4, not_p12, not_p3, not_p19, not_p22}
{p5, p2, p11, not_p21, p17, not_p4, not_p3, not_p19, not_p22}
{p5, p2, p11, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p5, p2, p11, not_p21, p17, not_p4, not_p25, not_p22, not_p5}
{p5, p2, p11, p17, not_p4, not_p12, not_p25, not_p19, not_p22}
{p5, p2, p11, not_p21, p17, not_p4, not_p25, not_p19, not_p22}
{p5, p14, p2, p11, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p5, p14, p2, p11, not_p21, p17, not_p4, p25, not_p22, not_p5}
{p5, p14, p2, p11, p17, not_p4, p25, not_p12, not_p19, not_p22}
{p5, p14, p2, p11, not_p21, p17, not_p4, p25, not_p19, not_p22}
{p5, p9, p2, p11, p17, not_p4, not_p12, not_p22, not_p5}
{p5, p9, p2, p11, not_p21, p17, not_p4, not_p22, not_p5}
{p5, p9, p2, p11, p17, not_p4, not_p12, not_p19, not_p22}
{p5, p9, p2, p11, not_p21, p17, not_p4, not_p19, not_p22}
{p5, p3, p22, p16, p14, p2, p17, not_p4, not_p22}
{p5, p3, p22, p16, p2, p17, not_p4, not_p3, not_p22}
{p5, p3, p22, p16, p2, p17, not_p4, not_p25, not_p22}
{p5, p3, p22, p16, p9, p2, p17, not_p4, not_p22}
{p5, p3, p16, p2, p17, not_p4, p25, not_p12, not_p3, not_p22, not_p5}
{p5, p3, p16, p2, not_p21, p17, not_p4, p25, not_p3, not_p22, not_p5}
{p5, p3, p16, p2, p21, p17, not_p4, not_p12, not_p3, not_p22, not_p5}
{p5, p3, p16, p2, not_p21, p21, p17, not_p4, not_p3, not_p22, not_p5}
{p5, p3, p16, p2, p17, not_p4, p25, not_p12, not_p3, not_p19, not_p22}
{p5, p3, p16, p2, not_p21, p17, not_p4, p25, not_p3, not_p19, not_p22}
{p5, p3, p16, p2, p21, p17, not_p4, not_p12, not_p3, not_p19, not_p22}
{p5, p3, p16, p2, not_p21, p21, p17, not_p4, not_p3, not_p19, not_p22}
{p5, p3, p16, p14, p2, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p5, p3, p16, p2, p17, not_p4, p25, not_p12, not_p25, not_p22, not_p5}
{p5, p3, p16, p9, p2, p17, not_p4, p25, not_p12, not_p22, not_p5}
{p5, p3, p16, p14, p2, not_p21, p17, not_p4, p25, not_p22, not_p5}
{p5, p3, p16, p2, not_p21, p17, not_p4, p25, not_p25, not_p22, not_p5}
{p5, p3, p16, p9, p2, not_p21, p17, not_p4, p25, not_p22, not_p5}
{p5, p3, p16, p14, p2, p21, p17, not_p4, not_p12, not_p22, not_p5}
{p5, p3, p16, p2, p21, p17, not_p4, not_p12, not_p25, not_p22, not_p5}
{p5, p3, p16, p9, p2, p21, p17, not_p4, not_p12, not_p22, not_p5}
{p5, p3, p16, p2, not_p21, p21, p17, not_p4, not_p25, not_p22, not_p5}
{p5, p3, p16, p14, p2, not_p21, p21, p17, not_p4, not_p22, not_p5}
{p5, p3, p16, p9, p2, not_p21, p21, p17, not_p4, not_p22, not_p5}
{p5, p3, p16, p14, p2, p17, not_p4, p25, not_p12, not_p19, not_p22}
{p5, p3, p16, p2, p17, not_p4, p25, not_p12, not_p25, not_p19, not_p22}
{p5, p3, p16, p9, p2, p17, not_p4, p25, not_p12, not_p19, not_p22}
{p5, p3, p16, p14, p2, not_p21, p17, not_p4, p25, not_p19, not_p22}
{p5, p3, p16, p2, not_p21, p17, not_p4, p25, not_p25, not_p19, not_p22}
{p5, p3, p16, p9, p2, not_p21, p17, not_p4, p25, not_p19, not_p22}
{p5, p3, p16, p14, p2, p21, p17, not_p4, not_p12, not_p19, not_p22}
{p5, p3, p16, p2, p21, p17, not_p4, not_p12, not_p25, not_p19, not_p22}
{p5, p3, p16, p9, p2, p21, p17, not_p4, not_p12, not_p19, not_p22}
{p5, p3, p16, p14, p2, not_p21, p21, p17, not_p4, not_p19, not_p22}
{p5, p3, p16, p2, not_p21, p21, p17, not_p4, not_p25, not_p19, not_p22}
{p5, p3, p16, p9, p2, not_p21, p21, p17, not_p4, not_p19, not_p22}
{p3, p12, p14, p2, p4, not_p12, not_p8, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, p4, not_p8, not_p22, not_p5}
{p3, p22, p12, p14, p2, p4, not_p8, not_p22, not_p5}
{p3, p12, p14, p2, p4, not_p12, not_p19, not_p22, not_p5}
{p3, p12, p14, p2, not_p21, p4, not_p19, not_p22, not_p5}
{p3, p22, p12, p14, p2, p4, not_p19, not_p22, not_p5}
{p6, p3, p12, p14, p2, p4, not_p12, not_p22, not_p5}
{p6, p3, p12, p14, p2, not_p21, p4, not_p22, not_p5}
{p6, p3, p22, p12, p14, p2, p4, not_p22, not_p5}
{p3, p12, p2, p4, not_p12, not_p3, not_p8, not_p22, not_p5}
{p3, p12, p2, not_p21, p4, not_p3, not_p8, not_p22, not_p5}
{p3, p22, p12, p2, p4, not_p3, not_p8, not_p22, not_p5}
{p3, p12, p2, p4, not_p12, not_p25, not_p8, not_p22, not_p5}
{p3, p12, p2, not_p21, p4, not_p25, not_p8, not_p22, not_p5}
{p3, p22, p12, p2, p4, not_p25, not_p8, not_p22, not_p5}
{p3, p12, p9, p2, p4, not_p12, not_p8, not_p22, not_p5}
{p3, p12, p9, p2, not_p21, p4, not_p8, not_p22, not_p5}
{p3, p22, p12, p9, p2, p4, not_p8, not_p22, not_p5}
{p3, p12, p2, p4, not_p12, not_p3, not_p19, not_p22, not_p5}
{p3, p12, p2, not_p21, p4, not_p3, not_p19, not_p22, not_p5}
{p3, p22, p12, p2, p4, not_p3, not_p19, not_p22, not_p5}
{p3, p12, p2, p4, not_p12, not_p25, not_p19, not_p22, not_p5}
{p3, p12, p2, not_p21, p4, not_p25, not_p19, not_p22, not_p5}
{p3, p22, p12, p2, p4, not_p25, not_p19, not_p22, not_p5}
{p3, p12, p9, p2, p4, not_p12, not_p19, not_p22, not_p5}
{p3, p12, p9, p2, not_p21, p4, not_p19, not_p22, not_p5}
{p3, p22, p12, p9, p2, p4, not_p19, not_p22, not_p5}
{p6, p3, p12, p2, p4, not_p12, not_p3, not_p22, not_p5}
{p6, p3, p12, p2, not_p21, p4, not_p3, not_p22, not_p5}
{p6, p3, p22, p12, p2, p4, not_p3, not_p22, not_p5}
{p6, p3, p12, p2, p4, not_p12, not_p25, not_p22, not_p5}
{p6, p3, p12, p2, not_p21, p4, not_p25, not_p22, not_p5}
{p6, p3, p22, p12, p2, p4, not_p25, not_p22, not_p5}
{p6, p3, p12, p9, p2, p4, not_p12, not_p22, not_p5}
{p6, p3, p12, p9, p2, not_p21, p4, not_p22, not_p5}
{p6, p3, p22, p12, p9, p2, p4, not_p22, not_p5}
{p12, p14, p2, p11, not_p4, not_p12, not_p8, not_p22, not_p5}
{p12, p14, p2, p11, not_p21, not_p4, not_p8, not_p22, not_p5}
{p22, p12, p14, p2, p11, not_p4, not_p8, not_p22, not_p5}
{p12, p14, p2, p11, not_p4, not_p12, not_p19, not_p22, not_p5}
{p12, p14, p2, p11, not_p21, not_p4, not_p19, not_p22, not_p5}
{p22, p12, p14, p2, p11, not_p4, not_p19, not_p22, not_p5}
{p6, p12, p14, p2, p11, not_p4, not_p12, not_p22, not_p5}
{p6, p12, p14, p2, p11, not_p21, not_p4, not_p22, not_p5}
{p6, p22, p12, p14, p2, p11, not_p4, not_p22, not_p5}
{p12, p2, p11, not_p4, not_p12, not_p3, not_p8, not_p22, not_p5}
{p12, p2, p11, not_p21, not_p4, not_p3, not_p8, not_p22, not_p5}
{p22, p12, p2, p11, not_p4, not_p3, not_p8, not_p22, not_p5}
{p12, p2, p11, not_p4, not_p12, not_p25, not_p8, not_p22, not_p5}
{p12, p2, p11, not_p21, not_p4, not_p25, not_p8, not_p22, not_p5}
{p22, p12, p2, p11, not_p4, not_p25, not_p8, not_p22, not_p5}
{p12, p9, p2, p11, not_p4, not_p12, not_p8, not_p22, not_p5}
{p12, p9, p2, p11, not_p21, not_p4, not_p8, not_p22, not_p5}
{p22, p12, p9, p2, p11, not_p4, not_p8, not_p22, not_p5}
{p12, p2, p11, not_p4, not_p12, not_p3, not_p19, not_p22, not_p5}
{p12, p2, p11, not_p21, not_p4, not_p3, not_p19, not_p22, not_p5}
{p22, p12, p2, p11, not_p4, not_p3, not_p19, not_p22, not_p5}
{p12, p2, p11, not_p4, not_p12, not_p25, not_p19, not_p22, not_p5}
{p12, p2, p11, not_p21, not_p4, not_p25, not_p19, not_p22, not_p5}
{p22, p12, p2, p11, not_p4, not_p25, not_p19, not_p22, not_p5}
{p12, p9, p2, p11, not_p4, not_p12, not_p19, not_p22, not_p5}
{p12, p9, p2, p11, not_p21, not_p4, not_p19, not_p22, not_p5}
{p22, p12, p9, p2, p11, not_p4, not_p19, not_p22, not_p5}
{p6, p12, p2, p11, not_p4, not_p12, not_p3, not_p22, not_p5}
{p6, p12, p2, p11, not_p21, not_p4, not_p3, not_p22, not_p5}
{p6, p22, p12, p2, p11, not_p4, not_p3, not_p22, not_p5}
{p6, p12, p2, p11, not_p4, not_p12, not_p25, not_p22, not_p5}
{p6, p12, p2, p11, not_p21, not_p4, not_p25, not_p22, not_p5}
{p6, p22, p12, p2, p11, not_p4, not_p25, not_p22, not_p5}
{p6, p12, p9, p2, p11, not_p4, not_p12, not_p22, not_p5}
{p6, p12, p9, p2, p11, not_p21, not_p4, not_p22, not_p5}
{p6, p22, p12, p9, p2, p11, not_p4, not_p22, not_p5}
{p3, p12, p16, p14, p2, not_p4, not_p12, not_p8, not_p22, not_p5}
{p3, p12, p16, p14, p2, not_p21, not_p4, not_p8, not_p22, not_p5}
{p3, p22, p12, p16, p14, p2, not_p4, not_p8, not_p22, not_p5}
{p3, p12, p16, p14, p2, not_p4, not_p12, not_p19, not_p22, not_p5}
{p3, p12, p16, p14, p2, not_p21, not_p4, not_p19, not_p22, not_p5}
{p3, p22, p12, p16, p14, p2, not_p4, not_p19, not_p22, not_p5}
{p6, p3, p12, p16, p14, p2, not_p4, not_p12, not_p22, not_p5}
{p6, p3, p12, p16, p14, p2, not_p21, not_p4, not_p22, not_p5}
{p6, p3, p22, p12, p16, p14, p2, not_p4, not_p22, not_p5}
{p3, p12, p16, p2, not_p4, not_p12, not_p3, not_p8, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, not_p4, not_p3, not_p8, not_p22, not_p5}
{p3, p22, p12, p16, p2, not_p4, not_p3, not_p8, not_p22, not_p5}
{p3, p12, p16, p2, not_p4, not_p12, not_p25, not_p8, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, not_p4, not_p25, not_p8, not_p22, not_p5}
{p3, p22, p12, p16, p2, not_p4, not_p25, not_p8, not_p22, not_p5}
{p3, p12, p16, p9, p2, not_p4, not_p12, not_p8, not_p22, not_p5}
{p3, p12, p16, p9, p2, not_p21, not_p4, not_p8, not_p22, not_p5}
{p3, p22, p12, p16, p9, p2, not_p4, not_p8, not_p22, not_p5}
{p3, p12, p16, p2, not_p4, not_p12, not_p3, not_p19, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, not_p4, not_p3, not_p19, not_p22, not_p5}
{p3, p22, p12, p16, p2, not_p4, not_p3, not_p19, not_p22, not_p5}
{p3, p12, p16, p2, not_p4, not_p12, not_p25, not_p19, not_p22, not_p5}
{p3, p12, p16, p2, not_p21, not_p4, not_p25, not_p19, not_p22, not_p5}
{p3, p22, p12, p16, p2, not_p4, not_p25, not_p19, not_p22, not_p5}
{p3, p12, p16, p9, p2, not_p4, not_p12, not_p19, not_p22, not_p5}
{p3, p12, p16, p9, p2, not_p21, not_p4, not_p19, not_p22, not_p5}
{p3, p22, p12, p16, p9, p2, not_p4, not_p19, not_p22, not_p5}
{p6, p3, p12, p16, p2, not_p4, not_p12, not_p3, not_p22, not_p5}
{p6, p3, p12, p16, p2, not_p21, not_p4, not_p3, not_p22, not_p5}
{p6, p3, p22, p12, p16, p2, not_p4, not_p3, not_p22, not_p5}
{p6, p3, p12, p16, p2, not_p4, not_p12, not_p25, not_p22, not_p5}
{p6, p3, p12, p16, p2, not_p21, not_p4, not_p25, not_p22, not_p5}
{p6, p3, p22, p12, p16, p2, not_p4, not_p25, not_p22, not_p5}
{p6, p3, p12, p16, p9, p2, not_p4, not_p12, not_p22, not_p5}
{p6, p3, p12, p16, p9, p2, not_p21, not_p4, not_p22, not_p5}
{p6, p3, p22, p12, p16, p9, p2, not_p4, not_p22, not_p5}
{p5, p22, p14, p2, p11, not_p4, p25, not_p8, not_p22}
{p5, p14, p2, p11, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p5, p14, p2, p11, not_p21, not_p4, p25, not_p8, not_p22, not_p5}
{p5, p14, p2, p11, not_p4, p25, not_p12, not_p19, not_p8, not_p22}
{p5, p14, p2, p11, not_p21, not_p4, p25, not_p19, not_p8, not_p22}
{p2, p11, not_p4, p25, not_p12, not_p3, not_p8, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p25, not_p3, not_p8, not_p22, not_p5}
{p22, p2, p11, not_p4, p25, not_p3, not_p8, not_p22, not_p5}
{p2, p11, not_p4, p25, not_p12, not_p25, not_p8, not_p22, not_p5}
{p2, p11, not_p21, not_p4, p25, not_p25, not_p8, not_p22, not_p5}
{p22, p2, p11, not_p4, p25, not_p25, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p20, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p21, not_p20, not_p4, p25, not_p8, not_p22, not_p5}
{p22, p14, p2, p11, not_p20, not_p4, p25, not_p8, not_p22, not_p5}
{p14, p2, p11, p21, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p14, p2, p11, not_p21, p21, not_p4, p25, not_p8, not_p22, not_p5}
{p22, p14, p2, p11, p21, not_p4, p25, not_p8, not_p22, not_p5}
{p9, p2, p11, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p9, p2, p11, not_p21, not_p4, p25, not_p8, not_p22, not_p5}
{p22, p9, p2, p11, not_p4, p25, not_p8, not_p22, not_p5}
{p5, p3, p22, p16, p14, p2, not_p4, p25, not_p8, not_p22}
{p5, p3, p16, p14, p2, not_p4, p25, not_p12, not_p19, not_p8, not_p22}
{p5, p3, p16, p14, p2, not_p21, not_p4, p25, not_p19, not_p8, not_p22}
{p5, p3, p22, p14, p2, p4, p25, not_p8, not_p22}
{p5, p3, p14, p2, p4, p25, not_p12, not_p19, not_p8, not_p22}
{p5, p3, p14, p2, not_p21, p4, p25, not_p19, not_p8, not_p22}
{p3, p16, p2, not_p4, p25, not_p12, not_p3, not_p8, not_p22, not_p5}
{p3, p16, p2, not_p21, not_p4, p25, not_p3, not_p8, not_p22, not_p5}
{p3, p22, p16, p2, not_p4, p25, not_p3, not_p8, not_p22, not_p5}
{p3, p16, p2, not_p4, p25, not_p12, not_p25, not_p8, not_p22, not_p5}
{p3, p16, p2, not_p21, not_p4, p25, not_p25, not_p8, not_p22, not_p5}
{p3, p22, p16, p2, not_p4, p25, not_p25, not_p8, not_p22, not_p5}
{p3, p16, p14, p2, not_p20, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p16, p14, p2, not_p21, not_p20, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p16, p14, p2, not_p20, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p16, p14, p2, p21, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p16, p14, p2, not_p21, p21, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p16, p14, p2, p21, not_p4, p25, not_p8, not_p22, not_p5}
{p5, p3, p16, p14, p2, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p5, p3, p16, p14, p2, not_p21, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p16, p9, p2, not_p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p16, p9, p2, not_p21, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p16, p9, p2, not_p4, p25, not_p8, not_p22, not_p5}
{p3, p2, p4, p25, not_p12, not_p3, not_p8, not_p22, not_p5}
{p3, p2, not_p21, p4, p25, not_p3, not_p8, not_p22, not_p5}
{p3, p22, p2, p4, p25, not_p3, not_p8, not_p22, not_p5}
{p3, p2, p4, p25, not_p12, not_p25, not_p8, not_p22, not_p5}
{p3, p2, not_p21, p4, p25, not_p25, not_p8, not_p22, not_p5}
{p3, p22, p2, p4, p25, not_p25, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p20, p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p21, not_p20, p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p14, p2, not_p20, p4, p25, not_p8, not_p22, not_p5}
{p3, p14, p2, p21, p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p14, p2, not_p21, p21, p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p14, p2, p21, p4, p25, not_p8, not_p22, not_p5}
{p5, p3, p14, p2, p4, p25, not_p12, not_p8, not_p22, not_p5}
{p5, p3, p14, p2, not_p21, p4, p25, not_p8, not_p22, not_p5}
{p3, p9, p2, p4, p25, not_p12, not_p8, not_p22, not_p5}
{p3, p9, p2, not_p21, p4, p25, not_p8, not_p22, not_p5}
{p3, p22, p9, p2, p4, p25, not_p8, not_p22, not_p5}
"""
