input = """
:-p19,not_p19. 
p12|not_p15|p16|p17:-p6,p1. 
not_p20|not_p1|not_p4:-p7,p2,not p25. 
p15|p4|p3|p1:-p12,p5. 
p5|p11|p25|not_p20:-p17,not p7. 
p17|p12|p8|not_p8. 
p17|p6|not_p19|not_p8. 
p21|p22|p12|p25|not_p20:-p14. 
p15|not_p22|p24|not_p12. 
p21|p5|p12|not_p20:-p17. 
"""
output = """
{not_p8, not_p12}
{not_p8, p24}
{not_p8, not_p22}
{p15, not_p8}
{p17, not_p20, not_p12}
{p17, not_p20, p24}
{p17, not_p20, not_p22}
{p17, p15, not_p20}
{p17, p5, not_p12}
{p17, p5, p24}
{p17, p5, not_p22}
{p17, p15, p5}
{p17, p25, not_p12, p21}
{p17, p25, p24, p21}
{p17, p25, not_p22, p21}
{p17, p15, p25, p21}
{p17, p11, not_p12, p21}
{p17, p11, p24, p21}
{p17, p11, not_p22, p21}
{p17, p15, p11, p21}
{p12, p17, p25, not_p12}
{p12, p17, p25, p24}
{p12, p17, p25, not_p22}
{p12, p17, p15, p25}
{p12, p17, p11, not_p12}
{p12, p17, p11, p24}
{p12, p17, p11, not_p22}
{p12, p17, p15, p11}
{p8, not_p19, not_p12}
{p8, not_p19, p24}
{p8, not_p19, not_p22}
{p15, p8, not_p19}
{p12, not_p19, not_p12}
{p12, not_p19, p24}
{p12, not_p19, not_p22}
{p12, p15, not_p19}
{p6, p8, not_p12}
{p6, p8, p24}
{p6, p8, not_p22}
{p6, p15, p8}
{p12, p6, not_p12}
{p12, p6, p24}
{p12, p6, not_p22}
{p12, p6, p15}
"""
