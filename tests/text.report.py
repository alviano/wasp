report_testcase = None
report_command = None
report_begin = None
def reportStartTest(args, testcase):
    global report_testcase
    global report_command
    global report_begin
    report_command = " ".join(args)
    report_testcase = testcase
    report_begin = time.time()

def reportStreams(output, error):
    pass
    
def reportFailure(expected, actual):
    print("[FAIL!]", end=" ")

def reportSuccess(expected, actual):
    print("[OK]", end=" ");

def reportTimeout(output, error):
    print("[Timeout!]", end=" ")

def reportEndTest(args, testcase):
    global report_testcase
    global report_command
    global report_begin
    diff = time.time() - report_begin
    print("Running time: %.3fs; testcase: %s; command: %s" % (diff, report_testcase, report_command))
