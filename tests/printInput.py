#!/usr/bin/env python3

import sys

if len(sys.argv) != 2:
    sys.exit("Usage:\n\t%s <testcase>" % (sys.argv[0],))

testcase = sys.argv[1]

exec(open(testcase).read())
for i in ['input']:
    if i not in locals():
        sys.exit("Testcase %s does not provide variable '%s'" % (testcase, i))
        
print(input.strip())
