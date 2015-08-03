from devices.mirror import *

mirror221 = Mirror("mirror221", 1)
mirror222 = Mirror("mirror222", 2)
mirror221.color = False
mirror222.color = False
mirror221.com.timeout = 20
mirror221.com_init()

mirror222.com = mirror221.com

mirror221.getSerial()
mirror222.getSerial()

# pretend mirror221 is vertical / mirror222 is horizontal

