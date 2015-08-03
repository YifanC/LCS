__author__ = 'matthias'

import sys
import time
import signal

def sigint_handler(signal, frame):
    print "\nbye bye"
    print 'signal: ' + str(signal)
    raise SystemExit(1)

signal.signal(signal.SIGINT, sigint_handler)

print "this is a test script"
print sys.argv[1:]
i = 0
for i in range(10000):
    time.sleep(0.1)
    sys.stdout.write("\r%d" % i)
    sys.stdout.flush()

