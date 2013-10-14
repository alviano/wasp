report_begin = None
def reportStartTest(args, testcase):
    global report_begin
    print("Testing %s: %s" % (testcase, " ".join(args)), end=" ")
    report_begin = time.time()

def reportStreams(output, error):
    pass
    
def reportFailure(expected, actual):
    print("FAIL!", end=" ")
    #print("Expecting %s" % (expected,))
    #print("Found     %s" % (actual,))
    #print("Testcase input follows\n%s" % (input,))

def reportSuccess(expected, actual):
    print("OK", end=" ");

def reportTimeout(output, error):
    print("Timeout!", end=" ")

def reportEndTest(args, testcase):
    global report_begin
    diff = time.time() - report_begin
    print("Running time: %.3fs" % (diff,))
