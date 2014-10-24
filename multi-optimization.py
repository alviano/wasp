#!/usr/bin/env python3

import fileinput
import os
import signal
import subprocess
import time
import sys
import threading

begin = time.time()
lock = threading.Lock()
processes = []
lowerBound = None
upperBound = None
exitCode = None

def terminate():
    for process in processes:
        if process.getReturnCode() is None:
            process.terminate()

class Process(threading.Thread):
    def __init__(self, args):
        threading.Thread.__init__(self)
        
        self._process = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
    def run(self):
        global lock
        global processes
        global lowerBound
        global upperBound
        global exitCode
        
        self._process.stdin.flush()
        self._process.stdin.close()
        
        while True:
            line = self._process.stdout.readline().decode()
            if not line:
                break
                
            if line[0] == 'u':
                lock.acquire()
                newBound = int(line[2:])
                if upperBound is None or newBound < upperBound:
                    upperBound = newBound
                    print("u %d" % (newBound,), file=sys.stderr)
                lock.release()
            elif line[0] == 'l':
                lock.acquire()
                newBound = int(line[2:])
                if lowerBound is None or newBound > lowerBound:
                    lowerBound = newBound
                    print("l %d" % (newBound,), file=sys.stderr)
                lock.release()
            elif line[0] == 'v':
                lock.acquire()
                self.readModel()
                terminate()
                lock.release()
            else:
                break;
                
            if lowerBound and lowerBound == upperBound:
                lock.acquire()
                self.readModel()
                terminate()
                lock.release()

    def write(self, line):
        self._process.stdin.write(line)
        
    def getReturnCode(self):
        return self._process.returncode
        
    def terminate(self):
        self._process.terminate()
        
    def readModel(self):
        global exitCode
        
        if exitCode is not None:
            return
        while True:
            line = self._process.stdout.readline().decode()
            if not line:
                return
            if line[0] == 'v':
                break
        while True:
            line = self._process.stdout.readline().decode()
            if not line:
                break
            print(line, end="")
        exitCode = self._process.wait()

if __name__ == "__main__":
    wasp = sys.argv[1] if len(sys.argv) == 2 else "wasp"

    processes.append(Process([wasp, "--multi", "--oll"]))
    processes.append(Process([wasp, "--multi", "--mgd"]))
    #processes.append(Process([wasp, "--multi", "--opt"]))
    #processes.append(Process([wasp, "--multi", "--pmres"]))

    for line in fileinput.input("-"):
        line = line.encode()
        for process in processes:
            process.write(line)

    for process in processes:
        process.start()
    
    for process in processes:
        process.join()
    
    if lowerBound is not None and lowerBound == upperBound:
        sys.exit(30)
    sys.exit(exitCode)
