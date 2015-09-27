from devices.mirror import *

# Mirror in Box (not configured on laser server 2, no need to move!)
#mirror211 = Mirror("mirror211", 1)
#mirror212 = Mirror("mirror212", 2)
#mirror211.com_init()
#mirror212.com = mirror211.com

#mirror211.getSerial()
#mirror212.getSerial()

# Mirror on Feedthorough
mirror221 = Mirror("mirror221", 1)
mirror222 = Mirror("mirror222", 2)
mirror221.com_init()
mirror222.com = mirror221.com

# Mirror in Laserbox
mirror211 = Mirror("mirror211", 1)
mirror212 = Mirror("mirror212", 2)
mirror211.com_init()
mirror212.com = mirror211.com

mirror221.getSerial()
mirror222.getSerial()
# pretend mirror221 is vertical / mirror222 is horizontal

