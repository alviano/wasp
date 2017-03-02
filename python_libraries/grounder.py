import subprocess
from shutil import copyfile
import sys

pathgrounder = "/Users/carmine/tesi/wasp_ext_plugins/wasp/launch.sh"
encoding = None

def launchGrounder(filename, answer_set):
    global encoding
    grounder = subprocess.Popen([pathgrounder], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if encoding == None:
        with open(filename, 'r') as myfile:
            encoding=myfile.read() #.replace("not ", "__not_");

    print encoding
    grounder.stdin.write(encoding.encode())

    for i in answer_set:
        grounder.stdin.write(i.encode())
    grounder.stdin.close()
    print("finished grounding")
    sys.stdout.flush()
        
    print("starting parsing")
    sys.stdout.flush()
    output=[]
    while True:
        line = grounder.stdout.readline()
        if not line: break
        line = line.decode().strip()
        res = line.split(" ")
        res[1]=res[1][:-1]
        output.append(res[1].replace("__constraint(","").split(',k,'))

    print("end parsing")
    sys.stdout.flush()
    return output
