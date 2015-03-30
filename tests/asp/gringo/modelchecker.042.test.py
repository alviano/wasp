input = """
p21|p22|p11|p25|not_p20:-p17. 
p17|p12|p8|p25. 
p1|p15|p15|p11:-not p5,p24. 
p12|not_p15|p16|p18:-p9,p1. 
p15|p11|p24|p3. 
p3|not_p20|p18:-p13,not p22,not p14. 
not_p2|not_p24|p9:-p1,p12,not p11. 
p17|p12|p8|not_p8. 
p3|p20|p18:-p1,not p22. 
p24|p9|p24|p10:-p12,p23. 
p15|p4|p3|p1:-p12,p5. 
p6|p9|p6|not_p22:-p19,p23. 
not_p25|p14|p9|not_p3. 
:-p19,not_p19. 
p3|p21|not_p20:-p6,p23,not p4. 
p6|p4|p7|p18:-p9,p1. 
p24|p9|p24|not_p22:-p12,p23. 
p5|p11|p25|not_p20:-p17,not p7. 
p15|not_p22|p24|not_p12. 
p6|not_p15|p7|p17:-p6,p1. 
p6|p7:-p19,not p23,not p3,not p11. 
p12|not_p15|p16|p17:-p6,p1. 
p11|not_p20|not_p1|not_p4:-p24,p2. 
p17|p6|not_p19|not_p8. 
p11|p8|p4|p16. 
p3|p20|p18|p3:-p1,not p22. 
p24|not_p8|p6|not_p22:-p12,p23. 
p19|p1|p12:-p19,not p23,not p11. 
not_p5|not_p5|not_p19|p22. 
not_p23|p7|p21|p5:-p13,not p3. 
"""
output = """
{p11, p8, p24, not_p3, not_p19}
{p8, p1, p24, p20, not_p3, not_p19}
{p8, p1, p24, p3, not_p3, not_p19}
{p8, p1, p24, p18, not_p3, not_p19}
{p11, p8, p24, p14, not_p19}
{p8, p1, p24, p20, p14, not_p19}
{p8, p1, p24, p3, p14, not_p19}
{p8, p1, p24, p18, p14, not_p19}
{p11, p8, p24, not_p25, not_p19}
{p8, p1, p24, p20, not_p25, not_p19}
{p8, p1, p24, p3, not_p25, not_p19}
{p8, p1, p24, p18, not_p25, not_p19}
{p11, p8, p24, p9, not_p19}
{p8, p1, p24, p18, p9, not_p19}
{p8, p1, p24, p16, p9, p20, p7, not_p19}
{p8, p1, p24, not_p15, p9, p20, p7, not_p19}
{p12, p8, p1, p24, p9, p20, p7, not_p19}
{p8, p1, p24, p16, p9, p20, p6, not_p19}
{p8, p1, p24, not_p15, p9, p20, p6, not_p19}
{p12, p8, p1, p24, p9, p20, p6, not_p19}
{p8, p1, p24, p16, p9, p20, p4, not_p19}
{p8, p1, p24, not_p15, p9, p20, p4, not_p19}
{p8, p1, p24, p16, p9, p3, p7, not_p19}
{p8, p1, p24, not_p15, p9, p3, p7, not_p19}
{p12, p8, p1, p24, p9, p3, p7, not_p19}
{p8, p1, p24, p16, p9, p3, p6, not_p19}
{p8, p1, p24, not_p15, p9, p3, p6, not_p19}
{p12, p8, p1, p24, p9, p3, p6, not_p19}
{p8, p1, p24, p16, p9, p3, p4, not_p19}
{p8, p1, p24, not_p15, p9, p3, p4, not_p19}
{p8, p15, not_p3, not_p19}
{p8, p15, p14, not_p19}
{p8, p15, not_p25, not_p19}
{p8, p15, p9, not_p19}
{p8, p3, not_p3, not_p12, not_p19}
{p11, p8, not_p3, not_p12, not_p19}
{p8, p3, not_p3, not_p22, not_p19}
{p11, p8, not_p3, not_p22, not_p19}
{p8, p3, p14, not_p12, not_p19}
{p11, p8, p14, not_p12, not_p19}
{p8, p3, p14, not_p22, not_p19}
{p11, p8, p14, not_p22, not_p19}
{p8, p3, not_p25, not_p12, not_p19}
{p11, p8, not_p25, not_p12, not_p19}
{p8, p3, not_p25, not_p22, not_p19}
{p11, p8, not_p25, not_p22, not_p19}
{p8, p9, p3, not_p12, not_p19}
{p11, p8, p9, not_p12, not_p19}
{p8, p9, p3, not_p22, not_p19}
{p11, p8, p9, not_p22, not_p19}
{p21, p17, p8, p24, p5, not_p3, not_p5}
{p21, p17, p8, p24, p5, p14, not_p5}
{p21, p17, p8, p24, p5, not_p25, not_p5}
{p21, p17, p8, p24, p5, p9, not_p5}
{p22, p17, p8, p24, p5, not_p3}
{p22, p17, p8, p24, p5, p14}
{p22, p17, p8, p24, p5, not_p25}
{p22, p17, p8, p24, p5, p9}
{p11, p8, p24, not_p8, not_p3, not_p5}
{p11, p8, p24, not_p3, p6, not_p5}
{p8, p1, p24, not_p8, p20, not_p3, not_p5}
{p8, p1, p24, p3, not_p8, not_p3, not_p5}
{p8, p1, p24, p18, not_p8, not_p3, not_p5}
{p8, p1, p24, p16, p20, not_p3, p6, not_p5}
{p8, p1, p24, not_p15, p20, not_p3, p6, not_p5}
{p12, p8, p1, p24, not_p24, p20, not_p3, p6, not_p5}
{p12, p8, p1, p24, not_p2, p20, not_p3, p6, not_p5}
{p8, p1, p24, p16, p3, not_p3, p6, not_p5}
{p8, p1, p24, not_p15, p3, not_p3, p6, not_p5}
{p12, p8, p1, p24, p3, not_p24, not_p3, p6, not_p5}
{p12, p8, p1, p24, p3, not_p2, not_p3, p6, not_p5}
{p8, p1, p24, p16, p18, not_p3, p6, not_p5}
{p8, p1, p24, not_p15, p18, not_p3, p6, not_p5}
{p12, p8, p1, p24, p18, not_p24, not_p3, p6, not_p5}
{p12, p8, p1, p24, p18, not_p2, not_p3, p6, not_p5}
{not_p20, p17, p8, p1, p24, p20, not_p3, not_p5}
{not_p20, p17, p8, p1, p24, p3, not_p3, not_p5}
{not_p20, p17, p8, p1, p24, p18, not_p3, not_p5}
{p25, p17, p8, p1, p24, p20, not_p3, not_p5}
{p25, p17, p8, p1, p24, p3, not_p3, not_p5}
{p25, p17, p8, p1, p24, p18, not_p3, not_p5}
{p11, p8, p24, not_p8, p14, not_p5}
{p11, p8, p24, p14, p6, not_p5}
{p8, p1, p24, not_p8, p20, p14, not_p5}
{p8, p1, p24, p3, not_p8, p14, not_p5}
{p8, p1, p24, p18, not_p8, p14, not_p5}
{p8, p1, p24, p16, p20, p14, p6, not_p5}
{p8, p1, p24, not_p15, p20, p14, p6, not_p5}
{p12, p8, p1, p24, not_p24, p20, p14, p6, not_p5}
{p12, p8, p1, p24, not_p2, p20, p14, p6, not_p5}
{p8, p1, p24, p16, p3, p14, p6, not_p5}
{p8, p1, p24, not_p15, p3, p14, p6, not_p5}
{p12, p8, p1, p24, p3, not_p24, p14, p6, not_p5}
{p12, p8, p1, p24, p3, not_p2, p14, p6, not_p5}
{p8, p1, p24, p16, p18, p14, p6, not_p5}
{p8, p1, p24, not_p15, p18, p14, p6, not_p5}
{p12, p8, p1, p24, p18, not_p24, p14, p6, not_p5}
{p12, p8, p1, p24, p18, not_p2, p14, p6, not_p5}
{not_p20, p17, p8, p1, p24, p20, p14, not_p5}
{not_p20, p17, p8, p1, p24, p3, p14, not_p5}
{not_p20, p17, p8, p1, p24, p18, p14, not_p5}
{p25, p17, p8, p1, p24, p20, p14, not_p5}
{p25, p17, p8, p1, p24, p3, p14, not_p5}
{p25, p17, p8, p1, p24, p18, p14, not_p5}
{p11, p8, p24, not_p8, not_p25, not_p5}
{p11, p8, p24, not_p25, p6, not_p5}
{p8, p1, p24, not_p8, p20, not_p25, not_p5}
{p8, p1, p24, p3, not_p8, not_p25, not_p5}
{p8, p1, p24, p18, not_p8, not_p25, not_p5}
{p8, p1, p24, p16, p20, not_p25, p6, not_p5}
{p8, p1, p24, not_p15, p20, not_p25, p6, not_p5}
{p12, p8, p1, p24, not_p24, p20, not_p25, p6, not_p5}
{p12, p8, p1, p24, not_p2, p20, not_p25, p6, not_p5}
{p8, p1, p24, p16, p3, not_p25, p6, not_p5}
{p8, p1, p24, not_p15, p3, not_p25, p6, not_p5}
{p12, p8, p1, p24, p3, not_p24, not_p25, p6, not_p5}
{p12, p8, p1, p24, p3, not_p2, not_p25, p6, not_p5}
{p8, p1, p24, p16, p18, not_p25, p6, not_p5}
{p8, p1, p24, not_p15, p18, not_p25, p6, not_p5}
{p12, p8, p1, p24, p18, not_p24, not_p25, p6, not_p5}
{p12, p8, p1, p24, p18, not_p2, not_p25, p6, not_p5}
{not_p20, p17, p8, p1, p24, p20, not_p25, not_p5}
{not_p20, p17, p8, p1, p24, p3, not_p25, not_p5}
{not_p20, p17, p8, p1, p24, p18, not_p25, not_p5}
{p25, p17, p8, p1, p24, p20, not_p25, not_p5}
{p25, p17, p8, p1, p24, p3, not_p25, not_p5}
{p25, p17, p8, p1, p24, p18, not_p25, not_p5}
{p11, p8, p24, p9, not_p8, not_p5}
{p8, p1, p24, p18, p9, not_p8, not_p5}
{p8, p1, p24, p16, p9, not_p8, p20, p7, not_p5}
{p8, p1, p24, not_p15, p9, not_p8, p20, p7, not_p5}
{p12, p8, p1, p24, p9, not_p8, p20, p7, not_p5}
{p8, p1, p24, p16, p9, not_p8, p20, p4, not_p5}
{p8, p1, p24, not_p15, p9, not_p8, p20, p4, not_p5}
{p8, p1, p24, p16, p9, p3, not_p8, p7, not_p5}
{p8, p1, p24, not_p15, p9, p3, not_p8, p7, not_p5}
{p12, p8, p1, p24, p9, p3, not_p8, p7, not_p5}
{p8, p1, p24, p16, p9, p3, not_p8, p4, not_p5}
{p8, p1, p24, not_p15, p9, p3, not_p8, p4, not_p5}
{p11, p8, p24, p9, p6, not_p5}
{p8, p1, p24, p16, p18, p9, p6, not_p5}
{p8, p1, p24, not_p15, p18, p9, p6, not_p5}
{p12, p8, p1, p24, p18, p9, p6, not_p5}
{not_p20, p17, p8, p1, p24, p18, p9, not_p5}
{p25, p17, p8, p1, p24, p18, p9, not_p5}
{p8, p1, p24, p16, p9, p20, p6, not_p5}
{p8, p1, p24, not_p15, p9, p20, p6, not_p5}
{p12, p8, p1, p24, p9, p20, p6, not_p5}
{p8, p1, p24, p16, p9, p3, p6, not_p5}
{p8, p1, p24, not_p15, p9, p3, p6, not_p5}
{p12, p8, p1, p24, p9, p3, p6, not_p5}
{not_p20, p17, p8, p1, p24, not_p15, p9, p20, p7, not_p5}
{p25, p17, p8, p1, p24, not_p15, p9, p20, p7, not_p5}
{p21, p17, p8, p1, p24, not_p15, p9, p20, p7, not_p5}
{not_p20, p17, p12, p8, p1, p24, p9, p20, p7, not_p5}
{p25, p17, p12, p8, p1, p24, p9, p20, p7, not_p5}
{p21, p17, p12, p8, p1, p24, p9, p20, p7, not_p5}
{not_p20, p17, p8, p1, p24, not_p15, p9, p3, p7, not_p5}
{p25, p17, p8, p1, p24, not_p15, p9, p3, p7, not_p5}
{p21, p17, p8, p1, p24, not_p15, p9, p3, p7, not_p5}
{not_p20, p17, p12, p8, p1, p24, p9, p3, p7, not_p5}
{p25, p17, p12, p8, p1, p24, p9, p3, p7, not_p5}
{p21, p17, p12, p8, p1, p24, p9, p3, p7, not_p5}
{p22, p11, p8, p24, not_p8, not_p3}
{p22, p8, p1, p24, not_p8, not_p3}
{p22, p11, p8, p24, not_p3, p6}
{p22, p8, p1, p24, p16, not_p3, p6}
{p22, p8, p1, p24, not_p15, not_p3, p6}
{p22, p12, p8, p1, p24, not_p24, not_p3, p6}
{p22, p12, p8, p1, p24, not_p2, not_p3, p6}
{p22, not_p20, p17, p8, p1, p24, not_p3}
{p22, p25, p17, p8, p1, p24, not_p3}
{p22, p11, p8, p24, not_p8, p14}
{p22, p8, p1, p24, not_p8, p14}
{p22, p11, p8, p24, p14, p6}
{p22, p8, p1, p24, p16, p14, p6}
{p22, p8, p1, p24, not_p15, p14, p6}
{p22, p12, p8, p1, p24, not_p24, p14, p6}
{p22, p12, p8, p1, p24, not_p2, p14, p6}
{p22, not_p20, p17, p8, p1, p24, p14}
{p22, p25, p17, p8, p1, p24, p14}
{p22, p11, p8, p24, not_p8, not_p25}
{p22, p11, p8, p24, not_p25, p6}
{p22, p8, p1, p24, not_p8, not_p25}
{p22, p8, p1, p24, p16, not_p25, p6}
{p22, p8, p1, p24, not_p15, not_p25, p6}
{p22, p12, p8, p1, p24, not_p24, not_p25, p6}
{p22, p12, p8, p1, p24, not_p2, not_p25, p6}
{p22, not_p20, p17, p8, p1, p24, not_p25}
{p22, p25, p17, p8, p1, p24, not_p25}
{p22, p11, p8, p24, p9, not_p8}
{p22, p8, p1, p24, p18, p9, not_p8}
{p22, p8, p1, p24, p16, p9, not_p8, p7}
{p22, p8, p1, p24, not_p15, p9, not_p8, p7}
{p22, p12, p8, p1, p24, p9, not_p8, p7}
{p22, p8, p1, p24, p16, p9, not_p8, p4}
{p22, p8, p1, p24, not_p15, p9, not_p8, p4}
{p22, p11, p8, p24, p9, p6}
{p22, p8, p1, p24, p16, p9, p6}
{p22, p8, p1, p24, not_p15, p9, p6}
{p22, p12, p8, p1, p24, p9, p6}
{p22, not_p20, p17, p8, p1, p24, p18, p9}
{p22, p25, p17, p8, p1, p24, p18, p9}
{p22, p17, p8, p1, p24, not_p15, p9, p7}
{p22, p17, p12, p8, p1, p24, p9, p7}
{p8, p15, not_p8, not_p3, not_p5}
{p8, p15, not_p3, p6, not_p5}
{not_p20, p17, p8, p15, not_p3, not_p5}
{p25, p17, p8, p15, not_p3, not_p5}
{p21, p17, p8, p15, p5, not_p3, not_p5}
{p8, p15, not_p8, p14, not_p5}
{p8, p15, p14, p6, not_p5}
{not_p20, p17, p8, p15, p14, not_p5}
{p25, p17, p8, p15, p14, not_p5}
{p21, p17, p8, p15, p5, p14, not_p5}
{p8, p15, not_p8, not_p25, not_p5}
{p8, p15, not_p25, p6, not_p5}
{not_p20, p17, p8, p15, not_p25, not_p5}
{p25, p17, p8, p15, not_p25, not_p5}
{p21, p17, p8, p15, p5, not_p25, not_p5}
{p8, p15, p9, not_p8, not_p5}
{p8, p15, p9, p6, not_p5}
{not_p20, p17, p8, p15, p9, not_p5}
{p25, p17, p8, p15, p9, not_p5}
{p21, p17, p8, p15, p5, p9, not_p5}
{p22, p8, p15, not_p8, not_p3}
{p22, p8, p15, not_p3, p6}
{p22, not_p20, p17, p8, p15, not_p3}
{p22, p25, p17, p8, p15, not_p3}
{p22, p17, p8, p15, p5, not_p3}
{p22, p8, p15, not_p8, p14}
{p22, p8, p15, p14, p6}
{p22, not_p20, p17, p8, p15, p14}
{p22, p25, p17, p8, p15, p14}
{p22, p17, p8, p15, p5, p14}
{p22, p8, p15, not_p8, not_p25}
{p22, p8, p15, not_p25, p6}
{p22, not_p20, p17, p8, p15, not_p25}
{p22, p25, p17, p8, p15, not_p25}
{p22, p17, p8, p15, p5, not_p25}
{p22, p8, p15, p9, not_p8}
{p22, p8, p15, p9, p6}
{p22, not_p20, p17, p8, p15, p9}
{p22, p25, p17, p8, p15, p9}
{p22, p17, p8, p15, p5, p9}
{p8, p3, not_p8, not_p3, not_p12, not_p5}
{p8, p3, not_p3, p6, not_p12, not_p5}
{not_p20, p17, p8, p3, not_p3, not_p12, not_p5}
{p25, p17, p8, p3, not_p3, not_p12, not_p5}
{p21, p17, p8, p5, p3, not_p3, not_p12, not_p5}
{p11, p8, not_p8, not_p3, not_p12, not_p5}
{p11, p8, not_p3, p6, not_p12, not_p5}
{p8, p3, not_p8, not_p3, not_p22, not_p5}
{p8, p3, not_p3, p6, not_p22, not_p5}
{not_p20, p17, p8, p3, not_p3, not_p22, not_p5}
{p25, p17, p8, p3, not_p3, not_p22, not_p5}
{p21, p17, p8, p5, p3, not_p3, not_p22, not_p5}
{p11, p8, not_p8, not_p3, not_p22, not_p5}
{p11, p8, not_p3, p6, not_p22, not_p5}
{p22, p8, p3, not_p8, not_p3, not_p12}
{p22, p8, p3, not_p3, p6, not_p12}
{p22, not_p20, p17, p8, p3, not_p3, not_p12}
{p22, p25, p17, p8, p3, not_p3, not_p12}
{p22, p17, p8, p5, p3, not_p3, not_p12}
{p22, p11, p8, not_p8, not_p3, not_p12}
{p22, p11, p8, not_p3, p6, not_p12}
{p22, p8, p3, not_p8, not_p3, not_p22}
{p22, p8, p3, not_p3, p6, not_p22}
{p22, not_p20, p17, p8, p3, not_p3, not_p22}
{p22, p25, p17, p8, p3, not_p3, not_p22}
{p22, p17, p8, p5, p3, not_p3, not_p22}
{p22, p11, p8, not_p8, not_p3, not_p22}
{p22, p11, p8, not_p3, p6, not_p22}
{p8, p3, not_p8, p14, not_p12, not_p5}
{p8, p3, p14, p6, not_p12, not_p5}
{not_p20, p17, p8, p3, p14, not_p12, not_p5}
{p25, p17, p8, p3, p14, not_p12, not_p5}
{p21, p17, p8, p5, p3, p14, not_p12, not_p5}
{p11, p8, not_p8, p14, not_p12, not_p5}
{p11, p8, p14, p6, not_p12, not_p5}
{p8, p3, not_p8, p14, not_p22, not_p5}
{p8, p3, p14, p6, not_p22, not_p5}
{not_p20, p17, p8, p3, p14, not_p22, not_p5}
{p25, p17, p8, p3, p14, not_p22, not_p5}
{p21, p17, p8, p5, p3, p14, not_p22, not_p5}
{p11, p8, not_p8, p14, not_p22, not_p5}
{p11, p8, p14, p6, not_p22, not_p5}
{p22, p8, p3, not_p8, p14, not_p12}
{p22, p8, p3, p14, p6, not_p12}
{p22, not_p20, p17, p8, p3, p14, not_p12}
{p22, p25, p17, p8, p3, p14, not_p12}
{p22, p17, p8, p5, p3, p14, not_p12}
{p22, p11, p8, not_p8, p14, not_p12}
{p22, p11, p8, p14, p6, not_p12}
{p22, p8, p3, not_p8, p14, not_p22}
{p22, p8, p3, p14, p6, not_p22}
{p22, not_p20, p17, p8, p3, p14, not_p22}
{p22, p25, p17, p8, p3, p14, not_p22}
{p22, p17, p8, p5, p3, p14, not_p22}
{p22, p11, p8, not_p8, p14, not_p22}
{p22, p11, p8, p14, p6, not_p22}
{p8, p3, not_p8, not_p25, not_p12, not_p5}
{p8, p3, not_p25, p6, not_p12, not_p5}
{not_p20, p17, p8, p3, not_p25, not_p12, not_p5}
{p25, p17, p8, p3, not_p25, not_p12, not_p5}
{p21, p17, p8, p5, p3, not_p25, not_p12, not_p5}
{p11, p8, not_p8, not_p25, not_p12, not_p5}
{p11, p8, not_p25, p6, not_p12, not_p5}
{p8, p3, not_p8, not_p25, not_p22, not_p5}
{p8, p3, not_p25, p6, not_p22, not_p5}
{not_p20, p17, p8, p3, not_p25, not_p22, not_p5}
{p25, p17, p8, p3, not_p25, not_p22, not_p5}
{p21, p17, p8, p5, p3, not_p25, not_p22, not_p5}
{p11, p8, not_p8, not_p25, not_p22, not_p5}
{p11, p8, not_p25, p6, not_p22, not_p5}
{p22, p8, p3, not_p8, not_p25, not_p12}
{p22, p8, p3, not_p25, p6, not_p12}
{p22, not_p20, p17, p8, p3, not_p25, not_p12}
{p22, p25, p17, p8, p3, not_p25, not_p12}
{p22, p17, p8, p5, p3, not_p25, not_p12}
{p22, p11, p8, not_p8, not_p25, not_p12}
{p22, p11, p8, not_p25, p6, not_p12}
{p22, p8, p3, not_p8, not_p25, not_p22}
{p22, p8, p3, not_p25, p6, not_p22}
{p22, not_p20, p17, p8, p3, not_p25, not_p22}
{p22, p25, p17, p8, p3, not_p25, not_p22}
{p22, p17, p8, p5, p3, not_p25, not_p22}
{p22, p11, p8, not_p8, not_p25, not_p22}
{p22, p11, p8, not_p25, p6, not_p22}
{p8, p9, p3, not_p8, not_p12, not_p5}
{p8, p9, p3, p6, not_p12, not_p5}
{not_p20, p17, p8, p9, p3, not_p12, not_p5}
{p25, p17, p8, p9, p3, not_p12, not_p5}
{p21, p17, p8, p5, p9, p3, not_p12, not_p5}
{p11, p8, p9, not_p8, not_p12, not_p5}
{p11, p8, p9, p6, not_p12, not_p5}
{p8, p9, p3, not_p8, not_p22, not_p5}
{p8, p9, p3, p6, not_p22, not_p5}
{not_p20, p17, p8, p9, p3, not_p22, not_p5}
{p25, p17, p8, p9, p3, not_p22, not_p5}
{p21, p17, p8, p5, p9, p3, not_p22, not_p5}
{p11, p8, p9, not_p8, not_p22, not_p5}
{p11, p8, p9, p6, not_p22, not_p5}
{p22, p8, p9, p3, not_p8, not_p12}
{p22, p8, p9, p3, p6, not_p12}
{p22, not_p20, p17, p8, p9, p3, not_p12}
{p22, p25, p17, p8, p9, p3, not_p12}
{p22, p17, p8, p5, p9, p3, not_p12}
{p22, p11, p8, p9, not_p8, not_p12}
{p22, p11, p8, p9, p6, not_p12}
{p22, p8, p9, p3, not_p8, not_p22}
{p22, p8, p9, p3, p6, not_p22}
{p22, not_p20, p17, p8, p9, p3, not_p22}
{p22, p25, p17, p8, p9, p3, not_p22}
{p22, p17, p8, p5, p9, p3, not_p22}
{p22, p11, p8, p9, not_p8, not_p22}
{p22, p11, p8, p9, p6, not_p22}
{p11, p17, p24, not_p3, not_p5}
{p11, p17, p24, not_p3, not_p19}
{p22, p11, p17, p24, not_p3}
{p11, p17, not_p3, not_p12, not_p5}
{p11, p17, not_p3, not_p22, not_p5}
{p11, p17, p15, not_p3, not_p5}
{p11, p17, not_p3, not_p12, not_p19}
{p11, p17, not_p3, not_p22, not_p19}
{p11, p17, p15, not_p3, not_p19}
{p22, p11, p17, not_p3, not_p12}
{p22, p11, p17, not_p3, not_p22}
{p22, p11, p17, p15, not_p3}
{p11, p17, p24, p14, not_p5}
{p11, p17, p24, p14, not_p19}
{p22, p11, p17, p24, p14}
{p11, p17, p14, not_p12, not_p5}
{p11, p17, p14, not_p22, not_p5}
{p11, p17, p15, p14, not_p5}
{p11, p17, p14, not_p12, not_p19}
{p11, p17, p14, not_p22, not_p19}
{p11, p17, p15, p14, not_p19}
{p22, p11, p17, p14, not_p12}
{p22, p11, p17, p14, not_p22}
{p22, p11, p17, p15, p14}
{p11, p17, p24, not_p25, not_p5}
{p11, p17, p24, not_p25, not_p19}
{p22, p11, p17, p24, not_p25}
{p11, p17, not_p25, not_p12, not_p5}
{p11, p17, not_p25, not_p22, not_p5}
{p11, p17, p15, not_p25, not_p5}
{p11, p17, not_p25, not_p12, not_p19}
{p11, p17, not_p25, not_p22, not_p19}
{p11, p17, p15, not_p25, not_p19}
{p22, p11, p17, not_p25, not_p12}
{p22, p11, p17, not_p25, not_p22}
{p22, p11, p17, p15, not_p25}
{p11, p17, p24, p9, not_p5}
{p11, p17, p24, p9, not_p19}
{p22, p11, p17, p24, p9}
{p11, p17, p9, not_p12, not_p5}
{p11, p17, p9, not_p22, not_p5}
{p11, p17, p15, p9, not_p5}
{p11, p17, p9, not_p12, not_p19}
{p11, p17, p9, not_p22, not_p19}
{p11, p17, p15, p9, not_p19}
{p22, p11, p17, p9, not_p12}
{p22, p11, p17, p9, not_p22}
{p22, p11, p17, p15, p9}
{p22, not_p20, p17, p1, p24, p4, not_p3}
{not_p20, p17, p1, p24, p20, p4, not_p3, not_p5}
{not_p20, p17, p1, p24, p3, p4, not_p3, not_p5}
{not_p20, p17, p1, p24, p18, p4, not_p3, not_p5}
{not_p20, p17, p1, p24, p20, p4, not_p3, not_p19}
{not_p20, p17, p1, p24, p3, p4, not_p3, not_p19}
{not_p20, p17, p1, p24, p18, p4, not_p3, not_p19}
{p22, not_p20, p17, p1, p24, p4, p14}
{not_p20, p17, p1, p24, p20, p4, p14, not_p5}
{not_p20, p17, p1, p24, p3, p4, p14, not_p5}
{not_p20, p17, p1, p24, p18, p4, p14, not_p5}
{not_p20, p17, p1, p24, p20, p4, p14, not_p19}
{not_p20, p17, p1, p24, p3, p4, p14, not_p19}
{not_p20, p17, p1, p24, p18, p4, p14, not_p19}
{p22, not_p20, p17, p1, p24, p4, not_p25}
{not_p20, p17, p1, p24, p20, p4, not_p25, not_p5}
{not_p20, p17, p1, p24, p3, p4, not_p25, not_p5}
{not_p20, p17, p1, p24, p18, p4, not_p25, not_p5}
{not_p20, p17, p1, p24, p20, p4, not_p25, not_p19}
{not_p20, p17, p1, p24, p3, p4, not_p25, not_p19}
{not_p20, p17, p1, p24, p18, p4, not_p25, not_p19}
{not_p20, p17, p1, p24, p18, p9, p4, not_p5}
{not_p20, p17, p1, p24, p16, p9, p20, p4, not_p5}
{not_p20, p17, p1, p24, not_p15, p9, p20, p4, not_p5}
{not_p20, p17, p12, p1, p24, p9, p20, p4, not_p5}
{not_p20, p17, p1, p24, p16, p9, p3, p4, not_p5}
{not_p20, p17, p1, p24, not_p15, p9, p3, p4, not_p5}
{not_p20, p17, p12, p1, p24, p9, p3, p4, not_p5}
{p22, not_p20, p17, p1, p24, p18, p9, p4}
{p22, not_p20, p17, p1, p24, p16, p9, p4}
{p22, not_p20, p17, p1, p24, not_p15, p9, p4}
{p22, not_p20, p17, p12, p1, p24, p9, p4}
{not_p20, p17, p1, p24, p18, p9, p4, not_p19}
{not_p20, p17, p1, p24, p16, p9, p20, p4, not_p19}
{not_p20, p17, p1, p24, not_p15, p9, p20, p4, not_p19}
{not_p20, p17, p1, p24, p16, p9, p3, p4, not_p19}
{not_p20, p17, p1, p24, not_p15, p9, p3, p4, not_p19}
{p22, p25, p17, p1, p24, p4, not_p3}
{p25, p17, p1, p24, p20, p4, not_p3, not_p5}
{p25, p17, p1, p24, p3, p4, not_p3, not_p5}
{p25, p17, p1, p24, p18, p4, not_p3, not_p5}
{p25, p17, p1, p24, p20, p4, not_p3, not_p19}
{p25, p17, p1, p24, p3, p4, not_p3, not_p19}
{p25, p17, p1, p24, p18, p4, not_p3, not_p19}
{p22, p25, p17, p1, p24, p4, p14}
{p25, p17, p1, p24, p20, p4, p14, not_p5}
{p25, p17, p1, p24, p3, p4, p14, not_p5}
{p25, p17, p1, p24, p18, p4, p14, not_p5}
{p25, p17, p1, p24, p20, p4, p14, not_p19}
{p25, p17, p1, p24, p3, p4, p14, not_p19}
{p25, p17, p1, p24, p18, p4, p14, not_p19}
{p22, p25, p17, p1, p24, p4, not_p25}
{p25, p17, p1, p24, p20, p4, not_p25, not_p5}
{p25, p17, p1, p24, p3, p4, not_p25, not_p5}
{p25, p17, p1, p24, p18, p4, not_p25, not_p5}
{p25, p17, p1, p24, p20, p4, not_p25, not_p19}
{p25, p17, p1, p24, p3, p4, not_p25, not_p19}
{p25, p17, p1, p24, p18, p4, not_p25, not_p19}
{p25, p17, p1, p24, p18, p9, p4, not_p5}
{p25, p17, p1, p24, p16, p9, p20, p4, not_p5}
{p25, p17, p1, p24, not_p15, p9, p20, p4, not_p5}
{p25, p17, p12, p1, p24, p9, p20, p4, not_p5}
{p25, p17, p1, p24, p16, p9, p3, p4, not_p5}
{p25, p17, p1, p24, not_p15, p9, p3, p4, not_p5}
{p25, p17, p12, p1, p24, p9, p3, p4, not_p5}
{p22, p25, p17, p1, p24, p18, p9, p4}
{p22, p25, p17, p1, p24, p16, p9, p4}
{p22, p25, p17, p1, p24, not_p15, p9, p4}
{p22, p25, p17, p12, p1, p24, p9, p4}
{p25, p17, p1, p24, p18, p9, p4, not_p19}
{p25, p17, p1, p24, p16, p9, p20, p4, not_p19}
{p25, p17, p1, p24, not_p15, p9, p20, p4, not_p19}
{p25, p17, p1, p24, p16, p9, p3, p4, not_p19}
{p25, p17, p1, p24, not_p15, p9, p3, p4, not_p19}
{p21, p17, p24, p5, p4, not_p3, not_p5}
{p21, p17, p24, p5, p4, p14, not_p5}
{p21, p17, p24, p5, p4, not_p25, not_p5}
{p21, p17, p24, p5, p9, p4, not_p5}
{p21, p17, p24, p5, p4, not_p3, not_p19}
{p21, p17, p24, p5, p4, p14, not_p19}
{p21, p17, p24, p5, p4, not_p25, not_p19}
{p21, p17, p24, p5, p9, p4, not_p19}
{p22, p17, p24, p5, p4, not_p3}
{p22, p17, p24, p5, p4, p14}
{p22, p17, p24, p5, p4, not_p25}
{p22, p17, p24, p5, p9, p4}
{p22, p17, p1, p24, p16, p9, p7}
{p22, not_p20, p17, p1, p24, p16, not_p3}
{p22, p25, p17, p1, p24, p16, not_p3}
{p22, p17, p24, p5, p16, not_p3}
{p22, not_p20, p17, p1, p24, p16, p14}
{p22, p25, p17, p1, p24, p16, p14}
{p22, p17, p24, p5, p16, p14}
{p22, not_p20, p17, p1, p24, p16, not_p25}
{p22, p25, p17, p1, p24, p16, not_p25}
{p22, p17, p24, p5, p16, not_p25}
{p22, p17, p24, p5, p16, p9}
{p22, not_p20, p17, p1, p24, p16, p9, p6}
{p22, p25, p17, p1, p24, p16, p9, p6}
{p22, not_p20, p17, p1, p24, p16, p18, p9}
{p22, p25, p17, p1, p24, p16, p18, p9}
{not_p20, p17, p1, p24, p16, p20, not_p3, not_p5}
{not_p20, p17, p1, p24, p16, p3, not_p3, not_p5}
{not_p20, p17, p1, p24, p16, p18, not_p3, not_p5}
{not_p20, p17, p1, p24, p16, p20, not_p3, not_p19}
{not_p20, p17, p1, p24, p16, p3, not_p3, not_p19}
{not_p20, p17, p1, p24, p16, p18, not_p3, not_p19}
{p25, p17, p1, p24, p16, p20, not_p3, not_p5}
{p25, p17, p1, p24, p16, p3, not_p3, not_p5}
{p25, p17, p1, p24, p16, p18, not_p3, not_p5}
{p25, p17, p1, p24, p16, p20, not_p3, not_p19}
{p25, p17, p1, p24, p16, p3, not_p3, not_p19}
{p25, p17, p1, p24, p16, p18, not_p3, not_p19}
{p21, p17, p24, p5, p16, not_p3, not_p5}
{p21, p17, p24, p5, p16, not_p3, not_p19}
{not_p20, p17, p1, p24, p16, p20, p14, not_p5}
{not_p20, p17, p1, p24, p16, p3, p14, not_p5}
{not_p20, p17, p1, p24, p16, p18, p14, not_p5}
{not_p20, p17, p1, p24, p16, p20, p14, not_p19}
{not_p20, p17, p1, p24, p16, p3, p14, not_p19}
{not_p20, p17, p1, p24, p16, p18, p14, not_p19}
{p25, p17, p1, p24, p16, p20, p14, not_p5}
{p25, p17, p1, p24, p16, p3, p14, not_p5}
{p25, p17, p1, p24, p16, p18, p14, not_p5}
{p25, p17, p1, p24, p16, p20, p14, not_p19}
{p25, p17, p1, p24, p16, p3, p14, not_p19}
{p25, p17, p1, p24, p16, p18, p14, not_p19}
{p21, p17, p24, p5, p16, p14, not_p5}
{p21, p17, p24, p5, p16, p14, not_p19}
{not_p20, p17, p1, p24, p16, p9, p20, p7, not_p5}
{p25, p17, p1, p24, p16, p9, p20, p7, not_p5}
{p21, p17, p1, p24, p16, p9, p20, p7, not_p5}
{not_p20, p17, p1, p24, p16, p9, p20, p7, not_p19}
{p25, p17, p1, p24, p16, p9, p20, p7, not_p19}
{p21, p17, p1, p24, p16, p9, p20, p7, not_p19}
{not_p20, p17, p1, p24, p16, p9, p3, p7, not_p5}
{p25, p17, p1, p24, p16, p9, p3, p7, not_p5}
{p21, p17, p1, p24, p16, p9, p3, p7, not_p5}
{not_p20, p17, p1, p24, p16, p9, p3, p7, not_p19}
{p25, p17, p1, p24, p16, p9, p3, p7, not_p19}
{p21, p17, p1, p24, p16, p9, p3, p7, not_p19}
{not_p20, p17, p1, p24, p16, p20, not_p25, not_p5}
{not_p20, p17, p1, p24, p16, p3, not_p25, not_p5}
{not_p20, p17, p1, p24, p16, p18, not_p25, not_p5}
{not_p20, p17, p1, p24, p16, p20, not_p25, not_p19}
{not_p20, p17, p1, p24, p16, p3, not_p25, not_p19}
{not_p20, p17, p1, p24, p16, p18, not_p25, not_p19}
{p25, p17, p1, p24, p16, p20, not_p25, not_p5}
{p25, p17, p1, p24, p16, p3, not_p25, not_p5}
{p25, p17, p1, p24, p16, p18, not_p25, not_p5}
{p25, p17, p1, p24, p16, p20, not_p25, not_p19}
{p25, p17, p1, p24, p16, p3, not_p25, not_p19}
{p25, p17, p1, p24, p16, p18, not_p25, not_p19}
{p21, p17, p24, p5, p16, not_p25, not_p5}
{p21, p17, p24, p5, p16, not_p25, not_p19}
{p21, p17, p24, p5, p16, p9, not_p5}
{p21, p17, p24, p5, p16, p9, not_p19}
{not_p20, p17, p1, p24, p16, p18, p9, not_p5}
{p25, p17, p1, p24, p16, p18, p9, not_p5}
{not_p20, p17, p1, p24, p16, p9, p20, p6, not_p5}
{not_p20, p17, p1, p24, p16, p9, p3, p6, not_p5}
{p25, p17, p1, p24, p16, p9, p20, p6, not_p5}
{p25, p17, p1, p24, p16, p9, p3, p6, not_p5}
{not_p20, p17, p1, p24, p16, p18, p9, not_p19}
{p25, p17, p1, p24, p16, p18, p9, not_p19}
{not_p20, p17, p1, p24, p16, p9, p20, p6, not_p19}
{not_p20, p17, p1, p24, p16, p9, p3, p6, not_p19}
{p25, p17, p1, p24, p16, p9, p20, p6, not_p19}
{p25, p17, p1, p24, p16, p9, p3, p6, not_p19}
{not_p20, p17, p15, p4, not_p3, not_p5}
{not_p20, p17, p15, p4, not_p3, not_p19}
{p22, not_p20, p17, p15, p4, not_p3}
{not_p20, p17, p15, p4, p14, not_p5}
{not_p20, p17, p15, p4, p14, not_p19}
{p22, not_p20, p17, p15, p4, p14}
{not_p20, p17, p15, p4, not_p25, not_p5}
{not_p20, p17, p15, p4, not_p25, not_p19}
{p22, not_p20, p17, p15, p4, not_p25}
{not_p20, p17, p15, p9, p4, not_p5}
{not_p20, p17, p15, p9, p4, not_p19}
{p22, not_p20, p17, p15, p9, p4}
{not_p20, p17, p15, p16, not_p3, not_p5}
{not_p20, p17, p15, p16, not_p3, not_p19}
{p22, not_p20, p17, p15, p16, not_p3}
{not_p20, p17, p15, p16, p14, not_p5}
{not_p20, p17, p15, p16, p14, not_p19}
{p22, not_p20, p17, p15, p16, p14}
{not_p20, p17, p15, p16, not_p25, not_p5}
{not_p20, p17, p15, p16, not_p25, not_p19}
{p22, not_p20, p17, p15, p16, not_p25}
{not_p20, p17, p15, p16, p9, not_p5}
{not_p20, p17, p15, p16, p9, not_p19}
{p22, not_p20, p17, p15, p16, p9}
{p25, p17, p15, p4, not_p3, not_p5}
{p25, p17, p15, p4, not_p3, not_p19}
{p22, p25, p17, p15, p4, not_p3}
{p25, p17, p15, p4, p14, not_p5}
{p25, p17, p15, p4, p14, not_p19}
{p22, p25, p17, p15, p4, p14}
{p25, p17, p15, p4, not_p25, not_p5}
{p25, p17, p15, p4, not_p25, not_p19}
{p22, p25, p17, p15, p4, not_p25}
{p25, p17, p15, p9, p4, not_p5}
{p25, p17, p15, p9, p4, not_p19}
{p22, p25, p17, p15, p9, p4}
{p25, p17, p15, p16, not_p3, not_p5}
{p25, p17, p15, p16, not_p3, not_p19}
{p22, p25, p17, p15, p16, not_p3}
{p25, p17, p15, p16, p14, not_p5}
{p25, p17, p15, p16, p14, not_p19}
{p22, p25, p17, p15, p16, p14}
{p25, p17, p15, p16, not_p25, not_p5}
{p25, p17, p15, p16, not_p25, not_p19}
{p22, p25, p17, p15, p16, not_p25}
{p25, p17, p15, p16, p9, not_p5}
{p25, p17, p15, p16, p9, not_p19}
{p22, p25, p17, p15, p16, p9}
{p21, p17, p15, p5, p4, not_p3, not_p5}
{p21, p17, p15, p5, p4, p14, not_p5}
{p21, p17, p15, p5, p4, not_p25, not_p5}
{p21, p17, p15, p5, p9, p4, not_p5}
{p21, p17, p15, p5, p16, not_p3, not_p5}
{p21, p17, p15, p5, p16, p14, not_p5}
{p21, p17, p15, p5, p16, not_p25, not_p5}
{p21, p17, p15, p5, p16, p9, not_p5}
{p21, p17, p15, p5, p4, not_p3, not_p19}
{p21, p17, p15, p5, p4, p14, not_p19}
{p21, p17, p15, p5, p4, not_p25, not_p19}
{p21, p17, p15, p5, p9, p4, not_p19}
{p21, p17, p15, p5, p16, not_p3, not_p19}
{p21, p17, p15, p5, p16, p14, not_p19}
{p21, p17, p15, p5, p16, not_p25, not_p19}
{p21, p17, p15, p5, p16, p9, not_p19}
{p22, p17, p15, p5, p4, not_p3}
{p22, p17, p15, p5, p4, p14}
{p22, p17, p15, p5, p4, not_p25}
{p22, p17, p15, p5, p9, p4}
{p22, p17, p15, p5, p16, not_p3}
{p22, p17, p15, p5, p16, p14}
{p22, p17, p15, p5, p16, not_p25}
{p22, p17, p15, p5, p16, p9}
{p22, not_p20, p17, p3, p4, not_p3, not_p12}
{p22, p25, p17, p3, p4, not_p3, not_p12}
{p22, p17, p5, p3, p4, not_p3, not_p12}
{p22, not_p20, p17, p3, p4, p14, not_p12}
{p22, p25, p17, p3, p4, p14, not_p12}
{p22, p17, p5, p3, p4, p14, not_p12}
{p22, not_p20, p17, p3, p4, not_p25, not_p12}
{p22, p25, p17, p3, p4, not_p25, not_p12}
{p22, p17, p5, p3, p4, not_p25, not_p12}
{p22, not_p20, p17, p9, p3, p4, not_p12}
{p22, p25, p17, p9, p3, p4, not_p12}
{p22, p17, p5, p9, p3, p4, not_p12}
{p22, not_p20, p17, p16, p3, not_p3, not_p12}
{p22, p25, p17, p16, p3, not_p3, not_p12}
{p22, p17, p5, p16, p3, not_p3, not_p12}
{p22, not_p20, p17, p16, p3, p14, not_p12}
{p22, p25, p17, p16, p3, p14, not_p12}
{p22, p17, p5, p16, p3, p14, not_p12}
{p22, not_p20, p17, p16, p3, not_p25, not_p12}
{p22, p25, p17, p16, p3, not_p25, not_p12}
{p22, p17, p5, p16, p3, not_p25, not_p12}
{p22, not_p20, p17, p16, p9, p3, not_p12}
{p22, p25, p17, p16, p9, p3, not_p12}
{p22, p17, p5, p16, p9, p3, not_p12}
{p22, not_p20, p17, p3, p4, not_p3, not_p22}
{p22, p25, p17, p3, p4, not_p3, not_p22}
{p22, p17, p5, p3, p4, not_p3, not_p22}
{p22, not_p20, p17, p3, p4, p14, not_p22}
{p22, p25, p17, p3, p4, p14, not_p22}
{p22, p17, p5, p3, p4, p14, not_p22}
{p22, not_p20, p17, p3, p4, not_p25, not_p22}
{p22, p25, p17, p3, p4, not_p25, not_p22}
{p22, p17, p5, p3, p4, not_p25, not_p22}
{p22, not_p20, p17, p9, p3, p4, not_p22}
{p22, p25, p17, p9, p3, p4, not_p22}
{p22, p17, p5, p9, p3, p4, not_p22}
{p22, not_p20, p17, p16, p3, not_p3, not_p22}
{p22, p25, p17, p16, p3, not_p3, not_p22}
{p22, p17, p5, p16, p3, not_p3, not_p22}
{p22, not_p20, p17, p16, p3, p14, not_p22}
{p22, p25, p17, p16, p3, p14, not_p22}
{p22, p17, p5, p16, p3, p14, not_p22}
{p22, not_p20, p17, p16, p3, not_p25, not_p22}
{p22, p25, p17, p16, p3, not_p25, not_p22}
{p22, p17, p5, p16, p3, not_p25, not_p22}
{p22, not_p20, p17, p16, p9, p3, not_p22}
{p22, p25, p17, p16, p9, p3, not_p22}
{p22, p17, p5, p16, p9, p3, not_p22}
{not_p20, p17, p3, p4, not_p3, not_p12, not_p5}
{not_p20, p17, p16, p3, not_p3, not_p12, not_p5}
{not_p20, p17, p3, p4, not_p3, not_p22, not_p5}
{not_p20, p17, p16, p3, not_p3, not_p22, not_p5}
{not_p20, p17, p3, p4, not_p3, not_p12, not_p19}
{not_p20, p17, p16, p3, not_p3, not_p12, not_p19}
{not_p20, p17, p3, p4, not_p3, not_p22, not_p19}
{not_p20, p17, p16, p3, not_p3, not_p22, not_p19}
{p25, p17, p3, p4, not_p3, not_p12, not_p5}
{p25, p17, p16, p3, not_p3, not_p12, not_p5}
{p25, p17, p3, p4, not_p3, not_p22, not_p5}
{p25, p17, p16, p3, not_p3, not_p22, not_p5}
{p25, p17, p3, p4, not_p3, not_p12, not_p19}
{p25, p17, p16, p3, not_p3, not_p12, not_p19}
{p25, p17, p3, p4, not_p3, not_p22, not_p19}
{p25, p17, p16, p3, not_p3, not_p22, not_p19}
{p21, p17, p5, p3, p4, not_p3, not_p12, not_p5}
{p21, p17, p5, p16, p3, not_p3, not_p12, not_p5}
{p21, p17, p5, p3, p4, not_p3, not_p22, not_p5}
{p21, p17, p5, p16, p3, not_p3, not_p22, not_p5}
{p21, p17, p5, p3, p4, not_p3, not_p12, not_p19}
{p21, p17, p5, p16, p3, not_p3, not_p12, not_p19}
{p21, p17, p5, p3, p4, not_p3, not_p22, not_p19}
{p21, p17, p5, p16, p3, not_p3, not_p22, not_p19}
{not_p20, p17, p3, p4, p14, not_p12, not_p5}
{not_p20, p17, p3, p4, not_p25, not_p12, not_p5}
{not_p20, p17, p9, p3, p4, not_p12, not_p5}
{not_p20, p17, p16, p3, p14, not_p12, not_p5}
{not_p20, p17, p16, p3, not_p25, not_p12, not_p5}
{not_p20, p17, p16, p9, p3, not_p12, not_p5}
{not_p20, p17, p3, p4, p14, not_p22, not_p5}
{not_p20, p17, p3, p4, not_p25, not_p22, not_p5}
{not_p20, p17, p9, p3, p4, not_p22, not_p5}
{not_p20, p17, p16, p3, p14, not_p22, not_p5}
{not_p20, p17, p16, p3, not_p25, not_p22, not_p5}
{not_p20, p17, p16, p9, p3, not_p22, not_p5}
{not_p20, p17, p3, p4, p14, not_p12, not_p19}
{not_p20, p17, p3, p4, not_p25, not_p12, not_p19}
{not_p20, p17, p9, p3, p4, not_p12, not_p19}
{not_p20, p17, p16, p3, p14, not_p12, not_p19}
{not_p20, p17, p16, p3, not_p25, not_p12, not_p19}
{not_p20, p17, p16, p9, p3, not_p12, not_p19}
{not_p20, p17, p3, p4, p14, not_p22, not_p19}
{not_p20, p17, p3, p4, not_p25, not_p22, not_p19}
{not_p20, p17, p9, p3, p4, not_p22, not_p19}
{not_p20, p17, p16, p3, p14, not_p22, not_p19}
{not_p20, p17, p16, p3, not_p25, not_p22, not_p19}
{not_p20, p17, p16, p9, p3, not_p22, not_p19}
{p25, p17, p3, p4, p14, not_p12, not_p5}
{p25, p17, p3, p4, not_p25, not_p12, not_p5}
{p25, p17, p9, p3, p4, not_p12, not_p5}
{p25, p17, p16, p3, p14, not_p12, not_p5}
{p25, p17, p16, p3, not_p25, not_p12, not_p5}
{p25, p17, p16, p9, p3, not_p12, not_p5}
{p25, p17, p3, p4, p14, not_p22, not_p5}
{p25, p17, p3, p4, not_p25, not_p22, not_p5}
{p25, p17, p9, p3, p4, not_p22, not_p5}
{p25, p17, p16, p3, p14, not_p22, not_p5}
{p25, p17, p16, p3, not_p25, not_p22, not_p5}
{p25, p17, p16, p9, p3, not_p22, not_p5}
{p25, p17, p3, p4, p14, not_p12, not_p19}
{p25, p17, p3, p4, not_p25, not_p12, not_p19}
{p25, p17, p9, p3, p4, not_p12, not_p19}
{p25, p17, p16, p3, p14, not_p12, not_p19}
{p25, p17, p16, p3, not_p25, not_p12, not_p19}
{p25, p17, p16, p9, p3, not_p12, not_p19}
{p25, p17, p3, p4, p14, not_p22, not_p19}
{p25, p17, p3, p4, not_p25, not_p22, not_p19}
{p25, p17, p9, p3, p4, not_p22, not_p19}
{p25, p17, p16, p3, p14, not_p22, not_p19}
{p25, p17, p16, p3, not_p25, not_p22, not_p19}
{p25, p17, p16, p9, p3, not_p22, not_p19}
{p21, p17, p5, p3, p4, p14, not_p12, not_p5}
{p21, p17, p5, p3, p4, not_p25, not_p12, not_p5}
{p21, p17, p5, p9, p3, p4, not_p12, not_p5}
{p21, p17, p5, p16, p3, p14, not_p12, not_p5}
{p21, p17, p5, p16, p3, not_p25, not_p12, not_p5}
{p21, p17, p5, p16, p9, p3, not_p12, not_p5}
{p21, p17, p5, p3, p4, p14, not_p22, not_p5}
{p21, p17, p5, p3, p4, not_p25, not_p22, not_p5}
{p21, p17, p5, p9, p3, p4, not_p22, not_p5}
{p21, p17, p5, p16, p3, p14, not_p22, not_p5}
{p21, p17, p5, p16, p3, not_p25, not_p22, not_p5}
{p21, p17, p5, p16, p9, p3, not_p22, not_p5}
{p21, p17, p5, p3, p4, p14, not_p12, not_p19}
{p21, p17, p5, p3, p4, not_p25, not_p12, not_p19}
{p21, p17, p5, p9, p3, p4, not_p12, not_p19}
{p21, p17, p5, p16, p3, p14, not_p12, not_p19}
{p21, p17, p5, p16, p3, not_p25, not_p12, not_p19}
{p21, p17, p5, p16, p9, p3, not_p12, not_p19}
{p21, p17, p5, p3, p4, p14, not_p22, not_p19}
{p21, p17, p5, p3, p4, not_p25, not_p22, not_p19}
{p21, p17, p5, p9, p3, p4, not_p22, not_p19}
{p21, p17, p5, p16, p3, p14, not_p22, not_p19}
{p21, p17, p5, p16, p3, not_p25, not_p22, not_p19}
{p21, p17, p5, p16, p9, p3, not_p22, not_p19}
{p11, p25, p24, not_p8, not_p3, not_p5}
{p11, p25, p24, not_p8, not_p3, not_p19}
{p22, p11, p25, p24, not_p8, not_p3}
{p11, p25, p24, not_p8, p14, not_p5}
{p11, p25, p24, not_p8, p14, not_p19}
{p22, p11, p25, p24, not_p8, p14}
{p11, p25, p24, not_p8, not_p25, not_p5}
{p11, p25, p24, not_p8, not_p25, not_p19}
{p22, p11, p25, p24, not_p8, not_p25}
{p11, p25, p24, p9, not_p8, not_p5}
{p11, p25, p24, p9, not_p8, not_p19}
{p22, p11, p25, p24, p9, not_p8}
{p22, p25, p1, p24, not_p8, p4, not_p3}
{p25, p1, p24, not_p8, p20, p4, not_p3, not_p5}
{p25, p1, p24, p3, not_p8, p4, not_p3, not_p5}
{p25, p1, p24, p18, not_p8, p4, not_p3, not_p5}
{p25, p1, p24, not_p8, p20, p4, not_p3, not_p19}
{p25, p1, p24, p3, not_p8, p4, not_p3, not_p19}
{p25, p1, p24, p18, not_p8, p4, not_p3, not_p19}
{p22, p25, p1, p24, p16, not_p8, not_p3}
{p25, p1, p24, p16, not_p8, p20, not_p3, not_p5}
{p25, p1, p24, p16, p3, not_p8, not_p3, not_p5}
{p25, p1, p24, p16, p18, not_p8, not_p3, not_p5}
{p25, p1, p24, p16, not_p8, p20, not_p3, not_p19}
{p25, p1, p24, p16, p3, not_p8, not_p3, not_p19}
{p25, p1, p24, p16, p18, not_p8, not_p3, not_p19}
{p22, p25, p1, p24, not_p8, p4, p14}
{p25, p1, p24, not_p8, p20, p4, p14, not_p5}
{p25, p1, p24, p3, not_p8, p4, p14, not_p5}
{p25, p1, p24, p18, not_p8, p4, p14, not_p5}
{p25, p1, p24, not_p8, p20, p4, p14, not_p19}
{p25, p1, p24, p3, not_p8, p4, p14, not_p19}
{p25, p1, p24, p18, not_p8, p4, p14, not_p19}
{p22, p25, p1, p24, p16, not_p8, p14}
{p25, p1, p24, p16, not_p8, p20, p14, not_p5}
{p25, p1, p24, p16, p3, not_p8, p14, not_p5}
{p25, p1, p24, p16, p18, not_p8, p14, not_p5}
{p25, p1, p24, p16, not_p8, p20, p14, not_p19}
{p25, p1, p24, p16, p3, not_p8, p14, not_p19}
{p25, p1, p24, p16, p18, not_p8, p14, not_p19}
{p22, p25, p1, p24, not_p8, p4, not_p25}
{p25, p1, p24, not_p8, p20, p4, not_p25, not_p5}
{p25, p1, p24, p3, not_p8, p4, not_p25, not_p5}
{p25, p1, p24, p18, not_p8, p4, not_p25, not_p5}
{p25, p1, p24, not_p8, p20, p4, not_p25, not_p19}
{p25, p1, p24, p3, not_p8, p4, not_p25, not_p19}
{p25, p1, p24, p18, not_p8, p4, not_p25, not_p19}
{p22, p25, p1, p24, p16, not_p8, not_p25}
{p25, p1, p24, p16, not_p8, p20, not_p25, not_p5}
{p25, p1, p24, p16, p3, not_p8, not_p25, not_p5}
{p25, p1, p24, p16, p18, not_p8, not_p25, not_p5}
{p25, p1, p24, p16, not_p8, p20, not_p25, not_p19}
{p25, p1, p24, p16, p3, not_p8, not_p25, not_p19}
{p25, p1, p24, p16, p18, not_p8, not_p25, not_p19}
{p25, p1, p24, p18, p9, not_p8, p4, not_p5}
{p25, p1, p24, p18, p9, not_p8, p4, not_p19}
{p22, p25, p1, p24, p18, p9, not_p8, p4}
{p22, p25, p1, p24, p16, p9, not_p8, p4}
{p25, p1, p24, p16, p9, not_p8, p20, p4, not_p5}
{p25, p1, p24, p16, p9, p3, not_p8, p4, not_p5}
{p25, p1, p24, p16, p9, not_p8, p20, p4, not_p19}
{p25, p1, p24, p16, p9, p3, not_p8, p4, not_p19}
{p22, p25, p1, p24, not_p15, p9, not_p8, p4}
{p25, p1, p24, not_p15, p9, not_p8, p20, p4, not_p5}
{p25, p1, p24, not_p15, p9, p3, not_p8, p4, not_p5}
{p25, p1, p24, not_p15, p9, not_p8, p20, p4, not_p19}
{p25, p1, p24, not_p15, p9, p3, not_p8, p4, not_p19}
{p25, p1, p24, p16, p18, p9, not_p8, not_p5}
{p25, p1, p24, p16, p18, p9, not_p8, not_p19}
{p22, p25, p1, p24, p16, p18, p9, not_p8}
{p22, p25, p1, p24, p16, p9, not_p8, p7}
{p25, p1, p24, p16, p9, not_p8, p20, p7, not_p5}
{p25, p1, p24, p16, p9, p3, not_p8, p7, not_p5}
{p25, p1, p24, p16, p9, not_p8, p20, p7, not_p19}
{p25, p1, p24, p16, p9, p3, not_p8, p7, not_p19}
{p22, p25, p1, p24, p16, p9, not_p8, p6}
{p25, p1, p24, p16, p9, not_p8, p20, p6, not_p5}
{p25, p1, p24, p16, p9, p3, not_p8, p6, not_p5}
{p25, p1, p24, p16, p9, not_p8, p20, p6, not_p19}
{p25, p1, p24, p16, p9, p3, not_p8, p6, not_p19}
{p25, p15, not_p8, p4, not_p3, not_p5}
{p25, p15, p16, not_p8, not_p3, not_p5}
{p11, p25, p15, not_p8, not_p3, not_p5}
{p25, p15, not_p8, p4, not_p3, not_p19}
{p25, p15, p16, not_p8, not_p3, not_p19}
{p11, p25, p15, not_p8, not_p3, not_p19}
{p22, p25, p15, not_p8, p4, not_p3}
{p22, p25, p15, p16, not_p8, not_p3}
{p22, p11, p25, p15, not_p8, not_p3}
{p25, p15, not_p8, p4, p14, not_p5}
{p25, p15, p16, not_p8, p14, not_p5}
{p11, p25, p15, not_p8, p14, not_p5}
{p25, p15, not_p8, p4, p14, not_p19}
{p25, p15, p16, not_p8, p14, not_p19}
{p11, p25, p15, not_p8, p14, not_p19}
{p22, p25, p15, not_p8, p4, p14}
{p22, p25, p15, p16, not_p8, p14}
{p22, p11, p25, p15, not_p8, p14}
{p25, p15, not_p8, p4, not_p25, not_p5}
{p25, p15, p16, not_p8, not_p25, not_p5}
{p11, p25, p15, not_p8, not_p25, not_p5}
{p25, p15, not_p8, p4, not_p25, not_p19}
{p25, p15, p16, not_p8, not_p25, not_p19}
{p11, p25, p15, not_p8, not_p25, not_p19}
{p22, p25, p15, not_p8, p4, not_p25}
{p22, p25, p15, p16, not_p8, not_p25}
{p22, p11, p25, p15, not_p8, not_p25}
{p25, p15, p9, not_p8, p4, not_p5}
{p25, p15, p16, p9, not_p8, not_p5}
{p11, p25, p15, p9, not_p8, not_p5}
{p25, p15, p9, not_p8, p4, not_p19}
{p25, p15, p16, p9, not_p8, not_p19}
{p11, p25, p15, p9, not_p8, not_p19}
{p22, p25, p15, p9, not_p8, p4}
{p22, p25, p15, p16, p9, not_p8}
{p22, p11, p25, p15, p9, not_p8}
{p11, p25, not_p8, not_p3, not_p12, not_p5}
{p11, p25, not_p8, not_p3, not_p12, not_p19}
{p22, p11, p25, not_p8, not_p3, not_p12}
{p11, p25, not_p8, p14, not_p12, not_p5}
{p11, p25, not_p8, p14, not_p12, not_p19}
{p22, p11, p25, not_p8, p14, not_p12}
{p11, p25, not_p8, not_p25, not_p12, not_p5}
{p11, p25, not_p8, not_p25, not_p12, not_p19}
{p22, p11, p25, not_p8, not_p25, not_p12}
{p11, p25, p9, not_p8, not_p12, not_p5}
{p11, p25, p9, not_p8, not_p12, not_p19}
{p22, p11, p25, p9, not_p8, not_p12}
{p11, p25, not_p8, not_p3, not_p22, not_p5}
{p11, p25, not_p8, not_p3, not_p22, not_p19}
{p22, p11, p25, not_p8, not_p3, not_p22}
{p11, p25, not_p8, p14, not_p22, not_p5}
{p11, p25, not_p8, p14, not_p22, not_p19}
{p22, p11, p25, not_p8, p14, not_p22}
{p11, p25, not_p8, not_p25, not_p22, not_p5}
{p11, p25, not_p8, not_p25, not_p22, not_p19}
{p22, p11, p25, not_p8, not_p25, not_p22}
{p11, p25, p9, not_p8, not_p22, not_p5}
{p11, p25, p9, not_p8, not_p22, not_p19}
{p22, p11, p25, p9, not_p8, not_p22}
{p25, p3, not_p8, p4, not_p3, not_p12, not_p5}
{p25, p3, not_p8, p4, not_p3, not_p12, not_p19}
{p22, p25, p3, not_p8, p4, not_p3, not_p12}
{p25, p16, p3, not_p8, not_p3, not_p12, not_p5}
{p25, p16, p3, not_p8, not_p3, not_p12, not_p19}
{p22, p25, p16, p3, not_p8, not_p3, not_p12}
{p25, p3, not_p8, p4, not_p3, not_p22, not_p5}
{p25, p3, not_p8, p4, not_p3, not_p22, not_p19}
{p22, p25, p3, not_p8, p4, not_p3, not_p22}
{p25, p16, p3, not_p8, not_p3, not_p22, not_p5}
{p25, p16, p3, not_p8, not_p3, not_p22, not_p19}
{p22, p25, p16, p3, not_p8, not_p3, not_p22}
{p25, p3, not_p8, p4, p14, not_p12, not_p5}
{p25, p3, not_p8, p4, p14, not_p12, not_p19}
{p22, p25, p3, not_p8, p4, p14, not_p12}
{p25, p16, p3, not_p8, p14, not_p12, not_p5}
{p25, p16, p3, not_p8, p14, not_p12, not_p19}
{p22, p25, p16, p3, not_p8, p14, not_p12}
{p25, p3, not_p8, p4, p14, not_p22, not_p5}
{p25, p3, not_p8, p4, p14, not_p22, not_p19}
{p22, p25, p3, not_p8, p4, p14, not_p22}
{p25, p16, p3, not_p8, p14, not_p22, not_p5}
{p25, p16, p3, not_p8, p14, not_p22, not_p19}
{p22, p25, p16, p3, not_p8, p14, not_p22}
{p25, p3, not_p8, p4, not_p25, not_p12, not_p5}
{p25, p3, not_p8, p4, not_p25, not_p12, not_p19}
{p22, p25, p3, not_p8, p4, not_p25, not_p12}
{p25, p16, p3, not_p8, not_p25, not_p12, not_p5}
{p25, p16, p3, not_p8, not_p25, not_p12, not_p19}
{p22, p25, p16, p3, not_p8, not_p25, not_p12}
{p25, p3, not_p8, p4, not_p25, not_p22, not_p5}
{p25, p3, not_p8, p4, not_p25, not_p22, not_p19}
{p22, p25, p3, not_p8, p4, not_p25, not_p22}
{p25, p16, p3, not_p8, not_p25, not_p22, not_p5}
{p25, p16, p3, not_p8, not_p25, not_p22, not_p19}
{p22, p25, p16, p3, not_p8, not_p25, not_p22}
{p25, p9, p3, not_p8, p4, not_p12, not_p5}
{p25, p9, p3, not_p8, p4, not_p12, not_p19}
{p22, p25, p9, p3, not_p8, p4, not_p12}
{p25, p16, p9, p3, not_p8, not_p12, not_p5}
{p25, p16, p9, p3, not_p8, not_p12, not_p19}
{p22, p25, p16, p9, p3, not_p8, not_p12}
{p25, p9, p3, not_p8, p4, not_p22, not_p5}
{p25, p9, p3, not_p8, p4, not_p22, not_p19}
{p22, p25, p9, p3, not_p8, p4, not_p22}
{p25, p16, p9, p3, not_p8, not_p22, not_p5}
{p25, p16, p9, p3, not_p8, not_p22, not_p19}
{p22, p25, p16, p9, p3, not_p8, not_p22}
{p11, p12, p24, not_p8, not_p3, not_p5}
{p11, p12, p24, not_p8, p14, not_p5}
{p11, p12, p24, not_p8, not_p25, not_p5}
{p11, p12, p24, p9, not_p8, not_p5}
{p22, p11, p12, p24, not_p8, not_p3}
{p22, p11, p12, p24, not_p8, p14}
{p22, p11, p12, p24, not_p8, not_p25}
{p22, p11, p12, p24, p9, not_p8}
{p22, p12, p1, p24, p16, p9, not_p8, p7}
{p22, p12, p1, p24, p16, p18, p9, not_p8}
{p22, p12, p1, p24, p16, not_p24, not_p8, not_p3}
{p22, p12, p1, p24, p16, not_p2, not_p8, not_p3}
{p22, p12, p1, p24, p16, not_p24, not_p8, p14}
{p22, p12, p1, p24, p16, not_p2, not_p8, p14}
{p22, p12, p1, p24, p16, not_p24, not_p8, not_p25}
{p22, p12, p1, p24, p16, not_p2, not_p8, not_p25}
{p12, p1, p24, p16, p18, p9, not_p8, not_p5}
{p12, p1, p24, p16, p9, not_p8, p20, p7, not_p5}
{p12, p1, p24, p16, p9, p3, not_p8, p7, not_p5}
{p12, p1, p24, p16, not_p24, not_p8, p20, not_p3, not_p5}
{p12, p1, p24, p16, not_p24, not_p8, p20, p14, not_p5}
{p12, p1, p24, p16, not_p24, not_p8, p20, not_p25, not_p5}
{p12, p1, p24, p16, not_p2, not_p8, p20, not_p3, not_p5}
{p12, p1, p24, p16, not_p2, not_p8, p20, p14, not_p5}
{p12, p1, p24, p16, not_p2, not_p8, p20, not_p25, not_p5}
{p12, p1, p24, p16, p3, not_p24, not_p8, not_p3, not_p5}
{p12, p1, p24, p16, p3, not_p24, not_p8, p14, not_p5}
{p12, p1, p24, p16, p3, not_p24, not_p8, not_p25, not_p5}
{p12, p1, p24, p16, p3, not_p2, not_p8, not_p3, not_p5}
{p12, p1, p24, p16, p3, not_p2, not_p8, p14, not_p5}
{p12, p1, p24, p16, p3, not_p2, not_p8, not_p25, not_p5}
{p12, p1, p24, p16, p18, not_p24, not_p8, not_p3, not_p5}
{p12, p1, p24, p16, p18, not_p2, not_p8, not_p3, not_p5}
{p12, p1, p24, p16, p18, not_p24, not_p8, p14, not_p5}
{p12, p1, p24, p16, p18, not_p2, not_p8, p14, not_p5}
{p12, p1, p24, p16, p18, not_p24, not_p8, not_p25, not_p5}
{p12, p1, p24, p16, p18, not_p2, not_p8, not_p25, not_p5}
{p12, p1, p24, p9, not_p8, p20, p4, not_p5}
{p12, p1, p24, p9, p3, not_p8, p4, not_p5}
{p12, p1, p24, p18, p9, not_p8, p4, not_p5}
{p12, p1, p24, not_p24, not_p8, p20, p4, not_p3, not_p5}
{p12, p1, p24, not_p24, not_p8, p20, p4, p14, not_p5}
{p12, p1, p24, not_p24, not_p8, p20, p4, not_p25, not_p5}
{p12, p1, p24, not_p2, not_p8, p20, p4, not_p3, not_p5}
{p12, p1, p24, not_p2, not_p8, p20, p4, p14, not_p5}
{p12, p1, p24, not_p2, not_p8, p20, p4, not_p25, not_p5}
{p12, p1, p24, p3, not_p24, not_p8, p4, not_p3, not_p5}
{p12, p1, p24, p3, not_p24, not_p8, p4, p14, not_p5}
{p12, p1, p24, p3, not_p24, not_p8, p4, not_p25, not_p5}
{p12, p1, p24, p3, not_p2, not_p8, p4, not_p3, not_p5}
{p12, p1, p24, p3, not_p2, not_p8, p4, p14, not_p5}
{p12, p1, p24, p3, not_p2, not_p8, p4, not_p25, not_p5}
{p12, p1, p24, p18, not_p24, not_p8, p4, not_p3, not_p5}
{p12, p1, p24, p18, not_p2, not_p8, p4, not_p3, not_p5}
{p12, p1, p24, p18, not_p24, not_p8, p4, p14, not_p5}
{p12, p1, p24, p18, not_p2, not_p8, p4, p14, not_p5}
{p12, p1, p24, p18, not_p24, not_p8, p4, not_p25, not_p5}
{p12, p1, p24, p18, not_p2, not_p8, p4, not_p25, not_p5}
{p22, p12, p1, p24, not_p24, not_p8, p4, not_p3}
{p22, p12, p1, p24, not_p2, not_p8, p4, not_p3}
{p22, p12, p1, p24, not_p24, not_p8, p4, p14}
{p22, p12, p1, p24, not_p2, not_p8, p4, p14}
{p22, p12, p1, p24, p9, not_p8, p4}
{p22, p12, p1, p24, not_p24, not_p8, p4, not_p25}
{p22, p12, p1, p24, not_p2, not_p8, p4, not_p25}
{p12, p15, not_p8, p4, not_p3, not_p5}
{p12, p15, p16, not_p8, not_p3, not_p5}
{p11, p12, p15, not_p8, not_p3, not_p5}
{p12, p15, not_p8, p4, p14, not_p5}
{p12, p15, p16, not_p8, p14, not_p5}
{p11, p12, p15, not_p8, p14, not_p5}
{p12, p15, not_p8, p4, not_p25, not_p5}
{p12, p15, p16, not_p8, not_p25, not_p5}
{p11, p12, p15, not_p8, not_p25, not_p5}
{p12, p15, p9, not_p8, p4, not_p5}
{p12, p15, p16, p9, not_p8, not_p5}
{p11, p12, p15, p9, not_p8, not_p5}
{p22, p12, p15, not_p8, p4, not_p3}
{p22, p12, p15, p16, not_p8, not_p3}
{p22, p11, p12, p15, not_p8, not_p3}
{p22, p12, p15, not_p8, p4, p14}
{p22, p12, p15, p16, not_p8, p14}
{p22, p11, p12, p15, not_p8, p14}
{p22, p12, p15, not_p8, p4, not_p25}
{p22, p12, p15, p16, not_p8, not_p25}
{p22, p11, p12, p15, not_p8, not_p25}
{p22, p12, p15, p9, not_p8, p4}
{p22, p12, p15, p16, p9, not_p8}
{p22, p11, p12, p15, p9, not_p8}
{p11, p12, not_p8, not_p3, not_p12, not_p5}
{p11, p12, not_p8, p14, not_p12, not_p5}
{p11, p12, not_p8, not_p25, not_p12, not_p5}
{p11, p12, p9, not_p8, not_p12, not_p5}
{p11, p12, not_p8, not_p3, not_p22, not_p5}
{p11, p12, not_p8, p14, not_p22, not_p5}
{p11, p12, not_p8, not_p25, not_p22, not_p5}
{p11, p12, p9, not_p8, not_p22, not_p5}
{p22, p11, p12, not_p8, not_p3, not_p12}
{p22, p11, p12, not_p8, p14, not_p12}
{p22, p11, p12, not_p8, not_p25, not_p12}
{p22, p11, p12, p9, not_p8, not_p12}
{p22, p11, p12, not_p8, not_p3, not_p22}
{p22, p11, p12, not_p8, p14, not_p22}
{p22, p11, p12, not_p8, not_p25, not_p22}
{p22, p11, p12, p9, not_p8, not_p22}
{p12, p3, not_p8, p4, not_p3, not_p12, not_p5}
{p12, p3, not_p8, p4, p14, not_p12, not_p5}
{p12, p3, not_p8, p4, not_p25, not_p12, not_p5}
{p12, p9, p3, not_p8, p4, not_p12, not_p5}
{p12, p16, p3, not_p8, not_p3, not_p12, not_p5}
{p12, p16, p3, not_p8, p14, not_p12, not_p5}
{p12, p16, p3, not_p8, not_p25, not_p12, not_p5}
{p12, p16, p9, p3, not_p8, not_p12, not_p5}
{p12, p3, not_p8, p4, not_p3, not_p22, not_p5}
{p12, p3, not_p8, p4, p14, not_p22, not_p5}
{p12, p3, not_p8, p4, not_p25, not_p22, not_p5}
{p12, p9, p3, not_p8, p4, not_p22, not_p5}
{p12, p16, p3, not_p8, not_p3, not_p22, not_p5}
{p12, p16, p3, not_p8, p14, not_p22, not_p5}
{p12, p16, p3, not_p8, not_p25, not_p22, not_p5}
{p12, p16, p9, p3, not_p8, not_p22, not_p5}
{p22, p12, p3, not_p8, p4, not_p3, not_p12}
{p22, p12, p3, not_p8, p4, p14, not_p12}
{p22, p12, p3, not_p8, p4, not_p25, not_p12}
{p22, p12, p9, p3, not_p8, p4, not_p12}
{p22, p12, p16, p3, not_p8, not_p3, not_p12}
{p22, p12, p16, p3, not_p8, p14, not_p12}
{p22, p12, p16, p3, not_p8, not_p25, not_p12}
{p22, p12, p16, p9, p3, not_p8, not_p12}
{p22, p12, p3, not_p8, p4, not_p3, not_p22}
{p22, p12, p3, not_p8, p4, p14, not_p22}
{p22, p12, p3, not_p8, p4, not_p25, not_p22}
{p22, p12, p9, p3, not_p8, p4, not_p22}
{p22, p12, p16, p3, not_p8, not_p3, not_p22}
{p22, p12, p16, p3, not_p8, p14, not_p22}
{p22, p12, p16, p3, not_p8, not_p25, not_p22}
{p22, p12, p16, p9, p3, not_p8, not_p22}
{p11, p12, p24, not_p3, not_p19}
{p11, p12, p24, p14, not_p19}
{p11, p12, p24, not_p25, not_p19}
{p11, p12, p24, p9, not_p19}
{p12, p1, p24, p16, p18, p9, not_p19}
{p12, p1, p24, p16, p9, p20, p6, not_p19}
{p12, p1, p24, p16, p9, p20, p7, not_p19}
{p12, p1, p24, p16, p9, p3, p6, not_p19}
{p12, p1, p24, p16, p9, p3, p7, not_p19}
{p12, p1, p24, p16, not_p24, p20, not_p3, not_p19}
{p12, p1, p24, p16, not_p24, p20, p14, not_p19}
{p12, p1, p24, p16, not_p24, p20, not_p25, not_p19}
{p12, p1, p24, p16, not_p2, p20, not_p3, not_p19}
{p12, p1, p24, p16, not_p2, p20, p14, not_p19}
{p12, p1, p24, p16, not_p2, p20, not_p25, not_p19}
{p12, p1, p24, p16, p3, not_p24, not_p3, not_p19}
{p12, p1, p24, p16, p3, not_p24, p14, not_p19}
{p12, p1, p24, p16, p3, not_p24, not_p25, not_p19}
{p12, p1, p24, p16, p3, not_p2, not_p3, not_p19}
{p12, p1, p24, p16, p3, not_p2, p14, not_p19}
{p12, p1, p24, p16, p3, not_p2, not_p25, not_p19}
{p12, p1, p24, p16, p18, not_p24, not_p3, not_p19}
{p12, p1, p24, p16, p18, not_p2, not_p3, not_p19}
{p12, p1, p24, p16, p18, not_p24, p14, not_p19}
{p12, p1, p24, p16, p18, not_p2, p14, not_p19}
{p12, p1, p24, p16, p18, not_p24, not_p25, not_p19}
{p12, p1, p24, p16, p18, not_p2, not_p25, not_p19}
{p12, p1, p24, not_p24, p20, p4, not_p3, not_p19}
{p12, p1, p24, p3, not_p24, p4, not_p3, not_p19}
{p12, p1, p24, p18, not_p24, p4, not_p3, not_p19}
{p12, p1, p24, not_p2, p20, p4, not_p3, not_p19}
{p12, p1, p24, p3, not_p2, p4, not_p3, not_p19}
{p12, p1, p24, p18, not_p2, p4, not_p3, not_p19}
{p12, p1, p24, not_p24, p20, p4, p14, not_p19}
{p12, p1, p24, p3, not_p24, p4, p14, not_p19}
{p12, p1, p24, p18, not_p24, p4, p14, not_p19}
{p12, p1, p24, not_p2, p20, p4, p14, not_p19}
{p12, p1, p24, p3, not_p2, p4, p14, not_p19}
{p12, p1, p24, p18, not_p2, p4, p14, not_p19}
{p12, p1, p24, not_p24, p20, p4, not_p25, not_p19}
{p12, p1, p24, p3, not_p24, p4, not_p25, not_p19}
{p12, p1, p24, p18, not_p24, p4, not_p25, not_p19}
{p12, p1, p24, not_p2, p20, p4, not_p25, not_p19}
{p12, p1, p24, p3, not_p2, p4, not_p25, not_p19}
{p12, p1, p24, p18, not_p2, p4, not_p25, not_p19}
{p12, p1, p24, p9, p20, p4, not_p19}
{p12, p1, p24, p9, p3, p4, not_p19}
{p12, p1, p24, p18, p9, p4, not_p19}
{p12, p15, p4, not_p3, not_p19}
{p12, p15, p16, not_p3, not_p19}
{p11, p12, p15, not_p3, not_p19}
{p12, p15, p4, p14, not_p19}
{p12, p15, p16, p14, not_p19}
{p11, p12, p15, p14, not_p19}
{p12, p15, p4, not_p25, not_p19}
{p12, p15, p16, not_p25, not_p19}
{p11, p12, p15, not_p25, not_p19}
{p12, p15, p9, p4, not_p19}
{p12, p15, p16, p9, not_p19}
{p11, p12, p15, p9, not_p19}
{p11, p12, not_p3, not_p12, not_p19}
{p11, p12, p14, not_p12, not_p19}
{p11, p12, not_p25, not_p12, not_p19}
{p11, p12, p9, not_p12, not_p19}
{p11, p12, not_p3, not_p22, not_p19}
{p11, p12, p14, not_p22, not_p19}
{p11, p12, not_p25, not_p22, not_p19}
{p11, p12, p9, not_p22, not_p19}
{p12, p3, p4, not_p3, not_p12, not_p19}
{p12, p16, p3, not_p3, not_p12, not_p19}
{p12, p3, p4, not_p3, not_p22, not_p19}
{p12, p16, p3, not_p3, not_p22, not_p19}
{p12, p3, p4, p14, not_p12, not_p19}
{p12, p16, p3, p14, not_p12, not_p19}
{p12, p3, p4, p14, not_p22, not_p19}
{p12, p16, p3, p14, not_p22, not_p19}
{p12, p3, p4, not_p25, not_p12, not_p19}
{p12, p16, p3, not_p25, not_p12, not_p19}
{p12, p3, p4, not_p25, not_p22, not_p19}
{p12, p16, p3, not_p25, not_p22, not_p19}
{p12, p9, p3, p4, not_p12, not_p19}
{p12, p16, p9, p3, not_p12, not_p19}
{p12, p9, p3, p4, not_p22, not_p19}
{p12, p16, p9, p3, not_p22, not_p19}
{p22, p12, p1, p24, p9, p4, p6}
{p22, p12, p1, p24, not_p24, p4, not_p3, p6}
{p22, p12, p1, p24, not_p2, p4, not_p3, p6}
{p22, p12, p1, p24, not_p24, p4, p14, p6}
{p22, p12, p1, p24, not_p2, p4, p14, p6}
{p22, p12, p1, p24, not_p24, p4, not_p25, p6}
{p22, p12, p1, p24, not_p2, p4, not_p25, p6}
{p12, p1, p24, p9, p20, p4, p6, not_p5}
{p12, p1, p24, p9, p3, p4, p6, not_p5}
{p12, p1, p24, p18, p9, p4, p6, not_p5}
{p12, p1, p24, not_p24, p20, p4, not_p3, p6, not_p5}
{p12, p1, p24, not_p24, p20, p4, p14, p6, not_p5}
{p12, p1, p24, not_p24, p20, p4, not_p25, p6, not_p5}
{p12, p1, p24, not_p2, p20, p4, not_p3, p6, not_p5}
{p12, p1, p24, not_p2, p20, p4, p14, p6, not_p5}
{p12, p1, p24, not_p2, p20, p4, not_p25, p6, not_p5}
{p12, p1, p24, p3, not_p24, p4, not_p3, p6, not_p5}
{p12, p1, p24, p3, not_p24, p4, p14, p6, not_p5}
{p12, p1, p24, p3, not_p24, p4, not_p25, p6, not_p5}
{p12, p1, p24, p3, not_p2, p4, not_p3, p6, not_p5}
{p12, p1, p24, p3, not_p2, p4, p14, p6, not_p5}
{p12, p1, p24, p3, not_p2, p4, not_p25, p6, not_p5}
{p12, p1, p24, p18, not_p24, p4, not_p3, p6, not_p5}
{p12, p1, p24, p18, not_p2, p4, not_p3, p6, not_p5}
{p12, p1, p24, p18, not_p24, p4, p14, p6, not_p5}
{p12, p1, p24, p18, not_p2, p4, p14, p6, not_p5}
{p12, p1, p24, p18, not_p24, p4, not_p25, p6, not_p5}
{p12, p1, p24, p18, not_p2, p4, not_p25, p6, not_p5}
{p22, p12, p1, p24, p16, p9, p6}
{p22, p12, p1, p24, p16, not_p24, not_p3, p6}
{p22, p12, p1, p24, p16, not_p2, not_p3, p6}
{p22, p12, p1, p24, p16, not_p24, p14, p6}
{p22, p12, p1, p24, p16, not_p2, p14, p6}
{p22, p12, p1, p24, p16, not_p24, not_p25, p6}
{p22, p12, p1, p24, p16, not_p2, not_p25, p6}
{p12, p1, p24, p16, p9, p20, p6, not_p5}
{p12, p1, p24, p16, p9, p3, p6, not_p5}
{p12, p1, p24, p16, p18, p9, p6, not_p5}
{p12, p1, p24, p16, not_p24, p20, not_p3, p6, not_p5}
{p12, p1, p24, p16, not_p24, p20, p14, p6, not_p5}
{p12, p1, p24, p16, not_p24, p20, not_p25, p6, not_p5}
{p12, p1, p24, p16, not_p2, p20, not_p3, p6, not_p5}
{p12, p1, p24, p16, not_p2, p20, p14, p6, not_p5}
{p12, p1, p24, p16, not_p2, p20, not_p25, p6, not_p5}
{p12, p1, p24, p16, p3, not_p24, not_p3, p6, not_p5}
{p12, p1, p24, p16, p3, not_p24, p14, p6, not_p5}
{p12, p1, p24, p16, p3, not_p24, not_p25, p6, not_p5}
{p12, p1, p24, p16, p3, not_p2, not_p3, p6, not_p5}
{p12, p1, p24, p16, p3, not_p2, p14, p6, not_p5}
{p12, p1, p24, p16, p3, not_p2, not_p25, p6, not_p5}
{p12, p1, p24, p16, p18, not_p24, not_p3, p6, not_p5}
{p12, p1, p24, p16, p18, not_p2, not_p3, p6, not_p5}
{p12, p1, p24, p16, p18, not_p24, p14, p6, not_p5}
{p12, p1, p24, p16, p18, not_p2, p14, p6, not_p5}
{p12, p1, p24, p16, p18, not_p24, not_p25, p6, not_p5}
{p12, p1, p24, p16, p18, not_p2, not_p25, p6, not_p5}
{p11, p12, p24, not_p3, p6, not_p5}
{p11, p12, p24, p14, p6, not_p5}
{p11, p12, p24, not_p25, p6, not_p5}
{p11, p12, p24, p9, p6, not_p5}
{p22, p11, p12, p24, not_p3, p6}
{p22, p11, p12, p24, p14, p6}
{p22, p11, p12, p24, not_p25, p6}
{p22, p11, p12, p24, p9, p6}
{p12, p15, p4, not_p3, p6, not_p5}
{p12, p15, p16, not_p3, p6, not_p5}
{p11, p12, p15, not_p3, p6, not_p5}
{p12, p15, p4, p14, p6, not_p5}
{p12, p15, p16, p14, p6, not_p5}
{p11, p12, p15, p14, p6, not_p5}
{p12, p15, p4, not_p25, p6, not_p5}
{p12, p15, p16, not_p25, p6, not_p5}
{p11, p12, p15, not_p25, p6, not_p5}
{p12, p15, p9, p4, p6, not_p5}
{p12, p15, p16, p9, p6, not_p5}
{p11, p12, p15, p9, p6, not_p5}
{p22, p12, p15, p4, not_p3, p6}
{p22, p12, p15, p16, not_p3, p6}
{p22, p11, p12, p15, not_p3, p6}
{p22, p12, p15, p4, p14, p6}
{p22, p12, p15, p16, p14, p6}
{p22, p11, p12, p15, p14, p6}
{p22, p12, p15, p4, not_p25, p6}
{p22, p12, p15, p16, not_p25, p6}
{p22, p11, p12, p15, not_p25, p6}
{p22, p12, p15, p9, p4, p6}
{p22, p12, p15, p16, p9, p6}
{p22, p11, p12, p15, p9, p6}
{p11, p12, not_p3, p6, not_p12, not_p5}
{p11, p12, p14, p6, not_p12, not_p5}
{p11, p12, not_p25, p6, not_p12, not_p5}
{p11, p12, p9, p6, not_p12, not_p5}
{p22, p11, p12, not_p3, p6, not_p12}
{p22, p11, p12, p14, p6, not_p12}
{p22, p11, p12, not_p25, p6, not_p12}
{p22, p11, p12, p9, p6, not_p12}
{p11, p12, not_p3, p6, not_p22, not_p5}
{p11, p12, p14, p6, not_p22, not_p5}
{p11, p12, not_p25, p6, not_p22, not_p5}
{p11, p12, p9, p6, not_p22, not_p5}
{p22, p11, p12, not_p3, p6, not_p22}
{p22, p11, p12, p14, p6, not_p22}
{p22, p11, p12, not_p25, p6, not_p22}
{p22, p11, p12, p9, p6, not_p22}
{p12, p3, p4, not_p3, p6, not_p12, not_p5}
{p12, p3, p4, p14, p6, not_p12, not_p5}
{p12, p3, p4, not_p25, p6, not_p12, not_p5}
{p12, p9, p3, p4, p6, not_p12, not_p5}
{p12, p16, p3, not_p3, p6, not_p12, not_p5}
{p12, p16, p3, p14, p6, not_p12, not_p5}
{p12, p16, p3, not_p25, p6, not_p12, not_p5}
{p12, p16, p9, p3, p6, not_p12, not_p5}
{p12, p3, p4, not_p3, p6, not_p22, not_p5}
{p12, p3, p4, p14, p6, not_p22, not_p5}
{p12, p3, p4, not_p25, p6, not_p22, not_p5}
{p12, p9, p3, p4, p6, not_p22, not_p5}
{p12, p16, p3, not_p3, p6, not_p22, not_p5}
{p12, p16, p3, p14, p6, not_p22, not_p5}
{p12, p16, p3, not_p25, p6, not_p22, not_p5}
{p12, p16, p9, p3, p6, not_p22, not_p5}
{p22, p12, p3, p4, not_p3, p6, not_p12}
{p22, p12, p3, p4, p14, p6, not_p12}
{p22, p12, p3, p4, not_p25, p6, not_p12}
{p22, p12, p9, p3, p4, p6, not_p12}
{p22, p12, p16, p3, not_p3, p6, not_p12}
{p22, p12, p16, p3, p14, p6, not_p12}
{p22, p12, p16, p3, not_p25, p6, not_p12}
{p22, p12, p16, p9, p3, p6, not_p12}
{p22, p12, p3, p4, not_p3, p6, not_p22}
{p22, p12, p3, p4, p14, p6, not_p22}
{p22, p12, p3, p4, not_p25, p6, not_p22}
{p22, p12, p9, p3, p4, p6, not_p22}
{p22, p12, p16, p3, not_p3, p6, not_p22}
{p22, p12, p16, p3, p14, p6, not_p22}
{p22, p12, p16, p3, not_p25, p6, not_p22}
{p22, p12, p16, p9, p3, p6, not_p22}
"""
