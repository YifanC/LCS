from devices.mirror import *

mirror = Mirror("mirror211")
mirror.color = False
mirror.com.timeout = 20
mirror.com_init()
mirror.getSerial()
