__author__ = 'matthias'
from communication import *
from data import *

#encoder = Producer("encoder")
rc = Producer("runcontrol")

laser = LaserData()

rc.id = 1
rc.state = -1
rc.start()
rc.send_hello()
rc.state = 0


laser.count_trigger = 9999999.
print str(laser)
#encoder.send_data(laser)
laser.pos_att = 99.
rc.send_data(laser)
print str(laser)