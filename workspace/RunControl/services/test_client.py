__author__ = 'matthias'
from communication import *
from data import *
import signal
import sys


def sigint_handler(signal, frame):
    print "stopping assembler"
    print 'signal: ' + str(signal)
    rc.stop()
    sys.exit(1)
    raise SystemExit(1)


signal.signal(signal.SIGINT, sigint_handler)

rc = Producer("runcontrol")

laser = LaserData()

rc.id = 1
rc.state = -1
rc.start()
rc.send_hello()
rc.state = 0

laser.count_trigger = 9999999.
# print str(laser)
#encoder.send_data(laser)
#laser.pos_att = 99.
#rc.send_data(laser)
#print str(laser)
