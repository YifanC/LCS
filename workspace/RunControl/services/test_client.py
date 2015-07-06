__author__ = 'matthias'
from communication import *
from data import *

#encoder = Producer("encoder")
rc = Producer("runcontrol")

laser = LaserData()

rc.id = 1
rc.state = -1

#encoder.id = 2
#encoder.state = -1

#encoder.start()
rc.start()

#encoder.send_hello()
#encoder.state = 0

rc.send_hello()
rc.state = 0



laser.count_trigger = 9999999.
print str(laser)
#encoder.send_data(laser)
laser.pos_att = 99.
rc.send_data(laser)
print str(laser)