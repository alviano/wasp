#!/usr/bin/env python3

import datetime
import os
import subprocess
import sys
import time

if len(sys.argv) != 5:
    sys.exit("Usage:\n\t%s <binary> <testcase> <checker> <report-builder>" % (sys.argv[0],))

binary = sys.argv[1]
testcase = sys.argv[2]
checker = sys.argv[3]
reportbuilder = sys.argv[4]

for i in [testcase, checker, reportbuilder]:
    if not os.path.exists(i):
        sys.exit("File not found: %s" % (i,))

exec(open(testcase).read())
for i in ['input', 'output']:
    if i not in locals():
        sys.exit("Testcase %s does not provide variable '%s'" % (testcase, i))
if 'flags' not in locals():
    flags = ""
if 'timeout' not in locals():
    timeout = 100
    
exec(open(checker).read())
for i in ['checker']:
    if i not in locals():
        sys.exit("Checker %s does not provide variable '%s'" % (checker, i))

exec(open(reportbuilder).read())
for i in ['reportStartTest', 'reportStreams', 'reportTimeout', 'reportFailure', 'reportSuccess', 'reportEndTest']:
    if i not in locals():
        sys.exit("Report-builder %s does not provide variable '%s'" % (reportbuilder, i))


args = binary.split()
args.extend(flags.split())
reportStartTest(args, testcase)

first = 0
out = input.strip().encode()
err = ""

def run(args):
    global input
    global out
    global err
    global timeout
    
    try:
        if timeout < 0:
            return
        begin = time.time()
        process = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (out, err) = process.communicate(out, timeout=timeout)
        timeout = timeout - time.time() + begin
    except subprocess.TimeoutExpired:
        timeout = -1
        process.kill()
        
for i in range(0, len(args)):
    if args[i] == "|":
        run(args[first:i])
        first = i+1
if first < len(args):
    run(args[first:])
        
if timeout < 0:
    reportTimeout(out, err)
else:
    out = out.decode()
    err = err.decode()
    reportStreams(out, err)

    checker(out, err)

reportEndTest(args, testcase)
