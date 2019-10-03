import time
import json
import sys

s = time.time()
p = json.load(open(sys.argv[1]))
e = time.time()

print "Time: ",  e-s
print len(p)
