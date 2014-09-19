#!/usr/bin/env python3

import sys

if __name__ == "__main__":
    ok = 0
    fail = 0
    time_ = 0
    unknown = 0
    for line in sys.stdin:
        print(line, end="")
        line = line.strip().split()
        if line[0] == '[OK]':
            ok = ok + 1
        elif line[0] == '[FAIL!]':
            fail = fail + 1
        elif line[0] == '[TIME!]':
            time_ = time_ + 1
        else:
            unknown = unknown + 1
    print("Summary: %d lines, %d OK, %d FAIL!, %d TIME!, %d unknown" % (ok+fail+time_+unknown, ok, fail, time_, unknown))
