input = """
% This used to generate incorrect results (models were missing) and was
% provided by the Potsdam group.

not_p2|p5|not_p24|p9:-p16,p12. 
not_p2|p5|not_p24|p4:-p1,p12. 
not_p2|p5|not_p24|p4:-p16,p12. 
not_p2|not_p1|p9:-p1,p12,not p11. 
not_p2|not_p1|p9:-p16,p12,not p11. 
not_p2|not_p1|p4:-p1,p12,not p11. 
not_p2|not_p1|p4:-p16,p12,not p11. 
not_p2|not_p24|p9:-p1,p12,not p11. 
not_p2|not_p24|p9:-p16,p12,not p11. 
not_p2|not_p24|p4:-p1,p12,not p11. 
not_p2|not_p24|p4:-p16,p12,not p11. 
p12|not_p15|p16|p17:-p6,p1. 
p12|not_p15|p16|p17:-p9,p1. 
p12|not_p15|p16|p18:-p6,p1. 
p12|not_p15|p16|p18:-p9,p1. 
p12|not_p15|p7|p17:-p6,p1. 
p12|not_p15|p7|p17:-p9,p1. 
p12|not_p15|p7|p18:-p6,p1. 
p12|not_p15|p7|p18:-p9,p1. 
p12|p4|p16|p17:-p6,p1. 
p12|p4|p16|p17:-p9,p1. 
p12|p4|p16|p18:-p6,p1. 
p12|p4|p16|p18:-p9,p1. 
p12|p4|p7|p17:-p6,p1. 
p12|p4|p7|p17:-p9,p1. 
p12|p4|p7|p18:-p6,p1. 
p12|p4|p7|p18:-p9,p1. 
p6|not_p15|p16|p17:-p6,p1. 
p6|not_p15|p16|p17:-p9,p1. 
p6|not_p15|p16|p18:-p6,p1. 
p6|not_p15|p16|p18:-p9,p1. 
p6|not_p15|p7|p17:-p6,p1. 
p6|not_p15|p7|p17:-p9,p1. 
p6|not_p15|p7|p18:-p6,p1. 
p6|not_p15|p7|p18:-p9,p1. 
p6|p4|p16|p17:-p6,p1. 
p6|p4|p16|p17:-p9,p1. 
p6|p4|p16|p18:-p6,p1. 
p6|p4|p16|p18:-p9,p1. 
p6|p4|p7|p17:-p6,p1. 
p6|p4|p7|p17:-p9,p1. 
p6|p4|p7|p18:-p6,p1. 
p6|p4|p7|p18:-p9,p1. 
p1|p12|p2|not_p16:-not p23,not p3. 
p1|p12|p2:-p19,not p23,not p3. 
p1|p12|not_p16:-not p23,not p3,not p11. 
p1|p12:-p19,not p23,not p3,not p11. 
p1|p7|p2|not_p16:-not p23,not p3. 
p1|p7|p2:-p19,not p23,not p3. 
p1|p7|not_p16:-not p23,not p3,not p11. 
p1|p7:-p19,not p23,not p3,not p11. 
p6|p12|p2|not_p16:-not p23,not p3. 
p6|p12|p2:-p19,not p23,not p3. 
p6|p12|not_p16:-not p23,not p3,not p11. 
p6|p12:-p19,not p23,not p3,not p11. 
p6|p7|p2|not_p16:-not p23,not p3. 
p6|p7|p2:-p19,not p23,not p3. 
p6|p7|not_p16:-not p23,not p3,not p11. 
p6|p7:-p19,not p23,not p3,not p11. 
p19|p1|p12|p2|not_p16:-not p23. 
p19|p1|p12|p2:-p19,not p23. 
p19|p1|p12|not_p16:-not p23,not p11. 
p19|p1|p12:-p19,not p23,not p11. 
p19|p1|p7|p2|not_p16:-not p23. 
p19|p1|p7|p2:-p19,not p23. 
p19|p1|p7|not_p16:-not p23,not p11. 
p19|p1|p7:-p19,not p23,not p11. 
p19|p6|p12|p2|not_p16:-not p23. 
p19|p6|p12|p2:-p19,not p23. 
p19|p6|p12|not_p16:-not p23,not p11. 
p19|p6|p12:-p19,not p23,not p11. 
p19|p6|p7|p2|not_p16:-not p23. 
p19|p6|p7|p2:-p19,not p23. 
p19|p6|p7|not_p16:-not p23,not p11. 
p19|p6|p7:-p19,not p23,not p11. 
not_p19|not_p9|not_p25|p13:-p11,p21. 
not_p19|not_p9|not_p25|p13:-p13,p21. 
not_p19|not_p9|not_p25|not_p9:-p11,p21. 
p16|p22|not_p3|p13:-p13,p21. 
p16|p22|not_p3|not_p9:-p11,p21. 
p16|p22|not_p3|not_p9:-p13,p21. 
p4|p23|p21|not_p25|not_p20:-not p24. 
p4|p23|p21|not_p25:-p25,not p24. 
p4|p23|p21|p13|not_p20:-not p24. 
p4|p23|p21|p13:-p25,not p24. 
p4|p23|p2|not_p25|not_p20:-not p24. 
p4|p23|p2|not_p25:-p25,not p24. 
p4|p23|p2|p13|not_p20:-not p24. 
p4|p23|p2|p13:-p25,not p24. 
p4|p21|p21|not_p25|not_p20:-not p24. 
p4|p21|p21|not_p25:-p25,not p24. 
p4|p21|p21|p13|not_p20:-not p24. 
p4|p21|p21|p13:-p25,not p24. 
p4|p21|p2|not_p25|not_p20:-not p24. 
p4|p21|p2|not_p25:-p25,not p24. 
p4|p21|p2|p13|not_p20:-not p24. 
p4|p21|p2|p13:-p25,not p24. 
not_p18|p23|p21|not_p25|not_p20:-not p24. 
not_p18|p23|p21|not_p25:-p25,not p24. 
not_p18|p23|p21|p13|not_p20:-not p24. 
not_p18|p23|p21|p13:-p25,not p24. 
not_p18|p23|p2|not_p25|not_p20:-not p24. 
not_p18|p23|p2|not_p25:-p25,not p24. 
not_p18|p23|p2|p13|not_p20:-not p24. 
not_p18|p23|p2|p13:-p25,not p24. 
not_p18|p21|p21|not_p25|not_p20:-not p24. 
not_p18|p21|p21|not_p25:-p25,not p24. 
not_p18|p21|p21|p13|not_p20:-not p24. 
not_p18|p21|p21|p13:-p25,not p24. 
not_p18|p21|p2|not_p25|not_p20:-not p24. 
not_p18|p21|p2|not_p25:-p25,not p24. 
not_p18|p21|p2|p13|not_p20:-not p24. 
not_p18|p21|p2|p13:-p25,not p24. 
not_p12|p14|p15|not_p18:-p6,p24. 
not_p12|p14|p15|not_p18:-not p5,p24. 
not_p12|p14|p15|p11:-p6,p24. 
not_p12|p14|p15|p11:-not p5,p24. 
not_p12|p14|p2|not_p18:-p6,p24. 
not_p12|p14|p2|not_p18:-not p5,p24. 
not_p12|p14|p2|p11:-p6,p24. 
not_p12|p14|p2|p11:-not p5,p24. 
not_p12|p15|p15|not_p18:-p6,p24. 
not_p12|p15|p15|not_p18:-not p5,p24. 
not_p12|p15|p15|p11:-p6,p24. 
not_p12|p15|p15|p11:-not p5,p24. 
not_p12|p15|p2|not_p18:-p6,p24. 
not_p12|p15|p2|not_p18:-not p5,p24. 
not_p12|p15|p2|p11:-p6,p24. 
not_p12|p15|p2|p11:-not p5,p24. 
p1|p14|p15|not_p18:-p6,p24. 
p1|p14|p15|not_p18:-not p5,p24. 
p1|p14|p15|p11:-p6,p24. 
p1|p14|p15|p11:-not p5,p24. 
p1|p14|p2|not_p18:-p6,p24. 
p1|p14|p2|not_p18:-not p5,p24. 
p1|p14|p2|p11:-p6,p24. 
p1|p14|p2|p11:-not p5,p24. 
p1|p15|p15|not_p18:-p6,p24. 
p1|p15|p15|not_p18:-not p5,p24. 
p1|p15|p15|p11:-p6,p24. 
p1|p15|p15|p11:-not p5,p24. 
p1|p15|p2|not_p18:-p6,p24. 
p1|p15|p2|not_p18:-not p5,p24. 
p1|p15|p2|p11:-p6,p24. 
p1|p15|p2|p11:-not p5,p24. 
p11|p24|p8|not_p4:-p24,p2. 
p11|p24|p8|not_p4:-p7,p2. 
p11|p24|p8|not_p15:-p24,p2. 
p11|p24|p8|not_p15:-p7,p2. 
p11|p24|not_p1|not_p4:-p24,p2. 
p11|p24|not_p1|not_p4:-p7,p2. 
p11|p24|not_p1|not_p15:-p24,p2. 
p11|p24|not_p1|not_p15:-p7,p2. 
p11|not_p20|p8|not_p4:-p24,p2. 
p11|not_p20|p8|not_p4:-p7,p2. 
p11|not_p20|p8|not_p15:-p24,p2. 
p11|not_p20|p8|not_p15:-p7,p2. 
p11|not_p20|not_p1|not_p4:-p24,p2. 
p11|not_p20|not_p1|not_p4:-p7,p2. 
p11|not_p20|not_p1|not_p15:-p24,p2. 
p11|not_p20|not_p1|not_p15:-p7,p2. 
p24|p8|not_p4:-p24,p2,not p25. 
p24|p8|not_p4:-p7,p2,not p25. 
p24|p8|not_p15:-p24,p2,not p25. 
p24|p8|not_p15:-p7,p2,not p25. 
p24|not_p1|not_p4:-p24,p2,not p25. 
p24|not_p1|not_p4:-p7,p2,not p25. 
p24|not_p1|not_p15:-p24,p2,not p25. 
p24|not_p1|not_p15:-p7,p2,not p25. 
not_p20|p8|not_p4:-p24,p2,not p25. 
not_p20|p8|not_p4:-p7,p2,not p25. 
not_p20|p8|not_p15:-p24,p2,not p25. 
not_p20|p8|not_p15:-p7,p2,not p25. 
not_p20|not_p1|not_p4:-p24,p2,not p25. 
not_p20|not_p1|not_p4:-p7,p2,not p25. 
not_p20|not_p1|not_p15:-p24,p2,not p25. 
not_p20|not_p1|not_p15:-p7,p2,not p25. 
p24|p9|p6|p10:-p19,p23. 
p24|p9|p6|p10:-p12,p23. 
p24|p9|p6|not_p22:-p19,p23. 
p24|p9|p6|not_p22:-p12,p23. 
p24|p9|p24|p10:-p19,p23. 
p24|p9|p24|p10:-p12,p23. 
p24|p9|p24|not_p22:-p19,p23. 
p24|p9|p24|not_p22:-p12,p23. 
p24|not_p8|p6|p10:-p19,p23. 
p24|not_p8|p6|p10:-p12,p23. 
p24|not_p8|p6|not_p22:-p19,p23. 
p24|not_p8|p6|not_p22:-p12,p23. 
p24|not_p8|p24|p10:-p19,p23. 
p24|not_p8|p24|p10:-p12,p23. 
p24|not_p8|p24|not_p22:-p19,p23. 
p24|not_p8|p24|not_p22:-p12,p23. 
p6|p9|p6|p10:-p19,p23. 
p6|p9|p6|p10:-p12,p23. 
p6|p9|p6|not_p22:-p19,p23. 
p6|p9|p6|not_p22:-p12,p23. 
p6|p9|p24|p10:-p19,p23. 
p6|p9|p24|p10:-p12,p23. 
p6|p9|p24|not_p22:-p19,p23. 
p6|p9|p24|not_p22:-p12,p23. 
p6|not_p8|p6|p10:-p19,p23. 
p6|not_p8|p6|p10:-p12,p23. 
p3|p11|not_p20:-p5,p23,not p4. 
p3|p21|p1:-p6,p23,not p4. 
p3|p21|p1:-p5,p23,not p4. 
p3|p21|not_p20:-p6,p23,not p4. 
p3|p21|not_p20:-p5,p23,not p4. 
p3|p20|not_p20|p3:-p13,not p22. 
p3|p20|not_p20|p3:-p1,not p22. 
p3|p20|not_p20|p18:-p13,not p22. 
p3|p20|not_p20|p18:-p1,not p22. 
p3|p20|p18|p3:-p13,not p22. 
p3|p20|p18|p3:-p1,not p22. 
p3|p20|p18:-p13,not p22. 
p3|p20|p18:-p1,not p22. 
p3|not_p20|p3:-p13,not p22,not p14. 
p3|not_p20|p3:-p1,not p22,not p14. 
p3|not_p20|p18:-p13,not p22,not p14. 
p3|not_p20|p18:-p1,not p22,not p14. 
p3|p18|p3:-p13,not p22,not p14. 
p3|p18|p3:-p1,not p22,not p14. 
p3|p18:-p13,not p22,not p14. 
p3|p18:-p1,not p22,not p14. 
not_p21|p20|not_p20|p3:-p13,not p22. 
not_p21|p20|not_p20|p3:-p1,not p22. 
not_p21|p20|not_p20|p18:-p13,not p22. 
not_p21|p20|not_p20|p18:-p1,not p22. 
not_p21|p20|p18|p3:-p13,not p22. 
not_p21|p20|p18|p3:-p1,not p22. 
not_p21|p20|p18:-p13,not p22. 
not_p21|p20|p18:-p1,not p22. 
not_p21|not_p20|p3:-p13,not p22,not p14. 
not_p21|not_p20|p3:-p1,not p22,not p14. 
not_p21|not_p20|p18:-p13,not p22,not p14. 
not_p21|not_p20|p18:-p1,not p22,not p14. 
not_p21|p18|p3:-p13,not p22,not p14. 
not_p21|p18|p3:-p1,not p22,not p14. 
not_p21|p18:-p13,not p22,not p14. 
not_p21|p18:-p1,not p22,not p14. 
p5|p12|not_p20:-p17,not p7. 
p5|p12|not_p20:-p14,not p7. 
p5|p12|p25|not_p20:-p17,not p7. 
p5|p12|p25|not_p20:-p14,not p7. 
p5|p11|p12|not_p20:-p17,not p7. 
p5|p11|p12|not_p20:-p14,not p7. 
p5|p11|p25|not_p20:-p17,not p7. 
p5|p11|p25|not_p20:-p14,not p7. 
p22|p12|not_p20:-p17,not p7. 
p22|p12|not_p20:-p14,not p7. 
p22|p12|p25|not_p20:-p17,not p7. 
p22|p12|p25|not_p20:-p14,not p7. 
p22|p11|p12|not_p20:-p17,not p7. 
p22|p11|p12|not_p20:-p14,not p7. 
p22|p11|p25|not_p20:-p17,not p7. 
p22|p11|p25|not_p20:-p14,not p7. 
p21|p5|p12|not_p20:-p17. 
p21|p5|p12|not_p20:-p14. 
p21|p5|p12|p25|not_p20:-p17. 
p21|p5|p12|p25|not_p20:-p14. 
p21|p5|p11|p12|not_p20:-p17. 
p21|p5|p11|p12|not_p20:-p14. 
p21|p5|p11|p25|not_p20:-p17. 
p21|p5|p11|p25|not_p20:-p14. 
p21|p22|p12|not_p20:-p17. 
p21|p22|p12|not_p20:-p14. 
p21|p22|p12|p25|not_p20:-p17. 
p21|p22|p12|p25|not_p20:-p14. 
p21|p22|p11|p12|not_p20:-p17. 
p21|p22|p11|p12|not_p20:-p14. 
p21|p22|p11|p25|not_p20:-p17. 
p21|p22|p11|p25|not_p20:-p14. 
not_p23|not_p7|p21|p5:-not p24,not p3. 
not_p23|not_p7|p21|p5:-p13,not p3. 
not_p23|not_p7|p21|p11:-not p24,not p3. 
not_p23|not_p7|p21|p11:-p13,not p3. 
not_p23|not_p7|not_p17|p5:-not p24,not p3. 
not_p23|not_p7|not_p17|p5:-p13,not p3. 
not_p23|not_p7|not_p17|p11:-not p24,not p3. 
not_p23|not_p7|not_p17|p11:-p13,not p3. 
not_p23|p7|p21|p5:-not p24,not p3. 
not_p23|p7|p21|p5:-p13,not p3. 
not_p23|p7|p21|p11:-not p24,not p3. 
not_p23|p7|p21|p11:-p13,not p3. 
not_p23|p7|not_p17|p5:-not p24,not p3. 
not_p23|p7|not_p17|p5:-p13,not p3. 
p15|p3|p1:-p19,p5,not p3. 
p15|p3|p6:-p12,p5,not p3. 
p15|p3|p6:-p19,p5,not p3. 
p15|p1:-p12,p5,not p3,not p7. 
p15|p1:-p19,p5,not p3,not p7. 
p15|p6:-p12,p5,not p3,not p7. 
p15|p6:-p19,p5,not p3,not p7. 
p15|p4|p3|p1:-p12,p5. 
p15|p4|p3|p1:-p19,p5. 
p15|p4|p3|p6:-p12,p5. 
p15|p4|p3|p6:-p19,p5. 
p15|p4|p1:-p12,p5,not p7. 
p15|p4|p1:-p19,p5,not p7. 
p15|p4|p6:-p12,p5,not p7. 
p15|p4|p6:-p19,p5,not p7. 
p11|p8|p4|p16. 
p11|p8|p4|p18. 
p11|p8|not_p23|p16. 
p11|p8|not_p23|p18. 
p11|p1|p4|p16. 
p11|p1|p4|p18. 
p11|p1|not_p23|p16. 
p11|p1|not_p23|p18. 
p15|p8|p4|p16. 
p15|p8|p4|p18. 
p15|p8|not_p23|p16. 
p15|p8|not_p23|p18. 
p15|p1|p4|p16. 
p15|p1|p4|p18. 
p15|p1|not_p23|p16. 
p15|p1|not_p23|p18. 
p17|p6|p8|p25. 
p17|p6|p8|not_p8. 
p17|p6|not_p19|p25. 
p17|p6|not_p19|not_p8. 
p17|p12|p8|p25. 
p17|p12|p8|not_p8. 
p17|p12|not_p19|p25. 
p17|p12|not_p19|not_p8. 
p14|p6|p8|p25. 
p14|p6|p8|not_p8. 
p14|p6|not_p19|p25. 
p14|p6|not_p19|not_p8. 
p14|p12|p8|p25. 
p14|p12|p8|not_p8. 
p14|p12|not_p19|p25. 
p14|p12|not_p19|not_p8. 
p15|p14|p24|not_p3. 
p15|p14|p24|p3. 
p15|p14|p9|not_p3. 
p15|p14|p9|p3. 
p15|p11|p24|not_p3. 
p15|p11|p24|p3. 
p15|p11|p9|not_p3. 
p15|p11|p9|p3. 
not_p25|p14|p24|not_p3. 
not_p25|p14|p24|p3. 
not_p25|p14|p9|not_p3. 
not_p25|p14|p9|p3. 
not_p25|p11|p24|not_p3. 
not_p25|p11|p24|p3. 
not_p25|p11|p9|not_p3. 
not_p25|p11|p9|p3. 
p15|p2|p23|not_p12. 
p15|p2|p23|p15. 
p15|p2|p24|not_p12. 
p15|p2|p24|p15. 
p15|not_p22|p23|not_p12. 
p15|not_p22|p23|p15. 
p15|not_p22|p24|not_p12. 
p15|not_p22|p24|p15. 
p2|p23|not_p12:-not p23. 
p2|p23|p15:-not p23. 
p2|p24|not_p12:-not p23. 
p2|p24|p15:-not p23. 
not_p22|p23|not_p12:-not p23. 
not_p22|p23|p15:-not p23. 
not_p22|p24|not_p12:-not p23. 
not_p22|p24|p15:-not p23. 
not_p5|p15|not_p19|p22. 
not_p5|p15|not_p19|not_p12. 
not_p5|p15|p22. 
not_p5|p15|p22|not_p12. 
not_p5|not_p5|not_p19|p22. 
not_p5|not_p5|not_p19|not_p12. 
not_p5|not_p5|p22. 
not_p5|not_p5|p22|not_p12. 
not_p21|p15|not_p19|p22. 
not_p21|p15|not_p19|not_p12. 
not_p21|p15|p22. 
not_p21|p15|p22|not_p12. 
not_p21|not_p5|not_p19|p22. 
not_p21|not_p5|not_p19|not_p12. 
not_p21|not_p5|p22. 
not_p21|not_p5|p22|not_p12. 
not_p6:-not p6. 
:-p6,not_p6. 
not_p2:-not p2. 
:-p2,not_p2. 
not_p24:-not p24. 
:-p24,not_p24. 
not_p16:-not p16. 
:-p16,not_p16. 
not_p9:-not p9. 
:-p9,not_p9. 
not_p4:-not p4. 
:-p4,not_p4. 
not_p1:-not p1. 
:-p1,not_p1. 
not_p15:-not p15. 
:-p15,not_p15. 
not_p18:-not p18. 
:-p18,not_p18. 
not_p7:-not p7. 
:-p7,not_p7. 
not_p17:-not p17. 
:-p17,not_p17. 
not_p14:-not p14. 
:-p14,not_p14. 
not_p20:-not p20. 
:-p20,not_p20. 
not_p23:-not p23. 
:-p23,not_p23. 
not_p8:-not p8. 
:-p8,not_p8. 
not_p3:-not p3. 
:-p3,not_p3. 
not_p25:-not p25. 
:-p25,not_p25. 
not_p22:-not p22. 
:-p22,not_p22. 
not_p19:-not p19. 
:-p19,not_p19. 
not_p21:-not p21. 
:-p21,not_p21. 
not_p5:-not p5. 
:-p5,not_p5. 
not_p12:-not p12. 
:-p12,not_p12.
"""
output = """
{not_p2, not_p24, p1, p18, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, not_p24, p1, not_p16, p3, not_p19, not_p9, not_p25, not_p20, not_p18, not_p12, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, not_p24, p4, not_p1, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p15, p8, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, not_p24, not_p1, p11, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, not_p24, p16, p1, p6, p7, p3, not_p19, not_p9, not_p25, not_p20, not_p18, not_p12, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p2, not_p24, p16, p1, p6, p18, p7, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p24, p16, p12, not_p1, p18, p7, p2, not_p19, not_p9, not_p25, not_p3, not_p20, p15, p8, not_p4, not_p22, not_p21, not_p23, not_p17, not_p5, not_p6, not_p14}
{not_p24, p12, not_p1, p11, p17, p2, not_p16, not_p19, not_p9, not_p25, p13, not_p3, not_p18, p14, p15, not_p4, not_p22, not_p8, p20, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p2, not_p24, not_p1, p11, p17, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p14, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p24, p12, not_p1, p11, p6, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p14}
{not_p24, not_p1, p11, p2, not_p16, not_p19, not_p9, p21, not_p3, not_p20, p25, not_p18, not_p12, p14, p15, not_p4, not_p22, not_p8, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p2, not_p24, not_p1, p11, not_p16, p23, not_p19, not_p9, p21, not_p3, not_p20, p25, not_p18, not_p12, p14, p15, not_p4, not_p22, not_p8, not_p7, not_p17, not_p5, not_p6}
{not_p2, p9, p1, p11, p17, p18, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p12, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6, not_p14}
{not_p2, p9, not_p1, p11, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p18, not_p12, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p24, p12, p4, not_p1, p6, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p14}
{not_p2, not_p24, p4, not_p1, p17, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p14, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{p9, p4, not_p1, p2, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p18, not_p12, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, p9, p4, not_p1, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p18, not_p12, p14, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p2, p9, p4, p1, p17, p18, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p12, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6, not_p14}
{not_p2, not_p24, p4, not_p1, p11, not_p16, not_p19, not_p9, not_p3, not_p20, p25, not_p18, not_p12, p14, p15, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p2, not_p24, p16, p1, p17, p6, p18, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p14, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5}
{not_p2, not_p24, p16, p1, p11, p17, p6, p7, p3, not_p19, not_p9, not_p25, not_p20, not_p18, not_p12, p14, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p5}
{not_p2, not_p24, p16, p4, p1, p17, p6, p7, p3, not_p19, not_p9, not_p25, not_p20, not_p18, not_p12, p14, p15, not_p22, not_p8, not_p21, not_p23, not_p5}
{not_p2, not_p24, p16, p1, p11, p17, p6, p7, not_p19, not_p9, not_p25, p13, p21, not_p3, not_p18, not_p12, p14, p15, not_p4, not_p22, not_p8, p20, not_p23, not_p5}
{not_p2, not_p24, p16, p4, p1, p17, p6, p7, not_p19, not_p9, not_p25, p21, not_p3, not_p18, not_p12, p14, p15, not_p22, not_p8, p20, not_p23, not_p5}
{not_p2, p9, p16, p4, p1, p7, p3, p19, not_p20, p24, p25, not_p18, not_p12, p15, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5, not_p6, not_p14}
{not_p2, p9, p16, p1, p17, p6, p18, not_p19, not_p3, not_p20, p24, p25, not_p12, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p14}
{not_p2, p9, p16, p1, p6, p18, p7, not_p19, not_p3, not_p20, p24, p25, not_p12, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p2, p9, p16, p1, p11, p6, p7, p3, not_p19, not_p20, p24, p25, not_p18, not_p12, p15, not_p4, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p2, p9, p16, p4, p1, p6, p7, p3, not_p19, not_p20, p24, p25, not_p18, not_p12, p15, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p2, not_p24, p16, p4, p1, p11, p6, p7, p3, not_p19, not_p9, not_p20, p25, not_p18, not_p12, p14, p15, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5}
{not_p2, not_p24, p16, p4, p1, p11, p6, p7, not_p19, not_p9, p21, not_p3, p25, not_p18, not_p12, p14, p15, not_p22, not_p8, p20, not_p23, not_p17, not_p5}
{p9, p4, p1, not_p15, p2, not_p16, p3, not_p19, not_p25, not_p20, p24, not_p18, not_p12, p8, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{p9, p4, p1, not_p15, p17, p2, not_p16, p3, not_p19, not_p25, not_p20, p24, not_p18, not_p12, p14, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{p9, p4, p1, not_p15, p2, not_p16, p3, not_p19, not_p20, p24, p25, not_p18, not_p12, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{p9, p4, p1, not_p15, p18, p2, not_p16, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{p9, p4, not_p1, p11, not_p15, p2, not_p16, not_p19, not_p25, not_p3, not_p20, p24, not_p18, not_p12, p8, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p24, p4, not_p1, p11, not_p15, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p14, p8, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p24, p4, not_p1, p11, not_p15, p2, not_p16, not_p19, not_p9, not_p3, not_p20, p25, not_p18, not_p12, p14, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p24, p4, not_p1, p11, not_p15, p17, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, not_p12, p14, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p24, p12, p4, not_p1, p11, not_p15, p6, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p18, p14, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5}
{p9, p12, p4, not_p1, p11, not_p15, p6, p2, not_p16, not_p19, not_p25, not_p3, not_p20, p24, not_p18, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p14}
{p9, p4, p1, not_p15, p18, p2, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{p9, p4, not_p1, p11, not_p15, p2, not_p16, not_p19, not_p3, not_p20, p24, p25, not_p18, not_p12, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6, not_p14}
{p9, p4, p1, not_p15, p17, p18, p2, not_p16, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p14, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p24, p1, p11, not_p15, p2, not_p16, p3, not_p19, not_p9, not_p25, not_p20, not_p18, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{p9, p16, p1, not_p15, p6, p7, p2, p3, not_p19, not_p25, not_p20, p24, not_p18, not_p12, p8, not_p4, not_p22, not_p21, not_p23, not_p17, not_p5, not_p14}
{p9, p16, p1, not_p15, p17, p7, p2, p3, p19, not_p25, not_p20, p24, not_p18, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p23, not_p5, not_p6}
{not_p24, p12, p1, p11, not_p15, p2, not_p16, not_p19, not_p9, not_p25, p13, not_p3, not_p18, p14, p8, not_p4, not_p22, p20, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{p9, p1, not_p15, p17, p18, p2, not_p16, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p5, not_p6, not_p14}
{p9, p16, p1, not_p15, p6, p18, p7, p2, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p4, not_p22, not_p21, not_p23, not_p17, not_p5, not_p14}
{not_p24, p16, not_p1, p11, not_p15, p18, p2, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p24, p16, p12, not_p1, p11, not_p15, p18, p2, not_p19, not_p9, not_p25, p13, not_p3, p14, p8, not_p4, not_p22, p20, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{not_p24, p1, p11, not_p15, p18, p2, not_p16, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p23, not_p7, not_p17, not_p5, not_p6}
{p9, p16, p1, not_p15, p17, p6, p18, p2, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p14}
{p9, p16, p1, not_p15, p6, p18, p7, p2, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p4, not_p22, not_p8, not_p21, not_p23, not_p17, not_p5, not_p14}
{p9, p16, p1, not_p15, p17, p6, p18, p2, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p14, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5}
{not_p24, p16, not_p1, p11, not_p15, p17, p18, p2, not_p19, not_p9, not_p25, not_p3, not_p20, not_p12, p14, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p24, p16, p12, not_p1, p11, not_p15, p17, p18, p2, not_p19, not_p9, not_p25, p13, not_p3, p14, not_p4, not_p22, not_p8, p20, not_p21, not_p23, not_p7, not_p5, not_p6}
{not_p24, p16, p12, not_p1, p11, not_p15, p6, p18, p2, not_p19, not_p9, not_p25, p13, not_p3, p14, not_p4, not_p22, not_p8, p20, not_p21, not_p23, not_p7, not_p17, not_p5}
{not_p24, p16, p12, not_p1, p11, not_p15, p6, p18, p2, not_p19, not_p9, not_p25, not_p3, not_p20, p14, not_p4, not_p22, not_p8, not_p21, not_p23, not_p7, not_p17, not_p5}
{not_p2, p12, p1, p11, not_p15, not_p16, p23, not_p19, not_p9, not_p25, not_p3, p24, not_p18, p14, p8, not_p4, p10, not_p22, p20, not_p21, not_p7, not_p17, not_p5, not_p6}
{not_p2, p9, p12, p1, p11, not_p15, p6, not_p16, p23, p3, not_p19, not_p25, not_p20, p24, not_p18, p8, not_p4, not_p22, not_p21, not_p7, not_p17, not_p5, not_p14}
{not_p2, p9, p16, p1, not_p15, p17, p18, p23, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p14, not_p4, not_p22, not_p8, not_p21, not_p7, not_p5, not_p6}
{not_p2, p16, p1, p11, not_p15, p17, p18, p23, not_p19, not_p9, not_p25, not_p3, not_p20, p24, not_p12, p14, not_p4, not_p22, not_p8, not_p21, not_p7, not_p5, not_p6}
{not_p2, p9, p16, p1, not_p15, p17, p18, p23, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p4, not_p22, not_p8, not_p21, not_p7, not_p5, not_p6, not_p14}
{not_p2, p9, p16, p4, p1, not_p15, p18, p23, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p22, not_p8, not_p21, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, p9, p16, p1, not_p15, p18, p7, p23, not_p19, not_p3, not_p20, p24, p25, not_p12, not_p4, not_p22, not_p8, not_p21, not_p17, not_p5, not_p6, not_p14}
{not_p2, p16, p1, p11, not_p15, p18, p23, not_p19, not_p9, not_p3, not_p20, p24, p25, not_p12, p14, not_p4, not_p22, not_p8, not_p21, not_p7, not_p17, not_p5, not_p6}
{not_p2, p9, p4, p1, not_p15, not_p16, p23, p3, not_p19, not_p25, not_p20, p24, not_p18, not_p12, p8, not_p22, not_p21, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, p9, p16, p1, not_p15, p7, p23, p3, not_p19, not_p25, not_p20, p24, not_p18, not_p12, p8, not_p4, not_p22, not_p21, not_p17, not_p5, not_p6, not_p14}
{not_p2, p1, p11, not_p15, not_p16, p23, p3, not_p19, not_p9, not_p25, not_p20, p24, not_p18, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p7, not_p17, not_p5, not_p6}
{not_p2, p9, p1, not_p15, p17, p18, not_p16, p23, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p4, not_p22, not_p21, not_p7, not_p5, not_p6, not_p14}
{not_p2, p9, p4, p1, not_p15, p18, not_p16, p23, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p22, not_p21, not_p7, not_p17, not_p5, not_p6, not_p14}
{not_p2, p9, p16, p1, not_p15, p18, p7, p23, not_p19, not_p25, not_p3, not_p20, p24, not_p12, p8, not_p4, not_p22, not_p21, not_p17, not_p5, not_p6, not_p14}
{not_p2, p1, p11, not_p15, p18, not_p16, p23, not_p19, not_p9, not_p25, not_p3, not_p20, p24, not_p12, p14, p8, not_p4, not_p22, not_p21, not_p7, not_p17, not_p5, not_p6}
"""
