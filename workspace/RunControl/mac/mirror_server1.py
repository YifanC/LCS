from devices.mirror import *

# Mirror in Box
mirror111 = Mirror("mirror111", 1)
mirror112 = Mirror("mirror112", 2)
mirror111.com_init()
mirror112.com = mirror111.com
mirror111.color = False
mirror112.color = False
mirror111.getSerial()
mirror112.getSerial()

# Mirror on Feedthorough
mirror121 = Mirror("mirror121", 1)
mirror122 = Mirror("mirror122", 2)
mirror121.com_init()
mirror122.com = mirror121.com

mirror122.color = False
mirror121.color = False

mirror121.getSerial()
mirror122.getSerial()
# pretend mirror221 is vertical / mirror222 is horizontal

