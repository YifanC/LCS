__author__ = 'matthias'
from communication import *
import signal

def sigint_handler(signal, frame):
    print "stopping broker"
    print 'signal: ' + str(signal)
    raise SystemExit(1)

signal.signal(signal.SIGINT, sigint_handler)


handler = broker()


